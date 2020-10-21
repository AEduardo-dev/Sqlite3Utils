#include "../include/database.h"

using database::dbHandler;

/******************************Constructor (test)***************************
   Constructor for testing, it does not have any further use as it directly
   connects to the database test.db
***************************************************************************/

database::dbHandler::dbHandler() {
		std::string db_name = "test.db";
		const char *name = db_name.c_str();
		std::vector<const unsigned char*> v;


		rc = sqlite3_open(name, &db);

		if (rc) {
				fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
				/* If the db cannot be opened -> delete the object */
				delete this;
		} else {
				fprintf(stderr, "Opened %s database successfully\n", name);

				/* If db already exists, try to get the names of the tables in it*/
				// std::string exec_string = "SELECT name " \
				//                           "FROM sqlite_master " \
				//                           "WHERE type='table' " \
				//                           "ORDER BY name;";

				std::string exec_string = query::cmd_select + "name " \
				                          query::cmd_from + "sqlite_master " \
				                          query::cmd_from + opt_type 'table' \
				                          query::cmd_order_by + "name;";
				sql = exec_string.c_str();

				/* SQL Command is executed */
				rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
				if (rc != SQLITE_OK) {
						fprintf(stderr, "SQL error: %s\n", zErrMsg);
				} else {
						/* For each of the tables in the db if there are any */
						while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
								if(sqlite3_column_text(stmt, 0) != NULL) {
										/* Load the names to the handler variable */
										tables[sqlite3_column_text(stmt, 0)] = v;
										// tables.push_back(std::make_pair(sqlite3_column_text(stmt, 0), v));
										std::cout << sqlite3_column_text(stmt, 0) << '\n';
								}
						}
				}
				/* If something went wrong it means no tables names were loaded. Else-> all was loaded*/
				if (rc != SQLITE_DONE) {
						fprintf(stderr, "SQL error: %s\n", zErrMsg);
						fprintf(stdout, "Database %s does not have tables to load.\n", name);
				}
				else{
						fprintf(stdout, "Database %s loaded successfully.\n", name);
				}
				/* The command is ended */
				sqlite3_finalize(stmt);

				/* If some tables exist, load their fields as well */
				if (!tables.empty()) {
						for (auto x : tables) {
								/* Convert tables names for use in sql */
								std::string table_name(reinterpret_cast< char const* > (x.first));
								/* Get table info query */
								exec_string = query::cmd_pragma+ "table_info(" + table_name + ");";
								sql = exec_string.c_str();

								/* SQL Command is executed */
								rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
								if (rc != SQLITE_OK) {
										fprintf(stderr, "SQL error: %s\n", zErrMsg);
								} else {
										/* For each of the tables in the db if there are any */
										while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
												/* In PRAGMA command we get the names in second position */
												std::cout <<  sqlite3_column_text(stmt, 1) << '\n';
												if(sqlite3_column_text(stmt, 1) != NULL) {
														/* Load the names to the handler variable */
														x.second.push_back(sqlite3_column_text(stmt, 0));
														std::cout << x.second.back() << '\n';
												}
										}
								}
								/* If something went wrong it means no field names were loaded. Else-> all was loaded*/
								if (rc != SQLITE_DONE) {
										fprintf(stderr, "SQL error: %s\n", zErrMsg);
										fprintf(stdout, "Database %s does not have fields to load.\n", name);
								}
								else{
										fprintf(stdout, "Database %s loaded successfully.\n", name);
								}
								/* The command is ended */
								sqlite3_finalize(stmt);
						}
				}
		}


}

/******************************CONSTRUCTOR*********************************
   Constructor for the db handler. It connects to the database "db_name" and
   tries to load all the table and field names that may be in it for
   future use in the methods.
 ***************************************************************************/
database::dbHandler::dbHandler(std::string db_name) {
		const char *name = db_name.c_str();
		rc = sqlite3_open(name, &db);

		if (rc) {
				fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		} else {
				fprintf(stderr, "Opened %s database successfully\n", name);

				/* If db already exists, try to get the names of the tables in it*/
				// std::string exec_string = "SELECT name " \
				//                           "FROM sqlite_master " \
				//                           "WHERE type='table' " \
				//                           "ORDER BY name;";

				std::string exec_string = query::cmd_select + "name " \
				                          query::cmd_from + "sqlite_master " \
				                          query::cmd_where + query::opt_type(query::db_table) \
				                          query::cmd_order_by + "name;";
				sql = exec_string.c_str();

				/* SQL Command is executed */
				rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
				if (rc != SQLITE_OK) {
						fprintf(stderr, "SQL error: %s\n", zErrMsg);
				} else {
						/* For each of the tables in the db if there are any */
						while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
								if(sqlite3_column_text(stmt, 0) != NULL) {
										/* Load the names to the handler variable */
										tables[sqlite3_column_text(stmt, 0)] = v;
										std::cout << sqlite3_column_text(stmt, 0) << '\n';
								}
						}
				}
				/* If something went wrong it means no tables names were loaded. Else-> all was loaded*/
				if (rc != SQLITE_DONE) {
						fprintf(stderr, "SQL error: %s\n", zErrMsg);
						fprintf(stdout, "Database %s does not have tables to load.\n", name);
				}
				else{
						fprintf(stdout, "Database %s loading process successful.\n", name);
				}
				/* The command is ended */
				sqlite3_finalize(stmt);

				/* If some tables exist, load their fields as well */
				if (!tables.empty()) {
						for (auto x : tables) {
								/* Convert tables names for use in sql */
								std::string table_name(reinterpret_cast< char const* > (x.first));
								/* Get table info query */
								exec_string = query::cmd_pragma+ \
								              query::opt_table_info(table_name)+ \
								              query::end_query;
								sql = exec_string.c_str();

								/* SQL Command is executed */
								rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
								if (rc != SQLITE_OK) {
										fprintf(stderr, "SQL error: %s\n", zErrMsg);
								} else {
										/* For each of the tables in the db if there are any */
										while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
												/* In PRAGMA command we get the names in second position */
												std::cout <<  sqlite3_column_text(stmt, 1) << '\n';
												if(sqlite3_column_text(stmt, 1) != NULL) {
														/* Load the names to the handler variable */
														x.second.push_back(sqlite3_column_text(stmt, 0));
														std::cout << x.second.back() << '\n';
												}
										}
								}
								/* If something went wrong it means no field names were loaded. Else-> all was loaded*/
								if (rc != SQLITE_DONE) {
										fprintf(stderr, "SQL error: %s\n", zErrMsg);
										fprintf(stdout, "Database %s does not have fields to load.\n", name);
								}
								else{
										fprintf(stdout, "Database %s loaded successfully.\n", name);
								}
								/* The command is ended */
								sqlite3_finalize(stmt);
						}
				}
		}
}

/*********************************createTable**********************************
   Create a table inside the database with name table_name, name of the primary key field as
   primary_key and as many extra fields as we may desire sending them as a vector of pairs.

   Each of the pairs will contain the name of the field in the .first field and the options
   such as datatype and NULL/ NOT NULL options in the .second.
 ****************************************************************************/
bool database::dbHandler::createTable(std::string table_name, \
                                      std::pair<std::string, std::string> primary_key, \
                                      std::vector<std::pair<std::string, std::string> > fields) {
		std::string statement;
		std::string extra_options;

		/* Create SQL statement base */
		statement = query::cmd_create_table + table_name + "(" \
		            + primary_key.first + " " + primary_key.second +",";

		/* Generate each of the fields options in the table */
		extra_options = "";
		for (size_t i = 0; i < fields.size(); ++i) {
				extra_options += fields[i].first + " " + fields[i].second;

				/* For all the fields but the last one we need a coma at the end of the statement*/
				if (i < fields.size() - 1) {
						extra_options += ",";
				}
				/* For the last one we add the closing parenthesis and a semicolon*/
				else {
						extra_options += " );";
				}
		}
		/* Complete the statement */
		statement += extra_options;

		/* Convert to const char* for use in sqlote3_exec() */
		sql = statement.c_str();

		/* Execute SQL statement with callback*/
		rc = sqlite3_exec(db, sql, database::dbHandler::callback, 0, &zErrMsg);

		/*
		   Return failure if the command did not Execute properly.
		   Else, return success
		 */
		if (rc != SQLITE_OK) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
				return EXIT_FAILURE;

		} else {
				fprintf(stdout, "Table created successfully\n");
				/* Now we load the whole new table in the handler */
				std::vector<const unsigned char *> v;

				/* Load the primary key info */
				v.push_back((const unsigned char *)primary_key.first.c_str());

				/* Load the fields info */
				for (size_t i = 0; i < fields.size(); i++) {
						v.push_back((const unsigned char *)fields[i].first.c_str());
				}

				/* Put everything in the handler */
				tables.insert(std::pair<const unsigned char*, \
				                        std::vector<const unsigned char*> > \
				                  ((const unsigned char*)table_name.c_str(), v));
				return EXIT_SUCCESS;
		}
}


/**********************************insert**************************************
   Insert a row into the table "table_name" which field values correspond to
         the ones stored in the "values" vector.
 ****************************************************************************/
bool database::dbHandler::insertRecord(std::string table_name, \
                                       std::vector<std::string> values){
		std::string statement, fields, values_to_insert;

		/* Prepare the name of the fields needed to define the format of the data to insert */
		for (size_t i = 0; i < tables[(const unsigned char*)table_name.c_str()].size(); i++) {
				std::string field_name(reinterpret_cast< char const* > \
				                       (tables[(const unsigned char*)table_name.c_str()][i]));

				/* The last element does not have a coma after it */
				if (i < tables[(const unsigned char*)table_name.c_str()].size() - 1) {
						fields += field_name + ",";
				} else {
						fields += field_name;
				}

		}

		/* Now check if number of values is equal to the number of fields, if not-> insert error */
		/*TODO:12/10/2020.- Add control of null/not null if possible. Maybe use the values with the
		   names of the fields so the user can decide which ones to fill */

		if (values.size() != tables[(const unsigned char*)table_name.c_str()].size()) {
				fprintf(stderr, "SQL error: Number of variables differs from number of fields. Insert operation not possible\n");
				return EXIT_FAILURE;

		} else {
				/* Now we get all the values to be inserted in the row */
				for (size_t j = 0; j < values.size(); j++) {
						/* The last element does not have a coma after it */
						if (j < values.size() - 1) {
								values_to_insert += values[j] + ",";
						} else {
								values_to_insert += values[j];
						}
				}


				/* Create SQL statement */
				statement = query::cmd_insert_into + table_name + " (" + fields + " )"+ \
				            query::cmd_values + "(" + values_to_insert + ")"+ \
				            query::end_query;
				sql = statement.c_str();

				/* Execute SQL statement using the callback to see the insertion,
				   no other information is extracted here*/
				rc = sqlite3_exec(db, sql, database::dbHandler::callback, 0, &zErrMsg);

				if( rc != SQLITE_OK ) {
						fprintf(stderr, "SQL error: %s\n", zErrMsg);
						sqlite3_free(zErrMsg);
						return EXIT_FAILURE;
				} else {
						fprintf(stdout, "Records created successfully\n");
						return EXIT_SUCCESS;
				}
		}
}

/******************************callback*************************************
   Function used when sqlite3_exec() is called. It shows the information of
   the operation done on the database. It is used when we do not need to extract
   data from the query, otherwise the sqlite3_step() is used.
***************************************************************************/

int database::dbHandler::callback(void *NotUsed, int argc, \
                                  char **argv, char **azFilName) {
		int i;
		for (i = 0; i < argc; i++) {
				printf("%s = %s\n", azFilName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");
		return 0;
}


/******************************DESTRUCTOR*************************************
   Whenever the object is destroyed we close the db connection and we print
   a message.
 ***************************************************************************/

database::dbHandler::~dbHandler() {
		sqlite3_close(db);
		std::cout << "db destroyed" << '\n';
}

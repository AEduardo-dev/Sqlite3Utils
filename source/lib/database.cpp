#include "../include/database.h"
#include "../include/dbquery.h"

using database::dbHandler;

//TODO: 24/10/2020 Include descrition of arguments in methods docstrings

/******************************Constructor (test)***************************
   Constructor for testing, it does not have any further use as it directly
   connects to the database test.db
***************************************************************************/

database::dbHandler::dbHandler() {
		std::string db_name = "./db/test.db";
		const char *name = db_name.c_str();
		std::vector<std::string> tables_names, fields;

		rc = sqlite3_open(name, &db);

		if (rc) {
				fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
				/* If the db cannot be opened -> delete the object */
				delete this;
		} else {
				fprintf(stderr, "Opened %s database successfully\n", name);

				/* If db already exists, try to get the names of the tables in it
				   std::string exec_string = "SELECT name " \
				                          "FROM sqlite_master " \
				                          "WHERE type='table' " \
				                          "ORDER BY name;";
				 */

				std::string exec_string = query::cmd_select + "name" + \
				                          query::cl_from + "sqlite_master" + \
				                          query::cl_where + query::opt_type(query::db_table) + \
				                          query::cl_order_by + "name" + query::end_query;

				sql = exec_string.c_str();

				/* SQL Command is executed */
				/* For each of the tables in the db if there are any, extract the name of it (index 0)*/

				if (executeQuery(sql, {0}, tables_names) == EXIT_SUCCESS) {
						/* Load the names to the handler variable */
						for (auto key : tables_names) {
								this->tables[key] = fields;
								std::cout << key << '\n';
						}

						/* If some tables exist, load their fields as well */
						if (!this->tables.empty()) {

								for (auto table : this->tables) {
										/* Convert tables names for use in sql */
										std::string name = table.first;
										/* Get table info query */
										exec_string = query::cmd_pragma+ query::opt_table_info(name) \
										              +query::end_query;

										sql = exec_string.c_str();

										/* If something went wrong it means no field names were loaded. Else-> all was loaded*/
										if (executeQuery(sql, {1}, fields) == EXIT_SUCCESS) {
												/* Insert them to the tables storage */
												tables[table.first] = fields;
										}
										else{
												fprintf(stderr, "Error loading field names from %s\n", table.first.c_str());
										}
								}
						}
				}
				else {
						fprintf(stderr, "Error loading tables from %s\n", name);
				}
		}
}

/******************************CONSTRUCTOR*********************************
   Constructor for the db handler. It connects to the database "db_name" and
   tries to load all the table and field names that may be in it for
   future use in the methods.
 ***************************************************************************/
database::dbHandler::dbHandler(std::string db_path) {

		const char *path = db_path.c_str();
		std::string exec_string;
		std::vector<std::string> tables_names, fields;

		rc = sqlite3_open(path, &db);

		if (rc) {
				fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
				/* If the db cannot be opened -> delete the object */
				delete this;
		} else {
				fprintf(stderr, "Opened %s database successfully\n", path);
				this->db_name = path;

				/* If db already exists, try to get the names of the tables in it
				   std::string exec_string = "SELECT name " \
				                          "FROM sqlite_master " \
				                          "WHERE type='table' " \
				                          "ORDER BY name;";
				 */

				exec_string = query::cmd_select + "name" + \
				              query::cl_from + "sqlite_master" + \
				              query::cl_where + query::opt_type(query::db_table) + \
				              query::cl_order_by + "name" + query::end_query;

				sql = exec_string.c_str();

				/* SQL Command is executed */
				/* For each of the tables in the db if there are any, extract the name of it (index 0)*/

				if (executeQuery(sql, {0}, tables_names) == EXIT_SUCCESS) {
						/* Load the names to the handler variable */
						for (auto key : tables_names) {
								this->tables[key] = fields;
								std::cout << key << '\n';
						}

						/* If some tables exist, load their fields as well */
						if (!this->tables.empty()) {

								for (auto table : this->tables) {
										/* Convert tables names for use in sql */
										std::string name = table.first;
										/* Get table info query */
										exec_string = query::cmd_pragma+ query::opt_table_info(name) \
										              +query::end_query;

										sql = exec_string.c_str();

										/* If something went wrong it means no field names were loaded. Else-> all was loaded*/
										if (executeQuery(sql, {1}, fields) == EXIT_SUCCESS) {
												/* Insert them to the tables storage */
												tables[table.first] = fields;
										}
										else{
												fprintf(stderr, "Error loading field names from %s\n", table.first.c_str());
										}
								}
						}
				}
				else {
						fprintf(stderr, "Error loading tables from %s\n", path);
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
		std::string exec_string;
		std::string extra_options;

		/* Create SQL exec_string base */
		exec_string = query::cmd_create_table + table_name + \
		              "(" + primary_key.first + " " + primary_key.second +",";

		/* Generate each of the fields options in the table */
		for (size_t i = 0; i < fields.size(); ++i) {
				extra_options += fields[i].first + " " + fields[i].second;

				/* For all the fields but the last one we need a comma at the end of the exec_string*/
				if (i < fields.size() - 1) {
						extra_options += ",";
				}

				/* For the last one we add the closing parenthesis and a semicolon*/
				else {
						extra_options += " )" + query::end_query;
				}
		}

		/* Complete the statement */
		exec_string += extra_options;

		/* Convert to const char* for use in sqlite3_exec() */
		sql = exec_string.c_str();

		/* Execute SQL exec_string with callback*/
		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

		/*Return failure if the command did not Execute properly.
		   Else, return success */
		if (rc != SQLITE_OK) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
				return EXIT_FAILURE;

		} else {
				fprintf(stdout, "Table created successfully\n");
				/* Now we load the whole new table in the handler */


				std::vector<std::string> v;
				const std::string key = table_name;
				this->tables[key]=v;
				std::cout << "tables size= " << this->tables.size() << '\n';

				/* Load the primary key info */
				this->tables[key].push_back(primary_key.first);

				/* Load the fields info */
				for (size_t i = 0; i < fields.size(); i++) {
						this->tables[key].push_back(fields[i].first);
						std::cout << this->tables[key].back() << '\n';
				}

				std::cout << "tam of fields = "<< this->tables[key].size() << '\n';
				return EXIT_SUCCESS;
		}
}


/**********************************insertRecord*******************************
   Insert a row into the table "table_name" which field values correspond to
   the ones stored in the "values" vector. It can execute in two ways:

   - If the user wants to give a value to each of the fields of the table,
   then the query will not contain the names of them.

   -If, otherwise, the user wants to set only certain values, the ones passed
   in the values vector as the empty string "" will not be added to the query
 ****************************************************************************/
bool database::dbHandler::insertRecord(std::string table_name, std::vector<std::string> values){

		std::string exec_string, fields, values_to_insert;
		const std::string key = table_name;

		/* Check if number of values is equal to the number of fields, if not-> insert error */

		if (values.size() != this->tables[key].size()) {
				fprintf(stderr, "SQL error: Number of variables differs from number of fields. Insert operation not possible\n");
				return EXIT_FAILURE;

		} else {

				/* Check if we need to get the names of the fields to fill with data */
				if (std::binary_search(values.begin(), values.end(), "")) {

						/* Prepare the name of the fields needed to define the format of the data to insert */
						for (size_t i = 0; i < this->tables[key].size(); i++) {

								/* If the value is not empty we add it to the fields list */
								if (values[i] != "") {

										/* Get the name of the field*/
										std::string field_name(this->tables[key][i]);

										/* For all of them add a comma at the end*/
										fields += field_name + ",";
								}
						}

						/* Once the last one was written, get rid of the trailing comma and add a space*/
						fields.replace(fields.end()-1, fields.end(), " ");
				}

				/* Now we get all the values to be inserted in the row */
				for (size_t j = 0; j < values.size(); j++) {

						if (values[j] != "") {
								values_to_insert += values[j] + ",";
						}
				}
				/* The last element does not have a comma after it */
				values_to_insert.replace(values_to_insert.end()-1, values_to_insert.end(), " ");

				/* Create SQL query depending of the use case */
				if (!fields.empty()) {
						exec_string = query::cmd_insert_into + table_name + " (" + fields + " )"+ \
						              query::cl_values + "(" + values_to_insert + ")"+ \
						              query::end_query;
				} else {
						exec_string = query::cmd_insert_into + table_name + \
						              query::cl_values + "(" + values_to_insert + ")"+ \
						              query::end_query;
				}

				sql = exec_string.c_str();

				/* Execute SQL exec_string using the callback to see the insertion,
				   no other information is extracted here*/
				rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

				if( rc != SQLITE_OK ) {
						fprintf(stderr, "SQL error: %s\n", zErrMsg);
						sqlite3_free(zErrMsg);
						return EXIT_FAILURE;

				} else {
						fprintf(stdout, "Records created successfully.\n");
						/* Then exit with success value */
						return EXIT_SUCCESS;
				}
		}
}

/******************************deleteRecord***********************************
   Delete records from table table_name that fit a certain condition given.
   If this condition is "all", every record in the table will be deleted.
   The condition will be added after a WHERE clause in the query
 ****************************************************************************/
bool database::dbHandler::deleteRecords(std::string table_name, std::string condition){
		std::string exec_string;

		if(condition == "all") {
				exec_string = query::cmd_delete + query::cl_from + table_name + \
				              query::end_query;
		} else{
				exec_string = query::cmd_delete + query::cl_from + table_name + \
				              query::cl_where + condition + \
				              query::end_query;
		}

		/* Execute SQL exec_string using the callback since no information
		   needs to be extracted */
		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

		if( rc != SQLITE_OK ) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
				return EXIT_FAILURE;

		} else {
				fprintf(stdout, "Records deleted successfully.\n");
				/* Then exit with success value */
				return EXIT_SUCCESS;
		}
}



/******************************dropTable*************************************
   Drops the table table_name from the database linked to this handler
 ***************************************************************************/
bool database::dbHandler::dropTable(std::string table_name){
		std::string exec_string;

		exec_string = query::cmd_drop_table + table_name \
		              + query::end_query;

		sql = exec_string.c_str();

		/* Execute SQL exec_string using the callback */
		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

		if( rc != SQLITE_OK ) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
				return EXIT_FAILURE;
		} else {
				fprintf(stdout, "Table %s dropped successfully.\n", table_name.c_str());

				/* After dropping it we need to delete it from the tables map as well */
				this->tables.erase(table_name.c_str());

				/* Then exit with succes flag*/
				return EXIT_SUCCESS;
		}
}


/******************************selectRecords*********************************
   Selects the records from table table_name that fit the conditions given.
   Only the table name is necessary for this to execute, but fields are
   recommended as well. Any other argument has a default value that causes
   the condition to not be included in the query.
 ***************************************************************************/
bool database::dbHandler::selectRecords(std::string table_name, std::vector<std::string> fields, \
                                        bool select_distinct, std::string where_cond, \
                                        std::vector<std::string> group_by, \
                                        std::string having_cond, \
                                        std::vector<std::string> order_by, \
                                        std::string order_type, int limit, int offset){

		std::string exec_string, fields_list, group_list, order_list, condition = "";

		/* If it is not the wildcard */
		if (fields[0] != "*" && fields[0] != "") {

				/* Compose the fields we want to select */
				for(auto field : fields) {
						fields_list += field+",";
				}

				/* Then remove the last comma and add a space */
				fields_list.replace(fields_list.end()-1, fields_list.end(), " ");
		}
		else {
				fields_list = fields[0];
		}

		/* If we want to group the results */
		if (group_by[0] != "") {

				/* Compose the fields we want to select */
				for(auto column : order_by) {
						group_list += column+",";
				}

				/* Then remove the last comma and add a space */
				group_list.replace(group_list.end()-1, group_list.end(), " ");
		}

		/* If we want to order the results */
		if (order_by[0] != "") {

				/* Compose the fields we want to select */
				for(auto column : order_by) {
						order_list += column+",";
				}

				/* Then remove the last comma and add a space */
				order_list.replace(order_list.end()-1, order_list.end(), " ");
		}





		exec_string = query::cmd_select + ((select_distinct) ? query::cl_distinct : "") \
		              + fields_list + query::cl_from + table_name+ \
		              ((where_cond != "") ? query::cl_where + where_cond : "")+ \
		              ((group_by[0] != "") ? query::cl_group_by + group_list : "")+ \
		              ((having_cond != "") ? query::cl_having + having_cond : "")+ \
		              ((order_by[0] != "") ? query::cl_order_by + order_list + order_type : "")+ \
		              ((limit != 0) ? query::cl_limit(limit) : "") + \
		              ((offset != 0) ? query::cl_offset(offset) : "") + \
		              query::end_query;


		return EXIT_SUCCESS;

}

/******************************executeQuery*************************************
   Function used both for internal execution of predefined queries or for the
   custom use the user may find fit. It receives the query and the data it needs
   to retrieve from the output as arguments, and returns the data in the vector
   passed as reference and a success flag.
 ***************************************************************************/
bool database::dbHandler::executeQuery(const char *sql_query, std::vector<int> indexes_stmt, \
                                       std::vector<std::string> &data){

		/* First make sure we are working with an empty vector */
		data.clear();

		/* Then SQL Command is executed if no error occurs */
		rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);

		if (rc != SQLITE_OK) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				return EXIT_FAILURE;

		} else {
				/* Execute the command step by step */
				while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

						/* Get the data in the positions we want from the output */
						for (int x : indexes_stmt) {

								/* Check if the index we try to retrieve has something in it */
								if(sqlite3_column_text(stmt, x) != NULL) {
										/* Extract the data in text format and then put it in the vector */
										data.push_back(reinterpret_cast< char const* > \
										               (sqlite3_column_text(stmt, x)));
								}
						}
				}
		}
		/*
		   If something went wrong it means the query may be incorrect or the data does not exist.
		   Else-> all was executed and  the data extracted
		 */
		if (rc != SQLITE_DONE) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				return EXIT_FAILURE;
		}
		else {
				fprintf(stdout, "Query executed successfully.\n");
				return EXIT_SUCCESS;
		}

		/* The command is ended */
		sqlite3_finalize(stmt);

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
		std::cout << "dbHandler destroyed" << '\n';
}

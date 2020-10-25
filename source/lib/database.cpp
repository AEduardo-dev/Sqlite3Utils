#include "../include/database.h"
#include "../include/dbquery.h"

using handler::DbHandlerLite3;

/******************************Constructor (test)***************************
   Constructor for testing, it does not have any further use as it directly
   connects to the database test._db
***************************************************************************/

handler::DbHandlerLite3::DbHandlerLite3() {
		std::string _db_name = "./db/test.db";
		const char *name = _db_name.c_str();
		std::vector<std::string> tables_names, fields;

		_rc = sqlite3_open(name, &_db);

		if (_rc) {
				fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(_db));
				/* If the _db cannot be opened -> delete the object */
				delete this;
		} else {
				fprintf(stderr, "Opened %s database successfully\n", name);

				/* If _db already exists, try to get the names of the tables in it
				   std::string exec_string = "SELECT name " \
				                          "FROM sqlite_master " \
				                          "WHERE type='table' " \
				                          "ORDER BY name;";
				 */

				std::string exec_string = query::cmd_select + "name" + \
				                          query::cl_from + "sqlite_master" + \
				                          query::cl_where + query::opt_type(query::db_table) + \
				                          query::cl_order_by + "name" + query::end_query;

				_sql = exec_string.c_str();

				/* SQL Command is executed */
				/* For each of the tables in the _db if there are any, extract the name of it (index 0)*/

				if (executeQuery(_sql, {0}, tables_names) == EXIT_SUCCESS) {
						/* Load the names to the handler variable */
						for (auto key : tables_names) {
								this->_tables[key] = fields;
								std::cout << key << '\n';
						}

						/* If some tables exist, load their fields as well */
						if (!this->_tables.empty()) {

								for (auto table : this->_tables) {
										/* Convert tables names for use in sql */
										std::string name = table.first;
										/* Get table info query */
										exec_string = query::cmd_pragma+ query::opt_table_info(name) \
										              +query::end_query;

										_sql = exec_string.c_str();

										/* If something went wrong it means no field names were loaded. Else-> all was loaded*/
										if (executeQuery(_sql, {1}, fields) == EXIT_SUCCESS) {
												/* Insert them to the tables storage */
												_tables[table.first] = fields;
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
   Constructor for the _db handler. It connects to the database "_db_name" and
   tries to load all the table and field names that may be in it for
   future use in the methods.
 ***************************************************************************/
handler::DbHandlerLite3::DbHandlerLite3(std::string _db_path) {

		const char *path = _db_path.c_str();
		std::string exec_string;
		std::vector<std::string> tables_names, fields;

		_rc = sqlite3_open(path, &_db);

		if (_rc) {
				fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(_db));
				/* If the _db cannot be opened -> delete the object */
				delete this;
		} else {
				fprintf(stderr, "Opened %s database successfully\n", path);
				this->_db_name = path;

				/* If _db already exists, try to get the names of the tables in it
				   std::string exec_string = "SELECT name " \
				                          "FROM sqlite_master " \
				                          "WHERE type='table' " \
				                          "ORDER BY name;";
				 */

				exec_string = query::cmd_select + "name" + \
				              query::cl_from + "sqlite_master" + \
				              query::cl_where + query::opt_type(query::db_table) + \
				              query::cl_order_by + "name" + query::end_query;

				_sql = exec_string.c_str();

				/* SQL Command is executed */
				/* For each of the tables in the _db if there are any, extract the name of it (index 0)*/

				if (executeQuery(_sql, {0}, tables_names) == EXIT_SUCCESS) {
						/* Load the names to the handler variable */
						for (auto key : tables_names) {
								this->_tables[key] = fields;
								std::cout << key << '\n';
						}

						/* If some tables exist, load their fields as well */
						if (!this->_tables.empty()) {

								for (auto table : this->_tables) {
										/* Convert tables names for use in sql */
										std::string name = table.first;
										/* Get table info query */
										exec_string = query::cmd_pragma+ query::opt_table_info(name) \
										              +query::end_query;

										_sql = exec_string.c_str();

										/* If something went wrong it means no field names were loaded. Else-> all was loaded*/
										if (executeQuery(_sql, {1}, fields) == EXIT_SUCCESS) {
												/* Insert them to the tables storage */
												_tables[table.first] = fields;
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


void handler::DbHandlerLite3::closeConnection(){
		delete this;
}

/*********************************createTable**********************************
   Create a table inside the database with name table_name, name of the primary key field as
   primary_key and as many extra fields as we may desire sending them as a vector of pairs.

   Each of the pairs will contain the name of the field in the .first field and the options
   such as datatype and NULL/ NOT NULL options in the .second.
 ****************************************************************************/
bool handler::DbHandlerLite3::createTable(std::string table_name, \
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
		_sql = exec_string.c_str();

		/* Execute SQL exec_string with callback*/
		_rc = sqlite3_exec(_db, _sql, callback, 0, &_zErrMsg);

		/*Return failure if the command did not Execute properly.
		   Else, return success */
		if (_rc != SQLITE_OK) {
				fprintf(stderr, "SQL error: %s\n", _zErrMsg);
				sqlite3_free(_zErrMsg);
				return EXIT_FAILURE;

		} else {
				fprintf(stdout, "Table created successfully\n");
				/* Now we load the whole new table in the handler */


				std::vector<std::string> v;
				const std::string key = table_name;
				this->_tables[key]=v;
				std::cout << "tables size= " << this->_tables.size() << '\n';

				/* Load the primary key info */
				this->_tables[key].push_back(primary_key.first);

				/* Load the fields info */
				for (size_t i = 0; i < fields.size(); i++) {
						this->_tables[key].push_back(fields[i].first);
						std::cout << this->_tables[key].back() << '\n';
				}

				std::cout << "tam of fields = "<< this->_tables[key].size() << '\n';
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
bool handler::DbHandlerLite3::insertRecord(std::string table_name, std::vector<std::string> values){

		std::string exec_string, fields, values_to_insert;
		const std::string key = table_name;

		/* Check if number of values is equal to the number of fields, if not-> insert error */

		if (values.size() != this->_tables[key].size()) {
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
				exec_string = query::cmd_insert_into + table_name + \
				              ((!fields.empty()) ? " (" + fields + " )" : "" )+ \
				              query::cl_values + "(" + values_to_insert + ")"+ \
				              query::end_query;

				sql = exec_string.c_str();

				/* Execute SQL exec_string using the callback to see the insertion,
				   no other information is extracted here*/
				_rc = sqlite3_exec(_db, sql, callback, 0, &_zErrMsg);

				if( _rc != SQLITE_OK ) {
						fprintf(stderr, "SQL error: %s\n", _zErrMsg);
						sqlite3_free(_zErrMsg);
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
bool handler::DbHandlerLite3::deleteRecords(std::string table_name, std::string condition){

		std::string exec_string;

		exec_string = query::cmd_delete + query::cl_from + table_name + \
		              ((condition == "all") ? query::cl_where + condition : "") + \
		              query::end_query;

		/* Execute SQL exec_string using the callback since no information
		   needs to be extracted */
		_rc = sqlite3_exec(_db, sql, callback, 0, &_zErrMsg);

		if( _rc != SQLITE_OK ) {
				fprintf(stderr, "SQL error: %s\n", _zErrMsg);
				sqlite3_free(_zErrMsg);
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
bool handler::DbHandlerLite3::dropTable(std::string table_name){
		std::string exec_string;

		exec_string = query::cmd_drop_table + table_name \
		              + query::end_query;

		sql = exec_string.c_str();

		/* Execute SQL exec_string using the callback */
		_rc = sqlite3_exec(_db, sql, callback, 0, &_zErrMsg);

		if( _rc != SQLITE_OK ) {
				fprintf(stderr, "SQL error: %s\n", _zErrMsg);
				sqlite3_free(_zErrMsg);
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
std::vector<std::string>  handler::DbHandlerLite3::selectRecords(std::string table_name, std::vector<std::string> fields, \
                                                                 bool select_distinct, std::string where_cond, \
                                                                 std::vector<std::string> group_by, \
                                                                 std::string having_cond, \
                                                                 std::vector<std::string> order_by, \
                                                                 std::string order_type, int limit, int offset){

		std::string exec_string, fields_list, group_list, order_list, condition = "";
		std::vector<int> data_indexes;
		std::vector<std::string> select_data;

		/* If it is not the wildcard */
		if (fields[0] != "*") {

				data_indexes.push_back(0);
				/* Compose the fields we want to select and the indexes of the data that
				   will be extracted from the execution of the query*/
				for(auto field : fields) {
						fields_list += field+",";
						data_indexes.push_back(data_indexes.back()+1);
				}

				/* Then remove the last comma and add a space */
				fields_list.replace(fields_list.end()-1, fields_list.end(), " ");
		}
		else {
				fields_list = fields[0];

				for (unsigned int i = 0; i < tables[table_name.c_str()].size(); ++i) {
						data_indexes.push_back(i);
				}

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

		sql = exec_string.c_str();

		if(executeQuery(sql, data_indexes, select_data) == EXIT_SUCCESS) {
				return select_data;
		} else{
				fprintf(stdout, "Select operation failed, no data loaded\n");
				select_data.clear();
				return select_data;
		}



}

/******************************executeQuery*************************************
   Function used both for internal execution of predefined queries or for the
   custom use the user may find fit. It receives the query and the data it needs
   to retrieve from the output as arguments, and returns the data in the vector
   passed as reference and a success flag.
 ***************************************************************************/
bool handler::DbHandlerLite3::executeQuery(const char *sql_query, \
                                           std::vector<int> indexes__stmt, \
                                           std::vector<std::string> &data, \
                                           bool verbose){

		/* First make sure we are working with an empty vector */
		data.clear();

		/* Then SQL Command is executed if no error occurs */
		_rc = sqlite3_prepare_v2(_db, sql_query, -1, &_stmt, NULL);

		if (_rc != SQLITE_OK) {
				fprintf(stderr, "SQL error: %s\n", _zErrMsg);
				return EXIT_FAILURE;

		} else {
				/* Execute the command step by step */
				while ((_rc = sqlite3_step(_stmt)) == SQLITE_ROW) {

						/* Get the data in the positions we want from the output */
						for (int x : indexes__stmt) {

								/* Check if the index we try to retrieve has something in it */
								if(sqlite3_column_text(_stmt, x) != NULL) {
										/* Extract the data in text format and then put it in the vector */
										data.push_back(reinterpret_cast< char const* > \
										               (sqlite3_column_text(_stmt, x)));
										(verbose) ? std::cout << sqlite3_column_text(_stmt, x) << "  " : \
										    std::cout <<"";
								}
						}
						(verbose) ? std::cout << '\n' : \
						    std::cout <<"";
				}
		}
		/*
		   If something went wrong it means the query may be incorrect or the data does not exist.
		   Else-> all was executed and  the data extracted
		 */
		if (_rc != SQLITE_DONE) {
				fprintf(stderr, "SQL error: %s\n", _zErrMsg);
				return EXIT_FAILURE;
		}
		else {
				fprintf(stdout, "Query executed successfully.\n");
				return EXIT_SUCCESS;
		}

		/* The command is ended */
		sqlite3_finalize(_stmt);

}

/******************************callback*************************************
   Function used when sqlite3_exec() is called. It shows the information of
   the operation done on the database. It is used when we do not need to extract
   data from the query, otherwise the sqlite3_step() is used.
***************************************************************************/

int handler::DbHandlerLite3::callback(void *NotUsed, int argc, \
                                      char **argv, char **azFilName) {
		int i;
		for (i = 0; i < argc; i++) {
				printf("%s = %s\n", azFilName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");
		return 0;
}

/******************************DESTRUCTOR*************************************
   Whenever the object is destroyed we close the _db connection and we print
   a message.
 ***************************************************************************/

handler::DbHandlerLite3::~DbHandlerLite3() {
		sqlite3_close(_db);
		std::cout << "DbHandlerLite3 destroyed" << '\n';
}

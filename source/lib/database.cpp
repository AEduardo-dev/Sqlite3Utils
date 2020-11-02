#include "../include/sqlite3handler.hpp"
#include "../include/dbquery.hpp"

using handler::Sqlite3Db;

/******************************Constructor (test)***************************/

handler::Sqlite3Db::Sqlite3Db() {
		std::string _db_name = "test.db";
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

				std::string exec_string = query::cmd::select + "name" + \
				                          query::cl::from + "sqlite_master" + \
				                          query::cl::where + query::cl::type("table") + \
				                          query::cl::order_by + "name" + query::end_query;

				_sql = exec_string.c_str();

				/* SQL Command is executed */
				/* For each of the tables in the _db if there are any, extract the name of it (index 0)*/

				if (executeQuery(_sql, tables_names, {0}) == EXIT_SUCCESS) {
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
										exec_string = query::cmd::pragma+ query::cl::table_info(name) \
										              +query::end_query;

										_sql = exec_string.c_str();

										/* If something went wrong it means no field names were loaded. Else-> all was loaded*/
										if (executeQuery(_sql, fields, {1}) == EXIT_SUCCESS) {
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

/******************************CONSTRUCTOR*********************************/
handler::Sqlite3Db::Sqlite3Db(std::string _db_path) {

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
				this->_db_path = path;

				/* If _db already exists, try to get the names of the tables in it
				   std::string exec_string = "SELECT name " \
				                          "FROM sqlite_master " \
				                          "WHERE type='table' " \
				                          "ORDER BY name;";
				 */

				exec_string = query::cmd::select + "name" + \
				              query::cl::from + "sqlite_master" + \
				              query::cl::where + query::cl::type("table") + \
				              query::cl::order_by + "name" + query::end_query;

				_sql = exec_string.c_str();

				/* SQL Command is executed */
				/* For each of the tables in the _db if there are any, extract the name of it (index 0)*/
				if (executeQuery(_sql, tables_names, {0}) == EXIT_SUCCESS) {
						/* Load the names to the handler variable */
						for (auto key : tables_names) {
								this->_tables[key] = fields;
						}

						/* If some tables exist, load their fields as well */
						if (!this->_tables.empty()) {

								for (auto table : this->_tables) {
										/* Convert tables names for use in sql */
										std::string name = table.first;
										/* Get table info query */
										exec_string = query::cmd::pragma+ query::cl::table_info(name) \
										              +query::end_query;

										_sql = exec_string.c_str();

										/* If something went wrong it means no field names were loaded. Else-> all was loaded*/
										if (executeQuery(_sql, fields, {1}) == EXIT_SUCCESS) {
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

/******************************DESTRUCTOR*************************************/

handler::Sqlite3Db::~Sqlite3Db() {
		sqlite3_close(_db);
		std::cout << "Sqlite3Db destroyed" << '\n';
}

/******************************closeConnection*******************************/
void handler::Sqlite3Db::closeConnection(){
		sqlite3_close(_db);
}

/*********************************createTable**********************************/
bool handler::Sqlite3Db::createTable(std::string table_name, \
                                     std::vector<FieldDescription> fields) {
		std::string exec_string;
		std::string extra_options;

		/* Create SQL exec_string base */
		exec_string = query::cmd::create_table + table_name;

		/* Generate each of the fields options in the table */
		extra_options += "(";

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

		/*Return failure if the command did not Execute properly.
		   Else, return success */
		if (executeQuery(_sql) == EXIT_SUCCESS) {

				fprintf(stdout, "Table created successfully\n");
				/* Now we load the whole new table in the handler */

				std::vector<std::string> v;
				const std::string key = table_name;
				this->_tables[key]=v;

				/* Load the fields info */
				for (size_t i = 0; i < fields.size(); i++) {
						this->_tables[key].push_back(fields[i].first);
				}
				return EXIT_SUCCESS;

		} else {
				return EXIT_FAILURE;
		}
}


/**********************************insertRecord*******************************/
bool handler::Sqlite3Db::insertRecord(std::string table_name, std::vector<std::string> values){

		std::string exec_string, fields, values_to_insert;
		const std::string key = table_name;
		std::vector<std::string> field_types;
		bool type_error = false;

		/* Check if table exists in the database */
		if (this->_tables.find(key) == this->_tables.end()) {
				fprintf(stderr, "SQL error: No such table: %s\n", key.c_str());
				return EXIT_FAILURE;
		}

		/* Check if number of values is equal to the number of fields, if not-> insert error */
		if (values.size() != this->_tables[key].size()) {
				fprintf(stderr, "SQL error: Number of variables differs from number of fields. Insert operation not possible\n");
				return EXIT_FAILURE;

		} else {

				exec_string = query::cmd::pragma+ query::cl::table_info(table_name) \
				              +query::end_query;

				_sql = exec_string.c_str();

				/* Get type of data to be inserted in the field */
				if (executeQuery(_sql, field_types, {2}) == EXIT_FAILURE) {
						fprintf(stderr, "Error loading field types from %s\n", table_name.c_str());
						return EXIT_FAILURE;
				}

				/* Get the affinity corresponding to each type */
				for (size_t i = 0; i < field_types.size(); ++i) {
						field_types[i] = getAffinity(field_types[i]);
				}

				/* Check if we need to get the names of the fields to fill with data */

				if (std::find(values.begin(), values.end(), "") != values.end()) {
						/* Prepare the name of the fields needed to define the format of the data to insert */
						fields += "(";
						for (size_t j = 0; j < this->getFields(table_name).size(); ++j) {
								/* Get the name of the field*/
								/* For all of them add a comma at the end*/
								if (values[j] != "") {
										fields += this->getFields(table_name)[j] + ",";
								}
						}
						/* Once the last one was written, get rid of the trailing comma and add a space*/
						fields.replace(fields.end()-1, fields.end(), ")");
				}

				/* Now we get all the values to be inserted in the row */
				values_to_insert += "(";
				for (size_t k = 0; k < values.size(); ++k) {
						if (values[k] != "") {
								if (field_types[k] == query::affinity::text || \
								    field_types[k] == query::affinity::blob) {
										values_to_insert += "\'" + values[k] +"\',";

								}else{
										if (field_types[k] == "NULL" && values[k] == "NULL") {
												values_to_insert += values[k]+ ",";

										}else if(isAffined(field_types[k], values[k])) {
												values_to_insert += values[k]+ ",";

										}else{
												fprintf(stderr, "Type error in value %d. Expected %s affinity\n", static_cast<int>(k), field_types[k].c_str());
												type_error = true;
										}
								}
						}
						if(type_error)
								return EXIT_FAILURE;
				}
				/* The last element does not have a comma after it */
				values_to_insert.replace(values_to_insert.end()-1, values_to_insert.end(), ")");

				/* Create SQL query depending of the use case */
				exec_string = query::cmd::insert_into + table_name + \
				              ((!fields.empty()) ? fields : "" )+ \
				              query::cl::values + values_to_insert + \
				              query::end_query;

				_sql = exec_string.c_str();

				/* Execute SQL exec_string */

				// _rc = sqlite3_exec(_db, _sql, callback, 0, &_zErrMsg);

				if(executeQuery(_sql, handler::empty_vec, {}, true) == EXIT_SUCCESS) {
						fprintf(stdout, "Records created successfully.\n");
						/* Then exit with success value */
						return EXIT_SUCCESS;

				} else {
						return EXIT_FAILURE;
				}
		}
}

/******************************deleteRecord***********************************/
bool handler::Sqlite3Db::deleteRecords(std::string table_name, std::string condition){

		std::string exec_string;

		/* Generate parametrized query */
		exec_string = query::cmd::delete_ + query::cl::from + table_name + \
		              ((condition == "all") ? "" : query::cl::where + condition) + \
		              query::end_query;

		/* Convert to constant for use inside of Sqlite3 */
		_sql = exec_string.c_str();

		/* Execute the query and return the succes or failure of it */
		if(executeQuery(_sql) == EXIT_SUCCESS) {
				fprintf(stdout, "Records deleted successfully.\n");
				return EXIT_SUCCESS;

		} else {
				return EXIT_FAILURE;
		}
}



/******************************dropTable*************************************/
bool handler::Sqlite3Db::dropTable(std::string table_name){
		std::string exec_string;

		/* Generate parametrized query */
		exec_string = query::cmd::drop_table + table_name \
		              + query::end_query;

		/* Convert to constant for use inside of Sqlite3 */
		_sql = exec_string.c_str();

		/* Execute the query */
		if(executeQuery(_sql) == EXIT_SUCCESS) {
				fprintf(stdout, "Table %s dropped successfully.\n", table_name.c_str());

				/* After dropping the table, we need to delete it from the tables map as well */
				this->_tables.erase(table_name.c_str());

				/* Then exit with success flag*/
				return EXIT_SUCCESS;

		} else {
				return EXIT_FAILURE;
		}
}


/******************************selectRecords*********************************/
std::vector<std::string>  handler::Sqlite3Db::selectRecords(std::string table_name, \
                                                            std::vector<std::string> fields, \
                                                            bool select_distinct, \
                                                            std::string where_cond, \
                                                            std::vector<std::string> group_by, \
                                                            std::string having_cond, \
                                                            std::vector<std::string> order_by, \
                                                            std::string order_type, int limit, \
                                                            int offset){

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

				for (unsigned int i = 0; i < _tables[table_name.c_str()].size(); ++i) {
						data_indexes.push_back(i);
				}

		}

		/* If we want to group the results */
		if (!group_by.empty()) {

				/* Compose the fields we want to select */
				for(auto column : group_by) {
						group_list += column+",";
				}

				/* Then remove the last comma and add a space */
				group_list.replace(group_list.end()-1, group_list.end(), " ");
		}

		/* If we want to order the results */
		if (!order_by.empty()) {

				if(order_type != "ASC" && order_type != "DESC"){
						fprintf(stderr, "Order option does not match. It should be either \"ASC\" or \"DESC\", not \"%s\"\n", order_type.c_str());
						return empty_vec;
				}

				/* Compose the fields we want to select */
				for(auto column : order_by) {
						order_list += column+",";
				}

				/* Then remove the last comma and add a space */
				order_list.replace(order_list.end()-1, order_list.end(), " ");
		}

		exec_string = query::cmd::select + ((select_distinct) ? query::cl::distinct : "") \
		              + fields_list + query::cl::from + table_name+ \
		              ((where_cond != "") ? query::cl::where + where_cond : "")+ \
		              ((!group_by.empty()) ? query::cl::group_by + group_list : "")+ \
		              ((having_cond != "") ? query::cl::having + having_cond : "")+ \
		              ((!order_by.empty()) ? query::cl::order_by + order_list + order_type : "")+ \
		              ((limit != 0) ? query::cl::limit(limit) : "") + \
		              ((offset != 0) ? query::cl::offset(offset) : "") + \
		              query::end_query;

		_sql = exec_string.c_str();

		std::cout << _sql << '\n';

		if(executeQuery(_sql, select_data, data_indexes) == EXIT_SUCCESS) {
				return select_data;
		} else{
				fprintf(stderr, "Select operation failed, no data loaded\n");
				select_data.clear();
				return select_data;
		}



}

/******************************executeQuery***********************************/
bool handler::Sqlite3Db::executeQuery(const char *sql_query, \
                                      std::vector<std::string> &data, \
                                      std::vector<int> indexes_stmt, \
                                      bool verbose){

		/* First make sure we are working with an empty vector */
		data.clear();

		/* Then SQL Command is executed if no error occurs */
		_rc = sqlite3_prepare_v2(_db, sql_query, -1, &_stmt, NULL);

		if (_rc != SQLITE_OK) {
				_zErrMsg = sqlite3_errmsg(_db);
				fprintf(stderr, "SQL error: %s\n", _zErrMsg);
				return EXIT_FAILURE;

		} else {
				/* Execute the command step by step */
				while ((_rc = sqlite3_step(_stmt)) == SQLITE_ROW) {

						/* Get the data in the positions we want from the output */
						if(!indexes_stmt.empty())
								for (int x : indexes_stmt) {

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
				_zErrMsg = sqlite3_errmsg(_db);
				fprintf(stderr, "SQL error: %s\n", _zErrMsg);
				return EXIT_FAILURE;
		}
		else {
				return EXIT_SUCCESS;
		}
		/* The command is ended */
		sqlite3_finalize(_stmt);

}

/******************************getAffinity*******************************************/
const std::string handler::Sqlite3Db::getAffinity(std::string field_datatype){

		std::string affinity = "";

		/*!
		 * \brief Lambda to convert the input string to uppercase for further processing.
		 *
		 */
		std::for_each(field_datatype.begin(), field_datatype.end(), [](char & c){
				c = ::toupper(c);
		});

		if (field_datatype.find("NULL") != std::string::npos) {
				affinity = "NULL";

		}else if (field_datatype.find(query::affinity::int_affinity) != std::string::npos) {
				affinity = query::affinity::integer;

		}else if(field_datatype.find(query::affinity::blob_affinity) != std::string::npos|| \
		         field_datatype == "") {
				affinity = query::affinity::blob;

		}else{
				for(size_t i = 0; i < query::affinity::text_affinity.size(); ++i) {
						if (field_datatype.find(query::affinity::text_affinity[i]) != std::string::npos) {
								affinity = query::affinity::text;
								break;

						}
						else if (field_datatype.find(query::affinity::real_affinity[i]) != std::string::npos) {
								affinity = query::affinity::real;
								break;

						}
				}
				if (affinity == "")
						affinity = query::affinity::numeric;
		}

		return affinity;
}

bool handler::Sqlite3Db::isAffined(const std::string affinity, const std::string value_to_check){
		bool is_affined = false;

		if (affinity == query::affinity::integer) {
				if(isValidInt(value_to_check))
						is_affined = true;

		}else if (affinity == query::affinity::real) {
				if(isValidReal(value_to_check))
						is_affined = true;

		}else if (affinity == query::affinity::numeric) {
				if(isValidInt(value_to_check) || isValidReal(value_to_check))
						is_affined = true;
		}

		return is_affined;
}


/******************************callback*************************************/

int handler::Sqlite3Db::callback(void *NotUsed, int argc, \
                                 char **argv, char **azFilName) {
		int i;
		for (i = 0; i < argc; i++) {
				printf("%s = %s\n", azFilName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");
		return 0;
}

/*************************getters and setters******************************/

std::vector<std::string> handler::Sqlite3Db::getFields(std::string table_name){
		std::vector<std::string> fields;

		for(auto field : this->_tables[table_name.c_str()]) {
				fields.push_back(field);
		}
		return fields;
};

std::vector<std::string> handler::Sqlite3Db::getTables(){
		std::vector<std::string> names;

		for(auto table : this->_tables) {
				names.push_back(table.first);
		}
		return names;
};

std::string handler::Sqlite3Db::getDbPath(){
		return this->_db_path;
};


int handler::Sqlite3Db::getTablesSize(){
		return this->_tables.size();
};

#ifndef DATABASE_H
#define DATABASE_H

#include <algorithm>
#include <iostream>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h> //strlen
#include <sys/types.h>
#include <vector>
#include <map>

//TODO: 24/10/2020 DOCSTRING for DbHandlerLite3 class


//TODO: 24/10/2020 Include descrition of arguments in methods docstrings

/*! \brief namespace containing the DbHandlerLite3 class
 *         It is used as a container for the handlers available.
 *
 *  Detailed description starts here.
 */

namespace handler {

/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 */

class DbHandlerLite3 {
public:

		DbHandlerLite3();
		DbHandlerLite3(std::string db_name);
		void closeConnection();
		~DbHandlerLite3();
		bool createTable(std::string table_name, std::pair<std::string, std::string> primary_key, \
		                 std::vector<std::pair<std::string, std::string> > columns);
		bool insertRecord(std::string table_name, std::vector<std::string> values);
		bool deleteRecords(std::string table_name, std::string condition);
		bool dropTable(std::string table_name);
		std::vector<std::string>  selectRecords(std::string table_name, \
		                                        std::vector<std::string> fields = {"*"},  \
		                                        bool select_distinct = false, \
		                                        std::string where_cond = "", std::vector<std::string> group_by = {""}, \
		                                        std::string having_cond = "", std::vector<std::string> order_by = {""}, \
		                                        std::string order_type = "ASC", \
		                                        int limit = 0, int offset = 0);
		bool executeQuery(const char *sql_query, std::vector<int> indexes_stmt, \
		                  std::vector<std::string> &data, bool verbose = false);
		static int callback(void *NotUsed, int argc, char **argv, char **azColName);


		friend std::ostream &operator<< (std::ostream &output, const DbHandlerLite3 &dbHandler){
				std::string table_name;

				output << "Handler for database "<< dbHandler._db_name << '\n';
				output << '\n' << "The following information is managed: "<< '\n';
				output << dbHandler._tables.size() << " number of tables...";

				for (auto table : dbHandler._tables) {

						table_name = table.first;
						output << "Table: " <<table_name << '\n';
						output << "With "<< table.second.size() << \
						    " number of fields, which are:"<< '\n';

						for (auto field : table.second) {
								output << field << "  ";
						}
						output << '\n'<< '\n';
				}

				return output;
		};

		std::vector<std::string> getFields(std::string table_name){
				std::vector<std::string> fields;

				for(auto field : this->_tables[table_name.c_str()]) {
						fields.push_back(field);
				}
				return fields;
		};

private:
		/* Indicator of succes or failure */
		int _rc;
		/* Path to database file */
		const char *_db_name;
		/* Pointer to database */
		sqlite3 *_db;
		/* Command to be executed in the sql */
		const char *_sql;
		/* Pointer to the sql command output */
		sqlite3_stmt *_stmt;
		/* Error Messages are stored in this variable to print them */
		char *_zErrMsg = 0;
		/* Here the name of all tables and columns of each table are stored for fast sql statement composition*/
		std::map<const std::string, std::vector<std::string> > _tables;
};

} // namespace database



#endif // DATABASE_H

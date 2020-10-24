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

//TODO: 24/10/2020 DOCSTRING for dbHandler class


//TODO: 24/10/2020 Include descrition of arguments in methods docstrings

/******************************dbHandler Class******************************
   DOCSTRING
***************************************************************************/

namespace database {

class dbHandler {
public:
		/*********************************************************************
		                       CONSTRUCTORS AND DESTRUCTOR
		*********************************************************************/
		dbHandler();
		dbHandler(std::string db_name);
		void closeConnection();
		~dbHandler();
		/*********************************************************************
		                        CREATION AND ADDITION
		*********************************************************************/
		bool createTable(std::string table_name, std::pair<std::string, std::string> primary_key, \
		                 std::vector<std::pair<std::string, std::string> > columns);

		bool insertRecord(std::string table_name, std::vector<std::string> values);

		/*********************************************************************
		                        DELETION AND REMOVING
		*********************************************************************/
		bool deleteRecords(std::string table_name, std::string condition);
		bool dropTable(std::string table_name);

		/*********************************************************************
		                          DATA OPERATIONS
		*********************************************************************/
		std::vector<std::string>  selectRecords(std::string table_name, \
		                                        std::vector<std::string> fields = {"*"},  \
		                                        bool select_distinct = false, \
		                                        std::string where_cond = "", std::vector<std::string> group_by = {""}, \
		                                        std::string having_cond = "", std::vector<std::string> order_by = {""}, \
		                                        std::string order_type = "ASC", \
		                                        int limit = 0, int offset = 0);

		/*********************************************************************
		                  CALLBACKS AND CUSTOM EXECUTION
		*********************************************************************/
		bool executeQuery(const char *sql_query, std::vector<int> indexes_stmt, \
		                  std::vector<std::string> &data, bool verbose = false);

		static int callback(void *NotUsed, int argc, char **argv, char **azColName);

		/*********************************************************************
		                            OVERLOADS
		*********************************************************************/
		friend std::ostream &operator<< (std::ostream &output, const dbHandler &database){
				std::string table_name;

				output << "Handler for database "<< database.db_name << '\n';
				output << '\n' << "The following information is managed: "<< '\n';
				output << database.tables.size() << " number of tables...";

				for (auto table : database.tables) {

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

		/*********************************************************************
		                       GETTERS (AND SETTERS)
		*********************************************************************/
		std::vector<std::string> getFields(std::string table_name){
				std::vector<std::string> fields;

				for(auto field : this->tables[table_name.c_str()]) {
						fields.push_back(field);
				}
				return fields;
		};

private:
		/* Indicator of succes or failure */
		int rc;
		/* Path to database file */
		const char *db_name;
		/* Pointer to database */
		sqlite3 *db;
		/* Command to be executed in the sql */
		const char *sql;
		/* Pointer to the sql command output */
		sqlite3_stmt *stmt;
		/* Error Messages are stored in this variable to print them */
		char *zErrMsg = 0;
		/* Here the name of all tables and columns of each table are stored for fast sql statement composition*/
		std::map<const std::string, std::vector<std::string> > tables;
};

} // namespace database



#endif // DATABASE_H

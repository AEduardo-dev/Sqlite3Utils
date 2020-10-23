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
		bool removedb();

		/*********************************************************************
		                          DATA OPERATIONS
		*********************************************************************/
		bool selectRecords();

		/*********************************************************************
		                  CALLBACKS AND CUSTOM EXECUTION
		*********************************************************************/
		bool executeQuery(const char *sql_query, std::vector<int> indexes_stmt, \
		                  std::vector<std::string> &data);

		static int callback(void *NotUsed, int argc, char **argv, char **azColName);

		/*********************************************************************
		                            OVERLOADS
		*********************************************************************/
		dbHandler operator<<(dbHandler);

		/*********************************************************************
		                       GETTERS (AND SETTERS)
		*********************************************************************/
		std::vector<std::string> getFields(std::string table_name);

private:
		/* Indicator of succes or failure */
		int rc;
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

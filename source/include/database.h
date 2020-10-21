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
#include "dbquery.h"

/******************************dbHandler Class******************************
   DOCSTRING
***************************************************************************/

namespace database {

class dbHandler {
public:
		dbHandler();
		dbHandler(std::string db_name);
		~dbHandler();
		bool createTable(std::string table_name, std::pair<std::string, std::string> primary_key, std::vector<std::pair<std::string, std::string> > columns);
		bool insertRecord(std::string table_name, std::vector<std::string> values);
		bool updatedb();
		bool deleteRecord();
		bool search();

		dbHandler operator<<(dbHandler);
		static int callback(void *NotUsed, int argc, char **argv, char **azColName);

protected:
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
		std::map<const unsigned char*, std::vector<const unsigned char*> > tables;
};

} // namespace database



#endif // DATABASE_H

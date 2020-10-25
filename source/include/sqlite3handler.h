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

//TODO: 24/10/2020 DOCSTRING for Sqlite3Db class


//TODO: 24/10/2020 Include description of arguments in methods docstrings

/*! \brief namespace containing the Sqlite3Db class
 *
 *
 */

namespace handler {

/*! \brief Class for handling connection and operations in a sqlite3 database.
 *         Brief description continued.
 *
 *  This class contains all of the basic operations available in the sqlite3
 *	syntax, including a custom executeQuery() so that the user can pass their
 *	own SQL querys and obtain the data from them if preferred over the provided
 *	functionality.
 */

class Sqlite3Db {
public:
		/*!
		 * \brief Constructor for handler using test.db
		 */
		Sqlite3Db();
		/*!
		 * \brief Constructor for user defines database name.
		 * @param db_name name of the database no be connected to.
		 */
		Sqlite3Db(std::string db_name);
		/*!
		 * \brief destructor of the class Sqlite3Db.
		 *
		 * This desctructor will close the connection to the database and
		 * then destroy the object.
		 */
		~Sqlite3Db();
		void closeConnection();

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

		/*! \brief Shows all the information available from the handler
		 *
		 *  Prints all the useful information contained in the handler fields
		 *	such as the names of tables and their fields, and the number of them.
		 *
		 *
		 */
		friend std::ostream &operator<< (std::ostream &output, const Sqlite3Db &dbHandler){
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
		int _rc;            	//!< Flag that contains the status of the latest action executed
		const char *_db_name; //!< Path to database for file operations
		sqlite3 *_db;					//!< Pointer to the database provided in the constructor
		const char *_sql; 		//!< Pointer to the latest sql query in use
		sqlite3_stmt *_stmt; 	//!< Pointer to the sql query output data
		char *_zErrMsg = 0; 	//!< Pointer to sql error message generated during the query execution
		std::map<const std::string, std::vector<std::string> > _tables; //!< Map containing the names of tables in database and their fields

};

} // namespace database



#endif // DATABASE_H

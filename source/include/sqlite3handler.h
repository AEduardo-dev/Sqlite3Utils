#ifndef SQLITE3HANDLER_H
#define SQLITE3HANDLER_H

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

typedef std::map<const std::string, std::vector<std::string> > DbTables;
typedef std::pair<std::string, std::string> FieldDescription;
/*! \brief Class for handling connection and operations in a sqlite3 database.
 *         Brief description continued.
 *
 *  This class contains all of the basic operations available in the sqlite3
 *	syntax, including a custom executeQuery(), so that the user can pass their
 *	own querys and obtain the data from them, if preferred over the provided
 *	functionality.
 */

class Sqlite3Db {
public:

		/*!
		 * \brief Constructor for handler using test.db
		 *
		 * When creating a new object, the connection to the database is attempted.
		 * If no database exists with the name provided, it is created.
		 *
		 * Assuming the database already exists and contains some information, this
		 * constructor will load all the names of the tables present in the database as
		 * map keys. Assigned to each of the keys there will be a vector loaded with the
		 * names of the fields in each table.
		 */
		Sqlite3Db();

		/*!
		 * \brief Constructor for user defines database name.
		 *
		 * @param db_name name of the database to be connected to.
		 *
		 * @overload
		 */
		Sqlite3Db(std::string db_name);

		/*!
		 * \brief Destructor of the class Sqlite3Db.
		 *
		 * This desctructor will close the connection to the database and
		 * then destroy the object.
		 */
		~Sqlite3Db();

		/*!
		 * \brief Close connection to the current database
		 *
		 * The closing of the connection also destroys the handler, since it's main
		 * purpose is to manage the action in an open database
		 */
		void closeConnection();

		/*!
		 * \brief Create a table in the database with the specified parameters.
		 *
		 * Creates a table in the database with the provided name and with the descripted
		 * fields. Each
		 *
		 * @param  table_name  Name for the table to be created.
		 * @param  fields      Vector of desriptions, each of them containing name of field and data specifications of the field corresponding field.
		 *
		 * @return EXIT_SUCCESS if correct. Otherwise EXIT_FAILURE is returned.
		 *
		 * An example of usage could be as follows:
		 *
		 * \include createTable.cpp
		 *
		 */
		bool createTable(std::string table_name, std::vector<FieldDescription> fields);

		/*!
		 * \brief Insert record data inside of a table.
		 *
		 * Given the data to insert, the method will put it inside of the specified table if
		 * the fields match.
		 *
		 * @param  table_name Name of the table where the record will be added.
		 * @param  values     Container of the values of the fields to insert. The not defined values should contain an empty string ("").
		 *
		 * @return EXIT_SUCCESS if correct. Otherwise EXIT_FAILURE is returned.
		 *
		 * An example of usage could be as follows:
		 *
		 * \include insertRecord.cpp
		 *
		 */
		bool insertRecord(std::string table_name, std::vector<std::string> values);

		/*!
		 * \brief Delete the records from a table that meet the provided condition/s.
		 *
		 * Locates and deletes the records from the table which meet a
		 * condition given. If the condition is "all", all records are deleted.
		 *
		 * @param  table_name Name of the table where the records will be deleted.
		 * @param  condition  Condition to match for the deletion.
		 *
		 * @return EXIT_SUCCESS if correct. Otherwise EXIT_FAILURE is returned.
		 *
		 * An example of usage could be as follows:
		 *
		 * \include deleteRecords.cpp
		 */
		bool deleteRecords(std::string table_name, std::string condition);

		/*!
		 * \brief Drop the table specified
		 *
		 * Drops the specified table and deletes it's content from the DbInfo contained
		 * in the Handler.
		 *
		 * @param  table_name Name of the table to be dropped.
		 *
		 * @return EXIT_SUCCESS if correct. Otherwise EXIT_FAILURE is returned.
		 *
		 * An example of usage could be as follows:
		 *
		 * \include insertRecord.cpp
		 */
		bool dropTable(std::string table_name);

		/*!
		 * [selectRecords description]
		 * @param table_name      [description]
		 * @param fields          [description]
		 * @param select_distinct [description]
		 * @param where_cond      [description]
		 * @param group_by        [description]
		 * @param having_cond     [description]
		 * @param order_by        [description]
		 * @param order_type      [description]
		 * @param limit           [description]
		 * @param offset          [description]
		 * @return
		 */
		std::vector<std::string>  selectRecords(std::string table_name, \
		                                        std::vector<std::string> fields = {"*"},  \
		                                        bool select_distinct = false, \
		                                        std::string where_cond = "", std::vector<std::string> group_by = {""}, \
		                                        std::string having_cond = "", std::vector<std::string> order_by = {""}, \
		                                        std::string order_type = "ASC", \
		                                        int limit = 0, int offset = 0);

		/*!
		 * [executeQuery description]
		 * @param  sql_query    [description]
		 * @param  indexes_stmt [description]
		 * @param  data         [description]
		 * @param  verbose      [description]
		 * @return              [description]
		 */
		bool executeQuery(const char *sql_query, std::vector<int> indexes_stmt, \
		                  std::vector<std::string> &data, bool verbose = false);

		/*!
		 * [callback description]
		 * @param  NotUsed   [description]
		 * @param  argc      [description]
		 * @param  argv      [description]
		 * @param  azColName [description]
		 * @return           [description]
		 */
		static int callback(void *NotUsed, int argc, char **argv, char **azColName);

		/*!
		 * [getFields description]
		 * @param table_name [description]
		 */
		std::vector<std::string> getFields(std::string table_name);

		friend std::ostream& operator<< (std::ostream &output, const Sqlite3Db &sqlite3Db){
				std::string table_name;

				output << "Handler for database "<< sqlite3Db._db_name << '\n';
				output << '\n' << "The following information is managed: "<< '\n';
				output << sqlite3Db._tables.size() << " number of tables...";

				for (auto table : sqlite3Db._tables) {

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


private:
		int _rc;                           /*!< Flag that contains the status of the latest action executed.*/
		const char *_db_name;              /*!< Path to database for file operations.*/
		sqlite3 *_db;                      /*!<Pointer to the database provided in the constructor.*/
		const char *_sql;                  /*!<Pointer to the latest sql query in use.*/
		sqlite3_stmt *_stmt;               /*!<Pointer to the sql query output data.*/
		char *_zErrMsg = 0;                /*!<Pointer to sql error message generated during the query execution.*/
		DbTables _tables;                  /*!<Map containing the names of tables in database and their fields.*/

};


} // namespace database



#endif // SQLITE3HANDLER_H

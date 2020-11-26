/*
 * This file is part of Sqlite3Utils.
 *
 * Sqlite3Utils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sqlite3Utils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sqlite3Utils.  If not, see <https://www.gnu.org/licenses/>.
 */




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
#include "query.hpp"


/*! \brief Contains the Sqlite3Db class and it's types
 *
 * Namespace used to contain the Sqlite3Db class and all of it's members.
 * It also uses some type definitions to make the software cleaner.
 *
 */

namespace handler {


typedef std::map<const std::string, std::vector<std::string> > DbTables;/*!< Type that stores the information of the tables inside of the db.*/
typedef std::pair<std::string, std::string> FieldDescription;/*!< For use when defining a field in the create table statement*/
static std::vector<std::string> empty_vec;/*!< Empty vector used as default value for executeQuery, when no data is extracted, where a vector for the data is needed.*/

struct select_query_param {

		std::string table_name;/*!< Name of the table in which the select query will be applied */
		std::vector<std::string> fields = {"*"};/*!< Fields to be shown in the result */
		bool select_distinct = false;/*!< Flag for showing only distinct results */
		std::string where_cond = "";/*!< Condition to be applied with a WHERE clause*/
		std::vector<std::string> group_by = {};/*!< Fields grouped in the result*/
		std::string having_cond = "";/*!< Having filter condition to be applied*/
		std::vector<std::string> order_by = {};/*!< Fields or conditions to order the results*/
		std::string order_type = "ASC";/*!< Type of ordering of the results "ASC" or "DESC"*/
		int limit = 0;/*!< Maximum number of results to be processed*/
		int offset = 0;/*!< Starting point in the results to apply the limit quantity
		*/
};/*!< Structure used for storing all options that may be used during a select query.*/

/*! \brief Class for handling connection and operations in a sqlite3 database.
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
		 * \brief Constructor for user defined database name.
		 *
		 * @param db_name name of the database to be connected to.
		 *
		 * \include constructor.cpp
		 *
		 * @overload
		 */
		Sqlite3Db(std::string db_path);

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
		 * The closing of the connection is not recommended, since the main purpose of the
		 * Handler is to manage an open database while connected to it.
		 *
		 * \include closeConnection.cpp
		 */
		void closeConnection();

		/*!
		 * \brief Reconnects the handler to it's linked database
		 *
		 * While doing the reconnection all the data inside of the handler is renewed, in case
		 *  some changes took place during the time it was offline.
		 *
		 * @return         EXIT_SUCCESS if the db was reopened and it's information loaded
		 *  							 correctly. EXIT_FAILURE otherwise.
		 *
		 *
		 * An example of usage could be as follows:
		 *
		 *
		 * \include reconnectDb.cpp
		 *
		 */
		bool connectDb ();

		/*!
		 * \brief Create a table in the database with the specified parameters.
		 *
		 * Creates a table in the database with the provided name and with the descripted
		 * fields. Each
		 *
		 * @param  table_name  Name for the table to be created.
		 * @param  fields      Vector of desriptions, each of them containing name of field and
		 *  data specifications of the field corresponding field.
		 *
		 * @return EXIT_SUCCESS if correct. Otherwise EXIT_FAILURE is returned.
		 *
		 * An example of usage could be as follows:
		 *
		 *
		 * \include createTable.cpp
		 *
		 */
		bool createTable(std::string table_name, std::vector<FieldDescription> fields);

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
		 *
		 * \include dropTable.cpp
		 */
		bool dropTable(std::string table_name);

		/*!
		 * \brief Execute an SQLite query and receive the output selected.
		 *
		 *
		 * @param  sql_query    The query to be executed.
		 *
		 * @param  indexes_stmt The indexes of the output (each index is a field of the database)
		 * that will be extracted from the result of the query.
		 *
		 * @param  data         Container to store the data retrieved from the indexes_stmt.
		 *
		 * @param  verbose      If set to true, the result of the query will also be printed
		 * through the console. Default value is false.
		 *
		 * @return              EXIT_SUCCESS if correct. Otherwise EXIT_FAILURE is returned.
		 * The data vector is passed by reference, so the content of it is changed during the
		 * query execution.
		 *
		 * An example of usage could be as follows:
		 *
		 * \include executeQuery.cpp
		 */
		bool executeQuery(const char *sql_query, std::vector<std::string> &data = empty_vec, \
		                  std::vector<int> indexes_stmt = {}, bool verbose = false);

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
		 *
		 * \include insertRecord.cpp
		 *
		 */
		bool insertRecord(std::string table_name, std::vector<std::string> values);

		/*!
		 * \brief Selects and extracts the records that meet certain conditions.
		 *
		 * Extracts all the data that fits the descriptions and conditions passed as arguments.
		 * Most of them are optional, so only the table name is necessary if no other specific
		 * parameters are needed.
		 *
		 * @param table_name      Name of the table from where the data will be selected.
		 *
		 * @param fields          Container of the names of the fields of the table that will be
		 * 												retrieved from the results in string format. Default value is
		 *  											"*" to take all the fields.
		 *
		 * @param select_distinct Boolean flag to set whether or not only unique results should
		 * 												be selected. Default value is false.
		 *
		 * @param where_cond      If set, it contains the condition to apply with a WHERE clause
		 * 												in the select query composition. It is empty by default.
		 *
		 * @param group_by        If set, it contains the condition that will be used to group the
		 * 												results of the select query. It is empty  by default.
		 *
		 * @param having_cond     If set, it contains the condition applied to the query after a
		 * 												HAVING clause. It is empty by default.
		 *
		 * @param order_by        If set, it contains the condition that will be used to order the
		 * 												results of the select query. It is empty by default.
		 *
		 * @param order_type      Only applied if the order_by argument is set. Defines the type
		 * 												of ordering to be applied. The types are "ASC" or "DESC".
		 * 												Default value is "ASC"
		 *
		 * @param limit           If set, it defines the number of results that will be extracted
		 * 												from the select query data. Default value is 0 for no limit.
		 *
		 * @param offset          If set, it defines the number of results that will be skipped
		 * 												from the select query data before extracting them. Default value
		 * 												is 0 for none.
		 *
		 * @return 								A vector containing all the values retrieved in order. This means
		 * 												that, if three fuelds were given as argument, each group of three
		 * 												elements of this vector will correspond to a row of data.
		 *
		 * /include selectRecords.cpp
		 */
		std::vector<std::string>  selectRecords(std::string table_name, \
		                                        std::vector<std::string> fields = {"*"},  \
		                                        bool select_distinct = false, \
		                                        std::string where_cond = "", std::vector<std::string> group_by = {}, \
		                                        std::string having_cond = "", std::vector<std::string> order_by = {}, \
		                                        std::string order_type = "ASC", \
		                                        int limit = 0, int offset = 0);

		/*!
		 * \brief Selects and extracts the records that meet certain conditions.
		 *
		 * @param select_options Structure containing all the necessary options to be used during
		 * 											 the select statement.
		 *
		 * @overload
		 *
		 * /include selectRecordsStruct.cpp
		 */
		std::vector<std::string>  selectRecords(select_query_param select_options);

		/*!
		 * \brief Updates the information contained in the handler.
		 *
		 * If multiple connections are being used in the same database, it may be possible that a
		 *  table is changed, or a new one created, without the current hanlder knowing it. For
		 *  this purpose, execution of this method will updated all the tables and field information
		 *  that is stored in the handler, so it can operate normally from different connections.
		 *
		 * @return	EXIT_SUCCESS if the information was updated. EXIT_FAILURE if an error occurred
		 *  				during the process.
		 *
		 * An example of usage could be as follows, with two different programs working on same db:
		 *
		 * \include updateHandler1.cpp
		 *
		 * \include updateHandler2.cpp
		 */
		bool updateHandler();

		/*!
		 * \brief Updates the information contained on a table using a condition given.
		 *
		 * Method used to set the value of certain field/s of a table whenever the condition
		 * given is met.
		 *
		 * @param  table_name Table where the update operation will take place.
		 * @param  set_fields Container of pairs which have, as a first element, the name of a
		 * field at the table <table_name>, and as the second element, the value it should take
		 * when the condition is met.
		 * @param  where_cond Condition causing the values of the fields given to change to the
		 * updated value.
		 * @return            EXIT_SUCCESS if the operation completed succesfully, EXIT_FAILURE
		 *  otherwise
		 */
		bool updateTable(std::string table_name, std::vector<FieldDescription> set_fields,\
			 								std::string where_cond = "");

		/*!
		 * \brief Calculate the affinity token corresponding to a datatype given.
		 *
		 * Using the data affinities defined in the SQLite3 documentation, and with the rules
		 *  specified to calculate them, this method will return the affinity according to the
		 *  datatype it receives.
		 *
		 * @param  field_datatype The datatype for which the affinity token will be calculated.
		 *
		 * @return								Affinity values "INTEGER", "REAL", "TEXT", "BLOB" or "NUMERIC",
		 * 												depending on the input.
		 */
		const std::string getAffinity(const std::string field_datatype);

		/*!
		 * \brief Get field's names from a table in the database.
		 *
		 * @param table_name The name of the table which field's names are needed.
		 *
		 * @return A vector containing the names of the fields of the table.
		 */
		std::vector<std::string> getFields(std::string table_name);

		/*!
		 * \brief Get number of tables in the database.
		 *
		 * @return The number of tables.
		 */
		size_t getNumTables();

		/*!
		 * \brief Get tables information map stored in the handler.
		 *
		 * @return A vector containing the names of the tables.
		 */
		DbTables getTables();

		/*!
		 * \brief Get table's names from the database.
		 *
		 * @return A vector containing the names of the tables.
		 */
		std::vector<std::string> getTablesNames();

		/*!
		 * \brief Gets db relative path from the database.
		 *
		 * @return A vector containing the names of the tables.
		 */
		std::string getDbPath();


		friend std::ostream& operator<< (std::ostream &output, const Sqlite3Db &sqlite3Db){
				std::string table_name;

				output << "Handler for database "<< sqlite3Db._db_path << '\n';
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

		/*!
		 * \brief Callback to show the output of sqlite3_exec()
		 *
		 * When not using the executeQuery() method, it may be interesting to use the
		 *  sqlite3_exec() instead, and so, a callback to see the output of the operation is
		 *  higly recommended.
		 *
		 * @param  NotUsed   Not used.
		 * @param  argc      Number of arguments.
		 * @param  argv      Values of the arguments.
		 * @param  azColName Name of the column in the database.
		 *
		 */
		static int callback(void *NotUsed, int argc, char **argv, char **azColName);

		/*!
		 * \brief Compares the value of some field to it's corresponding affinity.
		 *
		 * The value of the data in a field is compared against it's affinity. The comparison
		 *  determines if the data given is valid for that field or not.
		 *
		 * It is important to notice that only "INTEGER", "REAL" and "NUMERIC" affinities need
		 * this validation. The "TEXT" or "BLOB" affinities can contain numbers, but numbers cannot
		 * contains characters inside of them, except from "," or "." for decimal depending on the
		 * country.
		 *
		 * @param  affinity The affinity token of the corresponding field to be checked.
		 * @param  value_to_check The value that needs to be validated before an operation.
		 *
		 * @return  True if the value is valid for the field given, false otherwise.
		 */
		bool isAffined(const std::string affinity, const std::string value_to_check);

		/*!
		 * \brief Get status of the handler's connection
		 *
		 * Used to check the conection status of the handler, it will print whether there is a
		 * satabase linked to it or not, and will return a value depending on it.
		 *
		 * @return True if the handler is connected to a database. False otherwise.
		 */
		bool isConnected();

		/*!
		 * \brief Check if a character is valid for being inside of a real number
		 *
		 * @param  c The character to be checked.
		 *
		 * @return   True if the character is not alphabetic or a space. False otherwise.
		 */
		static inline bool isReal(char c)
		{
				return !(isdigit(c) || (c == '.') || (c == ','));
		};

		/*!
		 * \brief Check if a character is valid for being inside of an integer number
		 *
		 * @param  c The character to be checked.
		 *
		 * @return   True if the character is not alphabetic, a space, or a comma or dot.
		 *  					False otherwise.
		 */
		static inline bool isInt(char c)
		{
				return (!isdigit(c));
		};

		/*!
		 * \brief Checks if a string is a valid integer number.
		 *
		 * @param  str String to be checked.
		 *
		 * @return     True if the string is a valid integer. False otherwise.
		 */
		bool isValidInt(const std::string &str)
		{
				if(!str.empty())
						return (find_if(str.begin(), str.end(), isInt) == str.end());
				else
						return false;
		};

		/*!
		 * \brief Checks if a string is a valid real number.
		 *
		 * @param  str String to be checked.
		 *
		 * @return     True if the string is a valid real (float, double...). False otherwise.
		 */
		bool isValidReal(const std::string &str)
		{
				if(!str.empty())
						return (find_if(str.begin(), str.end(), isReal) == str.end());
				else
						return false;
		};

private:
		int _rc;/*!< Flag that contains the status of the latest action executed.*/
		std::string _db_name;/*!< Relative path to database for file operations in string format.*/
		const char *_db_path;/*!< Relative path to database for file operations.*/
		sqlite3 *_db;/*!< Pointer to the database provided in the constructor.*/
		const char *_sql;/*!< Pointer to the latest sql query in use.*/
		sqlite3_stmt *_stmt;/*!< Pointer to the sql query output data.*/
		const char *_zErrMsg = 0;/*!< Pointer to sql error message generated during the query execution.*/
		DbTables _tables;/*!< Map containing the names of tables in database and their fields.*/

};


} // namespace database



#endif // SQLITE3HANDLER_H

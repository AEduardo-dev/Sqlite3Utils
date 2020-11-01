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


/*! \brief namespace containing the Sqlite3Db class
 *
 * Namespace used to contain the Sqlite3Db class and all of it's members.
 * It also uses some type definitions to make the software cleaner.
 *
 */

namespace handler {

typedef std::map<const std::string, std::vector<std::string> > DbTables;
typedef std::pair<std::string, std::string> FieldDescription;
static  std::vector<std::string> empty_vec;
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
		 * \brief Constructor for user defined database name.
		 *
		 * @param db_name name of the database to be connected to.
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
		 * \include insertRecord.cpp
		 */
		bool dropTable(std::string table_name);

		/*!
		 * \brief Execute an SQLite query and receive the output selected.
		 *
		 *
		 * @param  sql_query    The query to be executed.
		 * @param  indexes_stmt The indexes of the output (each index is a field of the database)
		 * that will be extracted from the result of the query.
		 * @param  data         Container to store the data retrieved from the indexes_stmt.
		 * @param  verbose      If set to true, the result of the query will also be printed
		 * through the console. Default value is false.
		 *
		 * @return              EXIT_SUCCESS if correct. Otherwise EXIT_FAILURE is returned.
		 * The data vector is passed by reference, so the content of it is changed during the
		 * query execution.
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
		 * @param fields          Container of the names of the fields of the table that will be retrieved from the results in string format. Default value is "*" to take all the fields.
		 * @param select_distinct Boolean flag to set whether or not only unique results should
		 * be selected. Default value is false.
		 * @param where_cond      If set, it contains the condition to apply with a WHERE clause in the select query composition. It is empty by default.
		 * @param group_by        If set, it contains the condition that will be used to group the results of the select query. It is empty by default.
		 * @param having_cond     If set, it contains the condition applied to the query after a HAVING clause. It is empty by default.
		 * @param order_by        If set, it contains the condition that will be used to order the results of the select query. It is empty by default.
		 * @param order_type      Only applied if the order_by argument is set. Defines the type
		 * of ordering to be applied. The types are "ASC" or "DES". Default value is "ASC"
		 * @param limit           If set, it defines the number of results that will be extracted
		 * from the select query data. Default value is 0 for no limit.
		 * @param offset          If set, it defines the number of results that will be skipped
		 * from the select query data before extracting them. Default value is 0 for none.
		 *
		 * @return A vector containing all the values retrieved in order. This means that, if three
		 * fuelds were given as argument, each group of three elements of this vector will
		 * correspond to a row of data.
		 */
		std::vector<std::string>  selectRecords(std::string table_name, \
		                                        std::vector<std::string> fields = {"*"},  \
		                                        bool select_distinct = false, \
		                                        std::string where_cond = "", std::vector<std::string> group_by = {""}, \
		                                        std::string having_cond = "", std::vector<std::string> order_by = {""}, \
		                                        std::string order_type = "ASC", \
		                                        int limit = 0, int offset = 0);

		/*!
     * [getAffinity description]
     * @param  field_datatype [description]
     * @param  value_to_check [description]
     * @return                [description]
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
		 *
		 * @return The number of tables.
		 */
		int getTablesSize();

		/*!
		 * \brief Get table's names from the database.
		 *
		 * @param table_name The name of the table which field's names are needed.
		 *
		 * @return A vector containing the names of the tables.
		 */
		std::vector<std::string> getTables();

		/*!
		 * \brief Gets db relative path from the database.
		 *
		 * @param table_name The name of the table which field's names are needed.
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
     * [checkFieldAffinity description]
     * @param  field_datatype [description]
     * @param  value_to_check [description]
     * @return                [description]
     */
    bool isAffined(const std::string affinity, const std::string value_to_check);

		/*!
		 * [IsNotAplhaReal description]
		 * @param  c [description]
		 * @return   [description]
		 */
		static inline bool isNotAplhaReal(char c)
		{
				return (isalpha(c) || (c == ' ') || (c == ',') || (c == '.'));
		};

		/*!
		 * [IsNotAplhaInt description]
		 * @param  c [description]
		 * @return   [description]
		 */
		static inline bool isNotAplhaInt(char c)
		{
				return (isalpha(c) || (c == ' ') || (c == ',') || (c == '.'));
		};

		/*!
		 * [IsValidInt description]
		 * @param  str [description]
		 * @return     [description]
		 */
		bool isValidInt(const std::string &str)
		{
				return find_if(str.begin(), str.end(), isNotAplhaInt) == str.end();
		};

		/*!
		 * [IsValidReal description]
		 * @param  str [description]
		 * @return     [description]
		 */
		bool isValidReal(const std::string &str)
		{
				return find_if(str.begin(), str.end(), isNotAplhaReal) == str.end();
		};

private:
		int _rc;            /*!< Flag that contains the status of the latest action executed.*/
		const char *_db_path;            /*!< Realtive path to database for file operations.*/
		sqlite3 *_db;            /*!< Pointer to the database provided in the constructor.*/
		const char *_sql;            /*!< Pointer to the latest sql query in use.*/
		sqlite3_stmt *_stmt;            /*!< Pointer to the sql query output data.*/
		const char *_zErrMsg = 0;            /*!<Pointer to sql error message generated during the query execution.*/
		DbTables _tables;            /*!< Map containing the names of tables in database and their fields.*/

};


} // namespace database



#endif // SQLITE3HANDLER_H

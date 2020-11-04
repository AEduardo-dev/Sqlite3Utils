#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sqlite3.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include "../../source/include/sqlite3handler.hpp"
#include "../../source/include/sqlite3query.hpp"

/*!
 * \brief Checks if the file given exists in the directory
 *
 * @param  name The name of the file to search.
 * @return      True if the file exists, false otherwise.
 */
inline bool exists(const std::string& name) {
		struct stat buffer;
		return (stat (name.c_str(), &buffer) == 0);
}

/* Global handler for use in multiple tests */
handler::Sqlite3Db UserHandler("MyDB.db");

/* Main table used during tests */
std::string table_name = "CONNECTIONS";

/* Declaration of the table's fields */
std::vector<handler::FieldDescription> table_definition = \
{{"ID",query::data::int_+query::data::primary_key+query::data::not_null}, \
		{"AGE", query::data::int_+query::data::not_null}, \
		{"PHONE", query::data::int_+query::data::null}, \
		{"NAME",query::data::char_+query::data::len(50)+query::data::not_null}};
/* Wrong declaration for failure tests */
std::vector<handler::FieldDescription> table_definition_wrong = \
{{"ID",query::data::int_+query::data::primary_key+query::data::not_null}, \
		{"AGE", query::data::int_+query::data::not_null+"k"}, \
		{"PHONE", query::data::int_+query::data::null}, \
		{"NAME",query::data::char_+query::data::len(50)+query::data::not_null}};

/* Vectors for comparison in query results */
std::vector<std::string> names_to_check = {"ID", "AGE", "PHONE", "NAME"};
std::vector<std::string> data_to_check = {"1", "32", "665", "ANTHON33", \
		                                      "2", "43", "Julia", \
		                                      "3", "23", "Edu"};

/********************CONSTRUCTOR AND DESTRUCTOR TESTS************************/
/* Check if db is created if it does not exist */
TEST(SqliteHandlerConstructor, CreateDatabase){
		EXPECT_FALSE(exists("test.db"));
		handler::Sqlite3Db testHandler;

		ASSERT_TRUE(exists("test.db"));
}

/* Check if multiple handlers can be connected to the same db */
TEST(SqliteHandlerConstructor, MultiConnectDB){
		EXPECT_TRUE(exists("test.db"));
		handler::Sqlite3Db firstConnection;
		handler::Sqlite3Db secondConnection;

		ASSERT_NO_FATAL_FAILURE(handler::Sqlite3Db thirdConnection);
}

/* If no tables exist the database should be loaded but no table will be in the handler */
TEST(SqliteHandlerConstructor, LoadEmptyDatabase){
		ASSERT_NO_THROW(handler::Sqlite3Db UserHandler("MyDB.db"));
		ASSERT_EQ(UserHandler.getNumTables(), 0);
}

/*******************CREATE TABLE FUNCTION*********************************/
/* Create a table normally */
TEST(SqliteCreateTable, CreateTable){
		ASSERT_EQ(UserHandler.createTable(table_name, table_definition), EXIT_SUCCESS);
		ASSERT_EQ(UserHandler.getNumTables(), 1);
		ASSERT_EQ(UserHandler.getFields(table_name), names_to_check);
}

/* Create a table with a typo in it's definition */
TEST(SqliteCreateTable, CreateTableWrongDefinition){
		ASSERT_EQ(UserHandler.createTable(table_name, table_definition_wrong), EXIT_FAILURE);
		ASSERT_EQ(UserHandler.getNumTables(), 1);
		ASSERT_EQ(UserHandler.getFields(table_name), names_to_check);
}

/**********************AFFINITY OPERATIONS*****************************/
/* Get the affinity of an integer number datatype and check a correct value against it */
TEST(SqliteAffinity, CheckIntAffinityUpper){
		ASSERT_EQ(UserHandler.getAffinity("MEDINT"), "INTEGER");
		ASSERT_TRUE(UserHandler.isAffined("INTEGER", "3386"));
}

TEST(SqliteAffinity, CheckIntAffinityLower){
		ASSERT_EQ(UserHandler.getAffinity("medint"), "INTEGER");
		ASSERT_TRUE(UserHandler.isAffined("INTEGER", "3386"));
}

/* Get the affinity of a non integer datatype and check if no false integer is generated */
TEST(SqliteAffinity, CheckIntWrongAffinity){
		ASSERT_NE(UserHandler.getAffinity("REAL"), "INTEGER");
}

/* Get the affinity of an integer number datatype and check a wrong value against it */
TEST(SqliteAffinity, CheckIntAffinityWrongValue){
		ASSERT_EQ(UserHandler.getAffinity("MEDINT"), "INTEGER");
		ASSERT_FALSE(UserHandler.isAffined("INTEGER", "33,86"));
}

/* Get the affinity of a real number datatype and check a correct value against it (comma)*/
TEST(SqliteAffinity, CheckRealAffinityComma){
		ASSERT_EQ(UserHandler.getAffinity("LONG DOUBLE"), "REAL");
		ASSERT_TRUE(UserHandler.isAffined("REAL", "33,86"));
}

/* Get the affinity of a real number datatype and check a correct value against it (dot)*/
TEST(SqliteAffinity, CheckRealAffinityDot){
		ASSERT_EQ(UserHandler.getAffinity("Short double"), "REAL");
		ASSERT_TRUE(UserHandler.isAffined("REAL", "33.86"));
}

/* Get the affinity of a non real datatype and check if no false real affinity is generated */
TEST(SqliteAffinity, CheckRealWrongAffinity){
		ASSERT_NE(UserHandler.getAffinity("LONG INT"), "REAL");
}

/* Get the affinity of a real number datatype and check a wrong value against it */
TEST(SqliteAffinity, CheckRealAffinityWrongValue){
		ASSERT_EQ(UserHandler.getAffinity("LONG DOUBLE"), "REAL");
		ASSERT_FALSE(UserHandler.isAffined("REAL", "33a86"));
}

/* Get the affinity of a text datatype */
TEST(SqliteAffinity, CheckTextAffinityUpper){
		ASSERT_EQ(UserHandler.getAffinity("CHAR (90)"), "TEXT");
}

TEST(SqliteAffinity, CheckTextAffinityLower){
		ASSERT_EQ(UserHandler.getAffinity("char(90)"), "TEXT");
}

/* Get the affinity of a real datatype and check no false text is received*/
TEST(SqliteAffinity, CheckTextWrongAffinity){
		ASSERT_NE(UserHandler.getAffinity("REAL"), "TEXT");
}

/* Get the affinity of a blob datatype */
TEST(SqliteAffinity, CheckBlobAffinityUpper){
		ASSERT_EQ(UserHandler.getAffinity("BLOB"), "BLOB");
}

TEST(SqliteAffinity, CheckBlobAffinityLower){
		ASSERT_EQ(UserHandler.getAffinity("blob"), "BLOB");
}

TEST(SqliteAffinity, CheckBlobAffinityNothing){
		ASSERT_EQ(UserHandler.getAffinity(""), "BLOB");
}

/* Get the affinity of a real datatype and check no false blob is received*/
TEST(SqliteAffinity, CheckBlobWrongAffinity){
		ASSERT_NE(UserHandler.getAffinity("Text"), "BLOB");
}

/*******************INSERT RECORDS FUNCTION******************************/
/* Correct insertion statement with all fields given */
TEST(SqliteInsertion, InsertCompleteCorrect){
		EXPECT_TRUE(UserHandler.getNumTables() == 1);

		std::vector<std::string> values_to_insert = {"1", "32", "665", "ANTHON33"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);
}

/* Correct insertion statement with some fields given. Respecting the not null constraints */
TEST(SqliteInsertion, InsertIncompleteCorrect){
		EXPECT_TRUE(UserHandler.getNumTables() == 1);

		std::vector<std::string> values_to_insert = {"2", "43", "", "Julia"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);
}

/* Incorrect insertion statement with some fields given. No respecting the not null constraints */
TEST(SqliteInsertion, InsertIncompleteIncorrect){
		EXPECT_TRUE(UserHandler.getNumTables() == 1);

		std::vector<std::string> values_to_insert = {"4", "", "", "Robert"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_FAILURE);
}

/* Incorrect insertion statement giving a non existent table name */
TEST(SqliteInsertion, InsertNoTable){
		EXPECT_TRUE(UserHandler.getNumTables() == 1);

		std::vector<std::string> values_to_insert = {"5", "32", "665", "BRUNO"};
		ASSERT_EQ(UserHandler.insertRecord("CONECTIONS", values_to_insert), EXIT_FAILURE);
}

/* Incorrect insertion statement giving incorrect datatype to a field */
TEST(SqliteInsertion, InsertIncorrectType){
		EXPECT_TRUE(UserHandler.getNumTables() == 1);

		std::vector<std::string> values_to_insert = {"Hello", "32", "435", "Albert"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_FAILURE);
}

/*******************DISCONNECTION AND CONNECTIONS**************************/
/* Disconnecting from the db causes no exceptions or errors */
TEST(SqliteConnection, DisconnectDB){
		handler::Sqlite3Db MyHandler("MyDB.db");
		ASSERT_NO_THROW(MyHandler.closeConnection());
}

/* Reconnecting to the db is possible and loads info */
TEST(SqliteConnection, ReconnectDB){
		handler::Sqlite3Db MyHandler("MyDB.db");
		ASSERT_NO_THROW(MyHandler.closeConnection());
		ASSERT_EQ(MyHandler.reconnectDb(), EXIT_SUCCESS);
		ASSERT_GT(MyHandler.getNumTables(), 0);
		ASSERT_EQ(MyHandler.getTables()[0], table_name);
}

/*********************OPERATIONS ON LOADED DB***************************/
/* Hanlder declarations loads all the information inside of the db (tables and field in the map) */
TEST(SqliteLoadedDb, LoadDatabase){
		EXPECT_TRUE(exists("MyDB.db"));
		handler::Sqlite3Db LoaderHandler("MyDB.db");

		ASSERT_GT(LoaderHandler.getNumTables(), 0);
		ASSERT_EQ(LoaderHandler.getTables()[0], table_name);
}

/* Check if the hanlder is able to operate the db just from the data loaded at initialization */
TEST(SqliteLoadedDb, InsertAfterLoad){
		EXPECT_TRUE(exists("MyDB.db"));
		handler::Sqlite3Db LoaderHandler("MyDB.db");
		EXPECT_GT(LoaderHandler.getNumTables(), 0);

		std::vector<std::string> values_to_insert = {"3", "23", "", "Edu"};
		ASSERT_EQ(LoaderHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);
}

/**************************SELECT OPERATIONS***********************/
/* Select all records inside of a table correctly */
TEST(SqliteSelectRecords, SelectAll){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);

		std::vector<std::string> data = UserHandler.selectRecords(table_name);
		ASSERT_EQ(data, data_to_check);
}

/* Select all records of a non existent table */
TEST(SqliteSelectRecords, SelectAllNoTable){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);

		ASSERT_TRUE(UserHandler.selectRecords("Conections").empty());
}

/* Select specific field from all records */
TEST(SqliteSelectRecords, SelectSpecific){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);

		std::vector<std::string> data = UserHandler.selectRecords(table_name, {table_definition[2].first});
		ASSERT_EQ(data.size(), 1);
		ASSERT_EQ(data[0], "665");
}

/* Select non existent specific field from all records of existent table */
TEST(SqliteSelectRecords, SelectSpecificNoField){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);

		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SURNAME"});
		ASSERT_TRUE(data.empty());
}

/* Select specific field from all records seeing only distinct results */
TEST(SqliteSelectRecords, SelectSpecificDistinct){

		/* Check the names in the records */
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> data_before = UserHandler.selectRecords(table_name, {"NAME"});

		/* The name already exists */
		ASSERT_EQ(data_before[1], "Julia");

		/* Insert duplicate of name */
		std::vector<std::string> values_to_insert = {"5", "999", "", "Julia"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);

		/* Select duplicate as well */
		std::vector<std::string> data_duplicates = UserHandler.selectRecords(table_name, {"NAME"}, false);
		ASSERT_FALSE(data_duplicates.empty());
		std::vector<std::string> names_duplicates = {"ANTHON33", "Julia", "Edu", "Julia"};
		ASSERT_EQ(data_duplicates, names_duplicates);

		/* Select only distincts */
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"NAME"}, true);
		ASSERT_FALSE(data.empty());
		std::vector<std::string> names = {"ANTHON33", "Julia", "Edu"};
		ASSERT_EQ(data, names);
}

/* Select all records from table with where condition for records selection */
TEST(SqliteSelectRecords, SelectAllWhere){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::string where_cond = "AGE > 100";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, where_cond);
		ASSERT_FALSE(data.empty());
		std::vector<std::string> record_to_extract = {"5", "999", "Julia"};
		ASSERT_EQ(data, record_to_extract);
}

/* Select all records from table with where condition for records selection */
TEST(SqliteSelectRecords, SelectSpecificWhere){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::string where_cond = "AGE > 100";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"AGE"}, \
		                                                          false, where_cond);
		ASSERT_FALSE(data.empty());
		std::vector<std::string> record_to_extract = {"999"};
		ASSERT_EQ(data, record_to_extract);
}

/* Select all records from table with wrong where condition for records selection */
TEST(SqliteSelectRecords, SelectAllWhereWrong){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::string where_cond = "AGEe > 100";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"AGE"}, \
		                                                          false, where_cond);
		ASSERT_TRUE(data.empty());
}

/* Select all records from table with where condition that selects nothing */
TEST(SqliteSelectRecords, SelectAllWhereNone){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::string where_cond = "AGE > 1000";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"AGE"}, \
		                                                          false, where_cond);
		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping by and using sum for select */
TEST(SqliteSelectRecords, SelectSpecificGroupBy){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SUM(AGE)", "NAME"}, \
		                                                          false, "", group_by);

		std::vector<std::string> data_to_get = {"32", "ANTHON33", "23", "Edu", "1042", "Julia"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping by a non existent field */
TEST(SqliteSelectRecords, SelectSpecificGroupByWrongField){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAMEe"};
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SUM(AGE)", "NAME"}, \
		                                                          false, "", group_by);

		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping by a field and ordering by the sum of a field*/
TEST(SqliteSelectRecords, SelectSpecificOrderByAscendentOrder){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string order_type = "ASC";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SUM(AGE)", "NAME"}, \
		                                                          false, "", group_by, "", \
		                                                          order_by, order_type);

		std::vector<std::string> data_to_get = {"23", "Edu", "32", "ANTHON33", "1042", "Julia"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping by a field and ordering by the sum of a field*/
TEST(SqliteSelectRecords, SelectSpecificOrderByDescendentOrder){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string order_type = "DESC";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SUM(AGE)", "NAME"}, \
		                                                          false, "", group_by, "", \
		                                                          order_by, order_type);

		std::vector<std::string> data_to_get = { "1042", "Julia", "32", "ANTHON33", "23", "Edu"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping by a field and ordering by a wrong field*/
TEST(SqliteSelectRecords, SelectSpecificOrderByWrongField){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGEe)"};
		std::string order_type = "DESC";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SUM(AGE)", "NAME"}, \
		                                                          false, "", group_by, "", \
		                                                          order_by, order_type);
		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping by a field and ordering by a wrong field*/
TEST(SqliteSelectRecords, SelectSpecificOrderByWrongOrderType){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string order_type = "DES";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SUM(AGE)", "NAME"}, \
		                                                          false, "", group_by, "", \
		                                                          order_by, order_type);
		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping and ordering using having for filter*/
TEST(SqliteSelectRecords, SelectSpecificHaving){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 2";
		std::string order_type = "DESC";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type);

		std::vector<std::string> data_to_get = { "1", "32", "665","ANTHON33", "3", "23", "Edu"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping and ordering using having to filter getting no results*/
TEST(SqliteSelectRecords, SelectSpecificHavingNone){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 1";
		std::string order_type = "DESC";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type);
		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping and ordering using a wrong having condition */
TEST(SqliteSelectRecords, SelectSpecificHavingWrong){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(namee) < 2";
		std::string order_type = "DESC";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type);
		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping and ordering using having and showing only 1 result */
TEST(SqliteSelectRecords, SelectSpecificLimit){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 2";
		std::string order_type = "DESC";
		int limit = 1;
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type, limit);

		std::vector<std::string> data_to_get = { "1", "32", "665","ANTHON33"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping and ordering using having and limiting to one with offset to show from second */
TEST(SqliteSelectRecords, SelectSpecificLimitOffset){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 2";
		std::string order_type = "DESC";
		int limit = 1;
		int offset = 1;
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type, limit, offset);

		std::vector<std::string> data_to_get = {"3", "23", "Edu"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping and ordering using having and limiting results to bigger number than actual number of results */
TEST(SqliteSelectRecords, SelectSpecificBiggerLimit){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 2";
		std::string order_type = "DESC";
		int limit = 10;
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type, limit);

		std::vector<std::string> data_to_get = { "1", "32", "665","ANTHON33", "3", "23", "Edu"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping and ordering using having and limiting results to bigger number than actual number of results */
TEST(SqliteSelectRecords, SelectSpecificBiggerOffset){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 2";
		std::string order_type = "DESC";
		int limit = 10;
		int offset = 10;
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type, limit, offset);

		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping and ordering using having and limiting results to bigger number than actual number of results */
TEST(SqliteSelectRecords, SelectStructSpecificAllConditions){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		handler::select_query_param select_query;
		select_query.table_name = table_name;
		select_query.fields = {"*"};
		select_query.group_by = {"NAME"};
		select_query.order_by = {"SUM(AGE)"};
		select_query.having_cond = "count(name) < 2";
		select_query.order_type = "DESC";
		select_query.limit = 10;
		std::vector<std::string> data = UserHandler.selectRecords(select_query);

		std::vector<std::string> data_to_get = { "1", "32", "665","ANTHON33", "3", "23", "Edu"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping and ordering using having and limiting results to bigger number than actual number of results */
TEST(SqliteSelectRecords, SelectStructSpecificAllConditionsNoResult){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		handler::select_query_param select_query;
		select_query.table_name = table_name;
		select_query.fields = {"*"};
		select_query.group_by = {"NAME"};
		select_query.order_by = {"SUM(AGE)"};
		select_query.having_cond = "count(name) < 2";
		select_query.order_type = "DESC";
		select_query.limit = 10;
		select_query.offset = 10;
		std::vector<std::string> data = UserHandler.selectRecords(select_query);

		ASSERT_TRUE(data.empty());
}


/**************************DELETE AND DROP OPERATIONS***********************/
/* Delete specific records of a table using condition */
TEST(SqliteDelete, DeleteRecord){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		ASSERT_EQ(UserHandler.deleteRecords(table_name, "ID > 7"), EXIT_SUCCESS);
}

/* Delete all records in a table */
TEST(SqliteDelete, DeleteAllRecords){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		ASSERT_EQ(UserHandler.deleteRecords(table_name, "all"), EXIT_SUCCESS);
}

/* Try to delete records of non existent table */
TEST(SqliteDelete, DeleteWrongTable){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		ASSERT_EQ(UserHandler.deleteRecords("CONECTIONS", "all"), EXIT_FAILURE);
}

/* Try to delete records with non possible condition */
TEST(SqliteDelete, DeleteWrongCondition){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		ASSERT_EQ(UserHandler.deleteRecords(table_name, "UD == 4"), EXIT_FAILURE);
}

/* Drop a table from the db */
TEST(SqliteDropTable, DropTable){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		ASSERT_EQ(UserHandler.dropTable(table_name), EXIT_SUCCESS);
}

/* Try to drop a table from the db that does not exist */
TEST(SqliteDropTable, DropNonexistentTable){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		ASSERT_EQ(UserHandler.dropTable("CONECTIONS"), EXIT_FAILURE);
}

/****************UPDATE AND MULTICONNECTION OPERATIONS*************/
//TODO: 3/11/2020 Angel Include tests for multiconnection and updating handler info.
/* Try to drop a table from the db that does not exist */
TEST(SqliteUpdateHandler, UpdateHandler){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);

		handler::Sqlite3Db NewHandler("MyDB.db");
		ASSERT_EQ(UserHandler.getNumTables(), NewHandler.getNumTables());

		std::vector<handler::FieldDescription> fields = \
		{{"NAME",query::data::char_+query::data::primary_key+query::data::not_null}, \
				{"SURNAME",query::data::char_+query::data::len(50)+query::data::not_null}};

		ASSERT_EQ(UserHandler.createTable("NEWTABLE", fields), EXIT_SUCCESS);
		std::vector<std::string> values_to_insert = {"Angel Eduardo", "Vega"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);

		ASSERT_NE(UserHandler.getNumTables(), NewHandler.getNumTables());
		ASSERT_GT(UserHandler.getNumTables(), NewHandler.getNumTables());

		ASSERT_EQ(NewHandler.updateHandler(), EXIT_SUCCESS);
		ASSERT_EQ(NewHandler.getNumTables(), UserHandler.getNumTables());
}


int main(int argc, char *argv[]) {
		testing::InitGoogleTest(&argc, argv);
		return RUN_ALL_TESTS();
}

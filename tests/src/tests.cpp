#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sqlite3.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include "../../source/include/sqlite3handler.hpp"
#include "../../source/include/dbquery.hpp"

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
		ASSERT_EQ(UserHandler.getTablesSize(), 0);
}

/*******************DISCONNECTION AND CONNECTIONS**************************/
/* Disconnecting from the db causes no exceptions or errors */
TEST(SqliteConnection, DisconnectDB){
		handler::Sqlite3Db MyHandler("MyDB.db");
		ASSERT_NO_THROW(MyHandler.closeConnection());
}

/*******************CREATE TABLE FUNCTION*********************************/
/* Create a table normally */
TEST(SqliteCreateTable, CreateTable){
		ASSERT_EQ(UserHandler.createTable(table_name, table_definition), EXIT_SUCCESS);
		ASSERT_EQ(UserHandler.getTablesSize(), 1);
		ASSERT_EQ(UserHandler.getFields(table_name), names_to_check);
}
/* Create a table with a typo in it's definition */
TEST(SqliteCreateTable, CreateTableWrongDefinition){
		ASSERT_EQ(UserHandler.createTable(table_name, table_definition_wrong), EXIT_FAILURE);
		ASSERT_EQ(UserHandler.getTablesSize(), 1);
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
		EXPECT_TRUE(UserHandler.getTablesSize() == 1);
		std::vector<std::string> values_to_insert = {"1", "32", "665", "ANTHON33"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);
}
/* Correct insertion statement with some fields given. Respecting the not null constraints */
TEST(SqliteInsertion, InsertIncompleteCorrect){
		EXPECT_TRUE(UserHandler.getTablesSize() == 1);
		std::vector<std::string> values_to_insert = {"2", "43", "", "Julia"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);
}
/* Incorrect insertion statement with some fields given. No respecting the not null constraints */
TEST(SqliteInsertion, InsertIncompleteIncorrect){
		EXPECT_TRUE(UserHandler.getTablesSize() == 1);
		std::vector<std::string> values_to_insert = {"4", "", "", "Robert"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_FAILURE);
}
/* Incorrect insertion statement giving a non existent table name */
TEST(SqliteInsertion, InsertNoTable){
		EXPECT_TRUE(UserHandler.getTablesSize() == 1);
		std::vector<std::string> values_to_insert = {"5", "32", "665", "BRUNO"};
		ASSERT_EQ(UserHandler.insertRecord("CONECTIONS", values_to_insert), EXIT_FAILURE);
}
/* Incorrect insertion statement giving incorrect datatype to a field */
TEST(SqliteInsertion, InsertIncorrectType){
		EXPECT_TRUE(UserHandler.getTablesSize() == 1);
		std::vector<std::string> values_to_insert = {"Hello", "32", "435", "Albert"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_FAILURE);
}

/*********************OPERATIONS ON LOADED DB***************************/
/* Hanlder declarations loads all the information inside of the db (tables and field in the map) */
TEST(SqliteLoadedDb, LoadDatabase){
		EXPECT_TRUE(exists("MyDB.db"));
		handler::Sqlite3Db LoaderHandler("MyDB.db");
		ASSERT_GT(LoaderHandler.getTablesSize(), 0);
		ASSERT_EQ(LoaderHandler.getTables()[0], table_name);
}
/* Check if the hanlder is able to operate the db just from the data loaded at initialization */
TEST(SqliteLoadedDb, InsertAfterLoad){
		EXPECT_TRUE(exists("MyDB.db"));
		handler::Sqlite3Db LoaderHandler("MyDB.db");
		EXPECT_GT(LoaderHandler.getTablesSize(), 0);
		std::vector<std::string> values_to_insert = {"3", "23", "", "Edu"};
		ASSERT_EQ(LoaderHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);
}

/**************************SELECT OPERATIONS***********************/
/* Select all records inside of a table correctly */
TEST(SqliteSelectRecords, SelectAll){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getTablesSize(), 0);
		std::vector<std::string> data = UserHandler.selectRecords(table_name);
		ASSERT_EQ(data, data_to_check);
}
/* Select all records of a non existent table */
TEST(SqliteSelectRecords, SelectAllNoTable){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getTablesSize(), 0);
		ASSERT_TRUE(UserHandler.selectRecords("Conections").empty());
}
/* Select specific field from all records */
TEST(SqliteSelectRecords, SelectSpecific){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getTablesSize(), 0);
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {table_definition[2].first});
		ASSERT_EQ(data.size(), 1);
		ASSERT_EQ(data[0], "665");
}
/* Select non existent specific field from all records */
TEST(SqliteSelectRecords, SelectSpecificNoField){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getTablesSize(), 0);
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SURNAME"});
		ASSERT_TRUE(data.empty());
}

/**************************DELETE AND DROP OPERATIONS***********************/
TEST(SqliteDelete, DeleteRecord){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getTablesSize(), 0);
		ASSERT_EQ(UserHandler.deleteRecords(table_name, "ID > 7"), EXIT_SUCCESS);
}
TEST(SqliteDelete, DeleteAllRecords){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getTablesSize(), 0);
		ASSERT_EQ(UserHandler.deleteRecords(table_name, "all"), EXIT_SUCCESS);
}
TEST(SqliteDelete, DeleteWrongTable){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getTablesSize(), 0);
		ASSERT_EQ(UserHandler.deleteRecords("CONECTIONS", "all"), EXIT_FAILURE);
}
TEST(SqliteDelete, DeleteWrongCondition){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getTablesSize(), 0);
		ASSERT_EQ(UserHandler.deleteRecords(table_name, "UD == 4"), EXIT_FAILURE);
}

TEST(SqliteDropTable, DropNonexistentTable){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getTablesSize(), 0);
		ASSERT_EQ(UserHandler.dropTable("CONECTIONS"), EXIT_FAILURE);
}
TEST(SqliteDropTable, DropTable){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getTablesSize(), 0);
		ASSERT_EQ(UserHandler.dropTable(table_name), EXIT_SUCCESS);
}



int main(int argc, char *argv[]) {
		testing::InitGoogleTest(&argc, argv);
		return RUN_ALL_TESTS();
}

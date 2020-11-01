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

handler::Sqlite3Db UserHandler("MyDB.db");

std::string table_name = "CONNECTIONS";

std::vector<handler::FieldDescription> table_definition = \
{{"ID",query::data::int_+query::data::primary_key+query::data::not_null}, \
		{"AGE", query::data::int_+query::data::not_null}, \
		{"PHONE", query::data::int_+query::data::null}, \
		{"NAME",query::data::char_+query::data::len(50)+query::data::not_null}};

std::vector<std::string> names_to_check = {"ID", "AGE", "PHONE", "NAME"};

/********************CONSTRUCTOR AND DESTRUCTOR TESTS************************/
TEST(SqliteHandlerConstructor, CreateDatabase){
		EXPECT_FALSE(exists("test.db"));
		handler::Sqlite3Db testHandler;
		ASSERT_TRUE(exists("test.db"));
}

TEST(SqliteHandlerConstructor, MultiConnectDB){
		EXPECT_TRUE(exists("test.db"));
		handler::Sqlite3Db firstConnection;
		handler::Sqlite3Db secondConnection;
		ASSERT_NO_FATAL_FAILURE(handler::Sqlite3Db thirdConnection);
}

TEST(SqliteHandlerConstructor, LoadEmptyDatabase){
		ASSERT_NO_THROW(handler::Sqlite3Db UserHandler("MyDB.db"));
		ASSERT_EQ(UserHandler.getTablesSize(), 0);
}

/*******************DISCONNECTION AND CONNECTIONS**************************/
TEST(SqliteConnection, DisconnectDB){
		handler::Sqlite3Db MyHandler("MyDB.db");
		ASSERT_NO_THROW(MyHandler.closeConnection());
}


/*******************CREATE TABLE FUNCTION*********************************/
TEST(SqliteCreateTable, CreateTable){
		ASSERT_EQ(UserHandler.createTable(table_name, table_definition), EXIT_SUCCESS);
		ASSERT_EQ(UserHandler.getTablesSize(), 1);
		ASSERT_EQ(UserHandler.getFields(table_name), names_to_check);
}


/*******************INSERT RECORDS FUNCTION******************************/
TEST(SqliteInsertion, InsertCompleteCorrect){
		EXPECT_TRUE(UserHandler.getTablesSize() == 1);
		std::vector<std::string> values_to_insert = {"1", "32", "665", "ANTHON33"};

		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);
}

TEST(SqliteInsertion, InsertIncorrectType){
		EXPECT_TRUE(UserHandler.getTablesSize() == 1);
		std::vector<std::string> values_to_insert = {"Hello", "32", "435", "Albert"};

		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_FAILURE);
}

TEST(SqliteInsertion, InsertIncompleteCorrect){
		EXPECT_TRUE(UserHandler.getTablesSize() == 1);
		std::vector<std::string> values_to_insert = {"3", "43", "", "Julia"};

		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);
}

TEST(SqliteInsertion, InsertIncompleteIncorrect){
		EXPECT_TRUE(UserHandler.getTablesSize() == 1);
		std::vector<std::string> values_to_insert = {"4", "", "", "Robert"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_FAILURE);
}

TEST(SqliteInsertion, InsertNoTable){
		EXPECT_TRUE(UserHandler.getTablesSize() == 1);
		std::vector<std::string> values_to_insert = {"8", "32", "665", "BRUNO"};
		ASSERT_EQ(UserHandler.insertRecord("CONECTIONS", values_to_insert), EXIT_FAILURE);
}

/*********************OPERATIONS ON LOADED DB***************************/
TEST(SqliteLoadedDb, LoadDatabase){
		EXPECT_TRUE(exists("MyDB.db"));
		handler::Sqlite3Db LoaderHandler("MyDB.db");
		ASSERT_GT(LoaderHandler.getTablesSize(), 0);
		ASSERT_EQ(LoaderHandler.getTables()[0], table_name);
}

TEST(SqliteLoadedDb, InsertAfterLoad){
		EXPECT_TRUE(exists("MyDB.db"));
		handler::Sqlite3Db LoaderHandler("MyDB.db");
		EXPECT_GT(LoaderHandler.getTablesSize(), 0);
		std::vector<std::string> values_to_insert = {"7", "23", "", "Edu"};
		ASSERT_EQ(LoaderHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);
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

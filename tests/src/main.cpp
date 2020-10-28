#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sqlite3.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include "../../source/include/sqlite3handler.hpp"
#include "../../source/include/dbquery.hpp"

inline bool exists(const std::string& name) {
		struct stat buffer;
		return (stat (name.c_str(), &buffer) == 0);
}

handler::Sqlite3Db UserHandler("MyDB.db");

TEST(SqlitehandlerTest, CreateDatabase){
		EXPECT_FALSE(exists("test.db"));
		handler::Sqlite3Db testHandler;
		ASSERT_TRUE(exists("test.db"));
}

TEST(SqlitehandlerTest, MultiConnectDB){
		EXPECT_TRUE(exists("test.db"));
		handler::Sqlite3Db firstConnection;
		handler::Sqlite3Db secondConnection;
}

TEST(SqlitehandlerTest, DisconnectDB){
		handler::Sqlite3Db MyHandler("MyDB.db");
		ASSERT_NO_THROW(MyHandler.closeConnection());
}

TEST(SqlitehandlerTest, LoadEmptyDatabase){
		ASSERT_NO_THROW(handler::Sqlite3Db UserHandler("MyDB.db"));
		ASSERT_EQ(UserHandler.getTablesSize(), 0);
}

TEST(SqlitehandlerTest, CreateTable){
		std::vector<handler::FieldDescription> v = {{"ID",query::data::int_+query::data::primary_key+query::data::not_null}, \
				{"AGE", query::data::int_+query::data::not_null}, \
				{"PHONE", query::data::int_+query::data::not_null}, \
				{"NAME",query::data::char_+query::data::len(50)+query::data::not_null}};
		std::vector<std::string> names = {"ID", "AGE", "PHONE", "NAME"};

		ASSERT_EQ(UserHandler.createTable("CONNECTIONS", v), EXIT_SUCCESS);
		ASSERT_EQ(UserHandler.getTablesSize(), 1);
		ASSERT_EQ(UserHandler.getFields("CONNECTIONS"), names);
}

TEST(SqlitehandlerTest, InsertValues){
	EXPECT_TRUE(UserHandler.getTablesSize() == 1);
	std::vector<std::string> names = {"ID", "AGE", "PHONE", "NAME"};
	std::vector<std::string> values = {"1", "32", "2865631", "ANTHON33"};

		ASSERT_EQ(UserHandler.insertRecord("CONNECTIONS", values), EXIT_SUCCESS);
}

TEST(SqlitehandlerTest, InsertValuesIncomplete){
	EXPECT_TRUE(UserHandler.getTablesSize() == 1);
	std::vector<std::string> names = {"ID", "AGE", "PHONE", "NAME"};
	std::vector<std::string> values = {"1", "", "2865631", "ANTHON33"};

		ASSERT_ANY_THROW(UserHandler.insertRecord("CONNECTIONS", values));
}


int main(int argc, char *argv[]) {
		testing::InitGoogleTest(&argc, argv);
		return RUN_ALL_TESTS();
}

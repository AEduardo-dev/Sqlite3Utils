#include <handler.hpp>
#include <query.hpp>

int main(int argc, char const *argv[]) {

		handler::Sqlite3Db MyHandler("mydatabase.db");

		/* Supposing that there are already some tables and records in the database */

		/* Prepare the query options that will be used */
		std::string table_name = table_name;
		std::vector<std::string> fields = {"*"};
		bool select_distinct = false;
		std::string where_cond = "";
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 2";
		std::string order_type = "DESC";
		int limit = 10;
		int offset = 0;

		/* Define a vector for the data that will be extracted */
		std::vector<std::string> data;

		data = MyHandler.selectRecords(table_name, fields, select_distinct, where_cond, group_by, \
		                               having_cond, order_by, order_type, limit, offset);

		if (!data.empty()) {
		/*
		 Process the data
		 */
		}

		return 0;
}
TEST(SqliteSelectRecords, SelectStructSpecificAllConditions){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getTablesSize(), 0);


		std::vector<std::string> data_to_get = { "1", "32", "665","ANTHON33", "3", "23", "Edu"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

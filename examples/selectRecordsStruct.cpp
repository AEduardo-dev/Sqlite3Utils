#include <sqlite3handler.hpp>
#include <sqlite3query.hpp>

int main(int argc, char const *argv[]) {

		handler::Sqlite3Db MyHandler("mydatabase.db");

		/* Supposing that there are already some tables and records in the database */

		/* Prepare the query options that will be used using the structure of the  */
		handler::select_query_param select_query;
		select_query.table_name = "table_name";
		select_query.fields = {"*"};
		select_query.select_distinct = false;
		select_query.group_by = {"NAME"};
		select_query.order_by = {"SUM(AGE)"};
		select_query.having_cond = "count(name) < 2";
		select_query.order_type = "DESC";
		select_query.limit = 10;
		select_query.offset = 0;

		/* Define a vector for the data that will be extracted */
		std::vector<std::string> data;

		data = MyHandler.selectRecords(select_query);

		if (!data.empty()) {
				/*
				   Process the data
				 */
		}

		return 0;
}

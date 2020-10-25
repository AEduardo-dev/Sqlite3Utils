#include "../include/sqlite3handler.h"
#include "../include/dbquery.h"

using namespace query;

int main(int argc, char const *argv[]) {
		handler::Sqlite3Db myHandler;

		std::vector<handler::FieldDescription> v = {{"ID",query::data::int_+query::data::primary_key+query::data::not_null}, \
				{"AGE", query::data::int_+query::data::not_null}, \
				{"PHONE", query::data::int_+query::data::not_null}, \
				{"NAME",query::data::char_+query::data::len(50)+query::data::not_null}};

		myHandler.createTable("COMPANY", v);

		//In table company created previously
		std::string table_name = "COMPANY";
		//ID  AGE    PHONE      NAME
		std::vector<std::string> values = {"1", "34", "5521664", "James"};
		//Insert record with all fields defined
		myHandler.insertRecord(table_name, values);

		//ID  AGE PHONE  NAME
		std::vector<std::string> values_incomplete = {"2", "34", "", "Thomas"};
		//Insert record with one or more fields not specified
		myHandler.insertRecord(table_name, values_incomplete);

		table_name = "COMPANY";
		std::string condition = "ID == 1";

		//Delete record that meets condition
		myHandler.deleteRecords(table_name, condition);

		//Delete all records in the table "table_name"
		myHandler.deleteRecords(table_name, condition="all");



		return 0;
}

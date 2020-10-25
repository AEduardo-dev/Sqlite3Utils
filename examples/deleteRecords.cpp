#include "../source/include/dbquery.h"
#include "../source/include/sqlite3handler.h"

int main(int argc, char const *argv[]) {
		handler::Sqlite3Db myHandler;

		//In table company created previously
		std::string table_name = "COMPANY";
		std::string condition = "ID == 1";

		//Delete record that meets condition
		myHandler.deleteRecords(table_name, condition);

		//Delete all records in the table "table_name"
		myHandler.deleteRecords(table_name, condition="all");

		return 0;
}

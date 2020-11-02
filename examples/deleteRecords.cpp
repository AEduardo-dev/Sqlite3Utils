#include <sqlite3handler.hpp>

int main(int argc, char const *argv[]) {
		handler::Sqlite3Db myHandler;

		//In table company created previously
		std::string table_name = "COMPANY";
		std::string condition = "ID == 1";

		//Delete record that meets condition
		if (myHandler.deleteRecords(table_name, condition) == EXIT_SUCCESS){
			...
		}

		//Delete all records in the table "table_name"
		if (myHandler.deleteRecords(table_name, condition="all")){
			...
		}

		return 0;
}

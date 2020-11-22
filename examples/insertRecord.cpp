#include <handler.hpp>
#include <query.hpp>

int main(int argc, char const *argv[]) {
		handler::Sqlite3Db myHandler;

    //In table company created previously
		std::string table_name = "COMPANY";
		                                  //ID  AGE    PHONE      NAME
		std::vector<std::string> values = {"1", "34", "5521664", "James"};
    //Insert record with all fields defined
		if (MyHandler.insertRecord(table_name, values) == EXIT_SUCCESS){
			...
		}

		                                              //ID  AGE PHONE  NAME
		std::vector<std::string> values_incomplete = {"2", "34", "", "Thomas"};
    //Insert record with one or more fields not specified
		if (MyHandler.insertRecord(table_name, values_incomplete) == EXIT_SUCCESS){
			...
		}

		return 0;
}

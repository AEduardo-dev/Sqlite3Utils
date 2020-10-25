#include "../source/include/dbquery.h"
#include "../source/include/sqlite3handler.h"

int main(int argc, char const *argv[]) {
		handler::Sqlite3Db myHandler;

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

		return 0;
}

#include <handler.hpp>

int main(int argc, char const *argv[]) {
		handler::Sqlite3Db myHandler;

		//In table company created previously
		std::string table_name = "COMPANY";

		//Delete record that meets condition
		if(myHandler.dropTable(table_name) == EXIT_SUCCESS){
      ...
    } else{
      (error handling)
      ...
    }


		return 0;
}

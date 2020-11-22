#include "../include/handler.hpp"
#include "../include/query.hpp"
#include "../Sqlite3UtilsConfig.h"

using namespace query;

void printOptions();

int main(int argc, char const *argv[]) {

		if (argc < 2) {
				// report version
				std::cout << argv[0] << " Version " << Sqlite3Utils_VERSION_MAJOR << "."
				          << Sqlite3Utils_VERSION_MINOR << std::endl;
				std::cout << "Usage: " << argv[0] << " <database_name || database_path> " << std::endl;
				return 1;
		}

		handler::Sqlite3Db myHandler(argv[1]);

		std::cout << "Select the operation to apply on " << myHandler.getDbPath() << '\n';
		printOptions();

		return 0;
}


void printOptions(){
	std::cout << "0 - Create Table	\n";
	std::cout << "1 - Drop Table 		\n";
	std::cout << "2 - Update Table	\n";
	std::cout << "3 - Insert Record 	\n";
	std::cout << "4 - Select Records 	\n";
	std::cout << "5 - Delete Records 	\n";
	std::cout << "6 - Update Database Information 	\n";
	std::cout << "7 - Get Database Information 			\n"
	std::cout << "8 - Disconnect	\n";
}

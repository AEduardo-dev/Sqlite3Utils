#include <sqlite3handler.hpp>
#include <sqlite3query.hpp>
#include <DatabaseHandlerConfig.h>


using namespace query;

int main(int argc, char const *argv[]) {

		if (argc < 2) {
				// report version
				std::cout << argv[0] << " Version " << DatabaseHandler_VERSION_MAJOR << "."
				          << DatabaseHandler_VERSION_MINOR << std::endl;
				std::cout << "Usage: " << argv[0] << " <database_name || database_path> " << std::endl;
				return 1;
		}

		handler::Sqlite3Db myHandler(argv[1]);

		std::cout << "Select the operation to apply on " << myHandler.getDbPath() << '\n';



		return 0;
}

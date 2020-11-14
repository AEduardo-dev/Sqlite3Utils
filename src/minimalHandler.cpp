#include "../include/handler.hpp"
#include "../include/query.hpp"
#include "../Sqlite3UtilsConfig.h"

using namespace query;

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

		/* Declaration of the table's fields */
		std::vector<handler::FieldDescription> table_definition = \
		{{"ID",query::data::int_+query::data::primary_key+query::data::not_null}, \
				{"AGE", query::data::int_+query::data::not_null}, \
				{"PHONE", query::data::int_+query::data::null}, \
				{"NAME",query::data::char_+query::data::len(50)+query::data::not_null}};

		if(myHandler.createTable("NEWTABLE", table_definition) == EXIT_SUCCESS)
			std::cout << "Funciona" << '\n';

		return 0;
}

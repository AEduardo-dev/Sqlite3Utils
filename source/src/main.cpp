#include "../include/database.h"
#include "../include/dbquery.h"

using namespace query;

int main(int argc, char const *argv[]) {
		database::dbHandler myHandler;
		std::pair<std::string, std::string> primary_key = {"ID", db_int+db_primary_key+opt_not_null};
		std::vector<std::pair<std::string, std::string> > v = {{"SOCKET", db_int+opt_not_null}, \
				{"PORT", db_int+opt_not_null}, \
				{"ADDRESS",db_char+opt_data_len(50)+opt_not_null}};

		myHandler.createTable("CONNECTIONS", primary_key, v);

		return 0;
}

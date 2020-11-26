#include <sqlite3utils-1.0.0/handler.hpp>
#include <sqlite3utils-1.0.0/query.hpp>


int main(int argc, char const *argv[]) {

		/* Stablish first connection to database */
		handler::Sqlite3Db MyHandler("mydatabase.db");
    std::string table_name = "table name"
    std::vector<handler::FieldDescription> set_fields = {{"field_name", "value to apply"}, ...};
    /*
    ...
     */
    while (/* condition */) {
      /* update a table created in previous steps */
      MyHandler.updateTable(table_name, set_fields, "")();
      /*
  		   ....
  		   operations on db.
  		   ...
  		 */
    }

		return 0;
}

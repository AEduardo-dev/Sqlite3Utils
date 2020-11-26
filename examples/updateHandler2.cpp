#include <sqlite3handler.hpp>
#include <sqlite3query.hpp>


int main(int argc, char const *argv[]) {

		/* Stablish second connection to the database */
		handler::Sqlite3Db AnotherHandler("mydatabase.db");

    while (/* condition */) {
      /* update every time before acting on the db in case something is changed */
      AnotherHandler.updateHandler();
      /*
  		   ....
  		   operations on db.
  		   ...
  		 */
    }

		return 0;
}

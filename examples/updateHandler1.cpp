#include <handler.hpp>
#include <query.hpp>


int main(int argc, char const *argv[]) {

		/* Stablish first connection to database */
		handler::Sqlite3Db MyHandler("mydatabase.db");

    while (/* condition */) {
      /* update every time before acting on the db in case something is changed */
      MyHandler.updateHandler();
      /*
  		   ....
  		   operations on db.
  		   ...
  		 */
    }

		return 0;
}

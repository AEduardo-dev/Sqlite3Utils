#include <handler.hpp>


int main(int argc, char const *argv[]) {
  hanlder::Sqlite3Db MyHandler("dbname.db");

  /*
  ..............
  operations on database
  .......
   */
  MyHandler.closeConnection();
  return 0;
}

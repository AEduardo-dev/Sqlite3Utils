#include <sqlite3handler.hpp>


int main(int argc, char const *argv[]) {
  hanlder::Sqlite3Db MyHandler("dbname.db");

  /*
  ..............
  operations on database
  .......
   */
  MyHandler.closeConnection();

  ...

  if(MyHandler.reconnectDb() == EXIT_SUCCESS){
    ...
  }
  return 0;
}

#include <sqlite3handler.hpp>
#include <sqlite3query.hpp>

int main(int argc, char const *argv[]) {

  handler::Sqlite3Db MyHandler("mydatabase.db");
  /*
  ...
  */
  /* Generate custom user query */
  const char* query = (query::cmd::select + query::cl::from + "My Table" \
                + query::end_query).c_str();
  /* Vector for data storage */
  std::vector<std::string> data;
  /* Indexes to get the data from in each row of the result */
  std::vector<int> indexes_stmt = {1,3};
  /* Flag for verbose mode */
  bool verbose = true;

  /* Execute the query with the settings defined and extracting data */
  MyHandler.executeQuery(query, data, indexes_stmt, verbose);
  /*
  Process the data
  ....
   */

  /* Generate another custom user query */
  query = (query::cmd::drop_table + "My Table" \
               + query::end_query).c_str();

  /* Execute the query in simple mode, no data extraction and not verbose mode */
  MyHandler.executeQuery(query);

  return 0;
}

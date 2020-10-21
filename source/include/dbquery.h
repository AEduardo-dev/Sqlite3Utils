#ifndef DBQUERY_H
#define DBQUERY_H

#include <string>

/******************************************************************************
   Definition file used for easier sql query composition
******************************************************************************/

namespace query {

//TODO:12/10/2020.-Instead of define use const string

/*************************************
            DATATYPES
*************************************/
const std::string db_char               = "CHAR ";
const std::string db_int                = "INT ";
const std::string db_primary_key        = "PRIMARY KEY ";
const std::string db_table              = "table";
/*************************************
            OPTIONS
*************************************/
const std::string opt_not_null          = "NOT NULL ";
const std::string opt_null              = "NULL ";


const std::string opt_data_len(int x){
		return " (" + std::to_string(x) + ")";
};
const std::string opt_table_info(const std::string table_name){
		return "table_info("+ table_name +")";
};
const std::string opt_type(const std::string type){
		const std::string single_quote = "'";
		return " type="+single_quote+type+single_quote;
};

/*************************************
            DB-QUERY
*************************************/
const std::string cmd_create_table      = "CREATE TABLE ";
const std::string cmd_from              = "FROM ";
const std::string cmd_insert            = "INSERT ";
const std::string cmd_insert_into       = "INSERT INTO ";
const std::string cmd_order_by          = "ORDER BY ";
const std::string cmd_pragma            = "PRAGMA ";
const std::string cmd_select            = "SELECT ";
const std::string cmd_values            = "VALUES ";
const std::string cmd_where             = "WHERE ";

/*************************************
            SPECIALS
*************************************/
const std::string end_query          =";";


} /* macros */

#endif // DBQUERY_H

#include "../include/dbquery.h"

/******************************************************************************
   Definition file used for easier sql query composition
******************************************************************************/


const std::string query::opt_data_len(int x){
		return ("(" + std::to_string(x) + ") ");
}

const std::string query::opt_table_info(const std::string table_name){
		return ("table_info("+ table_name +")");
}

const std::string query::opt_type(const std::string type){
		const std::string single_quote = "'";
		return (" type="+single_quote+type+single_quote);
}

const std::string query::trigger(const std::string db_name, const std::string trigger_name){
		return (db_name+"."+trigger_name);
}

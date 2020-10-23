#include "../include/dbquery.h"

/******************************************************************************
   Definition file used for easier sql query composition
******************************************************************************/
const std::string query::cl_glob(const std::string pattern){
		const std::string single_quote = "'";
		return ("GLOB "+single_quote+pattern+single_quote+" ");
}

const std::string query::cl_like(const std::string pattern){
		const std::string single_quote = "'";
		return ("LIKE "+single_quote+pattern+single_quote+" ");
}

const std::string cl_limit(int limit_value){
		return ("LIMIT "+std::to_string(limit_value)+" ");
}

const std::string query::cl_not(const std::string clause){
		return ("NOT "+clause+" ");
}

const std::string cl_offset(int offset_value){
		return ("OFFSET "+std::to_string(offset_value)+" ");
}

const std::string query::opt_data_len(int x){
		return ("("+std::to_string(x)+") ");
}

const std::string query::opt_table_info(const std::string table_name){
		return ("table_info("+table_name+")"+" ");
}

const std::string query::opt_type(const std::string type){
		const std::string single_quote = "'";
		return (" type="+single_quote+type+single_quote+" ");
}

const std::string query::table(const std::string db_name, const std::string table_name){
		return (db_name+"."+table_name+" ");
}

const std::string query::trigger(const std::string db_name, const std::string trigger_name){
		return (db_name+"."+trigger_name+" ");
}

const std::string query::view(const std::string db_name, const std::string view_name){
		return (db_name+"."+view_name+" ");
}

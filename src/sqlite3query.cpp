#include "../include/query.hpp"

const std::string query::cl::glob(const std::string pattern){
		const std::string single_quote = "'";
		return (" GLOB "+single_quote+pattern+single_quote+" ");
}

const std::string query::data::len(int length){
		return (" ("+std::to_string(length)+") ");
}

const std::string query::cl::like(const std::string pattern){
		const std::string single_quote = "'";
		return (" LIKE "+single_quote+pattern+single_quote+" ");
}

const std::string query::cl::limit(int limit_value){
		return (" LIMIT "+std::to_string(limit_value)+" ");
}

const std::string query::cl::offset(int offset_value){
		return (" OFFSET "+std::to_string(offset_value)+" ");
}

const std::string query::cl::table_info(const std::string table_name){
		return (" table_info("+table_name+")"+" ");
}

const std::string query::cl::type(const std::string type){
		const std::string single_quote = "'";
		return (" type="+single_quote+type+single_quote+" ");
}

const std::string query::data::trigger(const std::string db_name, const std::string trigger_name){
		return (" "+db_name+"."+trigger_name+" ");
}

const std::string query::data::view(const std::string db_name, const std::string view_name){
		return (" "+db_name+"."+view_name+" ");
}

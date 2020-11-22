/*
 * This file is part of Sqlite3Utils.
 *
 * Sqlite3Utils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sqlite3Utils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Sqlite3Utils.  If not, see <https://www.gnu.org/licenses/>.
 */


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

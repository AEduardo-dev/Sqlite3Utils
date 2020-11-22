#include <handler.hpp>
#include <query.hpp>

int main(int argc, char const *argv[]) {
		handler::Sqlite3Db myHandler;

		std::vector<handler::FieldDescription> v = {{"ID",query::data::int_+query::data::primary_key+query::data::not_null}, \
				{"AGE", query::data::int_+query::data::not_null}, \
				{"PHONE", query::data::int_+query::data::not_null}, \
				{"NAME",query::data::char_+query::data::len(50)+query::data::not_null}};

		if (myHandler.createTable("COMPANY", v) == EXIT_SUCCESS) {
			...
		}

		return 0;
}

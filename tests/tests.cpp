#include <gtest/gtest.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include "../include/handler.hpp"
#include "../include/query.hpp"

/*!
 * \brief Checks if the file given exists in the directory
 *
 * @param  name The name of the file to search.
 * @return      True if the file exists, false otherwise.
 */
inline bool exists(const std::string& name) {
		struct stat buffer;
		return (stat (name.c_str(), &buffer) == 0);
}

/* Global handler for use in multiple tests */
handler::Sqlite3Db UserHandler("MyDB.db");

/* Main table used during tests */
std::string table_name = "CONNECTIONS";

/* Declaration of the table's fields */
std::vector<handler::FieldDescription> table_definition = \
{{"ID",query::data::int_+query::data::primary_key+query::data::not_null}, \
		{"AGE", query::data::int_+query::data::not_null}, \
		{"PHONE", query::data::int_+query::data::null}, \
		{"NAME",query::data::char_+query::data::len(50)+query::data::not_null}};
/* Wrong declaration for failure tests */
std::vector<handler::FieldDescription> table_definition_wrong = \
{{"ID",query::data::int_+query::data::primary_key+query::data::not_null}, \
		{"AGE", query::data::int_+query::data::not_null+"k"}, \
		{"PHONE", query::data::int_+query::data::null}, \
		{"NAME",query::data::char_+query::data::len(50)+query::data::not_null}};

/* Vectors for comparison in query results */
std::vector<std::string> names_to_check = {"ID", "AGE", "PHONE", "NAME"};
std::vector<std::string> data_to_check = {"1", "32", "665", "ANTHON33", \
		                                      "2", "43", "Julia", \
		                                      "3", "23", "Edu"};

/********************CONSTRUCTOR AND DESTRUCTOR TESTS************************/
/* Check if db is created if it does not exist */
TEST(Handler_Constructor, Should_Create_Database){
		EXPECT_FALSE(exists("CreatedDB.db"));
		handler::Sqlite3Db testHandler("CreatedDB.db");

		ASSERT_TRUE(exists("CreatedDB.db"));
}

/* Check if multiple handlers can be connected to the same db */
TEST(Handler_Constructor, Can_MultiConnect_DB){
		EXPECT_TRUE(exists("MyDB.db"));
		handler::Sqlite3Db firstConnection("MyDB.db");
		handler::Sqlite3Db secondConnection("MyDB.db");

		ASSERT_NO_FATAL_FAILURE(handler::Sqlite3Db thirdConnection("MyDB.db"));
}

/* If no tables exist the database should be loaded but no table will be in the handler */
TEST(Handler_Constructor, Loads_Empty_Database){
		ASSERT_NO_THROW(handler::Sqlite3Db MyHandler("MyDB.db"));
		ASSERT_EQ(UserHandler.getNumTables(), 0);
}

/* Contructor wrong db name */
TEST(Handler_Constructor, Should_Accept_Name_Without_Extension){
		ASSERT_NO_THROW(handler::Sqlite3Db MyHandler("NoExtensionDB"));
		ASSERT_EQ(UserHandler.getNumTables(), 0);
}

/*******************CREATE TABLE FUNCTION*********************************/
/* Create a table normally */
TEST(Create_Table, Works_Correctly){
		ASSERT_EQ(UserHandler.createTable(table_name, table_definition), EXIT_SUCCESS);
		ASSERT_EQ(UserHandler.getNumTables(), 1);
		ASSERT_EQ(UserHandler.getFields(table_name), names_to_check);
}

/* Create a table with a typo in it's definition */
TEST(Create_Table, Fails_If_Wrong_Query){
		ASSERT_EQ(UserHandler.createTable(table_name, table_definition_wrong), EXIT_FAILURE);
		ASSERT_EQ(UserHandler.getNumTables(), 1);
		ASSERT_EQ(UserHandler.getFields(table_name), names_to_check);
}

/**********************AFFINITY OPERATIONS*****************************/
TEST(Affinity_Check, Detects_Integer_Affinity_Lowercase){
		ASSERT_EQ(UserHandler.getAffinity("medint"), "INTEGER");
		ASSERT_TRUE(UserHandler.isAffined("INTEGER", "3386"));
}

/* Get the affinity of an integer number datatype and check a correct value against it */ TEST(Affinity_Check, Detects_Integer_Affinity_Uppercase){
		ASSERT_EQ(UserHandler.getAffinity("MEDINT"), "INTEGER");
		ASSERT_TRUE(UserHandler.isAffined("INTEGER", "3386"));
}

/* Get the affinity of a non integer datatype and check if no false integer is generated */
TEST(Affinity_Check, No_False_Integer_Affinity){
		ASSERT_NE(UserHandler.getAffinity("REAL"), "INTEGER");
}

/* Get the affinity of an integer number datatype and check a wrong value against it */
TEST(Affinity_Check, Integer_Affinity_Fails_When_Real_Value){
		ASSERT_EQ(UserHandler.getAffinity("MEDINT"), "INTEGER");
		ASSERT_FALSE(UserHandler.isAffined("INTEGER", "33,86"));
}

/* Get the affinity of an integer number datatype and check a wrong value against it */
TEST(Affinity_Check, Integer_Affinity_Fails_When_Text_Value){
		ASSERT_EQ(UserHandler.getAffinity("MEDINT"), "INTEGER");
		ASSERT_FALSE(UserHandler.isAffined("INTEGER", "33a86"));
}

/* Get the affinity of an integer number datatype and check a wrong value against it */
TEST(Affinity_Check, Integer_Affinity_Fails_When_Symbol_Value){
		ASSERT_EQ(UserHandler.getAffinity("MEDINT"), "INTEGER");
		ASSERT_FALSE(UserHandler.isAffined("INTEGER", "33+86"));
}

/* Get the affinity of an integer number datatype and check a wrong value against it */
TEST(Affinity_Check, Integer_Affinity_Fails_When_Empty_Value){
		ASSERT_EQ(UserHandler.getAffinity("MEDINT"), "INTEGER");
		ASSERT_FALSE(UserHandler.isAffined("INTEGER", ""));
}

/* Get the affinity of a real number datatype in lower case*/
TEST(Affinity_Check, Detects_Real_Affinity_Lowercase){
		ASSERT_EQ(UserHandler.getAffinity("double"), "REAL");
}

/* Get the affinity of a real number datatype in upper case*/
TEST(Affinity_Check, Detects_Real_Affinity_Uppercase){
		ASSERT_EQ(UserHandler.getAffinity("DOUBLE"), "REAL");
}

/* Get the affinity of a real number datatype and check a correct value against it (comma)*/
TEST(Affinity_Check, Real_Affinity_Works_With_Comma_Decimals){
		ASSERT_EQ(UserHandler.getAffinity("LONG DOUBLE"), "REAL");
		ASSERT_TRUE(UserHandler.isAffined("REAL", "33,86"));
}

/* Get the affinity of a real number datatype and check a correct value against it (dot)*/
TEST(Affinity_Check, Real_Affinity_Works_With_Dot_Decimals){
		ASSERT_EQ(UserHandler.getAffinity("Short double"), "REAL");
		ASSERT_TRUE(UserHandler.isAffined("REAL", "33.86"));
}

/* Get the affinity of a non real datatype and check if no false real affinity is generated */
TEST(Affinity_Check, No_False_Real_Affinity){
		ASSERT_NE(UserHandler.getAffinity("LONG INT"), "REAL");
}

/* Get the affinity of a real number datatype and check a wrong value against it */
TEST(Affinity_Check, Real_Affinity_Works_With_Integer_Value){
		ASSERT_EQ(UserHandler.getAffinity("LONG DOUBLE"), "REAL");
		ASSERT_TRUE(UserHandler.isAffined("REAL", "3386"));
}

/* Get the affinity of an integer number datatype and check a wrong value against it */
TEST(Affinity_Check, Real_Affinity_Fails_When_Text_Value){
		ASSERT_EQ(UserHandler.getAffinity("LONG DOUBLE"), "REAL");              ASSERT_FALSE(UserHandler.isAffined("REAL", "33aasd86"));
}

/* Get the affinity of an integer number datatype and check a wrong value against it */
TEST(Affinity_Check, Real_Affinity_Fails_When_Symbol_Value){
		ASSERT_EQ(UserHandler.getAffinity("LONG DOUBLE"), "REAL");              ASSERT_FALSE(UserHandler.isAffined("REAL", "33/86"));
}

/* Get the affinity of an integer number datatype and check a wrong value against it */
TEST(Affinity_Check, Real_Affinity_Fails_When_Empty_Value){
		ASSERT_EQ(UserHandler.getAffinity("LONG DOUBLE"), "REAL");              ASSERT_FALSE(UserHandler.isAffined("REAL", ""));
}

/* Get the affinity of a text datatype in lower case */
TEST(Affinity_Check, Detects_Text_Affinity_Lowercase){
		ASSERT_EQ(UserHandler.getAffinity("char(90)"), "TEXT");
}

/* Get the affinity of a text datatype in upper case */
TEST(Affinity_Check, Detects_Text_Affinity_Uppercase){
		ASSERT_EQ(UserHandler.getAffinity("CHAR (90)"), "TEXT");
}

/* Get the affinity of a real datatype and check no false text is received*/
TEST(Affinity_Check, No_False_Text_Affinity){
		ASSERT_NE(UserHandler.getAffinity("REAL"), "TEXT");
}

/* Get the affinity of a blob datatype in lower case */
TEST(Affinity_Check, Detects_Blob_Affinity_Lowercase){
		ASSERT_EQ(UserHandler.getAffinity("blob"), "BLOB");
}

/* Get the affinity of a blob datatype in upper case */
TEST(Affinity_Check, Detects_Blob_Affinity_Uppercase){
		ASSERT_EQ(UserHandler.getAffinity("BLOB"), "BLOB");
}

/* Get the affinity of a blob datatype with no datatype defined */
TEST(Affinity_Check, Detects_Blob_Affinity_Empty_Type){
		ASSERT_EQ(UserHandler.getAffinity(""), "BLOB");
}

/* Get the affinity of a real datatype and check no false blob is received*/
TEST(Affinity_Check, No_False_Blob_Affinity){
		ASSERT_NE(UserHandler.getAffinity("Text"), "BLOB");
}

/* Check all affinities with mixed upper and lower case */
TEST(Affinity_Check, Detects_Affinity_Mixed_Lower_And_Upper){
		ASSERT_EQ(UserHandler.getAffinity("ChAr (100)"), "TEXT");
		ASSERT_EQ(UserHandler.getAffinity("BlOB"), "BLOB");
		ASSERT_EQ(UserHandler.getAffinity("InTeGer"), "INTEGER");
		ASSERT_EQ(UserHandler.getAffinity("ReaL"), "REAL");
}

/*******************INSERT RECORDS FUNCTION******************************/
/* Correct insertion statement with all fields given */
TEST(Insert_Record, Succeeds_With_Complete_Insertion_Description){
		EXPECT_TRUE(UserHandler.getNumTables() == 1);

		std::vector<std::string> values_to_insert = {"1", "32", "665", "ANTHON33"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);
}

/* Correct insertion statement with some fields given. Respecting the not null constraints */
TEST(Insert_Record, Succeeds_With_Incomplete_Insertion_Description){
		EXPECT_TRUE(UserHandler.getNumTables() == 1);

		std::vector<std::string> values_to_insert = {"2", "43", "", "Julia"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);
}

/* Incorrect insertion statement with some fields given. No respecting the not null constraints */
TEST(Insert_Record, Fails_With_Missed_NULL_Constraint_Description){
		EXPECT_TRUE(UserHandler.getNumTables() == 1);

		std::vector<std::string> values_to_insert = {"4", "", "", "Robert"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_FAILURE);
}

/* Incorrect insertion statement giving a non existent table name */
TEST(Insert_Record, Fails_With_Wrong_Table_Name){
		EXPECT_TRUE(UserHandler.getNumTables() == 1);

		std::vector<std::string> values_to_insert = {"5", "32", "665", "BRUNO"};
		ASSERT_EQ(UserHandler.insertRecord("CONECTIONS", values_to_insert), EXIT_FAILURE);
}

/* Incorrect insertion statement giving incorrect datatype to a field */
TEST(Insert_Record, Fails_With_Wrong_Datatype){
		EXPECT_TRUE(UserHandler.getNumTables() == 1);

		std::vector<std::string> values_to_insert = {"Hello", "32", "435", "Albert"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_FAILURE);
}
/*********************OPERATIONS ON LOADED DB***************************/
/* Hanlder declarations loads all the information inside of the db (tables and field in the map) */
TEST(Loaded_Data, Succeeds_Load_Database_Information_Through_Constructor){
		EXPECT_TRUE(exists("MyDB.db"));
		handler::Sqlite3Db LoaderHandler("MyDB.db");

		ASSERT_GT(LoaderHandler.getNumTables(), 0);
		ASSERT_EQ(LoaderHandler.getTablesNames()[0], table_name);
}

/* Check if the hanlder is able to operate the db just from the data loaded at initialization */
TEST(Loaded_Data, Succeeds_Insert_Operation_After_Load){
		EXPECT_TRUE(exists("MyDB.db"));
		handler::Sqlite3Db LoaderHandler("MyDB.db");
		EXPECT_GT(LoaderHandler.getNumTables(), 0);

		std::vector<std::string> values_to_insert = {"3", "23", "", "Edu"};
		ASSERT_EQ(LoaderHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);
}

/**************************SELECT OPERATIONS***********************/
/* Select all records inside of a table correctly */
TEST(Select_Records, Succeeds_Select_All_Records){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);

		std::vector<std::string> data = UserHandler.selectRecords(table_name);
		ASSERT_EQ(data, data_to_check);
}

/* Select all records of a non existent table */
TEST(Select_Records, Fails_Select_All_When_Wrong_Table){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);

		std::vector<std::string> data = UserHandler.selectRecords("Conections");
		ASSERT_TRUE(data.empty());
}

/* Select specific field from all records */
TEST(Select_Records, Succeeds_Select_Specific_Field_From_Record){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);

		std::vector<std::string> data = UserHandler.selectRecords(table_name, {table_definition[2].first});
		ASSERT_EQ(data.size(), 1);
		ASSERT_EQ(data[0], "665");
}

/* Select non existent specific field from all records of existent table */
TEST(Select_Records, Returns_Empty_When_Blank_Field){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);

		std::vector<std::string> data = UserHandler.selectRecords(table_name, {" "});
		ASSERT_TRUE(data.empty());
}

/* Select non existent specific field from all records of existent table */
TEST(Select_Records, Fails_When_Wrong_Field_Name){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);

		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SURNAME"});
		ASSERT_TRUE(data.empty());
}

/* Select specific field from all records seeing only distinct results */
TEST(Select_Records, Succeeds_Select_Specific_Field_With_Only_Unique_Results){

		/* Check the names in the records */
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> data_before = UserHandler.selectRecords(table_name, {"NAME"});

		/* The name already exists */
		ASSERT_EQ(data_before[1], "Julia");

		/* Insert duplicate of name */
		std::vector<std::string> values_to_insert = {"5", "999", "", "Julia"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_SUCCESS);

		/* Select duplicate as well */
		std::vector<std::string> data_duplicates = UserHandler.selectRecords(table_name, {"NAME"}, false);
		ASSERT_FALSE(data_duplicates.empty());
		std::vector<std::string> names_duplicates = {"ANTHON33", "Julia", "Edu", "Julia"};
		ASSERT_EQ(data_duplicates, names_duplicates);

		/* Select only distincts */
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"NAME"}, true);
		ASSERT_FALSE(data.empty());
		std::vector<std::string> names = {"ANTHON33", "Julia", "Edu"};
		ASSERT_EQ(data, names);
}

/* Select all records from table with where condition for records selection */
TEST(Select_Records, Succeeds_Select_All_With_WHERE_Condition){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::string where_cond = "AGE > 100";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, where_cond);
		ASSERT_FALSE(data.empty());
		std::vector<std::string> record_to_extract = {"5", "999", "Julia"};
		ASSERT_EQ(data, record_to_extract);
}

/* Select all records from table with where condition for records selection */
TEST(Select_Records, Succeeds_Select_Specific_Field_With_WHERE_Condition){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::string where_cond = "AGE > 100";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"AGE"}, \
		                                                          false, where_cond);
		ASSERT_FALSE(data.empty());
		std::vector<std::string> record_to_extract = {"999"};
		ASSERT_EQ(data, record_to_extract);
}

/* Select all records from table with wrong where condition for records selection */
TEST(Select_Records, Fails_Select_All_When_Wrong_WHERE_Condition){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::string where_cond = "AGEe > 100";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"AGE"}, \
		                                                          false, where_cond);
		ASSERT_TRUE(data.empty());
}

/* Select all records from table with where condition that selects nothing */
TEST(Select_Records, Returns_Empty_When_WHERE_Condition_Not_Met){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::string where_cond = "AGE > 1000";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"AGE"}, \
		                                                          false, where_cond);
		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping by and using sum for select */
TEST(Select_Records, Succeeds_Group_Results_By){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SUM(AGE)", "NAME"}, \
		                                                          false, "", group_by);

		std::vector<std::string> data_to_get = {"32", "ANTHON33", "23", "Edu", "1042", "Julia"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping by a non existent field */
TEST(Select_Records, Fails_Group_Results_By_When_Wrong_Field_Name){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAMEe"};
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SUM(AGE)", "NAME"}, \
		                                                          false, "", group_by);

		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping by a field and ordering by the sum of a field*/
TEST(Select_Records, Succeeds_Order_Results_Ascendent_Order){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string order_type = "ASC";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SUM(AGE)", "NAME"}, \
		                                                          false, "", group_by, "", \
		                                                          order_by, order_type);

		std::vector<std::string> data_to_get = {"23", "Edu", "32", "ANTHON33", "1042", "Julia"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping by a field and ordering by the sum of a field*/
TEST(Select_Records, Succeeds_Order_Results_Descendent_Order){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string order_type = "DESC";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SUM(AGE)", "NAME"}, \
		                                                          false, "", group_by, "", \
		                                                          order_by, order_type);

		std::vector<std::string> data_to_get = { "1042", "Julia", "32", "ANTHON33", "23", "Edu"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping by a field and ordering by a wrong field*/
TEST(Select_Records, Fails_Order_Results_When_Wrong_Fields_Name){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGEe)"};
		std::string order_type = "DESC";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SUM(AGE)", "NAME"}, \
		                                                          false, "", group_by, "", \
		                                                          order_by, order_type);
		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping by a field and ordering by a wrong field*/
TEST(Select_Records, Fails_Order_Results_When_Wrong_Order_Type){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string order_type = "DES";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"SUM(AGE)", "NAME"}, \
		                                                          false, "", group_by, "", \
		                                                          order_by, order_type);
		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping and ordering using having for filter*/
TEST(Select_Records, Succeeds_Select_Records_With_HAVING_Condition){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 2";
		std::string order_type = "DESC";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type);

		std::vector<std::string> data_to_get = { "1", "32", "665","ANTHON33", "3", "23", "Edu"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping and ordering using having to filter getting no results*/
TEST(Select_Records, Returns_Empty_When_HAVING_Condition_Not_Met){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 1";
		std::string order_type = "DESC";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type);
		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping and ordering using a wrong having condition */
TEST(Select_Records, Fails_When__Wrong_HAVING_Condition){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(namee) < 2";
		std::string order_type = "DESC";
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type);
		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping and ordering using having and showing only 1 result */
TEST(Select_Records, Succeeds_Limit_Number_Of_Results){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 2";
		std::string order_type = "DESC";
		int limit = 1;
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type, limit);

		std::vector<std::string> data_to_get = { "1", "32", "665","ANTHON33"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping and ordering using having and showing all results as any numbre <= 0 is considered as no limit stablished */
TEST(Select_Records, No_Limit_When_Negative_Limit_Number){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 2";
		std::string order_type = "DESC";
		int limit = -1;
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type, limit);

		std::vector<std::string> data_to_get = { "1", "32", "665","ANTHON33"};
		ASSERT_FALSE(data.empty());
}

/* Select all records from table grouping and ordering using having and limiting to one with offset to show from second */
TEST(Select_Records, Succeeds_Limit_Number_Of_Results_With_Offset){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 2";
		std::string order_type = "DESC";
		int limit = 1;
		int offset = 1;
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type, limit, offset);

		std::vector<std::string> data_to_get = {"3", "23", "Edu"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping and ordering using having and limiting results to bigger number than actual number of results */
TEST(Select_Records, Succeeds_When_Limit_Number_More_Than_Results_Number){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 2";
		std::string order_type = "DESC";
		int limit = 10;
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type, limit);

		std::vector<std::string> data_to_get = { "1", "32", "665","ANTHON33", "3", "23", "Edu"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping and ordering using having and limiting results to bigger number than actual number of results */
TEST(Select_Records, Returns_Empty_When_Offset_Number_More_Than_Results_Number){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 2";
		std::string order_type = "DESC";
		int limit = 10;
		int offset = 10;
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type, limit, offset);

		ASSERT_TRUE(data.empty());
}

/* Select all records from table grouping and ordering using having and showing only 1 result */
TEST(Select_Records, No_Offset_When_Negative_Offset_Number){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		std::vector<std::string> group_by = {"NAME"};
		std::vector<std::string> order_by = {"SUM(AGE)"};
		std::string having_cond = "count(name) < 2";
		std::string order_type = "DESC";
		int limit = -1;
		int offset = -1;
		std::vector<std::string> data = UserHandler.selectRecords(table_name, {"*"}, \
		                                                          false, "", group_by, having_cond, \
		                                                          order_by, order_type, limit, offset);

		std::vector<std::string> data_to_get = { "1", "32", "665","ANTHON33"};
		ASSERT_FALSE(data.empty());
}

/* Select all records from table grouping and ordering using having and limiting results to bigger number than actual number of results */
TEST(Select_Records, Succeeds_Select_Struct_Overload_With_All_Options_Filled){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		handler::select_query_param select_query;
		select_query.table_name = table_name;
		select_query.fields = {"*"};
		select_query.group_by = {"NAME"};
		select_query.order_by = {"SUM(AGE)"};
		select_query.having_cond = "count(name) < 2";
		select_query.order_type = "DESC";
		select_query.limit = 10;
		std::vector<std::string> data = UserHandler.selectRecords(select_query);

		std::vector<std::string> data_to_get = { "1", "32", "665","ANTHON33", "3", "23", "Edu"};
		ASSERT_FALSE(data.empty());
		ASSERT_EQ(data, data_to_get);
}

/* Select all records from table grouping and ordering using having and limiting results to bigger number than actual number of results */
TEST(Select_Records, Succeeds_Select_Struct_Overload_With_No_Condition_Met){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		handler::select_query_param select_query;
		select_query.table_name = table_name;
		select_query.fields = {"*"};
		select_query.group_by = {"NAME"};
		select_query.order_by = {"SUM(AGE)"};
		select_query.having_cond = "count(name) < 2";
		select_query.order_type = "DESC";
		select_query.limit = 10;
		select_query.offset = 10;
		std::vector<std::string> data = UserHandler.selectRecords(select_query);

		ASSERT_TRUE(data.empty());
}

/****************UPDATE AND MULTICONNECTION OPERATIONS*************/

/* Update operation when no change has been applied on the db */
TEST(Update_Handler, Succeeds_Update_With_No_Changes){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		handler::DbTables tables = UserHandler.getTables();
		ASSERT_EQ(UserHandler.updateHandler(), EXIT_SUCCESS);
		ASSERT_EQ(UserHandler.getTables(), tables);
}

/* Update a second handler with the database information changed by another handler */
TEST(Update_Handler, Succeeds_Update_With_Parallel_Handler_Changes_On_DB){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);

		handler::Sqlite3Db NewHandler("MyDB.db");
		ASSERT_EQ(UserHandler.getNumTables(), NewHandler.getNumTables());

		std::vector<handler::FieldDescription> fields = \
		{{"NAME",query::data::char_+query::data::primary_key+query::data::not_null}, \
				{"SURNAME",query::data::char_+query::data::len(50)+query::data::not_null}};

		ASSERT_EQ(UserHandler.createTable("NEWTABLE", fields), EXIT_SUCCESS);
		std::vector<std::string> values_to_insert = {"Angel Eduardo", "Vega"};
		ASSERT_EQ(UserHandler.insertRecord("NEWTABLE", values_to_insert), EXIT_SUCCESS);

		handler::DbTables tables = UserHandler.getTables();

		ASSERT_NE(UserHandler.getNumTables(), NewHandler.getNumTables());
		ASSERT_GT(UserHandler.getNumTables(), NewHandler.getNumTables());

		ASSERT_EQ(NewHandler.updateHandler(), EXIT_SUCCESS);
		ASSERT_EQ(NewHandler.getTables(), UserHandler.getTables());
}

/*****************************EXECUTE QUERY********************************/
/* Custom query wanting no output */
TEST(Execute_Custom_Query, Succeeds_With_Correct_Query_Syntax){
		std::string query = query::cmd::select + "*" + query::cl::from + table_name;
		ASSERT_EQ(UserHandler.executeQuery(query.c_str()), EXIT_SUCCESS);
}

/* Custom query extracting output */
TEST(Execute_Custom_Query, Succeeds_With_Data_Output){
		std::string query = query::cmd::select + "*" + query::cl::from + table_name;
		std::vector<std::string> data_vec;
		ASSERT_EQ(UserHandler.executeQuery(query.c_str(), data_vec, {0, 1, 2, 3}), EXIT_SUCCESS);
		ASSERT_FALSE(data_vec.empty());
}

/* Custom query with wrong query definition and no output expected */
TEST(Execute_Custom_Query, Fails_With_Incorrect_Query_Syntax){
		std::string query = query::cmd::select + query::cl::from + table_name;
		std::vector<std::string> data_vec;
		ASSERT_EQ(UserHandler.executeQuery(query.c_str()), EXIT_FAILURE);
		ASSERT_TRUE(data_vec.empty());
}

/* Custom query extracting output */
TEST(Execute_Custom_Query, Returns_Empty_With_Incorrect_Query_Syntax){
		std::string query = query::cmd::select + query::cl::from + table_name;
		std::vector<std::string> data_vec;
		ASSERT_EQ(UserHandler.executeQuery(query.c_str(), data_vec, {0, 1, 2, 3}), EXIT_FAILURE);
		ASSERT_TRUE(data_vec.empty());
}

/*Try to extract data with custom query selecting an index where no data is stored */
TEST(Execute_Custom_Query, Returns_Empty_With_Incorrect_Data_Index){
		std::string query = query::cmd::select + query::cl::from + table_name;
		std::vector<std::string> data_vec;
		ASSERT_EQ(UserHandler.executeQuery(query.c_str(), data_vec, {5}), EXIT_FAILURE);
		ASSERT_TRUE(data_vec.empty());
}

/*******************DISCONNECTION AND CONNECTION**************************/
/* Disconnecting from the db causes no exceptions or errors */
TEST(Connection_Operations, Succeeds_Disconnect_From_DB){
		ASSERT_NO_THROW(UserHandler.closeConnection());
		ASSERT_FALSE(UserHandler.isConnected());
}

/* Disconnecting from the db while already disconnected causes no exceptions or errors */
TEST(Connection_Operations, Fails_Disconnect_When_Already_Disconnected){
		ASSERT_FALSE(UserHandler.isConnected());
		ASSERT_NO_THROW(UserHandler.closeConnection());
		ASSERT_FALSE(UserHandler.isConnected());
}

/* Trying to create a table while disconnected is not possible */
TEST(Connection_Operations, Fails_Create_Table_When_Disconnected){
		EXPECT_FALSE(UserHandler.isConnected());
		ASSERT_EQ(UserHandler.createTable("DISCONNECT", table_definition), EXIT_FAILURE);
}

/* Insertion of elements while disconnected is not possible */
TEST(Connection_Operations, Fails_Insertion_When_Disconnected){
		EXPECT_FALSE(UserHandler.isConnected());
		std::vector<std::string> values_to_insert = {"10", "86", "5654668", "Robert"};
		ASSERT_EQ(UserHandler.insertRecord(table_name, values_to_insert), EXIT_FAILURE);
}

/* Selection of elements while disconnected is not possible */
TEST(Connection_Operations, Fails_Select_When_Disconnected){
		EXPECT_FALSE(UserHandler.isConnected());
		handler::select_query_param select_options;
		select_options.table_name = table_name;
		ASSERT_EQ(UserHandler.selectRecords(select_options), handler::empty_vec);
}

/* Reconnecting to the db is possible and loads info */
TEST(Connection_Operations, Succeeds_Connect_To_DB){
		EXPECT_FALSE(UserHandler.isConnected());
		ASSERT_EQ(UserHandler.connectDb(), EXIT_SUCCESS);
		ASSERT_TRUE(UserHandler.isConnected());
		ASSERT_GT(UserHandler.getNumTables(), 0);
		ASSERT_EQ(UserHandler.getTablesNames()[0], table_name);
}

/* Reconnecting to the db is possible when already connected and throws no error */
TEST(Connection_Operations, Fails_Connect_When_Already_Connected){
		ASSERT_TRUE(UserHandler.isConnected());
		ASSERT_EQ(UserHandler.connectDb(), EXIT_FAILURE);
}

/******************************UPDATE TABLE*********************************/
/* Update a table giving the same value to a field for all records */
TEST(Update_Table, Succeeds_Update){
		std::pair<std::string, std::string> field_update = {"NAME", "New Name"};
		ASSERT_EQ(UserHandler.updateTable(table_name, {field_update}), EXIT_SUCCESS);
		std::vector<std::string> new_names = UserHandler.selectRecords(table_name, {"NAME"});
		std::vector<std::string> expected_names = {"New Name","New Name","New Name","New Name"};
		ASSERT_EQ(new_names, expected_names);
}

TEST(Update_Table, Succeeds_Update_With_Condition){
		std::pair<std::string, std::string> field_update = {"AGE", "99"};
		ASSERT_EQ(UserHandler.updateTable(table_name, {field_update}, "AGE > 40"), EXIT_SUCCESS);
		std::vector<std::string> new_ages = UserHandler.selectRecords(table_name, {"AGE"});
		std::vector<std::string> expected_ages= {"32", "99", "23", "99"};
		ASSERT_EQ(new_ages, expected_ages);
}

TEST(Update_Table, Fails_Update_When_Wrong_Table){
		std::pair<std::string, std::string> field_update = {"NAME", "New Name"};
		ASSERT_EQ(UserHandler.updateTable("WRONG_TABLE", {field_update}), EXIT_FAILURE);
}

TEST(Update_Table, Fails_Update_When_Wrong_Field){
		std::pair<std::string, std::string> wrong_field_update = {"NAMEe", "New Name"};
		ASSERT_EQ(UserHandler.updateTable(table_name, {wrong_field_update}), EXIT_FAILURE);
}

TEST(Update_Table, Fails_Update_When_Wrong_Condition){
		std::pair<std::string, std::string> field_update = {"NAME", "New Name"};
		ASSERT_EQ(UserHandler.updateTable(table_name, {field_update}, "GE == 0"), EXIT_FAILURE);
}


/**************************DELETE AND DROP OPERATIONS***********************/
/* Delete specific records of a table using condition */
TEST(Deletion_Operations, Succeeds_Delete_Record){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		ASSERT_EQ(UserHandler.deleteRecords(table_name, "ID > 7"), EXIT_SUCCESS);
}

/* Delete all records in a table */
TEST(Deletion_Operations, Succeeds_Delete_All_Records_Of_Table){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		ASSERT_EQ(UserHandler.deleteRecords(table_name, "all"), EXIT_SUCCESS);
}

/* Try to delete records of non existent table */
TEST(Deletion_Operations, Fails_Delete_Record_When_Wrong_Table){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		ASSERT_EQ(UserHandler.deleteRecords("CONECTIONS", "all"), EXIT_FAILURE);
}

/* Try to delete records with non possible condition */
TEST(Deletion_Operations, Fails_Delete_Record_When_Wrong_Condition){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		ASSERT_EQ(UserHandler.deleteRecords(table_name, "UD == 4"), EXIT_FAILURE);
}

/* Try to drop a table from the db that does not exist */
TEST(SqliteDropTable, Fails_Drop_Table_When_Wrong_Table){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		ASSERT_EQ(UserHandler.dropTable("CONECTIONS"), EXIT_FAILURE);
}

/* Drop a table from the db */
TEST(SqliteDropTable, Succeeds_Drop_Table){
		EXPECT_TRUE(exists("MyDB.db"));
		EXPECT_GT(UserHandler.getNumTables(), 0);
		ASSERT_EQ(UserHandler.dropTable(table_name), EXIT_SUCCESS);
}



int main(int argc, char *argv[]) {
		testing::InitGoogleTest(&argc, argv);
		return RUN_ALL_TESTS();
}

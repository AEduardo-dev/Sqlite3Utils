#ifndef DBQUERY_H
#define DBQUERY_H

#include <string>

/******************************************************************************
   Definition file used for easier sql query composition
******************************************************************************/

namespace query {

/*********************************************************************
                              CLAUSES
*********************************************************************/
const std::string cl_and               = "AND ";
const std::string cl_as                = "AS ";
const std::string cl_before            = "BEFORE ";
const std::string cl_between           = "BETWEEN ";
const std::string cl_count             = "COUNT ";
const std::string cl_distinct          = "DISTINCT ";
const std::string cl_exists            = "EXISTS ";
const std::string cl_for               = "FOR ";
const std::string cl_for_each          = "FOR EACH ";
const std::string cl_from              = "FROM ";
const std::string cl_glob(const std::string pattern);
const std::string cl_group_by          = "GROUP BY ";
const std::string cl_having            = "HAVING ";
const std::string cl_in                = "IN ";
const std::string cl_like(const std::string pattern);
const std::string cl_limit(int limit_value);
const std::string cl_not(const std::string clause);
const std::string cl_offset(int offset_value);
const std::string cl_on                = "ON ";
const std::string cl_or                = "OR ";
const std::string cl_order_by          = "ORDER BY ";
const std::string cl_sum               = "SUM ";
const std::string cl_values            = "VALUES ";
const std::string cl_where             = "WHERE ";

/*********************************************************************
                        DB-QUERY STATEMENTS
*********************************************************************/
const std::string cmd_add_column        = "ADD COLUMN ";
const std::string cmd_alter_table       = "ALTER TABLE ";
const std::string cmd_analyze           = "ANALYZE ";
const std::string cmd_attach_db         = "ATTACH DATABASE ";
const std::string cmd_begin             = "BEGIN ";
const std::string cmd_begin_txn         = "BEGIN EXCLUSIVE TRANSACTION ";
const std::string cmd_commit            = "COMMIT ";
const std::string cmd_create            = "CREATE ";
const std::string cmd_create_uniq_indx  = "CREATE UNIQUE INDEX";
const std::string cmd_create_table      = "CREATE TABLE ";
const std::string cmd_create_trigger    = "CREATE TRIGGER ";
const std::string cmd_create_view       = "CREATE VIEW ";
const std::string cmd_create_virtual_tbl= "CREATE VIRTUAL TABLE ";
const std::string cmd_delete            = "DELETE ";
const std::string cmd_detach_db         = "DETACH DATABASE ";
const std::string cmd_drop_indx         = "DROP INDEX ";
const std::string cmd_drop_table        = "DROP TABLE ";
const std::string cmd_drop_trigger      = "DROP TRIGGER ";
const std::string cmd_drop_view         = "DROP VIEW ";
const std::string cmd_explain           = "EXPLAIN ";
const std::string cmd_insert            = "INSERT ";
const std::string cmd_insert_into       = "INSERT INTO ";
const std::string cmd_insert_on         = "INSERT ON ";
const std::string cmd_pragma            = "PRAGMA ";
const std::string cmd_reindex           = "REINDEX ";
const std::string cmd_release_savepoint = "RELEASE ";
const std::string cmd_rename_to         = "RENAME TO ";
const std::string cmd_rollback          = "ROLLBACK";
const std::string cmd_rollback_savepoint= "ROLLBACK TO SAVEPOINT ";
const std::string cmd_savepoint         = "SAVEPOINT ";
const std::string cmd_select            = "SELECT ";
const std::string cmd_update            = "UPDATE ";
const std::string cmd_using             = "USING ";
const std::string cmd_vacuum            = "VACUUM";

/*********************************************************************
                        DATATYPES AND DB TYPES
*********************************************************************/
const std::string db_char               = "CHAR ";
const std::string db_int                = "INT ";
const std::string db_primary_key        = "PRIMARY KEY ";
const std::string db_row                = "ROW ";
const std::string db_table              = "TABLE ";

/*********************************************************************
                        ADDITIONAL OPTIONS
*********************************************************************/
const std::string opt_not_null          = "NOT NULL ";
const std::string opt_null              = "NULL ";

const std::string opt_data_len(int);
const std::string opt_table_info(const std::string);
const std::string opt_type(const std::string);

/*********************************************************************
                            SPECIALS
*********************************************************************/
const std::string trigger(const std::string db_name, const std::string trigger_name);
const std::string view(const std::string db_name, const std::string view_name);
const std::string table(const std::string db_name, const std::string table_name);
const std::string end_query             =";";

} /* macros */

#endif // DBQUERY_H

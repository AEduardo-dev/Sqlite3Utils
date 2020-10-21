#ifndef DBQUERY_H
#define DBQUERY_H

#include <string>

/******************************************************************************
   Definition file used for easier sql query composition
******************************************************************************/

namespace query {

//TODO:12/10/2020.-Instead of define use const string

/*********************************************************************
                              DATATYPES
*********************************************************************/
const std::string db_char               = "CHAR ";
const std::string db_int                = "INT ";
const std::string db_primary_key        = "PRIMARY KEY ";
const std::string db_table              = "table";
/*********************************************************************
                              OPTIONS
*********************************************************************/
const std::string opt_as                = "As ";
const std::string opt_not_null          = "NOT NULL ";
const std::string opt_null              = "NULL ";
const std::string opt_on                = "ON ";
const std::string opt_data_len(int);
const std::string opt_table_info(const std::string);
const std::string opt_type(const std::string);

/********************************************************************
                LOGICAL OPERATORS AND CONDITIONS
********************************************************************/
const std::string cond_and              = "AND ";
const std::string cond_between          = "BETWEEN ";
const std::string cond_or               = "OR ";


/*********************************************************************
                              DB-QUERY
*********************************************************************/
const std::string cmd_add_column        = "ADD COLUMN ";
const std::string cmd_alter_table       = "ALTER TABLE ";
const std::string cmd_analyze           = "ANALYZE ";
const std::string cmd_attach_db         = "ATTACH DATABASE ";
const std::string cmd_begin             = "BEGIN;";
const std::string cmd_begin_txn         = "BEGIN EXCLUSIVE TRANSACTION;";
const std::string cmd_commit            = "COMMIT;";
const std::string cmd_create            = "CREATE ";
const std::string cmd_create_uniq_indx  = "CREATE UNIQUE INDEX";
const std::string cmd_create_table      = "CREATE TABLE ";
const std::string cmd_create_trigger    = "CREATE TRIGGER ";
const std::string cmd_from              = "FROM ";
const std::string cmd_insert            = "INSERT ";
const std::string cmd_insert_into       = "INSERT INTO ";
const std::string cmd_insert_on         = "INSERT ON ";
const std::string cmd_order_by          = "ORDER BY ";
const std::string cmd_pragma            = "PRAGMA ";
const std::string cmd_rename_to         = "RENAME TO ";
const std::string cmd_select            = "SELECT ";
const std::string cmd_values            = "VALUES ";
const std::string cmd_where             = "WHERE ";

/*********************************************************************
                            SPECIALS
*********************************************************************/
const std::string trigger(const std::string db_name, const std::string trigger_name);
const std::string end_query             =";";


} /* macros */

#endif // DBQUERY_H

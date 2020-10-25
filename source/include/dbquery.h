#ifndef DBQUERY_H
#define DBQUERY_H

#include <string>
//TODO: 24/10/2020 DOCSTRING for dbquery class

/******************************************************************************
   Definition file used for easier sql query composition
******************************************************************************/

namespace query {

/*********************************************************************
                              CLAUSES
*********************************************************************/
namespace cl{
    const std::string and_              = " AND ";
    const std::string as                = " AS ";
    const std::string before            = " BEFORE ";
    const std::string between           = " BETWEEN ";
    const std::string count             = " COUNT ";
    const std::string distinct          = " DISTINCT ";
    const std::string exists            = " EXISTS ";
    const std::string for_              = " FOR ";
    const std::string for_each          = " FOR EACH ";
    const std::string from              = " FROM ";
    const std::string glob(const std::string pattern);
    const std::string group_by          = " GROUP BY ";
    const std::string having            = " HAVING ";
    const std::string in                = " IN ";
    const std::string like(const std::string pattern);
    const std::string limit(int limit_value);
    const std::string not_(const std::string clause);
    const std::string offset(int offset_value);
    const std::string on                = " ON ";
    const std::string or_               = " OR ";
    const std::string order_by          = " ORDER BY ";
    const std::string sum               = " SUM ";
    const std::string table_info(const std::string table_name);
    const std::string type(const std::string type);
    const std::string values            = " VALUES ";
    const std::string where             = " WHERE ";
} /* cl */


/*********************************************************************
                        DB-QUERY STATEMENTS
*********************************************************************/
namespace cmd{
    const std::string add_column        = " ADD COLUMN ";
    const std::string alter_table       = " ALTER TABLE ";
    const std::string analyze           = " ANALYZE ";
    const std::string attach_db         = " ATTACH DATABASE ";
    const std::string begin             = " BEGIN ";
    const std::string begin_txn         = " BEGIN EXCLUSIVE TRANSACTION ";
    const std::string commit            = " COMMIT ";
    const std::string create            = " CREATE ";
    const std::string create_uniq_indx  = " CREATE UNIQUE INDEX";
    const std::string create_table      = " CREATE TABLE ";
    const std::string create_trigger    = " CREATE TRIGGER ";
    const std::string create_view       = " CREATE VIEW ";
    const std::string create_virtual_tbl= " CREATE VIRTUAL TABLE ";
    const std::string delete_           = " DELETE ";
    const std::string detach_db         = " DETACH DATABASE ";
    const std::string drop_indx         = " DROP INDEX ";
    const std::string drop_table        = " DROP TABLE ";
    const std::string drop_trigger      = " DROP TRIGGER ";
    const std::string drop_view         = " DROP VIEW ";
    const std::string explain           = " EXPLAIN ";
    const std::string insert            = " INSERT ";
    const std::string insert_into       = " INSERT INTO ";
    const std::string insert_on         = " INSERT ON ";
    const std::string pragma            = " PRAGMA ";
    const std::string reindex           = " REINDEX ";
    const std::string release_savepoint = " RELEASE ";
    const std::string rename_to         = " RENAME TO ";
    const std::string rollback          = " ROLLBACK";
    const std::string rollback_savepoint= " ROLLBACK TO SAVEPOINT ";
    const std::string savepoint         = " SAVEPOINT ";
    const std::string select            = " SELECT ";
    const std::string update            = " UPDATE ";
    const std::string using_            = " USING ";
    const std::string vacuum            = " VACUUM";
} /* cmd */


/*********************************************************************
                        DATATYPES AND DB TYPES
*********************************************************************/
namespace data{
    const std::string char_              = " CHAR ";
    const std::string len(int);
    const std::string int_               = " INT ";
    const std::string not_null           = " NOT NULL ";
    const std::string null               = " NULL ";
    const std::string primary_key        = " PRIMARY KEY ";
    const std::string row                = " ROW ";
    const std::string table              = " TABLE ";
    const std::string trigger(const std::string db_name, const std::string trigger_name);
    const std::string view(const std::string db_name, const std::string view_name);
} /* data */
const std::string end_query             =";";

} /* query */

#endif // DBQUERY_H

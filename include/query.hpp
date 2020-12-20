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


#ifndef DBQUERY_H
#define DBQUERY_H

#include <string>
#include <vector>

/******************************************************************************
   Definition file used for easier sql query composition
******************************************************************************/
/*!
 * \brief Contains the macros for queries definitons.
 */
namespace query {
  /*!
   * \brief The data affinity definitions are stored in this namespace.
   */
    namespace affinity {
      const std::string integer           = "INTEGER";
      const std::string int_affinity      = "INT";
      const std::string text              = "TEXT";
      const std::vector<std::string> text_affinity = {"CHAR", "CLOB", "TEXT"};
      const std::string blob              = "BLOB";
      const std::string blob_affinity     = "BLOB";
      const std::string real              = "REAL";
      const std::vector<std::string> real_affinity = {"REAL", "FLOA", "DOUB"};
      const std::string numeric           = "NUMERIC";

    }

/*********************************************************************
                              CLAUSES
*********************************************************************/
  /*!
   * \brief List including all the clauses available in the sqlite3 syntax
   */
   namespace cl {

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

      /*!
       * \brief generates a GLOB sqlite3 clause with the pattern given.
       *
       * @param  pattern The pattern to be included. GLOB patterns use UNIX type wildcards
       * (* and ?). Some examples of patterns could be 'XXXX*', '*XXXX*', '?XXXX?', '????'...
       *
       * @return         The composed GLOB clause. "GLOB '[pattern]'"
       */
      const std::string glob(const std::string pattern);
      const std::string group_by          = " GROUP BY ";
      const std::string having            = " HAVING ";
      const std::string in                = " IN ";

      /*!
       * \brief generates a LIKE sqlite3 clause with the pattern given.
       *
       * @param  pattern The pattern to be included. It's wildcards consist of '%' and '_'.
       * Some examples of patterns could be 'XXXX%', '%XXXX%', '_XXXX_', '____'...
       *
       * @return         The composed LIKE clause. "LIKE '[pattern]'"
       */
      const std::string like(const std::string pattern);

      /*!
       * \brief generates a LIMIT sqlite3 clause with the value given.
       *
       * @param  limit_value Integer number defining the limit.
       *
       * @return         The composed LIMIT clause. "LIMIT [limit_value]"
       */
      const std::string limit(int limit_value);
      const std::string not_               = " NOT ";

      /*!
       * \brief generates an OFFSET sqlite3 clause with the value given.
       *
       * @param  offset_value Integer number defining the offset from where a limit clause
       * could be applied.
       *
       * @return         The composed OFFSET clause. "OFFSET [offset_value]"
       */
      const std::string offset(int offset_value);
      const std::string on                = " ON ";
      const std::string or_               = " OR ";
      const std::string order_by          = " ORDER BY ";
      const std::string set               = " SET ";
      const std::string sum               = " SUM ";

      /*!
       * \brief Generates a table_info clause used in PRAGMA statements.
       *
       * @param  table_name The name of the table that the information is needed from.
       *
       * @return            The composed table_info clause. "table_info([table_name])"
       */
      const std::string table_info(const std::string table_name);

      /*!
       * \brief Generates a type='' clause for WHERE clause.
       *
       * @param  type The type that will be added to the clause.
       *
       * @return      The composed type clause. "type='[type]'"
       */
      const std::string type(const std::string type);
      const std::string values            = " VALUES ";
      const std::string where             = " WHERE ";
    } /* cl */


/*********************************************************************
                        DB-QUERY STATEMENTS
*********************************************************************/
    /*!
     * \brief List of the commands in the sqlite3 syntax
     */
    namespace cmd {

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
    /*!
     * \brief Includes some of the different datatypes and database selectable parts.
     */
    namespace data {

      const std::string char_              = " CHAR ";
      /*!
       * \brief Generates sqlite3 definition of length for a datatype.
       *
       * @param  length Integer number to be defined as length limit.
       *
       * @return        The composed length for a datatype. "([length])"
       */
      const std::string len(int length);
      const std::string int_               = " INT ";
      const std::string not_null           = " NOT NULL ";
      const std::string null               = " NULL ";
      const std::string primary_key        = " PRIMARY KEY ";
      const std::string row                = " ROW ";
      const std::string table              = " TABLE ";

      /*!
       * \brief Generates a trigger definition with the database and trigger name given.
       *
       * @param  db_name      Name of the database where the trigger is located.
       * @param  trigger_name Name of the trigger.
       *
       * @return              The trigger declaration. "db_name.trigger_name"
       */
      const std::string trigger(const std::string db_name, const std::string trigger_name);

      /*!
       * \brief Generates a view definition with the database and view name given.
       *
       * @param  db_name      Name of the database where the view is located.
       * @param  view_name Name of the view.
       *
       * @return              The view declaration. "db_name.view_name"
       */
      const std::string view(const std::string db_name, const std::string view_name);

    } /* data */

    /*********************************************************************
                            DATATYPES AND DB TYPES
    *********************************************************************/
        /*!
         * \brief Includes some of the different datatypes and database selectable parts.
         */
        namespace pragma {

          const std::string auto_vacuum           = "auto_vacuum";
          const std::string cache_size            = "cache_size";
          const std::string case_sensitive_like   = "case_sensitive_like";
          const std::string count_changes         = "count_changes";
          const std::string database_list         = "database_list";
          const std::string encoding              = "encoding";
          const std::string freelist_count        = "freelist_count";
          const std::string index_info            = "index_info";
          const std::string index_list            = "index_list";
          const std::string journal_mode          = "journal_mode";
          const std::string max_page_count        = "max_page_count";
          const std::string page_count            = "page_count";
          const std::string page_size             = "page_size";
          const std::string parser_trace          = "parser_trace";
          const std::string recursive_triggers    = "recursive_triggers";
          const std::string schema_version        = "schema_version";
          const std::string secure_delete         = "secure_delete";
          const std::string sql_trace             = "sql_trace";
          const std::string synchronous           = "synchronous";
          const std::string temp_store            = "temp_store";
          const std::string temp_store_directory  = "temp_store_directory";
          const std::string user_version          = "user_version";
          const std::string writable_schema       = "writable_schema";

        } /* pragma */

    const std::string end_query             =";";

} /* query */

#endif // DBQUERY_H

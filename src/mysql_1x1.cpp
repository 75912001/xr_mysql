#include <mysql/mysqld_error.h>

#include <xr_error.h>
#include "mysql_1x1.h"

xr_mysql::mysql_1x1_t::mysql_1x1_t(xr_mysql::mysql_if_t *db,
								   const char *dbname_pre, const char *tablename_pre)
{
	this->db = db;
	::strcpy(this->db_name, dbname_pre);
	::strcpy(this->table_name, tablename_pre);
	::strcpy(this->db_name_pre, dbname_pre);
	::strcpy(this->table_name_pre, tablename_pre);
	this->ret = 0;
}

char *xr_mysql::mysql_1x1_t::get_table_name()
{
	::sprintf(this->db_table_name, "%s.%s", this->db_name, this->table_name);
	return this->db_table_name;
}

int xr_mysql::mysql_1x1_t::exec_update_sql(char *sql_str)
{
	int affected_rows = 0;
	int dbret = this->db->exec_update_sql(sql_str, &affected_rows);
	if (SUCC == dbret)
	{
		if (1 != affected_rows)
		{
			return xr::ERROR::DB_AFFECTED_ROWS;
		}
	}
	else
	{
		if (ER_DUP_ENTRY == dbret)
		{
			//insert时,记录已存在
			return xr::ERROR::DB_RECORDS_EXIST;
		}
		return xr::ERROR::DB_SYS;
	}
	return SUCC;
}

int xr_mysql::mysql_1x1_t::record_is_existed(char *sql_str)
{
	QUERY_ONE_BEGIN(sql_str);
	QUERY_ONE_END();
	return SUCC;
}

uint64_t xr_mysql::mysql_1x1_t::get_auto_increment_id()
{
	return (uint64_t)mysql_insert_id(&(this->db->handle));
}

int xr_mysql::mysql_1x1_t::exec_update_sqls(char *sql_str)
{
	int affected_rows = 0;
	if (SUCC == this->db->exec_update_sql(sql_str, &affected_rows))
	{
		return SUCC;
	}
	return xr::ERROR::DB_SYS;
}
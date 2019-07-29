#include <mysql/mysqld_error.h>

#include <xr_ecode.h>
#include "mysql_1x1.h"

xr::mysql_1x1_t::mysql_1x1_t(xr::mysql_if_t* db, 
	const char* dbname_pre, const char* tablename_pre) 
{ 
	this->db = db;
	::strcpy(this->db_name, dbname_pre);
	::strcpy(this->table_name, tablename_pre);
	::strcpy(this->db_name_pre, dbname_pre);
	::strcpy(this->table_name_pre, tablename_pre);
	this->ret = 0;
}

char* xr::mysql_1x1_t::get_table_name()
{
	::sprintf(this->db_table_name, "%s.%s", this->db_name, this->table_name);
	return this->db_table_name;
}

int xr::mysql_1x1_t::exec_update_sql(char* sql_str)
{
	int affected_rows = 0; 
	int dbret == this->db->exec_update_sql(sql_str, &affected_rows);
	if (SUCC == dbret){
		if (1 != affected_rows){
			return xr::ECODE_DB::DB_AFFECTED_ROWS;	
		}
	} else {
		if (ER_DUP_ENTRY == dbret){
			//insert时,记录已存在
			return  xr::ECODE_DB::RECORDS_EXIST;
		}
		return xr::ECODE_DB::SYS;
	}
	return SUCC;
}	

int  xr::mysql_1x1_t::record_is_existed(char* sql_str)
{
	STD_QUERY_ONE_BEGIN(sql_str);
	STD_QUERY_ONE_END();
	return SUCC;
}

uint64_t xr::mysql_1x1_t::get_auto_increment_id()
{
	return (uint64_t)mysql_insert_id(&(this->db->handle));
}

int xr::mysql_1x1_t::exec_update_sqls(char * sql_str)
{
	int affected_rows = 0;
	if (SUCC == this->db->exec_update_sql(sql_str, &affected_rows)){
		return SUCC;
	}
	return xr::ECODE_DB::SYS;
}
//表1x1

#pragma once

#include <xr_include.h>

#include "mysql_util.h"
#include "mysql_if.h"

namespace xr_mysql{
	class mysql_1x1_t{
	public:
		mysql_if_t* db;
		uint32_t ret;
		char sqlstr[SQL_STR_MAX_LEN];
		char db_name[DB_NAME_MAX_LEN];
		char table_name[TABLE_NAME_MAX_LEN];
		//: db.table 
		char db_table_name[DB_TABLE_NAME_MAX_LEN];

		char table_name_pre[TABLE_NAME_MAX_LEN];
		char db_name_pre[DB_NAME_MAX_LEN];
	public:
		virtual char*  get_table_name();	
		//查询一条记录是否存在
		int record_is_existed(char* sql_str);
		//更新数据(一行数据)
		int exec_update_sql(char* sql_str);
		uint64_t get_auto_increment_id();
		//影响行数为多行的接口(insert ,update ,delete,都可以使用)
		int exec_update_sqls(char* sql_str);

		mysql_1x1_t(xr::mysql_if_t* db, const char* dbname_pre, const char* tablename_pre); 
	};
}
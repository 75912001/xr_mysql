#pragma once
#include "mysql_1x1.h"

namespace xr_mysql{
	class mysql_10x10_t : public mysql_1x1_t
	{
	public:
		virtual char* get_table_name(uint32_t id);
		virtual char* get_table_name(uint64_t id);
	public:
		mysql_10x10_t(mysql_if_t* db, const char* db_name_pre, const char* table_name_pre)
			: el::mysql_1x1_t(db, db_name_pre, table_name_pre){}
	};
}
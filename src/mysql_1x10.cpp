#include "mysql_1x10.h"

char * xr_mysql::mysql_1x10::get_table_name(uint32_t id)
{
	::sprintf (this->db_table_name, "%s.%s_%01d",
	this->db_name, this->table_name_pre, id%10);
	return this->db_table_name;
}
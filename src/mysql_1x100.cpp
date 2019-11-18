#include "mysql_1x100.h"

char *xr_mysql::mysql_1x100_t::get_table_name(uint32_t id)
{
	::sprintf(this->db_table_name, "%s.%s_%02d",
			  this->db_name, this->table_name_pre, id % 100);
	return this->db_table_name;
}
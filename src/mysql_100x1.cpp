#include "mysql_100x1.h"

char *xr_mysql::mysql_100x1_t::get_table_name(uint32_t id)
{
	::sprintf(this->db_name, "%s_%02d", this->db_name_pre, id % 100);
	::sprintf(this->db_table_name, "%s.%s",
			  this->db_name, this->table_name_pre);
	return this->db_table_name;
}
#include "mysql_10x10.h"

char *xr_mysql::mysql_10x10_t::get_table_name(uint32_t id)
{
	::sprintf(this->db_name, "%s_%01d", this->db_name_pre, id % 10);

	::sprintf(this->db_table_name, "%s.%s_%01d",
			  this->db_name, this->table_name_pre, (id / 10) % 10);
	return this->db_table_name;
}

char *xr_mysql::mysql_10x10_t::get_table_name(uint64_t id)
{
	::sprintf(this->db_name, "%s_%01" PRIu64, this->db_name_pre, id % 10);

	::sprintf(this->db_table_name, "%s.%s_%01" PRIu64,
			  this->db_name, this->table_name_pre, (id / 10) % 10);
	return this->db_table_name;
}
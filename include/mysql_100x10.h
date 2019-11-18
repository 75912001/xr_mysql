#pragma once

#include "mysql_1x1.h"

namespace xr_mysql
{
class mysql_100x10_t : public mysql_1x1_t
{
protected:
	virtual char *get_table_name(uint32_t id);

public:
	mysql_100x10_t(mysql_if_t *db, const char *db_name_pre, const char *table_name_pre)
		: mysql_1x1_t(db, db_name_pre, table_name_pre) {}
};
} // namespace xr_mysql
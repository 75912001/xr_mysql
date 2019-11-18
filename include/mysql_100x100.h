//表路由100x100

#pragma once

#include "mysql_1x1.h"

namespace xr_mysql
{
class mysql_100x100_t : public mysql_1x1_t
{
protected:
	//获取DB.TABLE形式的表名
	virtual char *get_table_name(uint32_t id);

public:
	//表名的前面部分：如： t_user_pet_00 --  t_user_pet_99, 写入的是t_user_pet
	mysql_100x100_t(mysql_if_t *db, const char *db_name_pre, const char *table_name_pre)
		: mysql_1x1_t(db, db_name_pre, table_name_pre) {}
};
} // namespace xr_mysql
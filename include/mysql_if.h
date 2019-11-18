//mysql接口

#pragma once

#include <mysql/mysql.h>
#include <xr_util.h>

namespace xr_mysql
{
class mysql_if_t
{
public:
	std::string host;
	uint16_t port;
	std::string user;
	std::string pass;
	bool is_log_sql; //是否可以将sql打印到日志中
	MYSQL handle;
	mysql_if_t(const std::string &host, const std::string &user,
			   const std::string &pass, uint16_t port = 3306,
			   bool log_sql = false, const char *a_unix_socket = NULL);
	virtual ~mysql_if_t();
	void show_error_log(const char *sql);
	int select_db(char *db_name);
	int exec_query_sql(const char *sql, MYSQL_RES **result);
	int exec_update_sql(const char *sql, int *affected_rows, int len = 0);

	//提交
	int commit();
	//回滚
	int rollback();

private:
	char unix_socket[255];
	int connect_server();
	//关闭自动提交.提高性能.但是一定要手动提交.
	int close_db_autocommit();
	int execsql(const char *sql, int len = 0);
	inline const int get_errno() { return mysql_errno(&handle); }
	inline const char *get_error() { return mysql_error(&handle); }
};
} // namespace xr_mysql

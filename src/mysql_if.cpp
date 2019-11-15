#include <mysql/mysqld_error.h>
#include <mysql/errmsg.h>

#include <xr_log.h>
#include <xr_ecode.h>

#include "mysql_if.h"

#define SAFE_STRNCPY(__dst__, src) {\
	if (__src__){\
		::strncpy(__dst__, __src__, sizeof(__dst__) - 1); \
		__dst__[sizeof(__dst__) - 1] = '\0'; \
	} else {\
		__dst__[0] = '\0'; \
	}\
}

xr_mysql::mysql_if_t::mysql_if_t (const std::string& host, const std::string& user,
								  const std::string& pass,
								  uint16_t port, bool log_sql, const char * a_unix_socket)
{
	this->host = host;
	this->user = user;
	this->pass = pass;
	this->port = port;
	this->is_log_sql = log_sql;
	::memset(this->unix_socket, 0, sizeof(this->unix_socket));

	mysql_init(&handle);

	if (NULL != a_unix_socket) {
		SAFE_STRNCPY(this->unix_socket, a_unix_socket);
	}

	INFOR_LOG("======DB conn msg [host:%s][user:%s][pass:%s][port:%u][is_log_sql:%u][unix_socket:%s]======",
		this->host.c_str(), this->user.c_str(), this->pass.c_str(), 
		this->port, this->is_log_sql, this->unix_socket);
	this->connect_server();
}

void  xr_mysql::mysql_if_t::show_error_log(const char* sql) 
{
	ERROR_LOG("sqlerr[errno:%d, error:%s, sql:%s]", this->get_errno(), this->get_error(), sql);
}

xr_mysql::mysql_if_t::~mysql_if_t ()
{
	mysql_close(&this->handle);
}

int xr_mysql::mysql_if_t::connect_server ()
{
	char* us = NULL;
	if ('\0' != this->unix_socket[0]){
		us = this->unix_socket;
	}

	my_bool reconnect = true;
	mysql_options(&this->handle, MYSQL_OPT_RECONNECT, (const char *)&reconnect);

	//CLIENT_FOUND_ROWS	//Return the number of found (matched) rows, not the number of changed rows.
	unsigned long flag = CLIENT_FOUND_ROWS;
	if (NULL == mysql_real_connect(&this->handle, this->host.c_str(), 
		this->user.c_str(), this->pass.c_str(), NULL, this->port, us, flag)){
		ALERT_LOG("db connect is err [code:%d, %s]\n", this->get_errno(), this->get_error());
		return el::ERR_DB::SYS;
	}
	INFOR_LOG("db connect is ok [code:%d]\n", this->get_errno());

	INFOR_LOG("New client character set: %s", mysql_character_set_name(&this->handle));
	int t_ret = mysql_set_character_set(&this->handle, "utf8");
	if (0 == t_ret){
		INFOR_LOG("New client character set: %s", mysql_character_set_name(&this->handle));
	} else {
		INFOR_LOG("New client character set: %s, %d",
			mysql_character_set_name(&this->handle), t_ret);
	}
	return this->close_db_autocommit();
}

int xr_mysql::mysql_if_t::exec_query_sql (const char* sql, MYSQL_RES** result)
{
	if (SUCC == this->execsql(sql)){
		if(NULL == (*result = mysql_store_result(&this->handle))){
			this->show_error_log(sql);
		} else {
			return SUCC;
		}
	}
	return xr::ERROR::DB_SYS;
}

int xr_mysql::mysql_if_t::exec_update_sql(const char* sql, int* affected_rows, int len)
{
	if (SUCC == this->execsql(sql, len)){
		*affected_rows = (int)mysql_affected_rows(&(this->handle));
		return SUCC;
	}
	return this->get_errno();
}

int el::mysql_if_t::execsql(const char* sql, int len)
{
	if (SUCC != this->is_log_sql){
		TRACE_LOG("sql:[%s]", sql);
	}
	if(0 == len){
		len = strlen(sql) + 1;
	}

	int ret = mysql_real_query(&this->handle, sql, len);
	if (SUCC == ret){//SUCC
		return ret;
	} else if (CR_SERVER_GONE_ERROR == this->get_errno()){//server go away, must reconnect
		if (SUCC == this->connect_server()){
			ret = mysql_query(&this->handle, sql);
			if (SUCC != ret){
				ret = this->get_errno(); 
			}
		}else{
			ret = this->get_errno(); 
		}
	}else{
		this->show_error_log(sql);
		ret = this->get_errno();
	}	
	return ret;
}

int xr_mysql::mysql_if_t::select_db(char* db_name)
{
	int ret = mysql_select_db(&this->handle, db_name);
	if (SUCC != ret) {
		this->show_error_log(db_name);
	}
	return this->get_errno();
}

int el::mysql_if_t::close_db_autocommit()
{
	if (SUCC != mysql_autocommit(&this->handle, 0)){
		ALERT_LOG("db mysql_autocommit close is err [code:%d, %s]\n", this->get_errno(), this->get_error());
		return xr::ERROR_DB::SYS;
	}
	return SUCC;
}

int xr_mysql::mysql_if_t::commit()
{
	if (SUCC != mysql_commit(&this->handle)){
		ALERT_LOG("db mysql_commit is err [code:%d, %s]\n", this->get_errno(), this->get_error());
		return xr::ERROR_DB::SYS;
	}
	return SUCC;
}

int xr_mysql::mysql_if_t::rollback()
{
	mysql_rollback(&this->handle);
	return SUCC;
}


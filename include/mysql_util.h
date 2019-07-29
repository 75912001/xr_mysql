#pragma once

#include <mysql/mysql.h>
#include <xr_util.h>

#define  GEN_SQLSTR(__sqlstr__, ...)  ::snprintf(__sqlstr__, sizeof(__sqlstr__), __VA_ARGS__)
const uint32_t SQL_STR_MAX_LEN = 16384;//sql str 最大长度	
const uint32_t DB_NAME_MAX_LEN = 50;
const uint32_t TABLE_NAME_MAX_LEN = 50;
const uint32_t DB_TABLE_NAME_MAX_LEN = DB_NAME_MAX_LEN + TABLE_NAME_MAX_LEN;

#define MYSQL_STR_LEN(__n__) ((__n__) * 2 + 1)

#define SET_MYSQL_STRING(__dst__, __src__, __n__) \
	mysql_real_escape_string(&(this->db->handle), __dst__, __src__, __n__)

#define GEN_MYSQL_STRING(__new_name__, __val_name__, __val_len__) \
	char __new_name__[MYSQL_STR_LEN(__val_len__)];\
	memset(__new_name__, 0, sizeof(__new_name__));\
	SET_MYSQL_STRING(__new_name__, __val_name__, __val_len__);

//依次得到row[i]	
// 在STD_QUERY_WHILE_BEGIN  和 STD_QUERY_ONE_BEGIN	
#define NEXT_FIELD 	 (row[++_fi])

#define GET_FIELD_UINT64(__out__)\
	(__out__) = (decltype(__out__))xr::atoll_safe(NEXT_FIELD)

#define GET_NEXT_FIELD_INT_WITH_TYPE(__out__, __type__)\
	(__out__) = (__type__)xr::atoll_safe(NEXT_FIELD)

#define GET_NEXT_FIELD_BIN(__out__, __max_len__)\
{\
	++_fi;\
	mysql_fetch_lengths(res);\
	uint32_t real_len = res->lengths[_fi] < __max_len__ ? res->lengths[_fi] : __max_len__;\
	::memcpy(__out__, row[_fi], real_len);\
}

#define GET_NEXT_FIELD_BIN_REAL_LEN(__out__, __max_len__, __real_len__)\
{\
	++_fi;\
	mysql_fetch_lengths(res);\
	__real_len__ = res->lengths[_fi] < __max_len__ ? res->lengths[_fi] : __max_len__;\
	::memcpy(__out__, row[_fi], __real_len__);\
}

#define GET_NEXT_FIELD_STRING(__out__, __max_len__)\
{\
	++_fi;\
	mysql_fetch_lengths(res);\
	uint32_t real_len = res->lengths[_fi] < __max_len__ ? res->lengths[_fi] : __max_len__;\
	__out__.append(row[_fi], real_len);\
}

#define STD_QUERY_ONE_BEGIN(__sqlstr__) {\
	MYSQL_RES* res = NULL;\
	MYSQL_ROW row;\
	uint32_t ret = this->db->exec_query_sql(__sqlstr__, &res);\
	if (SUCC == ret){\
		auto rowcount = mysql_num_rows(res);\
		if (1 != rowcount) {\
			mysql_free_result(res);\
			return xr::ECODE_DB::AFFECTED_ROWS;\
		}else {\
			row = mysql_fetch_row(res);\
			int _fi;\
			_fi = -1;

#define STD_QUERY_ONE_END()\
			mysql_free_result(res);\
		}\
	}else {\
		return xr::ECODE_DB::SYS;\
	}\
}

//----------------------列表模式-----------------------------	
#define STD_QUERY_WHILE_BEGIN( __sqlstr__, __item_list__ )  \
{ \
	decltype(__item_list__)& _item_list = __item_list__;\
	MYSQL_RES* res = NULL;\
	MYSQL_ROW row;\
	if (SUCC == (this->db->exec_query_sql(__sqlstr__, &res))){\
		while((row = mysql_fetch_row(res))){\
			decltype(__item_list__[0]) _item ;\
			int _fi;\
			_fi = -1;

#define STD_QUERY_WHILE_END()  \
			_item_list.push_back(_item);\
		}\
		mysql_free_result(res);	\
	}else {\
		return xr::ECODE_DB::SYS;\
	}\
}

#define STD_QUERY_WHILE_END_FOR_SET()  \
			_item_list.insert(_item);\
		}\
		mysql_free_result(res);	\
	}else {\
		return xr::ECODE_DB::SYS;\
	}\
}

//----------------------用于protobuf的列表模式-----------------------------	
#define PB_STD_QUERY_WHILE_BEGIN(__sqlstr__)  \
{ \
	MYSQL_RES* res = NULL;\
	MYSQL_ROW row;\
	if (SUCC == (this->db->exec_query_sql(__sqlstr__, &res))){\
		while((row = mysql_fetch_row(res))){\
			int _fi;\
			_fi = -1;

#define PB_STD_QUERY_WHILE_END()  \
		}\
		mysql_free_result(res);	\
	}else {\
		return xr::ECODE_DB::SYS;\
	}\
}

#define STD_QUERY_ONE_END_WITHOUT_RETURN()  \
				mysql_free_result(res);		 \
			} \
		}else { \
			return xr::ECODE_DB::SYS;	 \
		}\
	}
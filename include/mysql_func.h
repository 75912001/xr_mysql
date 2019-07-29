#pragma once

#include <xr_tcp.h>
#include <proto_header.h>
#include <if.h>
#include "mysql_if.h"

class Cfunc_route_base 
{
public:
	xr::mysql_if_t* db;//db 连接
	xr_server::PROTO_CMD cmd;/*用于保存当前命令号，只是为了方便 */
public:
    xr::tcp_peer_t* peer;           //连接信息
    int ret;                        //用于保存操作返回值，只是为了方便
    xr_server::proto_head_t head;   //包头
    unsigned char data[1024*1024*8];         //发送缓冲区
public:
	Cfunc_route_base(el::mysql_if_t* db){
		this->db = db;
		this->cmd = 0;
        this->peer = NULL;
        this->ret = 0;
	}

    inline int s2peer(uint32_t len){
        return xr_server::s2peer(this->peer, this->data, len);
    }

    inline int send_ret(xr_server::proto_head_t& proto_head, xr_server::PROTO_RET ret){
        return xr_server::send_ret(this->peer, proto_head, ret);
    }
};
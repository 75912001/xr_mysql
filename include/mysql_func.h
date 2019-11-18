#pragma once

#include <xr_tcp.h>
#include <proto_header.h>
#include <if.h>
#include "mysql_if.h"

class func_route_base_t
{
public:
    xr_mysql::mysql_if_t *db; //db 连接
public:
    xr::tcp_peer_t *peer;                //连接信息
    xr_server::proto_head_t head;        //包头
    unsigned char data[1024 * 1024 * 8]; //发送缓冲区
    uint32_t data_len;                   //发送缓冲区长度
public:
    func_route_base_t(xr_mysql::mysql_if_t *db)
    {
        this->db = db;
        this->peer = NULL;
        this->data_len = 0;
    }

    inline int s2peer()
    {
        return xr_server::s2peer(this->peer, this->data, this->data_len);
    }

    inline int send_ret(xr_server::proto_head_t &proto_head, xr_server::PROTO_RET ret)
    {
        return xr_server::send_ret(this->peer, proto_head, ret);
    }
};
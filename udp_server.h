#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>&msg
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <map>
//#include <unistd.h>
#include "comm.h"
#include "data_pool.h"
#include "udp_data.h"

#define default_ip  "127.0.0.1"
#define default_port 8080

class udp_server{

	void add_user(std::string &_key_ip, struct sockaddr_in &client);
	void del_user(std::string &_key_ip, std::string &msg);

	public:
		void init();
		udp_server(const std::string &_ip = default_ip, const short &_port = default_port);

		int recv_data(); // 1->1
		int send_data(struct sockaddr_in &_client, socklen_t len, std::string &_msg);// 1-
int s>1
		int broadcast_data();// 1->n

		~udp_server();
	private:
		std::string ip;
		short port;
		int sock;

		std::map<std::string, struct sockaddr_in > online_user;//将收到的数据广播给online user
		data_pool msg_pool;//将受到的数据投递到数据池
};









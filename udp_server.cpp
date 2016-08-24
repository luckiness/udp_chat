#include "udp_server.h"

using namespace std;

udp_server::udp_server(const string &_ip, const short &_port)
	:ip(_ip)
	,port(_port)
	,sock(-1)


	,msg_pool(64)
{}

void udp_server::init()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if( sock < 0 ){
		print_log(strerror(errno), __FUNCTION__, __LINE__);
		exit(1);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	if( ip == "any"){
		local.sin_addr.s_addr = htonl(INADDR_ANY);
	}else{
		local.sin_addr.s_addr = inet_addr(ip.c_str());
	}
	socklen_t len = sizeof(local);
	if( bind(sock, (struct sockaddr*)&local, len) < 0 ){
		print_log(strerror(errno), __FUNCTION__, __LINE__);
		exit(2);
	}
}

void udp_server::add_user(std::string &_key_ip, struct sockaddr_in &client)
{
	std::map<std::string, struct sockaddr_in>::iterator _iter = online_user.find( _key_ip );
	if( _iter == online_user.end() ){
		//add
		//std::cout<<"add new user done... "<<_key_ip<<std::endl;
		online_user.insert(std::pair<std::string, struct sockaddr_in>(_key_ip, client));
	}else{
		return;
	}
}

void udp_server::del_user(std::string &_key_ip, std::string &msg)
{
	std::string _name;
	std::string _school;
	std::string _msg;
	std::string _cmd;

	udp_data data;
	data.to_val(_name,_msg, _school,_cmd, msg);
	if( _cmd == "QUIT"){
		online_user.erase(_key_ip);
	}
}


int udp_server::recv_data() // 1->1
{
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	char buf[_SIZE_];
	buf[0] = '\0';
	ssize_t _s = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&client, &len);
	if( _s > 0 ){
		buf[_s] = '\0';
		std::string _out = buf;
		msg_pool.data_put(_out);
		print_log(_out.c_str(), __FUNCTION__, __LINE__);

		//client.sin_addr.s_addr->XXX.XXX.XXX.XXX
		std::string _key_ip = inet_ntoa(client.sin_addr);
		print_log(_key_ip.c_str(), __FUNCTION__, __LINE__);
		add_user(_key_ip, client);
		del_user(_key_ip, _out);
	}else if( _s == 0 ){
		const char *msg = "client is close";
		print_log(msg , __FUNCTION__, __LINE__);
	}else{
		print_log(strerror(errno), __FUNCTION__, __LINE__);
	}
	return _s;
}

int udp_server::send_data(struct sockaddr_in &_client, socklen_t len, std::string &_msg)// 1->1
{
	ssize_t _s = sendto(sock, _msg.c_str(), _msg.size(), 0, (struct sockaddr*)&_client, len);
	if( _s < 0 ){
		print_log(strerror(errno), __FUNCTION__, __LINE__);
	}
	return _s;
}

int udp_server::broadcast_data()// 1->n
{
	std::string _msg;
	msg_pool.data_get(_msg);
	std::map<std::string, struct sockaddr_in>::iterator _iter = online_user.begin();
	for( ; _iter != online_user.end(); _iter++){
		send_data( _iter->second, sizeof(_iter->second), _msg);// 1->1
	}
	return 0;
}

udp_server::~udp_server()
{
	if( sock > 0 ){
		close(sock);
	}
}




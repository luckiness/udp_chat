#include "udp_server.h"

using namespace std;
//生产并且接受数据
void *recv_data(void *arg)
{
	udp_server *_ser = (udp_server*)arg;
	while(1){
		_ser->recv_data();
	}
}
//消费数据
void* broadcast_data(void *arg)
{
	udp_server *_ser = (udp_server*)arg;
	while(1){
		_ser->broadcast_data();// 1->n
	}
}

static void usage(const char *proc)
{
	std::cout<<"Usage: "<<proc<<" [ip|any] [port]"<<std::endl;
}

int main(int argc, char *argv[])
{
	if( argc != 3){
		usage(argv[0]);
		exit(1);
	}
	std::string _ip = argv[1];
	short _port = atoi(argv[2]);

	//daemon(0, 0);

	udp_server _ser( _ip, _port );
	_ser.init();
	std::string _msg;
	pthread_t th1, th2;
	pthread_create(&th1, NULL, recv_data, (void*)&_ser);
	pthread_create(&th2, NULL, broadcast_data, (void*)&_ser);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

//	while(1){
//	    _ser.recv_data();
//		_ser.broadcast_data();// 1->n
//	}
	return 0;
}

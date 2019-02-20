#include "csocket.h"

int Init_Socket(int domain,int type,int protocol)
{
	int sock = socket(domain, type, protocol);
	if (sock == -1) {
		perror("socket create error:");
		return -1;
	}

	//设置地址复用
	Reuseaddr_Socket(sock);

	//套接字设置非阻塞模式
	//Setnonblocking_Socket(sock);

	//绑定ip端口
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	socklen_t addr_len = sizeof(addr);
	int ret = bind(sock, (const struct sockaddr*)&addr, addr_len);
	if (ret == -1) {
		perror("bind error");
		close(sock);
		return -1;
	}

	ret = listen(sock, SOMAXCONN);
	if (ret == -1) {
		perror("listen error");
		close(sock);
		return -1;
	}
	return sock;
}

//开始监听网络连接
int Accpet_Socket(int sock)
{
	if (sock == -1) {
		perror("Accpet_Server error:");
		return -1;
	}
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(struct sockaddr);
	int client_sock = accept(sock, (struct sockaddr*)&clientaddr, &len);
	if (client_sock == -1) {
		perror("Accept_server error accept:");
		return -1;
	}

	//地址复用
	Reuseaddr_Socket(client_sock);

	printf("客户端IP:%s\n", inet_ntoa(clientaddr.sin_addr));
	printf("客户端端口:%d\n", ntohs(clientaddr.sin_port));

	return client_sock;
}


//非阻塞
int Setnonblocking_Socket(int sock)
{
	int ret = fcntl(sock, F_SETFL, O_NONBLOCK);
	if (ret == -1) {
		perror("Setnonblocking_Server");
		return -1;
	}
	return 0;
}

//地址复用
int Reuseaddr_Socket(int sock)
{
	if (sock == -1) {
		perror("Reuseaddr_Server error:");
		return -1;
	}
	int opt = -1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
	return 0;
}
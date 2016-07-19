#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "msg_process.h"

int skfd = -1;
int debug_fd = -1; // 调试文件

int main(int argc, char *argv[])
{
	char reg_msg[50] = {'\0'};
	/*判断特殊输入*/
	if (argc != 6)
	{
		printf("Usage: ./%s server_ip server_port client_ip client_port client_id\n", argv[0]);
		return -1;
	}
	/*新建调试文件*/ 
	if((debug_fd = open("/home/game/game/debug.txt",O_RDWR | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR | S_IXUSR)) == -1)
	{
		printf("Err: debug file doesn't created\n");
		exit(-1);
	}
	/* 获取输入参数 */
	in_addr_t server_ip = inet_addr(argv[1]);
	in_port_t server_port = htons(atoi(argv[2])); 
	in_addr_t client_ip = inet_addr(argv[3]);
	in_port_t client_port = htons(atoi(argv[4])); 
	int client_id = atoi(argv[5]);
	
	if(0>server_port)
	{
		printf("Usage:%s hostname portnumber\n",argv[1]);
		return -1;
	}
	if(0>client_port)
	{
		printf("Usage:%s clientname portnumber\n",argv[3]);
		return -1;
	}
	
	/* 创建socket */
	skfd = socket(AF_INET, SOCK_STREAM, 0);
	if(skfd < 0)
	{
		printf("init socket failed!\n");
		return -1;
	}
	
	/* 设置socket选项，地址重复使用，防止程序非正常退出，下次启动时bind失败 */
	int is_reuse_addr = 1;
	setsockopt(skfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&is_reuse_addr, sizeof(is_reuse_addr));
	
	/* 绑定指定ip和port，不然会被server拒绝 */
	struct sockaddr_in client_addr;
	bzero(&client_addr, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = client_ip;
	client_addr.sin_port = client_port;
	if(bind(skfd, (struct sockaddr*)&client_addr, sizeof(client_addr)))
	{
		printf("bind failed!\n"); 
		return -1;
	}
	
	/* 连接server */
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = server_ip;
	server_addr.sin_port = server_port;
	while(connect(skfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		usleep(100000); /* sleep 100ms, 然后重试，保证无论server先起还是后起，都不会有问题 */
	}
	
	/* 向server注册 */
	snprintf(reg_msg, sizeof(reg_msg) - 1, "reg: %d %s need_notify \n", client_id, "HuaweiCode"); 
	send(skfd, reg_msg, strlen(reg_msg) + 1, 0);
	
	/* 接收server消息，进入游戏 */
	while(1)
	{
		char buffer[1024] = "\0";
		int length = recv(skfd, buffer, sizeof(buffer) - 1, 0);
		if(length > 0)
		{ 
			/* on_server_message返回-1（比如收到game over消息），则跳出循环，关闭socket，安全退出程序 */
			if (-1 == process(length, buffer))
			{
				break;
			}
		} 
	}
	
	/* 关闭socket */
	close(skfd);	
	close(debug_fd);
	return 0;
}


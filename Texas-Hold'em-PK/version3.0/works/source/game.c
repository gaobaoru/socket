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
int debug_fd = -1; // �����ļ�

int main(int argc, char *argv[])
{
	char reg_msg[50] = {'\0'};
	/*�ж���������*/
	if (argc != 6)
	{
		printf("Usage: ./%s server_ip server_port client_ip client_port client_id\n", argv[0]);
		return -1;
	}
	/*�½������ļ�*/ 
	if((debug_fd = open("/home/game/game/debug.txt",O_RDWR | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR | S_IXUSR)) == -1)
	{
		printf("Err: debug file doesn't created\n");
		exit(-1);
	}
	/* ��ȡ������� */
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
	
	/* ����socket */
	skfd = socket(AF_INET, SOCK_STREAM, 0);
	if(skfd < 0)
	{
		printf("init socket failed!\n");
		return -1;
	}
	
	/* ����socketѡ���ַ�ظ�ʹ�ã���ֹ����������˳����´�����ʱbindʧ�� */
	int is_reuse_addr = 1;
	setsockopt(skfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&is_reuse_addr, sizeof(is_reuse_addr));
	
	/* ��ָ��ip��port����Ȼ�ᱻserver�ܾ� */
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
	
	/* ����server */
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = server_ip;
	server_addr.sin_port = server_port;
	while(connect(skfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		usleep(100000); /* sleep 100ms, Ȼ�����ԣ���֤����server�����Ǻ��𣬶����������� */
	}
	
	/* ��serverע�� */
	snprintf(reg_msg, sizeof(reg_msg) - 1, "reg: %d %s need_notify \n", client_id, "HuaweiCode"); 
	send(skfd, reg_msg, strlen(reg_msg) + 1, 0);
	
	/* ����server��Ϣ��������Ϸ */
	while(1)
	{
		char buffer[1024] = "\0";
		int length = recv(skfd, buffer, sizeof(buffer) - 1, 0);
		if(length > 0)
		{ 
			/* on_server_message����-1�������յ�game over��Ϣ����������ѭ�����ر�socket����ȫ�˳����� */
			if (-1 == process(length, buffer))
			{
				break;
			}
		} 
	}
	
	/* �ر�socket */
	close(skfd);	
	close(debug_fd);
	return 0;
}


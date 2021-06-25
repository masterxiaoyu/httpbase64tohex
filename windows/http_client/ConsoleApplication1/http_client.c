#include<winsock2.h> 
#include<windows.h>   
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#pragma comment(lib,"ws2_32")  

#define BUFFZER_SIZE 4096
#define SERVERADDR "127.0.0.1"
#define RESPONSE_HEADER "HTTP/1.1\r\nAccept-Language:zh-cn\r\n\r\n"
#define RESPONSE_BODY "<Welcome!>\n"

int main(int argc, char* argv[]) {
    int i;
	int n;
    SOCKET fd;          
    SOCKADDR_IN server_addr;
    WSADATA wsadata;
	char Buffer[8193];
	char *head;
	char *aa;
	char *bb;
	char t[BUFFZER_SIZE];
	
	memset(t,0,BUFFZER_SIZE);
	memset(Buffer,0,BUFFZER_SIZE);
	head=argv[1];
	aa=argv[2];
	bb=argv[3];
	
    i = WSAStartup(0x0202, &wsadata);
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    
    server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr(SERVERADDR);
	server_addr.sin_port = htons(6666);

    //bind(fd, (struct sockaddr*)&addr, sizeof(SOCKADDR_IN));

	while(1)
	{
    connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
	if(strcmp(head,"POST")==0)
	{
		appect_post(head,aa,bb,t);
		send(fd,t,sizeof(t),0);
		recv(fd, Buffer, 8192, 0);
		printf("%s\n",Buffer);
	}
	if(strcmp(head,"GET")==0)
	{
		appect_get(head,aa,bb,t);
		send(fd,t,sizeof(t),0);
		recv(fd, Buffer, 8192, 0);
		printf("%s\n",Buffer);
	}
    //closesocket(fd);
    system("pause");
	}
    return 0;
}

int appect_post(char *HEAD,char *AA,char *BB,char *temp)
{
	char buf1[BUFFZER_SIZE]=RESPONSE_HEADER;
	char buf2[BUFFZER_SIZE];
	int body_len;

	memset(temp,0,BUFFZER_SIZE);
	memset(buf2,0,BUFFZER_SIZE);

	strcpy(temp,HEAD);
	strcat(temp," ");
	strcat(temp,buf1);
	strcat(temp,"Content-Length: ");
	body_len=strlen("a=")+strlen("&b=")+strlen(AA)+strlen(BB);
	sprintf(buf2,"%d",body_len);
	strcat(temp,buf2);
	strcat(temp,"\r\n\r\n");
	strcat(temp,"a=");
	strcat(temp,AA);
	strcat(temp,"&b=");
	strcat(temp,BB);
	strcat(temp,"\n");
}

int appect_get(char *HEAD,char *AA,char *BB,char *temp)
{
	char buf3[BUFFZER_SIZE];
	int body_len;
	memset(temp,0,BUFFZER_SIZE);
	strcpy(temp,HEAD);
	strcat(temp," ");
	strcat(temp,"http://localhost:8888?a=");
	strcat(temp,AA);
	strcat(temp,"&b=");
	strcat(temp,BB);
	strcat(temp," ");
	strcat(temp,"HTTP/1.1\r\nAccept-Language:zh-cn\r\n");
	strcat(temp,"Content-Length: ");
	body_len=strlen(RESPONSE_BODY)-1;
	sprintf(buf3,"%d",body_len);
	strcat(temp,buf3);
	strcat(temp,"\r\n\r\n");
	strcat(temp,RESPONSE_BODY);	
}
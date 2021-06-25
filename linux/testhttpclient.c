#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8888
#define BUFFZER_SIZE 4096
#define RESPONSE_HEADER "http://localhost:8888 HTTP/1.1\r\nAccept-Language:zh-cn\r\n"
#define RESPONSE_BODY "<Welcome!>\n"
int main(int argc,char *argv[])
{
	if(argc != 4)
	{
		printf("input error\n");
		return -1;
	}
	//char *str;
	//str=argv[4];
	char *aa;
	aa=argv[2];
	char *bb;
	bb=argv[3];
	char *head;
	head=argv[1];
	char buf[1024]={0};
	int port = PORT;
	char t[BUFFZER_SIZE];
	/*char buf1[BUFFZER_SIZE]=RESPONSE_HEADER;
	char temp[BUFFZER_SIZE];
	strcpy(temp,buf1);
	strcat(temp,"a=")
	strcat(temp,aa);
	strcat(temp,"&b=");
	strcat(temp,bb);
	strcat(temp,'\n');
	strcat(temp,str);
	*/
	struct sockaddr_in client_sockaddr;
	struct sockaddr_in server_sockaddr;
	socklen_t length = sizeof(struct sockaddr_in);
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);	
	connect(sockfd,(struct sockaddr*)&server_sockaddr,length);
	if(strcmp(head,"POST")==0)
	{
		appect_post(head,aa,bb,t);
		send(sockfd,t,sizeof(t),0);
	recv(sockfd,buf,sizeof(buf),0);
	printf("%s\n",buf);	
	}
	if(strcmp(head,"GET")==0)
	{
		appect_get(head,aa,bb,t);
		send(sockfd,t,sizeof(t),0);
	recv(sockfd,buf,sizeof(buf),0);
	printf("%s\n",buf);	
		
	}
	//send(sockfd,temp,sizeof(temp),0);
//send(sockfd,RESPONSE_HEADER RESPONSE_BODY,sizeof(RESPONSE_HEADER RESPONSE_BODY),0);
	//close(sockfd);
	return 0;
}

int appect_post(char *HEAD,char *AA,char *BB,char *temp)
{
	char buf1[BUFFZER_SIZE]=RESPONSE_HEADER;
	char buf2[BUFFZER_SIZE];
	strcpy(temp,HEAD);
	strcat(temp," ");
	strcat(temp,buf1);
	strcat(temp,"Content-Length: ");
	int body_len=strlen("a=")+strlen("&b=")+strlen(AA)+strlen(BB);
	sprintf(buf2,"%d",body_len);
	strcat(temp,buf2);
	strcat(temp,"\r\n\r\n");
	strcat(temp,"a=");
	strcat(temp,AA);
	strcat(temp,"&b=");
	strcat(temp,BB);
	strcat(temp,"\n");
	//strcat(temp,STR);
	
}

int appect_get(char *HEAD,char *AA,char *BB,char *temp)
{
	char buf1[BUFFZER_SIZE]="http://localhost:8888?a=";
	char buf2[BUFFZER_SIZE]="HTTP/1.1\r\nAccept-Language:zh-cn\r\n";
	char buf3[BUFFZER_SIZE];
	strcpy(temp,HEAD);
	strcat(temp," ");
	strcat(temp,buf1);
	strcat(temp,AA);
	strcat(temp,"&b=");
	strcat(temp,BB);
	strcat(temp," ");
	strcat(temp,buf2);
	strcat(temp,"Content-Length: ");
	int body_len=strlen(RESPONSE_BODY)-1;
	sprintf(buf3,"%d",body_len);
	strcat(temp,buf3);
	strcat(temp,"\r\n\r\n");
	strcat(temp,RESPONSE_BODY);	
}

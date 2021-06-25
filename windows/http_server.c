#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include<winsock2.h>
#include <WS2tcpip.h>
#include<windows.h>  
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"./libtest.lib")
//#pragma comment(lib,"./Win32Project2.lib")


#define BUFFER_SIZE 4096
#define RESPONSE_HEADER "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n"

int der_to_pem (char *der,int der_len,char *pem,int *pem_len);
int pem_to_der(char *pem,int pem_len,char *der,int *der_len);
int bcd_to_hex(char *bcd,int len,char *hex);

int main(int argc,char *argv[])
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	SOCKADDR_IN addrSrv;
	SOCKET sockSrv;
	SOCKADDR_IN addrClient;
	int len;
	char recvBuf[1000];
	char buf1[BUFFER_SIZE];
	char buf2[BUFFER_SIZE];
	char buf3[BUFFER_SIZE];
	char buf4[BUFFER_SIZE];
	char temp[BUFFER_SIZE];
	wVersionRequested = MAKEWORD( 1, 1);
	memset(recvBuf,0,sizeof(recvBuf));
	memset(temp,0,sizeof(temp));
	memset(buf1,0,BUFFER_SIZE);
	memset(buf2,0,BUFFER_SIZE);
	memset(buf3,0,BUFFER_SIZE);
	memset(buf4,0,BUFFER_SIZE);
	err = WSAStartup( wVersionRequested, &wsaData);
	if(err != 0)
	{
		return ;
	}
	if(LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return ;
	}

	sockSrv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addrSrv.sin_addr.S_un.S_addr =INADDR_ANY;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6666);

	bind(sockSrv,(SOCKADDR *)&addrSrv,sizeof(addrSrv));

	listen(sockSrv,SOCK_STREAM);

	len = sizeof(addrClient);

	while(1)
	{
		SOCKET sockConn = accept(sockSrv, (SOCKADDR *)&addrClient,&len);
		printf(">>client$\t%s:%d connected!\n",inet_ntoa(addrClient.sin_addr),ntohs(addrClient.sin_port));
		 
		recv(sockConn,recvBuf,sizeof(recvBuf),0);
		printf("%s\n",recvBuf);
		if(strncmp(recvBuf,"POST",3)==0)
		{
			accept_post(recvBuf,buf3,buf4);
			sprintf(buf1,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n");
			sprintf(buf2,"Content-Length: ");
			strcpy(temp,buf1);
			strcat(temp,buf2);
			strcat(temp,buf3);
			strcat(temp,"\r\n\r\n");
			strcat(temp,buf4);
			send(sockConn,temp,sizeof(temp),0);
		}

		if(strncmp(recvBuf,"GET",3)==0)
		{
			sprintf(buf1,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n");
			sprintf(buf2,"Content-Length: ");
			accept_get(recvBuf,buf3,buf4);
			strcpy(temp,buf1);
			strcat(temp,buf2);
			strcat(temp,buf3);
			strcat(temp,"\r\n\r\n");
			strcat(temp,buf4);
			send(sockConn,temp,sizeof(temp),0);
		
		}
		//closesocket(sockConn);
		system("pause");
	}
}	

int accept_post(char *ret,char *M,char *BUF)
{
	char *p=NULL;
	char *s=NULL;
	char *t=NULL;
	char *k=NULL;
	int m,n,i,cnt,derlen,len;
	char x[BUFFER_SIZE];
	char y[BUFFER_SIZE];
	char z[BUFFER_SIZE];
    unsigned char Der[BUFFER_SIZE];
	memset(Der,0,BUFFER_SIZE);
	memset(x,0,BUFFER_SIZE);
	memset(y,0,BUFFER_SIZE);
	memset(z,0,BUFFER_SIZE);
	p=strstr(ret,"=");
	s=strstr(ret,"&");    
	t=strstr(ret,"Content-Length: ");
	k=strstr(t,"\r");
	m=k-t;

	strncpy(z,(t+16),(m-16));
	i=atoi(z);

	n=s-(p+1);
	strncpy(x,(p+1),n);
	strncpy(y,(s+3),(i-8));
	
	if(strcmp(x,"pem")==0)
	{
		pem_to_der(y,strlen(y),Der,&derlen);
		sprintf(M,"%d",derlen);
		for(cnt=0;cnt<derlen;cnt++)
		{
			sprintf(BUF+strlen(BUF),"0x%02x",Der[cnt]);
		}
	}
	if(strcmp(x,"hex")==0)
	{
		bcd_to_hex(y,strlen(y),Der);

		der_to_pem(Der,strlen(Der),BUF,&len);

		sprintf(M,"%d",len);
	}
}

int accept_get(char *ret,char *M,char *BUF)
{
	char *p=NULL; 
	char *s=NULL;
	char *k=NULL;
	int m,n,cnt,derlen,len;
	char x[BUFFER_SIZE];
	char y[BUFFER_SIZE];
	unsigned char Der[BUFFER_SIZE];
	memset(Der,0,BUFFER_SIZE);
	memset(x,0,BUFFER_SIZE);
	memset(y,0,BUFFER_SIZE);

	p=strstr(ret,"?");
	s=strstr(ret,"&b=");    
	k=strstr(ret,"H");

	n=s-(p+3);
	m=(k-1)-(s+3);
	strncpy(x,(p+3),n);
	strncpy(y,(s+3),m);
	if(strcmp(x,"pem")==0)
	{
		pem_to_der(y,strlen(y),Der,&derlen);
		sprintf(M,"%d",derlen);
		for(cnt=0;cnt<derlen;cnt++)
		{
			sprintf(BUF+strlen(BUF),"0x%02x",Der[cnt]);
		}
	}
	if(strcmp(x,"hex")==0)
	{
		bcd_to_hex(y,strlen(y),Der);
		der_to_pem(Der,strlen(Der),BUF,&len);
		sprintf(M,"%d",len);
	}
	return 0;
}

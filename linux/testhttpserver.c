#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#define PORT 8888
#define BUFFER_SIZE 4096
#define RESPONSE_HEADER "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n"
#define RESPONSE_BODY "<Hello! world!>\n"
#define MAX 100

unsigned char   base[64]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
int  accept_get(char *ret,char *M,char *BUF);
int  accept_post(char *ret,char *M,char *BUF);

int handle(int conn){
    int len = 0;
    char buf1[BUFFER_SIZE];
	char buf2[BUFFER_SIZE];
	char buf3[BUFFER_SIZE];
	char buf4[BUFFER_SIZE];
	char temp[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    char buf[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    len = recv(conn, buffer, BUFFER_SIZE, 0);
    strncpy(buf,buffer,3);
	memset(temp,0,sizeof(temp));
	memset(buf1,0,BUFFER_SIZE);
	memset(buf2,0,BUFFER_SIZE);
	memset(buf3,0,BUFFER_SIZE);
	memset(buf4,0,BUFFER_SIZE);
	memset(temp,0,sizeof(temp));
    if(strncmp(buf,"POST",3)==0)
    {
       
        sprintf(buf1,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n");
        sprintf(buf2,"Content-Length: ");
	    accept_post(buffer,buf3,buf4);
        strcpy(temp,buf1);
        strcat(temp,buf2);
        strcat(temp,buf3);
        strcat(temp,"\r\n\r\n");
        strcat(temp,buf4);
        send(conn,temp,sizeof(temp),0);
    }

    if(strncmp(buf,"GET",3)==0)
    {
        sprintf(buf1,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n");
        sprintf(buf2,"Content-Length: ");
	    accept_get(buffer,buf3,buf4);
        strcpy(temp,buf1);
        strcat(temp,buf2);
        strcat(temp,buf3);
        strcat(temp,"\r\n\r\n");
        strcat(temp,buf4);
        send(conn,temp,sizeof(temp),0);
    }
    if (len <= 0 ) {
        printf ("recv error");
        return -1;
    } else {
        printf("%s\n\n",buffer);
    }

    close(conn);//关闭连接
}

int main(int argc,char *argv[]){
    int port = PORT;
    struct sockaddr_in client_sockaddr;     
    struct sockaddr_in server_sockaddr;
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    int opt = 1; 
    int conn;
    socklen_t length = sizeof(struct sockaddr_in);
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listenfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1){
        printf("bind error!\n");
        return -1;   
    }  

    if(listen(listenfd, 10) < 0) {
        printf("listen failed!\n");
        return -1;   
    }

    while(1){
        conn = accept(listenfd, (struct sockaddr*)&client_sockaddr, &length);
        if(conn < 0){
            printf("connect error!\n");
            continue;
        }
        if (handle(conn) < 0) {
            printf("connect error!\n");
            close(conn);
            continue;
        }  
    }
    return 0;
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

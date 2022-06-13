# include  <stdio.h>
# include  <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>
void write_rout(int sock,char * buf);
void read_rout(int sock,char * buf);
int main(int argc,char *argv[]){

	int ClientSock=socket(AF_INET,SOCK_STREAM,0);
	
	if(ClientSock<0)
	
	printf("error");
	
	struct sockaddr_in serverAdd;
	
	memset(&serverAdd,0,sizeof(struct sockaddr_in));
	
	serverAdd.sin_family=AF_INET;
	
	serverAdd.sin_port=htons(4441);
	
	serverAdd.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	if(connect(ClientSock,(struct sockaddr *)&serverAdd,sizeof(struct sockaddr_in))==0)
	
	printf("connection succfully \n");
	
	else{
	
	printf("echec :( \n");
	
	exit(1);}
	
	char buffer[1024];
	pid_t pid=fork();
	if(pid==0)
	write_rout(ClientSock,buffer);
	else
	read_rout(ClientSock,buffer);
	return 0;
}

void write_rout(int sock,char * buf){
	while(1){
	scanf("%s",buf);
	send(sock,buf,1024,0);
	if(*buf=='?')
	{
	close(sock);
	printf("client is disconnected");
	return ;
	}
	memset(buf,0,1024);
	}
}
void read_rout(int sock,char * buf){
	while(1)
	{
	recv(sock,buf,1024,0);
	if(strlen(buf)==0){
	close(sock);
	return;
	}
	printf("Message from the server : %s\n",buf);
	memset(buf,0,1024);
	}
}






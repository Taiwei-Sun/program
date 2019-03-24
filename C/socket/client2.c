#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
//#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>

//unsigned long int htonl(unsigned long int hostlong);
//unsigned short int htons(unsigned short int hostshort);


int main()
{
 int sockfd;
 int len;
 //struct sockaddr_un address;
 struct sockaddr_in address;
 int result;
 char ch='A';

 //sockfd=socket(AF_UNIX, SOCK_STREAM, 0);
 sockfd=socket(AF_INET, SOCK_STREAM, 0);
 
 /*address.sun_family=AF_UNIX;
 strcpy(address.sun_path, "server_socket");*/
 address.sin_family=AF_INET;
 address.sin_addr.s_addr=inet_addr("127.0.0.1");
 //address.sin_addr.s_addr=htonl(INADDR_ANY);
 address.sin_port=htons(9734);

 len=sizeof(address);

 result=connect(sockfd, (struct sockaddr*)&address, len);
 
 if(result==-1)
 {
  perror("oops: client1");
  exit(1);
 }

 write(sockfd, &ch, 1);
 read(sockfd, &ch, 1);
 printf("char from server = %c\n", ch);
 close(sockfd);
 exit(0);

}



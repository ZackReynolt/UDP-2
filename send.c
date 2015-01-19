#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <signal.h>

struct in_addr localInterface;
struct sockaddr_in groupSock;
int sd;

void INThandler(int);

int main (){
signal(SIGINT, INThandler);
while(1){
    /* Create a datagram socket on which to send. */

    sd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sd < 0){
      perror("Opening datagram socket error");
      exit(1);
    }else
	printf("Opening the datagram socket...OK.\n");

    /* Initialize the group sockaddr structure with a */
    /* group address of 225.1.1.1 and port 5555. */

    memset((char *) &groupSock, 0, sizeof(groupSock));
    groupSock.sin_family = AF_INET;
    groupSock.sin_addr.s_addr = inet_addr("226.1.1.1");
    groupSock.sin_port = htons(50789);     

    /* Set local interface for outbound multicast datagrams. */
    /* The IP address specified must be associated with a local, */
    /* multicast capable interface. */

    localInterface.s_addr = inet_addr("141.57.9.14");

    if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0){
      perror("Setting local interface error");
      exit(1);
    }else
      printf("Setting the local interface...OK\n");

    /* Send a message to the multicast group specified by the*/
    /* groupSock sockaddr structure. */
    /*int datalen = 1024;*/

    time_t td;
    time(&td);
    char dtbuf[128];
    int n;
    n = (int) strftime(dtbuf, sizeof dtbuf, "%A %b %d %H:%M:%S %Y", localtime(&td));

    char ip[13];
    strcpy(ip,"141.57.9.14");

    strcat(dtbuf," ");
    strncat(dtbuf,ip,sizeof(ip));
    strcat(dtbuf," ");

    //Getting hostname
    struct hostent *he;
    struct in_addr ipv4;
    inet_pton(AF_INET, ip, &ipv4);
    he = gethostbyaddr(&ipv4, sizeof ipv4, AF_INET);
    strcat(dtbuf,he->h_name);

    if(sendto(sd, dtbuf, 128, 0, (struct sockaddr*)&groupSock, sizeof(groupSock)) < 0){
      perror("Sending datagram message error");
    }else
      printf("Sending datagram message...OK\n");
    //wait seconds
    sleep(5);
}
return 0;
}

void INThandler(int sig){
	char c;

	signal(sig,SIG_IGN);
	printf("Closing conection with security\n");
	close(sd);
	exit(0);
}

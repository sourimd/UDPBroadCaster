// If broadcast a message to all host on the same network as that of the sender
// then use 255.255.255.255 ... If broadcast message to all host in a different
// network then use 255s in the hostid part. For example if network address is
// 145.132.X.X with subnet mast of 255.255.0.0 then broadcast IP will be
// 145.132.255.255

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void error( const char * msg){
	perror(msg);
	exit(1);
}

int main( int argc, char * argv[] ){
	int sock;
	struct sockaddr_in broadcastAddress;
	char * broadcastIP;
	unsigned short broadcastPort;
	char * broadcastMessage;
	unsigned int broadcastMessageLength;
	int broadcastPermission;

	if( argc < 4){
		error("Invalid number of arguments");
	}

	broadcastIP = argv[1];
	broadcastPort = atoi(argv[2]);
	broadcastMessage = argv[3];

	sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if(sock < 0){
		error("Unable to create socket");
	}

	// setting permission for broadcast
	broadcastPermission = 1;
	//changing socket option so that it can be used for broadcasting
	setsockopt( sock, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission, sizeof(broadcastPermission) );

	//set up broadcast address structure
	memset( &broadcastAddress, 0, sizeof(broadcastAddress)); // zero out address structure
	broadcastAddress.sin_family = AF_INET;
	broadcastAddress.sin_addr.s_addr = inet_addr( broadcastIP );
	broadcastAddress.sin_port = htons( broadcastPort );

	broadcastMessageLength = strlen( broadcastMessage );
	// broadcasting message
	sendto( sock, broadcastMessage, broadcastMessageLength, 0, (struct sockaddr *)&broadcastAddress, sizeof(broadcastAddress) );
}
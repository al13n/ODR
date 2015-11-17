/*
 *
 *  The tcpechotimesrv.c file is part of the assignment 1 of the Network Programming.
 *  It is a simple TCP implementation, basically, it is
 *  an academic project of CSE533 of Stony Brook University in fall
 *  2015. For more details, please refer to Readme.
 *
 *  Copyright (C) 2015 Dongju Ok   <dongju@stonybrook.edu,
 *                                  yardbirds79@gmail.com>
 *
 *  It is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//#include "unpifiplus.h"
#include "unp.h"
#include "global.h"
//#include "unpthread.h"
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
//#include "unprtt.h"
//#include <math.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <setjmp.h>
//#include <string.h>


/*
 * External Functions
 */



/*
 * Local Functions
 */




int 
main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_un servaddr, cliaddr;
	socklen_t len;
	int r = 0;

	unlink(UNIXDG_PATH);

	if( (sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0){             
                printf("CLIENT: socket error\n"); exit(1);              
        }     
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXDG_PATH);

	if( (r = bind( sockfd, (SA *) &servaddr, sizeof(servaddr))) < 0){
		printf("SERVER: bind() error\n"); exit(1);
	}

	len = sizeof(cliaddr);
#if(1)
	while(1)
	{
		char buf[MAXLINE];
	
		recvfrom( sockfd, buf, 10, 0, &cliaddr, &len); 
		printf("SERVER: receving msg \n");

		memset(buf, 0, MAXLINE);
		//sleep(1);
		strcpy(buf, "Hellofafafafafafafafdsafdsafsfsdafaf");
		sendto( sockfd, buf, 10, 0, &cliaddr /*&servaddr*/, len );
//		printf("SERVER: sending data\n");
	}
#else
	dg_echo( sockfd, (SA *) &cliaddr, sizeof(cliaddr));	
#endif
	printf("SERVER: hello..\n");
	while(1);


	


}

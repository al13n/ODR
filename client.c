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
	struct sockaddr_un cliaddr, servaddr;
	socklen_t len;
	int r = 0;	
	char upath[MAXLINE];
	int fd;
	//unlink(UNIXDG_PATH);
	//mkstemp(  );
//	char temp[] = "/tmp/fileXXXXXX";

	memset(upath, 0, MAXLINE);

	strcpy(upath, UNIXDG_PATH);

//	fd = mkstemp(temp);
//	if(fd < 0) 
//		printf("CLIENT: mkstemp error\n");

	//while(1);


	if( (fd = mkstemp(upath)) < 0 ){
		printf("CLIENT: fd = %d\n", fd);
		printf("CLIENT: mkstemp() error\n"); exit(1);
	}

	unlink(upath);

	if( (sockfd = socket(AF_LOCAL, SOCK_DGRAM,0)) < 0){
		printf("CLIENT: socket error\n"); exit(1);
	}

	bzero( &cliaddr, sizeof(cliaddr));
	cliaddr.sun_family = AF_LOCAL;
	strcpy( servaddr.sun_path, UNIXDG_PATH);

	//printf("CLIENT: Hello..\n");
	
	if( (r = bind( sockfd, (SA *) &cliaddr, sizeof(cliaddr))) < 0){
		printf("CLIENT: bind() error\n"); exit(1);
	}
	
	bzero( &servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXDG_PATH);
#if(1)

	len = sizeof(servaddr);
	while(1)
	{
		char buf[MAXLINE];

		sleep(1);
		strncpy(buf, "Client request", 10);
printf("CLIENT: in while loop\n");
		sendto( sockfd, buf, 10, 0, &servaddr, len);
		printf("CLIENT: sending msg \n");

		memset(buf, 0, MAXLINE);
		if( (r = recvfrom(sockfd, buf, 10, 0, NULL /*&servaddr*/, NULL /* sizeof(servaddr)*/)) > 0){
			buf[r] = 0;
			printf("CLIENT: receving %s\n", buf); 
		}
		printf("CLIENT: receving data \n");
	}
#else
	dg_cli(stdin, sockfd, (SA *)&servaddr, sizeof(servaddr));
#endif
	printf("CLIENT: Hello..\n");
	while(1);

}

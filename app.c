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
#include "app.h"
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



void
msg_send( int sockfd, char *canonical_ip, int port_num
		, char *msg, int flag)
{
	struct sockaddr_un ipaddr;
	socklen_t len;
	int r = 0;
	int buf_len = 0;

	char buf[MAXLINE];
	char buf1[MAXLINE];
	char buf2[MAXLINE];

	memset(buf, 0, MAXLINE);
	memset(buf1, 0, MAXLINE);
	memset(buf2, 0, MAXLINE);

	sprintf(buf1, "%d\0", port_num);
	sprintf(buf2, "%d\0", flag);

	buf_len = strlen(canonical_ip) + strlen(buf1)
		+ strlen(msg) + strlen(buf2) + 4;;

	sprintf(buf, "%s %s %s %s\0", canonical_ip, buf1, msg, buf2);

	bzero( &ipaddr, sizeof(ipaddr));
	ipaddr.sun_family = AF_LOCAL;
	strcpy(ipaddr.sun_path, UNIXDG_PATH);

	len = sizeof(ipaddr);

	if( (r = sendto( sockfd, buf, MAXLINE /*buf_len*/, 0,  &ipaddr,  len )) < 0){
		printf("msg_send() error\n"); exit(1);
	}

}

void
msg_recv( int sockfd, char *msg, char *canonical_ip
		, int *port_num)
{
	struct sockaddr_un ipaddr;
	socklen_t len;
	char buf[MAXLINE];
	char buf1[MAXLINE];
	char buf2[MAXLINE];
	char buf3[MAXLINE];
	fd_set rset;
	int r;

    FD_ZERO(&rset);


	memset(buf, 0, MAXLINE);
	memset(buf1, 0, MAXLINE);
	memset(buf2, 0, MAXLINE);
	memset(buf3, 0, MAXLINE);

	bzero( &ipaddr, sizeof(ipaddr));
	ipaddr.sun_family = AF_LOCAL;
	strcpy( ipaddr.sun_path, UNIXDG_PATH);

	FD_ZERO(&rset);

	for( ; ; ){
		FD_SET(sockfd, &rset);

		Select( sockfd + 1, &rset, NULL, NULL, NULL);
		if( FD_ISSET( sockfd, &rset ) ){

			if( (r = recvfrom( sockfd, buf, MAXLINE, 0, &ipaddr, &len)) < 0){
				printf("msg_recv() error\n"); exit(1);
			}	

			sscanf(buf,"%s %s %s", buf1, buf2, buf3);
	
 			strcpy(canonical_ip, buf1);
			strcpy(port_num, buf2);
			strcpy(msg, buf3);

			break;
		}
	}
	


}

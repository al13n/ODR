/*i
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
void vm2ip( char *ip, int vm_num );


int 
main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_un cliaddr, servaddr;
	socklen_t len;
	int r = 0;	
	char upath[MAXLINE];
	int fd;

	memset(upath, 0, MAXLINE);

	strcpy(upath, UNIXDG_PATH);



	if( (fd = mkstemp(upath)) < 0 ){
		printf("CLIENT: fd = %d\n", fd);
		printf("CLIENT: mkstemp() error\n"); exit(1);
	}

	unlink(upath);
//	close( fd );


	if( (sockfd = socket(AF_LOCAL, SOCK_DGRAM,0)) < 0){
		printf("CLIENT: socket error\n"); exit(1);
	}

	bzero( &cliaddr, sizeof(cliaddr));
	cliaddr.sun_family = AF_LOCAL;
	strcpy( cliaddr.sun_path, upath );
	//strcpy( cliaddr.sun_path, UNIXDG_PATH );
	
	if( (r = bind( sockfd, (SA *) &cliaddr, SUN_LEN(&cliaddr))) < 0){
		printf("CLIENT: bind() error\n"); exit(1);
	}
	
	len = sizeof(servaddr);
	while(1)
	{
		char buf[MAXLINE];
		char *p;
		char user_cmd[ MAXLINE ];
		char ip[ MAXLINE ];
		char canonical_ip[ MAXLINE ];
		char msg[ MAXLINE ];
		int vm_num;
		printf("<target node> <message>, ex) 10 hello ");
		fflush( stdin );
		scanf("%d %s", &vm_num, msg );
		
		//vm2ip( ip, 7 );
		vm2ip( canonical_ip, vm_num );

		//par_u_msg( canonical_ip, msg, user_cmd );
		
	
		msg_send( sockfd, canonical_ip, WELL_KNOWN_SERVER_PORT, msg, 0 /*int flag*/);

		//msg_recv( int sockfd, char *msg, char *canonical_ip, int *port_num);


	}


}

void par_u_msg( char *canonical_ip, char *msg, char *input )
{
	int vm_num;
	sscanf( input, "%d %s", &vm_num, msg );
	vm2ip( canonical_ip, vm_num ); 
	printf("CLIENT: par_u_msg %s\n", canonical_ip );
	printf("CLIENT: msg %s\n", msg );

}

int ip2vm( char *canonical_ip )
{



}

void vm2ip( char *ip, int vm_num )
{
	FILE *fd;
	char buf[MAXLINE];
	char buf1[ MAXLINE ];
	char *line = NULL;
	int len = 0;
	int read;
	char *p, *p1;
	int i = 0;
#if(1)

	struct hostent *p_h;
	char **pp_h;
	char hostname[10];
	char r_ip[20];
	sprintf( hostname, "VM%d", vm_num );
	p_h = gethostbyname( hostname );
	
	pp_h = p_h->h_addr;
	inet_ntop( AF_INET, p_h->h_addr  , r_ip, INET_ADDRSTRLEN );
	printf("vm2ip %s\n", r_ip );

	memcpy( ip, r_ip, INET_ADDRSTRLEN );

	//gethostbyaddresssd

#else
	if ( (fd = fopen( "/etc/hosts", "r" )) == -1 ){
		printf("CLIENT: vm2ip() error \n");
	}

	while( ( ( read = getline( &line, &len, fd) )) != -1 ){
		if( i > 8 ){
			//printf("%d\n", read );
			//printf("%s\n", line );
			p1 = strtok( line, " " );
			p = strtok( NULL, "\n" );
			//printf("%s\n", p );
			
			sprintf( buf1, "vm%d\0", vm_num );
			if( strcmp( p, buf1 ) == 0 ){
				strcpy( ip, p1 );
				printf("ip:%s\n", p1 ); 
				fclose( fd );
				return ;

			} 
			
		}
	
		i++;
	}

	printf("CLIENT: vm2ip() error no vm\n");

	fclose( fd );
#endif

}




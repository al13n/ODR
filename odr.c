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
#include "hw_addrs.h"
#include "linux/if_packet.h"
#include "linux/if_ether.h"
#include "linux/if_arp.h"
#include "global.h"
#include "aodr.h"
//#include "unpthread.h"
//#include "unprtt.h"
//#include <math.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <setjmp.h>
//#include <string.h>


/*
 * Global Variables
 */    


/*
 * External Functions
 */



/*
 * Local Functions
 */


/*
 * Global Variables
 */

int 
main(int argc, char **argv)
{
	int sockfd = 0, sockfd_pf = 0, max_fd = 0;
	struct sockaddr_un addr_un;
	socklen_t len;
	int r = 0;
	fd_set rset;
	
	struct sockaddr_ll addr_ll;
	socklen_t len_ll;
	int ip;
	char ip_str[MAXLINE];

	char buf[MAXLINE];
	char r_src_ip[MAXLINE];
	char r_src_port[MAXLINE];
	char r_dest_ip[MAXLINE];
	char r_dest_port[MAXLINE];
	char hw_addr[6];
	

	get_hw_info( hw_addr, ip_str );
	
	if( (sockfd = socket( AF_LOCAL, SOCK_DGRAM, 0)) < 0){
		printf("ODR: socket error\n"); exit(1);
	}

	bzero( &addr_un, sizeof(addr_un));
	addr_un.sun_family = AF_LOCAL;
	strcpy( addr_un.sun_path, UNIXDG_PATH);

	if( (sockfd_pf = socket( PF_PACKET, SOCK_RAW, htons(1606) )) < 0){
                printf("ODR: socket_pf error %d\n", errno); exit(1);
        }
	else{
		printf("ODR: socket_pf created\n");
	}
	bzero( &addr_ll, sizeof(addr_ll));

	if( sockfd_pf > sockfd)
		max_fd = sockfd_pf;
	else	max_fd = sockfd;
	
#if(1)
	/* TEST CODE by Dongju */

	struct odr_header t_odr_header_info;
	int t_broadcast_id = 45;
	char *t_dest_addr = "192.168.10.1\0";
	int t_hop_cnt = 321;
	//odr_header_info.header_type = RREQ_TYPE;
	//memcpy( odr_header_info.src_ip, ip_str, INET_ADDRSTRLEN );
	//odr_header_info.src_port = WELL_KNOWN_CLIENT_PORT;
	//memcpy( odr_header_info.dest_ip, "123.345.455.789\0", INET_ADDRSTRLEN );
	//odr_header_info.dest_port = WELL_KNOWN_SERVER_PORT;
	//strcpy( odr_header_info.user_data, "Hello Dongju...\0" );	

	char dest_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	create_rreq( &t_odr_header_info, RREQ_TYPE, ip_str, t_broadcast_id,  t_dest_addr, t_hop_cnt  );
	send_pfpacket( sockfd_pf, &t_odr_header_info, dest_mac, hw_addr, buf);


	if( (r = recvfrom( sockfd_pf, buf, ETH_FRAME_LEN, 0, &addr_ll, &len_ll)) < 0){
        	printf("ODR: PF_PACKET recvfrom() error \n"); exit(1);
        }


	printf("ODR: %x %x %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5] );
	printf("ODR: %x %x %x %x %x %x\n", buf[6], buf[7], buf[8], buf[9], buf[10], buf[11] );

	printf("\nODR: receving rreq packet \n");
	struct odr_header r_odr_header_info;
	int packet_type = 0;
	char r_dest_mac[ ETH_ALEN ];
	char r_src_mac[ ETH_ALEN ];

	get_macs_pfpacket( r_dest_mac, r_src_mac, buf  );
	get_info_odrheader( &packet_type,  &r_odr_header_info, buf ); 


	printf("ODR: dest_mac %x %x %x %x %x %x \n", r_dest_mac[0]&0xff, r_dest_mac[1]&0xff, r_dest_mac[2]&0xff, r_dest_mac[3]&0xff, r_dest_mac[4]&0xff, r_dest_mac[5]&0xff );
	printf("ODR: src_mac %x %x %x %x %x %x \n", r_src_mac[0]&0xff, r_src_mac[1]&0xff, r_src_mac[2]&0xff, r_src_mac[3]&0xff, r_src_mac[4]&0xff, r_src_mac[5] &0xff);
	printf("ODR: dest_ip %s\n", r_odr_header_info.dest_ip );
	printf("ODR: src_ip %s\n", r_odr_header_info.src_ip );


	/* building rt */
	



	while(1);

#endif

	for( ; ; ){
		FD_ZERO(&rset);
		FD_SET( sockfd_pf, &rset);
		FD_SET( sockfd, &rset);
		Select( max_fd + 1, &rset, NULL, NULL, NULL);
		if( FD_ISSET( sockfd, &rset ) ){
			if( (r = recvfrom( sockfd, buf, MAXLINE, 0, &addr_un, &len)) < 0){
			printf("ODR: recvfrom() error\n"); exit(1);
			}

			/* TODO: string data convert */
		}	

		if( FD_ISSET( sockfd_pf, &rset )){
			if( (r = recvfrom( sockfd_pf, buf, ETH_FRAME_LEN, 0, &addr_ll, &len_ll)) < 0){
				printf("ODR: PF_PACKET recvfrom() error \n"); exit(1);
			}

			struct odr_header *odr_header_info;
			
			odr_header_info = (struct odr_header *)((void *)buf + SIZE_OF_PF_HEADER);

			printf("ODR: receving packet\n");
			printf("ODR: frame type : %d\n", odr_header_info->header_type );
			printf("ODR: dest_ip : %s\n", odr_header_info->dest_ip );
			printf("ODR: dest_port: %d\n", odr_header_info->dest_port );
			printf("ODR: src_ip : %s\n", odr_header_info->src_ip );
			printf("ODR: src_port: %d\n", odr_header_info->src_port );
			printf("ODR: user msg: %s\n", odr_header_info->user_data );


		}
	
		


	}

 

}



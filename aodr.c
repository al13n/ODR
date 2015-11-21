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
struct routing_table *head = NULL;
struct routing_table *cur = NULL;

struct sunp_port_table *head_sunp = NULL;
struct sunp_port_table *cur_sunp = NULL;

/*
 * External Functions
 */



/*
 * Local Functions
 */



/*
 * Global Variables
 */


void 
get_hw_info(char hw_addr[], char *ip_str)
{

	struct hwa_info *hwa, *hwahead;
	struct sockaddr *sa;
        char *ptr;
        int i, prflag;  
	char mac_buf[MAXLINE];

        for (hwahead = hwa = Get_hw_addrs(); hwa != NULL; hwa = hwa->hwa_next)
        {
                //printf("name:%s MAC:%s\n ", hwa->if_name, hwa->if_haddr);
                //printf("index:%d Alias:%d\n", hwa->if_index, hwa->ip_alias);
                //printf("%s\n", sock_ntop_host(hwa->ip_addr, sizeof(struct sockaddr)));
                printf("%s :%s", hwa->if_name, ((hwa->ip_alias) == IP_ALIAS) ? " (alias)\n" : "\n");


                if ( (sa = hwa->ip_addr) != NULL)
                        printf("        IP addr = %s\n", Sock_ntop_host(sa, sizeof(*sa)));
                prflag = 0;
                i = 0;
                do{
                        if(hwa->if_haddr[i] != '\0'){
                                prflag = 1;
        			//bzero(&addr_ll, sizeof(addr_ll));
                                break;
                        }
                }while(++i < IF_HADDR);

                if(prflag){
                        printf("        HW addr = ");
                        char *p;
                        int j= 0;
                        p = ptr = hwa->if_haddr;
                        i = IF_HADDR;
                        j = 0;
                        do{
                                printf("%.2x%s", *ptr++ & 0xff, (i == 1) ? " " : ":");
                                sprintf(mac_buf + j *3, "%.2x%s", *p++ & 0xff, (i == 1) ? "\0" : ":");
                                 if( strncmp(hwa->if_name, "eth0", 4) == 0){
                                        hw_addr[6-i] = hwa->if_haddr[6-i];
                                }
                                j++;
                        }while(--i > 0);
                }
                printf("\n      interface index = %d\n\n", hwa->if_index);

                if( strncmp(hwa->if_name, "eth0", 4) == 0){

                        //ip = sa->sin_addr;
                        strcpy(ip_str, sock_ntop_host(sa, sizeof(*sa)));
                        printf("ODR: ip_str: %s\n", ip_str);
                        printf("ODR: mac_buf: %s\n", mac_buf);
                        //sockfd_ip  = socket(AF_PACKET, SOCK_RAW, htons(1234));

                        //bzero(&addr_ll, sizeof(addr_ll));
                        //addr_ll.sll_family = PF_PACKET;
                        //addr_ll.sll_protocol = htons(1234);
                        //addr_ll.sll_ifindex = hwa->if_index;

                        //if( (r = bind(sockfd_ip, (SA*) &addr_ll, sizeof(addr_ll))) < 0 ){
                        //      printf("ODR: bind() error\n"); exit(1);
                        //}

                        //printf("ODR: bind() success\n");

                }
        }

        free_hwa_info(hwahead);
}

void 
get_macs_pfpacket( char dest_mac[], char src_mac[], char *frame )
{
	char *buf = frame;
	memcpy( dest_mac, buf, ETH_ALEN );
	memcpy( src_mac, buf + ETH_ALEN,  ETH_ALEN ); 

}

void
get_info_odrheader(int *packet_type,  struct odr_header *r_odr_header_info, char *frame )
{
	unsigned int type = (unsigned int )*( frame + SIZE_OF_PF_HEADER );
	struct odr_header  *p = ( struct odr_header *)( frame + SIZE_OF_PF_HEADER );


	
	*packet_type = (int)type;	
	


	if ( type == RREQ_TYPE ){
		memcpy( r_odr_header_info->src_ip, p->src_ip,  INET_ADDRSTRLEN );  
		memcpy( r_odr_header_info->dest_ip, p->dest_ip,  INET_ADDRSTRLEN ); 
		r_odr_header_info->hop_cnt = p->hop_cnt;
		r_odr_header_info->broadcast_id = p->broadcast_id;
	} else if( type == RREP_TYPE ){
		memcpy( r_odr_header_info->src_ip, p->src_ip, INET_ADDRSTRLEN ); 
                memcpy( r_odr_header_info->dest_ip, p->dest_ip, INET_ADDRSTRLEN );
                r_odr_header_info->hop_cnt = p->hop_cnt;
                r_odr_header_info->lifetime = p->lifetime;
	} else if( type == PAYLOAD_TYPE ){
		memcpy( r_odr_header_info->src_ip, p->src_ip, INET_ADDRSTRLEN );
                memcpy( r_odr_header_info->dest_ip, p->dest_ip, INET_ADDRSTRLEN );
		memcpy( r_odr_header_info->user_data, p->user_data,  512 );
		/* TODO: what should I do in PAYLOAD case */
	} else
		printf("ODR_APP: get_info_odrheader() type error \n");

}  


void
create_rreq( struct odr_header *odr_header_info, rreq_type_num type, char *src_addr, int broadcast_id, char *dest_addr, int hop_cnt  )
{
	memset( odr_header_info, 0, sizeof( struct odr_header ) );	

	odr_header_info->header_type = (unsigned int) RREQ_TYPE;
	memcpy( odr_header_info->src_ip, src_addr,  INET_ADDRSTRLEN );
	memcpy( odr_header_info->dest_ip, dest_addr,  INET_ADDRSTRLEN );
	odr_header_info->broadcast_id = broadcast_id;
	odr_header_info->hop_cnt = hop_cnt;
}

void
create_rrep( struct odr_header *odr_header_info, rrep_type_num type, char *src_addr, int lifetime, char *dest_addr, int hop_cnt  )
{
	memset( odr_header_info, 0, sizeof( struct odr_header ) );

	odr_header_info->header_type = (unsigned int) RREP_TYPE;
        memcpy( odr_header_info->src_ip, src_addr,  INET_ADDRSTRLEN );
        memcpy( odr_header_info->dest_ip, dest_addr, INET_ADDRSTRLEN );
        odr_header_info->lifetime = lifetime;
        odr_header_info->hop_cnt = hop_cnt;
}



#define ETH1_INDEX	3
#define PROTOCOL_NUM	1234
#define MAC_LEN		6

void 
send_pfpacket( int sock_pf, struct odr_header *odr_header_info, char *dest_mac, char *src_mac,  char *buf, int b_broadcast_RREQ)
{
	struct sockaddr_ll addr_ll;
	//unsigned char src_mac[6] = { 0x00, 0x0c, 0x29, 0xa3, 0x1f,  0x23 };
	//unsigned char mac[6] = { src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5]};
	char mac_chunk[6];
	unsigned char broadcast_mac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	char pf_frame[MAXLINE];
	int offset_userdata;
	int r = 0;
	unsigned char *etherhead = pf_frame;
	struct ethhdr *eh = (struct ethhdr *)etherhead;
	short int etherTypeT = 0 /* htons(0x8200) */;

	int i = 0;
	char *p;
	char temp_buf[20];

	memset( pf_frame, 0, MAXLINE);

	addr_ll.sll_family = PF_PACKET;
	addr_ll.sll_protocol = htons( ETH_P_IP );
	addr_ll.sll_ifindex = ETH1_INDEX;
	addr_ll.sll_halen = ETH_ALEN /* 6*/;
	addr_ll.sll_hatype = ARPHRD_ETHER;
	addr_ll.sll_pkttype = PACKET_OTHERHOST;

//	p = strtok(  src_mac, ":");
	//strcpy( mac_chunk, p);
//	mac_chunk[i] = *p;
	//printf("p = %x\n",* p);
/*	while( p == NULL){
		i++;
		p = strtok(NULL, ":");
		//strcpy(mac_chunk + i *2 , p);
		mac_chunk[i] = htons(p);
	}
*/
	/* fill in data */
	if( b_broadcast_RREQ == TRUE )
		memcpy( pf_frame, broadcast_mac, MAC_LEN );
	else
		memcpy( pf_frame, dest_mac /*src_mac*/, MAC_LEN );
	memcpy( pf_frame + MAC_LEN, src_mac, MAC_LEN);
	//memset( pf_frame + 2 * MAC_LEN, 0x11, PACKET_TYPE_LEN); 
	//memcpy( pf_frame + 2 * MAC_LEN, &(etherTypeT), sizeof(etherTypeT));	
	eh->h_proto = htons( 1606 );

	offset_userdata = MAC_LEN * 2 + 2;


	build_ODRheader( pf_frame + offset_userdata, odr_header_info );
	memcpy(  pf_frame + offset_userdata + SIZE_OF_ODR_HEADER, buf, 100);

	if( (r = sendto( sock_pf, pf_frame, ETH_FRAME_LEN /*2 * MAC_LEN + PACKET_TYPE_LEN + 100*/, 0, (struct sockaddr *)&addr_ll, sizeof(addr_ll))) < 0){
		printf("ODR: send_pfpacket sendto() error\n"); exit(1);
	}

	printf("ODR: send)_pfpacket sends message \n");
}

#define	MAX_IP_LEN	16
#define	MAX_PORT_LEN	10
void
build_ODRheader( char *buf, struct odr_header *odr_header_info)
{
	int offset = 0;

	//if( ( buf == NULL ) || ( src_ip == NULL ) || ( src_port == NULL ) || ( dest_ip == NULL ) || (dest_port == NULL ) )
	//	printf("ODR: build_ODRheader() parameter is NULL error\n");

	
	//if ( ( strlen(src_ip) > MAX_IP_LEN ) || ( strlen( src_port ) > MAX_PORT_LEN ) || ( strlen( dest_ip ) > MAX_IP_LEN ) || ( strlen( dest_port) > MAX_PORT_LEN) )
	//	printf("ODR: build_ODRheader() parameter length is wrong error\n");


	
#if(1)
	memcpy( buf, (void *) odr_header_info, sizeof( struct odr_header ) );
#else 
	memcpy( buf, dest_ip, strlen( dest_ip ) + 1 ) ;
	offset = strlen( dest_ip ) + 1;

	memcpy( buf + offset, dest_port, strlen( dest_port ) + 1  );
	offset = offset + strlen( dest_port ) + 1;

	memcpy( buf + offset, src_ip, strlen( src_ip ) + 1 );
	offset = offset + strlen( src_ip ) + 1;

	memcpy( buf + offset, src_port, strlen( src_port )  + 1);
	offset = offset + strlen( src_port ) + 1;
#endif
	if ( offset >= SIZE_OF_ODR_HEADER )
		printf("ODR: build_ODRheader() header size error \n");
}

void build_entry_rt( struct routing_table *rt, char *dest_ip, char next_hop_mac[] )
{
	memset( rt, 0, sizeof( struct routing_table ) );
	memcpy( rt->dest_ip, dest_ip, INET_ADDRSTRLEN );
	memcpy( rt->next_hop_mac, next_hop_mac, ETH_ALEN );
} 


struct rounting_table * 
create_entry_rt( struct routing_table *input )
{
	struct routing_table *ptr = (struct routing_table *)malloc( sizeof( struct routing_table ) );
	
	if( ptr == NULL ){
		printf("ODR: create_entry_rt() ptr == NULL error\n");
	}

	memcpy( (void *)((ptr->dest_ip)), (void *)(input->dest_ip), INET_ADDRSTRLEN );
	memcpy( ptr->next_hop_mac, input->next_hop_mac, ETH_ALEN );

	ptr->next = NULL;

	head = cur = ptr;
	
	printf("ODR: create_entry_rt() success \n");
	
	return ptr;
}

struct rounting_table *
add_entry_rt( struct routing_table *input )
{
	if ( head == NULL )
		return ( create_entry_rt( input ) );

	struct routing_table *ptr = (struct routing_table *)malloc( sizeof( struct routing_table ) );
	if ( ptr == NULL )
		printf("ODR: add_entry_rt() ptr== NULL error \n");

	memcpy( ptr->dest_ip, input->dest_ip, strlen( input->dest_ip ) + 1 );
        memcpy( ptr->next_hop_mac, input->next_hop_mac, ETH_ALEN );

 	ptr->next = NULL;
	cur->next = ptr;
	cur = ptr;

	printf("ODR: add_entry_rt() success \n");
	return ptr;
}


/* TODO: which field is considered for look up rt */
struct routing_table *
search_entry_rt( char *ip, struct routing_table **prev )
{
	struct routing_table *ptr = head;
		
	while( ptr != NULL ){
		if( strcmp( ptr->dest_ip, ip ) ){
			return ptr;
		}
		else {
			prev = ptr;
			ptr = ptr->next;
 		}
	}

	return NULL;
}

struct routing_table *
del_entry_rt( struct routing_table *input )
{
	struct routing_table *prev = NULL;
	struct routing_table *del = NULL;

	del = search_entry_rt( input, &prev );
	
	if ( del == NULL )
		return -1;
	else {
		if ( prev != NULL )
			prev->next = del->next;

		if ( del == cur )
			cur = prev;
		else if ( del == head )
			head = del->next;
	}

	free( del );
	return 0;
}




struct sunp_port_table *
create_entry_spt( struct sunp_port_table *input )
{
        struct sunp_port_table *ptr = (struct sunp_port_table *)malloc( sizeof( struct sunp_port_table ) );

        if( ptr == NULL ){
                printf("ODR: create_entry_spt() ptr == NULL error\n");
        }

        //memcpy( (void *)((ptr->dest_ip)), (void *)(input->dest_ip), INET_ADDRSTRLEN );
        //memcpy( ptr->next_hop_mac, input->next_hop_mac, ETH_ALEN );
	memcpy( ptr->sunp, input->sunp, strlen( input->sunp ) + 1 );
        ptr->port = input->port;



        ptr->next = NULL;

        head_sunp = cur_sunp = ptr;

        printf("ODR: create_entry_spt() success \n");

        return ptr;
}


struct sunp_port_table *
add_entry_spt( struct sunp_port_table *input )
{
	if ( head_sunp == NULL )
		return ( create_entry_spt( input ) );

	struct sunp_port_table *ptr = (struct sunp_port_table *)malloc( sizeof( struct sunp_port_table ) );
	if ( ptr == NULL )
		printf("ODR: add_entry_spt() ptr == NULL error\n");

	memcpy( ptr->sunp, input->sunp, strlen( input->sunp ) + 1 );
	ptr->port = input->port;

	ptr->next = NULL;
	cur_sunp->next = ptr;
	cur_sunp = ptr;

	printf("ODR: add_entry_spt() success \n");
	return ptr;
}



struct sunp_port_table *
search_entry_spt( int port, struct routing_table **prev )
{
        struct sunp_port_table *ptr = head;

        while( ptr != NULL ){
                if( ptr->port == port ){
                        return ptr;
                }
                else {
                        prev = ptr;
                        ptr = ptr->next;
                }
        }

        return NULL;
}

struct sunp_port_table *
del_entry_spt( int port )
{
        struct sunp_port_table *prev = NULL;
        struct sunp_port_table *del = NULL;

        del = search_entry_rt( port, &prev );

        if ( del == NULL )
                return -1;
        else {
                if ( prev != NULL )
                        prev->next = del->next;

                if ( del == cur )
                        cur_sunp = prev;
                else if ( del == head_sunp )
                        head_sunp = del->next;
        }

        free( del );
        return 0;
}



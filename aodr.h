/*
 *
 *
 *  The global.h file is part of the assignment 1 of the Network Programming.
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

void
get_hw_info(char hw_addr[], char *ip_str);

void
get_macs_pfpacket( char dest_mac[], char src_mac[], char *frame );
void
get_info_odrheader(int *packet_type,  struct odr_header *r_odr_header_info, char *frame ); 

void
send_pfpacket( int sock_pf, struct odr_header *odr_header_info, char *dest_mac, char *src_mac,  char *buf, int b_broadcast_RREQ );
void
build_ODRheader( char *buf, struct odr_header *odr_header_info);

void 
build_entry_rt( struct routing_table *rt, char *dest_ip, char next_hop_mac[] );

struct rounting_table *
create_entry_rt( struct routing_table *input );
struct rounting_table *
add_entry_rt( struct routing_table *input );
struct routing_table *
search_entry_rt( char *ip, struct routing_table **prev );
struct routing_table *
del_entry_rt( struct routing_table *input );


struct sunp_port_table *
create_entry_spt( struct sunp_port_table *input );
struct sunp_port_table *
add_entry_spt( struct sunp_port_table *input );
struct sunp_port_table *
search_entry_spt( int port, struct routing_table **prev );
struct sunp_port_table *
del_entry_spt( int port );

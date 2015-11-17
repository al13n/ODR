/*
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define	DEBUG	0	

#define TRUE	1
#define FALSE	0

#define WELL_KNOWN_CLIENT_PORT	12445 
#define WELL_KNOWN_SERVER_PORT	12446

#define MAX_LINE	100


#define UNIXDG_PATH	"/tmp/fileXXXXXX"

#define RREQ_TYPE	0
#define RREP_TYPE	1
#define PAYLOAD_TYPE	2	
#define TEST_TYPE	5


#define SIZE_OF_PF_HEADER	14
#define SIZE_OF_ODR_HEADER	128



struct odr_header
{
	unsigned int header_type;

	char src_ip[INET_ADDRSTRLEN];
	unsigned int src_port;
	
	char dest_ip[ INET_ADDRSTRLEN ];
	unsigned int dest_port;

	/* rreq + rrep */
	int hop_cnt;

	/* rreq */
	int broadcast_id;

	/* rrep */
	int lifetime;

	char user_data[512];
};



struct routing_table
{
	char dest_ip[ INET_ADDRSTRLEN ];
	char next_hop_mac[6];

	struct routing_table *next;

} ;


typedef enum
{
	RREQ_NONE,
	RREQ_BROADCAST,
	RREQ_NORMAL,
	RREQ_ETC
} rreq_type_num ;

typedef enum
{
        RREP_NONE,
        RREP_NORMAL,
        RREP_ETC
} rrep_type_num ;	


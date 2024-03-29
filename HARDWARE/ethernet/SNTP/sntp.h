/*
 * sntp.h
 *
 *  Created on: 2014. 12. 15.
 *      Author: Administrator
 */

#ifndef SNTP_H_
#define SNTP_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "socket.h"
#include "dns.h"
#include "w5500init.h"
#include "socketdefines.h"
/*
 * @brief Define it for Debug & Monitor DNS processing.
 * @note If defined, it dependens on <stdio.h>
 */
#define _SNTP_DEBUG_

#define	MAX_SNTP_BUF_SIZE	sizeof(ntpformat)		///< maximum size of DNS buffer. */

/* for ntpclient */
typedef signed char s_char;
typedef unsigned long long tstamp;
typedef unsigned int tdist;

typedef struct _ntpformat
{

	uint8_t  dstaddr[4];        /* destination (local) address */
	char    version;        /* version number */
	char    leap;           /* leap indicator */
	char    mode;           /* mode */
	char    stratum;        /* stratum */
	char    poll;           /* poll interval */
	s_char  precision;      /* precision */
	tdist   rootdelay;      /* root delay */
	tdist   rootdisp;       /* root dispersion */
	char    refid;          /* reference ID */
	tstamp  reftime;        /* reference time */
	tstamp  org;            /* origin timestamp */
	tstamp  rec;            /* receive timestamp */
	tstamp  xmt;            /* transmit timestamp */


} ntpformat;

typedef struct _datetime
{
	uint16_t yy;
	uint8_t mo;
	uint8_t dd;
	uint8_t hh;
	uint8_t mm;
	uint8_t ss;
} datetime;

#define sntp_port 1234 //my sntp port, cho de gui ban ti ve
#define ntp_port		123                     //ntp server port number
#define SECS_PERDAY		86400UL             	// seconds in a day = 60*60*24
#define UTC_ADJ_HRS		7              	        // Vietnam : GMT+7
#define EPOCH			1900                    // NTP start year

void SNTP_init(void);
int8_t SNTP_run(void);

#ifdef __cplusplus
}
#endif

#endif /* SNTP_H_ */

/************************************************************
*
* THIS is ENC28J60 low level driver, modified to be  
* used for lwIP, based on work by Jonathan Granada (see below)
*
*              Vladimir S. Fonov 
*              vladimir <dot> fonov <at> gmail.com
*
*
*	This is the general driver for the ENC28J60
*	I changed some things to make it work with uIP
*	Some files of uIP have changes too.
*
*								edi87 [at] fibertel.com.ar
*								Jonathan Granade
*
************************************************************/
#ifndef ENC28J60_LOWLEVEL_H
#define ENC28J60_LOWLEVEL_H

#include <stdio.h>

#ifndef ETH_MAX_FRAMELEN
#define ETH_MAX_FRAMELEN 1500
#endif

#ifndef ENC28J60_MAX_FRAMELEN
#define ENC28J60_MAX_FRAMELEN	(ETH_MAX_FRAMELEN+18)	// maximum ethernet frame length
#endif

// ENC28J60 PINS CONFIGURATION
//#define ENC28J60_CONTROL_PORT	PORTB
//#define ENC28J60_CONTROL_DDR	DDRB

//#define ENC28J60_CONTROL_CS		0

//#define ENC28J60_SCK_PIN	  1
//#define ENC28J60_MISO_PIN	  3
//#define ENC28J60_MOSI_PIN	  2
//#define ENC28J60_SS_PIN		  0
//#define ENC28J60_RESET_PIN	6
//#define ENC28J60_INT_PIN	  7

//#undef  ENC28J60_ENABLE_RESET
//#define  ENC28J60_ENABLE_INT
#include "arch/cc.h"



//! initialize the ethernet interface for transmit/receive
extern void enc28j60Init(u8_t *eth_addr,u8_t DuplexState);

//! start sending packet
extern void enc28j60BeginPacketSend(u16_t packetLength);
//! transfer a portion of data to output buffer
extern void enc28j60PacketSend(u8_t *portion,u16_t length);
//! finish sending packet
extern void enc28j60EndPacketSend(void);


//! start recieving packet (check if there is a packet in input
//! queue, returns 0 if there are none
extern u16_t enc28j60BeginPacketReceive(void);
//! recieve a portion of packet 
extern void enc28j60PacketReceive(u8_t *portion,u16_t length);
//! finish recieving packet (flush remaining data if there is some left)
extern void enc28j60EndPacketReceive(void);

//! get the revision number of the ENC28J60 chip
extern u8_t enc28j60Revision(void);
//! check if there is connection
extern u8_t enc28j60linkup(void);

//! formatted print of important ENC28J60 registers
extern void enc28j60RegDump(void);

// enable ERRATA_B7 fixes
#define ENC28J60_ERRATA_B7

//! check if packet is currently being transmitted
extern u8_t   enc28j60PollPacketSending(void);

//! execute procedure for recovering from a receive overflow
/// this should be done when the receive memory fills up with packets
extern void enc28j60ReceiveOverflowRecover(void);


//! perform software reset
extern void enc28j60SoftwareReset(void);
//! do a ENC28J60 read operation
extern u8_t enc28j60ReadOp(u8_t op, u8_t address);
//! do a ENC28J60 write operation
extern void enc28j60WriteOp(u8_t op, u8_t address, u8_t data);
//! read the packet buffer memory
extern void enc28j60ReadBuffer(u16_t len, u8_t* data);
//! write the packet buffer memory
extern void enc28j60WriteBuffer(u16_t len, u8_t* data);
//! set the register bank for register at address
extern void enc28j60SetBank(u8_t address);
//! read ENC28J60 register 8bit
extern u8_t enc28j60Read(u8_t address);
//! write ENC28J60 register 8bit
extern void enc28j60Write(u8_t address, u8_t data);
//! read ENC28J60 register 8bit
extern u16_t enc28j60Read16(u8_t address);
//! write ENC28J60 register 8bit
extern void enc28j60Write16(u8_t address, u16_t data);
//! read a PHY register
extern u16_t enc28j60PhyRead(u8_t address);
//! write a PHY register
extern void enc28j60PhyWrite(u8_t address, u16_t data);

extern u8_t enc28j60getrev(void);

#endif  //ENC28J60_LOWLEVEL_H

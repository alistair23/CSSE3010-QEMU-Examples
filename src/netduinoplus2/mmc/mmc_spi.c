/*------------------------------------------------------------------------/
/  SPI MMCv3/SDv1/SDv2 (in SPI mode) control module
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2012, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/--------------------------------------------------------------------------/
 Features and Limitations:

 * Very Easy to Port
   It uses only 4 bit of GPIO port. No interrupt, no SPI port is used.

 * Platform Independent
   You need to modify only a few macros to control GPIO ports.

 * Low Speed
   The data transfer rate will be several times slower than hardware SPI.

/-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* Modified for NetduinoPlus - uses SPI           						   */
/*-------------------------------------------------------------------------*/


#include "diskio.h"		/* Common include file for FatFs and disk I/O layer */


/*-------------------------------------------------------------------------*/
/* Platform dependent macros and functions needed to be modified           */
/*-------------------------------------------------------------------------*/

#include "netduinoplus2.h"				
#include "debug_printf.h"
//#include "FreeRTOS.h"
//#include "task.h"

#define	INIT_PORT()	mmc_spi_init()	/* Initialize MMC control port (CS=H, CLK=L, DI=H, DO=in) */

#define DLY_US(n)	dly_us(n)	/* Delay n microseconds */

#define MMC_CS_LOW()       GPIO_ResetBits(NP2_MMC_SPI_CS_GPIO_PORT, NP2_MMC_SPI_CS_PIN)
#define MMC_CS_HIGH()      GPIO_SetBits(NP2_MMC_SPI_CS_GPIO_PORT, NP2_MMC_SPI_CS_PIN)


/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

/* MMC/SD command (SPI mode) */
#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define CMD13	(13)		/* SEND_STATUS */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD41	(41)		/* SEND_OP_COND (ACMD) */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */

/* Card type flags (CardType) */
#define CT_MMC		0x01		/* MMC ver 3 */
#define CT_SD1		0x02		/* SD ver 1 */
#define CT_SD2		0x04		/* SD ver 2 */
#define CT_SDC		0x06		/* SD */
#define CT_BLOCK	0x08		/* Block addressing */

//#define DEBUG 1
//#define DEBUGx 1

static
DSTATUS Stat = STA_NOINIT;	/* Disk status */

static
BYTE CardType;			/* b0:MMC, b1:SDv1, b2:SDv2, b3:Block addressing */

GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef  SPI_InitStructure;


/*-----------------------------------------------------------------------*/
/* SPI Functions						                                 */
/*-----------------------------------------------------------------------*/

//Delay function
void dly_us(int delay)
{
  while(delay--)
  {
	__asm volatile( "nop" );
  }
}

//Initialise SPI funtions.
void mmc_spi_init(void) {

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: mmc_spi_init\n\r");
#endif		

	RCC_PCLK1Config(RCC_CFGR_PPRE1_DIV16);
	RCC_PCLK1Config(RCC_CFGR_PPRE2_DIV16);
	
  	/* Enable the SPI periph */
  	RCC_APB1PeriphClockCmd(NP2_MMC_SPI_CLK, ENABLE);

  	/* Enable SCK, MOSI and MISO GPIO clocks */
  	RCC_AHB1PeriphClockCmd(NP2_MMC_SPI_SCK_GPIO_CLK | NP2_MMC_SPI_CS_GPIO_CLK, ENABLE);

	GPIO_PinAFConfig(NP2_MMC_SPI_SCK_GPIO_PORT, NP2_MMC_SPI_SCK_SOURCE, NP2_MMC_SPI_SCK_AF);
  	GPIO_PinAFConfig(NP2_MMC_SPI_MISO_GPIO_PORT, NP2_MMC_SPI_MISO_SOURCE, NP2_MMC_SPI_MISO_AF);
  	GPIO_PinAFConfig(NP2_MMC_SPI_MOSI_GPIO_PORT, NP2_MMC_SPI_MOSI_SOURCE, NP2_MMC_SPI_MOSI_AF); 
	
  	/* Enable CS/PWR  GPIO clock */
  
	/* SPI SCK pin configuration */
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  	GPIO_InitStructure.GPIO_Pin = NP2_MMC_SPI_SCK_PIN;
  	GPIO_Init(NP2_MMC_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  	/* SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		//Must be set as pull up
  	GPIO_InitStructure.GPIO_Pin = NP2_MMC_SPI_MISO_PIN;
  	GPIO_Init(NP2_MMC_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

	/* SPI  MOSI pin configuration */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Pin =  NP2_MMC_SPI_MOSI_PIN;
  	GPIO_Init(NP2_MMC_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

	 
  	/* SPI configuration -------------------------------------------------------*/
  	SPI_I2S_DeInit(NP2_MMC_SPI);
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  	SPI_InitStructure.SPI_CRCPolynomial = 0;
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_Init(NP2_MMC_SPI, &SPI_InitStructure);

  	/* Enable SPI3  */
  	SPI_Cmd(NP2_MMC_SPI, ENABLE);

  	/* Configure GPIO PIN for MMC Chip select */
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  	GPIO_InitStructure.GPIO_Pin = NP2_MMC_SPI_CS_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(NP2_MMC_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  	/* Deselect : Chip Select high */
  	GPIO_SetBits(NP2_MMC_SPI_CS_GPIO_PORT, NP2_MMC_SPI_CS_PIN);
	
	/* Configure GPIO PIN for MMC Power */
  	GPIO_InitStructure.GPIO_Pin = NP2_MMC_PWR_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(NP2_MMC_PWR_GPIO_PORT, &GPIO_InitStructure);
  	
	/* Turn Power off and on */	
	GPIO_ResetBits(NP2_MMC_PWR_GPIO_PORT, NP2_MMC_PWR_PIN);
	
	dly_us(10);
	
}

/*-----------------------------------------------------------------------*/
/* Transmit bytes to the card (SPI)                               */
/*-----------------------------------------------------------------------*/

static
void xmit_mmc (
	const BYTE* buff,	/* Data to be sent */
	UINT bc				/* Number of bytes to send */
)
{
	int i;
	uint8_t rxbyte;
#ifdef DEBUGx
	debug_printf("MMCSPI DEBUG: xmit_mmc: %d\n\r", bc);
#endif		

	//taskENTER_CRITICAL();
	
    for(i=0;i<bc; i++)                            		// then write all unsigned char in buffer(*pBuf)
    {
		//MMC_CS_LOW();
#ifdef DEBUGx
		debug_printf("*%d-", i);
#endif 	

		while (SPI_I2S_GetFlagStatus(NP2_MMC_SPI, SPI_I2S_FLAG_TXE) == RESET);

		SPI_I2S_SendData(NP2_MMC_SPI, buff[i]);

		dly_us(100);
		while (SPI_I2S_GetFlagStatus(NP2_MMC_SPI, SPI_I2S_FLAG_BSY) == SET);

		//dly_us(500);

  		/* Send a Byte through the SPI peripheral */
  		
  		//dly_us(100);
  		/* Wait to receive a Byte */
  		while (SPI_I2S_GetFlagStatus(NP2_MMC_SPI, SPI_I2S_FLAG_RXNE) == RESET);
		
  		/* Return the Byte read from the SPI bus */
  		rxbyte = (uint8_t)SPI_I2S_ReceiveData(NP2_MMC_SPI);
		
#ifdef DEBUGx
		debug_printf(" %d -", i);
#endif		

		//dly_us(100);
		//MMC_CS_HIGH();
		dly_us(100);
#ifdef DEBUGx
		debug_printf("%d ", rxbyte);
#endif 	
    }

#ifdef DEBUGx
		debug_printf(" End\n\r");
#endif		

	//taskEXIT_CRITICAL();
}



/*-----------------------------------------------------------------------*/
/* Receive bytes from the card (SPI)                              */
/*-----------------------------------------------------------------------*/

void rcvr_mmc (
	BYTE *buff,	/* Pointer to read buffer */
	UINT bc		/* Number of bytes to receive */
)
{

	int i;
	BYTE txbyte;

//if (bc > 200) {
//	bc = 400;
//}

#ifdef DEBUGx
if (bc > 200)
	debug_printf("MMCSPI DEBUG: rcvr_mmc %d\n\r", bc);
#endif

	//SPI_Cmd(NP2_MMC_SPI, ENABLE);
	//taskENTER_CRITICAL();

#ifdef DEBUGx
if (bc > 200)
	debug_printf("buff ");
#endif

	for(i=0;i<bc;i++)
    {

		//MMC_CS_LOW();

#ifdef DEBUG
	if (bc > 200)		
		debug_printf("#%i", i);
#endif 	

		dly_us(100);
		dly_us(100);
		dly_us(100);
		dly_us(100);
		//Wait for SPI TX buffer to be empty
		//while (SPI_I2S_GetFlagStatus(NP2_MMC_SPI, SPI_I2S_FLAG_TXE) == RESET);

#ifdef DEBUGx
	if (bc > 200)
		debug_printf("&");
#endif

		// Send dummy byte through the SPI peripheral 
  		//SPI_I2S_SendData(NP2_MMC_SPI, 0xFF);
		NP2_MMC_SPI->DR = 0xFF;

        //SPI_Write(BOARD_SD_SPI_BASE, 0, 0xFF);            // write dummy byte - must be non-command byte (0 to 58).
        //while(!SPI_IsFinished(BOARD_SD_SPI_BASE));  		// Wait for write to complete
        //buff[i] = SPI_Read(BOARD_SD_SPI_BASE);          	// Read unsigned char from SD Card
#ifdef DEBUGx
	if (bc > 200)
		debug_printf("?");
#endif

		dly_us(100);
		dly_us(100);
		dly_us(100);
		dly_us(100);
		//while (SPI_I2S_GetFlagStatus(NP2_MMC_SPI, SPI_I2S_FLAG_BSY) == SET); //{
			//dly_us(100);
		//}

#ifdef DEBUGx
	if (bc > 200)
		debug_printf("@");
#endif

		//dly_us(100);

		//while (SPI_I2S_GetFlagStatus(NP2_MMC_SPI, SPI_I2S_FLAG_BSY) == SET);

#ifdef DEBUG
	if (bc > 200)
		debug_printf("$");
#endif
	
		//dly_us(100);
		//dly_us(100);
		//dly_us(100);
		//dly_us(100);
  		// Wait to receive a Byte 
		//while (SPI_I2S_GetFlagStatus(NP2_MMC_SPI, SPI_I2S_FLAG_RXNE) == RESET);
		
#ifdef DEBUGx
	if (bc > 200)
		debug_printf("^");
#endif

		dly_us(100);
		dly_us(100);
		dly_us(100);
		dly_us(100);
	
			
		if ((i < 512) && ((i > 0) || (i == 0))) {
		//buff[i] = SPI_I2S_ReceiveData(NP2_MMC_SPI);
		txbyte = NP2_MMC_SPI->DR;
			//if (txbyte != 0) {
				buff[i+0] = txbyte;
			//} else {
			//	buff[i] = 0;
			//}
		}
		//SPI_I2S_ClearFlag(SPI3, SPI_I2S_FLAG_TXE);
		//SPI_I2S_ClearFlag(SPI3, SPI_I2S_FLAG_RXNE);	

#ifdef DEBUGx
		//if (buff[i] != 0)
		if (bc > 200)
			debug_printf("-%x ", buff[i]);		
#endif

		//dly_us(100);
		//MMC_CS_HIGH();
		dly_us(100);

#ifdef DEBUGx
	if (bc > 200)
		debug_printf(".");
#endif 	

    }

#ifdef DEBUGx
if (bc > 200)
	debug_printf(" -end\n\r");
#endif 	

	//taskEXIT_CRITICAL();
}



/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static
int wait_ready (void)	/* 1:OK, 0:Timeout */
{
	BYTE d;
	UINT tmr;


	for (tmr = 5000; tmr; tmr--) {	/* Wait for ready in timeout of 500ms */
		rcvr_mmc(&d, 1);
		
		if (d == 0xFF) break;
		DLY_US(100);
	}

	return tmr ? 1 : 0;
}



/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/

static
void deselect (void)
{
	BYTE d;

	MMC_CS_HIGH();
	dly_us(10);
	rcvr_mmc(&d, 1);	/* Dummy clock (force DO hi-z for multiple slave SPI) */

	//GPIO_SetBits(NP2_MMC_PWR_GPIO_PORT, NP2_MMC_PWR_PIN);
}



/*-----------------------------------------------------------------------*/
/* Select the card and wait for ready                                    */
/*-----------------------------------------------------------------------*/

static
int select (void)	/* 1:OK, 0:Timeout */
{
	BYTE d;

	//GPIO_ResetBits(NP2_MMC_PWR_GPIO_PORT, NP2_MMC_PWR_PIN);

	MMC_CS_LOW();
	dly_us(10);
	rcvr_mmc(&d, 1);	/* Dummy clock (force DO enabled) */

	if (wait_ready()) return 1;	/* OK */
	deselect();
	return 0;			/* Failed */
}



/*-----------------------------------------------------------------------*/
/* Receive a data packet from the card                                   */
/*-----------------------------------------------------------------------*/

static
int rcvr_datablock (	/* 1:OK, 0:Failed */
	BYTE *buff,			/* Data buffer to store received data */
	UINT btr			/* Byte count */
)
{
	BYTE d[2];
	UINT tmr;

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: rcvr_datablock\n\r");
#endif

	for (tmr = 1000; tmr; tmr--) {	/* Wait for data packet in timeout of 100ms */
		rcvr_mmc(d, 1);

#ifdef DEBUG
		debug_printf("MMCSPI DEBUG: rcvr_datablock d[0]=%d\n\r", d[0]);
#endif

		if (d[0] != 0xFF) break;
		DLY_US(100);
	}

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: rcvr_datablock x\n\r");
#endif

	if (d[0] != 0xFE) return 0;		/* If not valid data token, return with error */

#ifdef DEBUGx
	debug_printf("MMCSPI DEBUG: rcvr_datablock y\n\r");
#endif

	rcvr_mmc(buff, btr);			/* Receive the data block into buffer */

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: rcvr_datablock z\n\r");
#endif


	rcvr_mmc(d, 2);					/* Discard CRC */

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: rcvr_return\n\r");
#endif

	return 1;						/* Return with success */
}



/*-----------------------------------------------------------------------*/
/* Send a data packet to the card                                        */
/*-----------------------------------------------------------------------*/

static
int xmit_datablock (	/* 1:OK, 0:Failed */
	const BYTE *buff,	/* 512 byte data block to be transmitted */
	BYTE token			/* Data/Stop token */
)
{
	BYTE d[2];

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: xmit_datablock\n\r");		
#endif


	if (!wait_ready()) return 0;

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: xmit_datablock wait\n\r");		
#endif

	d[0] = token;
	xmit_mmc(d, 1);				/* Xmit a token */
#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: xmit_datablock token %X\n\r", d[0]);		
#endif

	if (token != 0xFD) {		/* Is it data token? */

#ifdef DEBUG
		debug_printf("MMCSPI DEBUG: xmit_datablock xmit_mmc\n\r");		
#endif

		xmit_mmc(buff, 512);	/* Xmit the 512 byte data block to MMC */

#ifdef DEBUG
		debug_printf("MMCSPI DEBUG: xmit_datablock rcvr\n\r");		
#endif

		rcvr_mmc(d, 2);			/* Xmit dummy CRC (0xFF,0xFF) */
		rcvr_mmc(d, 1);			/* Receive data response */
		if ((d[0] & 0x1F) != 0x05)	{/* If not accepted, return with error */

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: xmit_datablock d[0]=%X\n\r", d[0]);		
#endif

			return 0;

		}
	}

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: xmit_datablock return\n\r");		
#endif

	return 1;
}



/*-----------------------------------------------------------------------*/
/* Send a command packet to the card                                     */
/*-----------------------------------------------------------------------*/

BYTE send_cmd (		/* Returns command response (bit7==1:Send failed)*/
	BYTE cmd,		/* Command byte */
	DWORD arg		/* Argument */
)
{
	BYTE n, d, buf[6];

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: send_cmd %X\n\r", cmd);
#endif

	if (cmd & 0x80) {	/* ACMD<n> is the command sequense of CMD55-CMD<n> */
		cmd &= 0x7F;
		n = send_cmd(CMD55, 0);
		if (n > 1) return n;
	}

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: deselect\n\r");
#endif


	/* Select the card and wait for ready */
	deselect();
	if (!select()) return 0xAA;

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: send_cmd select\n\r");
#endif


	/* Send a command packet */
	buf[0] = 0x40 | cmd;			/* Start + Command index */
	buf[1] = (BYTE)(arg >> 24);		/* Argument[31..24] */
	buf[2] = (BYTE)(arg >> 16);		/* Argument[23..16] */
	buf[3] = (BYTE)(arg >> 8);		/* Argument[15..8] */
	buf[4] = (BYTE)arg;				/* Argument[7..0] */
	n = 0x01;						/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;		/* (valid CRC for CMD0(0)) */
	if (cmd == CMD8) n = 0x87;		/* (valid CRC for CMD8(0x1AA)) */
	buf[5] = n;
	xmit_mmc(buf, 6);

	/* Receive command response */
	if (cmd == CMD12) rcvr_mmc(&d, 1);	/* Skip a stuff byte when stop reading */
	n = 10;								/* Wait for a valid response in timeout of 10 attempts */
	do
		rcvr_mmc(&d, 1);
	while ((d & 0x80) && --n);

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: send_cmd return %d\n\r", d);
#endif


	return d;			/* Return with the response value */
}



/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv			/* Drive number (always 0) */
)
{
	DSTATUS s;
	BYTE d;


	if (drv) return STA_NOINIT;

	/* Check if the card is kept initialized */
	s = Stat;
	if (!(s & STA_NOINIT)) {
		if (send_cmd(CMD13, 0))	/* Read card status */
			s = STA_NOINIT;
		rcvr_mmc(&d, 1);		/* Receive following half of R2 */
		deselect();
	}
	Stat = s;

	return s;
}



/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv		/* Physical drive nmuber (0) */
)
{
	BYTE n, ty, cmd, buf[4];
	UINT tmr;
	DSTATUS s;

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: disk_initialize\n\r");
#endif


	if (drv) return RES_NOTRDY;

	INIT_PORT();				/* Initialize control port */

	for (n = 10; n; n--) rcvr_mmc(buf, 1);	/* 80 dummy clocks */

	ty = 0;
	if (send_cmd(CMD0, 0) == 1) {			/* Enter Idle state */
		if (send_cmd(CMD8, 0x1AA) == 1) {	/* SDv2? */
			rcvr_mmc(buf, 4);							/* Get trailing return value of R7 resp */
			if (buf[2] == 0x01 && buf[3] == 0xAA) {		/* The card can work at vdd range of 2.7-3.6V */
				for (tmr = 1000; tmr; tmr--) {			/* Wait for leaving idle state (ACMD41 with HCS bit) */
					if (send_cmd(ACMD41, 1UL << 30) == 0) break;
					DLY_US(1000);
				}
				if (tmr && send_cmd(CMD58, 0) == 0) {	/* Check CCS bit in the OCR */
					rcvr_mmc(buf, 4);
					ty = (buf[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* SDv2 */
				}
			}
		} else {							/* SDv1 or MMCv3 */
			if (send_cmd(ACMD41, 0) <= 1) 	{
				ty = CT_SD1; cmd = ACMD41;	/* SDv1 */
			} else {
				ty = CT_MMC; cmd = CMD1;	/* MMCv3 */
			}
			for (tmr = 1000; tmr; tmr--) {			/* Wait for leaving idle state */
				if (send_cmd(cmd, 0) == 0) break;
				DLY_US(1000);
			}
			if (!tmr || send_cmd(CMD16, 512) != 0)	/* Set R/W block length to 512 */
				ty = 0;
		}
	}
	CardType = ty;
	s = ty ? 0 : STA_NOINIT;
	Stat = s;

	deselect();

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: disk_initialize DSTATUS %d\n",s);
#endif


	return s;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,			/* Physical drive nmuber (0) */
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..128) */
)
{

//#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: disk_read - SECTOR: %d\n\r", sector);
//#endif


	//if (disk_status(drv) & STA_NOINIT) return RES_NOTRDY;
	//if (!count) return RES_PARERR;
	//if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert LBA to byte address if needed */

	if (count == 1) {	/* Single block read */

#ifdef DEBUG
		debug_printf("MMCSPI DEBUG: single disk_read cnt=%d\n\r", count);
#endif

		if ((send_cmd(CMD17, sector) == 0)	/* READ_SINGLE_BLOCK */
			&& rcvr_datablock(buff, 512))
			count = 0;
#ifdef DEBUG
		debug_printf("MMCSPI DEBUG: single disk_read end=%d\n\r", count);
#endif
	}
	else {				/* Multiple block read */
		if (send_cmd(CMD18, sector) == 0) {	/* READ_MULTIPLE_BLOCK */
			do {
#ifdef DEBUG
				debug_printf("MMCSPI DEBUG: multiple disk_read cnt%d\n\r", count);
#endif
		
				if (!rcvr_datablock(buff, 512)) break;

#ifdef DEBUG
				debug_printf("MMCSPI DEBUG: disk_read $\n\r");
#endif

				buff += 512;
#ifdef DEBUG
				debug_printf("MMCSPI DEBUG: disk_read cnt ^\n\r");
#endif

			} while (--count);
#ifdef DEBUG
			debug_printf("MMCSPI DEBUG: disk_read &\n\r", count);
#endif

			send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */

#ifdef DEBUG
			debug_printf("MMCSPI DEBUG: disk_read *\n\r", count);
#endif

		}
	}
	deselect();

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: disk_read return\n\r");
#endif


	return count ? RES_ERROR : RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0) */
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..128) */
)
{

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: disk_write\n\r");
#endif

	if (disk_status(drv) & STA_NOINIT) return RES_NOTRDY;
	if (!count) return RES_PARERR;
	if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert LBA to byte address if needed */

	if (count == 1) {	/* Single block write */
		if ((send_cmd(CMD24, sector) == 0)	/* WRITE_BLOCK */
			&& xmit_datablock(buff, 0xFE))
			count = 0;
	}
	else {				/* Multiple block write */
		if (CardType & CT_SDC) send_cmd(ACMD23, count);
		if (send_cmd(CMD25, sector) == 0) {	/* WRITE_MULTIPLE_BLOCK */
			do {
				if (!xmit_datablock(buff, 0xFC)) break;
				buff += 512;
			} while (--count);
			if (!xmit_datablock(0, 0xFD))	/* STOP_TRAN token */
				count = 1;
		}
	}
	deselect();

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: disk_write return\n\r");
#endif


	return count ? RES_ERROR : RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	BYTE n, csd[16];
	WORD cs;

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: disk_ioctl\n\r");
#endif


	if (disk_status(drv) & STA_NOINIT) return RES_NOTRDY;	/* Check if card is in the socket */

	res = RES_ERROR;
	switch (ctrl) {
		case CTRL_SYNC :		/* Make sure that no pending write process */
			if (select()) {
				deselect();
				res = RES_OK;
			}
			break;

		case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
			if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
				if ((csd[0] >> 6) == 1) {	/* SDC ver 2.00 */
					cs= csd[9] + ((WORD)csd[8] << 8) + 1;
					*(DWORD*)buff = (DWORD)cs << 10;
				} else {					/* SDC ver 1.XX or MMC */
					n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
					cs = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
					*(DWORD*)buff = (DWORD)cs << (n - 9);
				}
				res = RES_OK;
			}
			break;

		case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
			*(DWORD*)buff = 128;
			res = RES_OK;
			break;

		default:
			res = RES_PARERR;
	}

	deselect();

#ifdef DEBUG
	debug_printf("MMCSPI DEBUG: disk_ioctl return\n\r");
#endif

	return res;
}



/*-----------------------------------------------------------------------*/
/* This function is defined for only project compatibility               */

void disk_timerproc (void)
{
	/* Nothing to do */
}


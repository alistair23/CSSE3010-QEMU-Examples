#include "netduinoplus2.h"
#include "stm32f4xx_conf.h"
#include "debug_printf.h"

#include "enc28j60.h"
//#include "lowlevel.h"

static uint8_t Enc28j60Bank;
static uint16_t NextPacketPtr;

GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef  SPI_InitStructure;

//#define EXT_ENC	1		//Enable external ENC28J60

//Set pin definitions
#ifdef EXT_ENC
	#define ENC_SPI					NP2_SPI						//External SPI
	#define ENC_SPI_CLK				NP2_SPI_CLK
  	
	#define ENC_SPI_SCK_GPIO_CLK	NP2_SPI_SCK_GPIO_CLK		//GPIO SPI SCK Clock
	#define ENC_SPI_MISO_GPIO_CLK	NP2_SPI_MISO_GPIO_CLK		//GPIO SPI SCK Clock
	#define ENC_SPI_CS_GPIO_CLK		NP2_D8_GPIO_CLK

	#define ENC_SPI_SCK_PIN			NP2_SPI_SCK_PIN
	#define ENC_SPI_SCK_GPIO_PORT	NP2_SPI_SCK_GPIO_PORT
	#define ENC_SPI_SCK_SOURCE		NP2_SPI_SCK_SOURCE
	#define ENC_SPI_SCK_AF			NP2_SPI_SCK_AF

	#define ENC_SPI_MISO_PIN		NP2_SPI_MISO_PIN
  	#define ENC_SPI_MISO_GPIO_PORT	NP2_SPI_MISO_GPIO_PORT
	#define ENC_SPI_MISO_SOURCE		NP2_SPI_MISO_SOURCE
	#define ENC_SPI_MISO_AF			NP2_SPI_MISO_AF

	#define ENC_SPI_MOSI_PIN		NP2_SPI_MOSI_PIN
  	#define ENC_SPI_MOSI_GPIO_PORT	NP2_SPI_MOSI_GPIO_PORT
	#define ENC_SPI_MOSI_SOURCE		NP2_SPI_MOSI_SOURCE
	#define ENC_SPI_MOSI_AF			NP2_SPI_MOSI_AF  
    
	#define ENC_SPI_CS_PIN			NP2_D8_PIN
  	#define ENC_SPI_CS_GPIO_PORT	NP2_D8_GPIO_PORT

#else
	#define ENC_SPI					NP2_ENC_SPI						//onboard ethernet SPI
	#define ENC_SPI_CLK				NP2_ENC_SPI_CLK
  	
	#define ENC_SPI_SCK_GPIO_CLK	NP2_ENC_SPI_SCK_GPIO_CLK		//GPIO SPI SCK Clock
	#define ENC_SPI_MISO_GPIO_CLK	NP2_ENC_SPI_MISO_GPIO_CLK		//GPIO SPI SCK Clock
	#define ENC_SPI_CS_GPIO_CLK		NP2_D8_GPIO_CLK

	#define ENC_SPI_SCK_PIN			NP2_ENC_SPI_SCK_PIN
	#define ENC_SPI_SCK_GPIO_PORT	NP2_ENC_SPI_SCK_GPIO_PORT
	#define ENC_SPI_SCK_SOURCE		NP2_ENC_SPI_SCK_SOURCE
	#define ENC_SPI_SCK_AF			NP2_ENC_SPI_SCK_AF

	#define ENC_SPI_MISO_PIN		NP2_ENC_SPI_MISO_PIN
  	#define ENC_SPI_MISO_GPIO_PORT	NP2_ENC_SPI_MISO_GPIO_PORT
	#define ENC_SPI_MISO_SOURCE		NP2_ENC_SPI_MISO_SOURCE
	#define ENC_SPI_MISO_AF			NP2_ENC_SPI_MISO_AF

	#define ENC_SPI_MOSI_PIN		NP2_ENC_SPI_MOSI_PIN
  	#define ENC_SPI_MOSI_GPIO_PORT	NP2_ENC_SPI_MOSI_GPIO_PORT
	#define ENC_SPI_MOSI_SOURCE		NP2_ENC_SPI_MOSI_SOURCE
	#define ENC_SPI_MOSI_AF			NP2_ENC_SPI_MOSI_AF  
    
	#define ENC_SPI_CS_PIN			NP2_ENC_SPI_CS_PIN
  	#define ENC_SPI_CS_GPIO_PORT	NP2_ENC_SPI_CS_GPIO_PORT

#endif


// set CS to 0 = active
#define CSACTIVE() GPIO_ResetBits(ENC_SPI_CS_GPIO_PORT, ENC_SPI_CS_PIN)
// set CS to 1 = passive
#define CSPASSIVE() GPIO_SetBits(ENC_SPI_CS_GPIO_PORT, ENC_SPI_CS_PIN)
//
//#define waitspi() while(!(SPSR&(1<<SPIF)))

//#define DEBUG	1


//Delay function
void DelayUs(int delay)
{
  	while(delay--)
  	{
		__asm volatile( "nop" );
  	}
}


uint8_t enc_sendbyte(unsigned char txbyte) {

	uint8_t rxbyte;

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: sendbyte: T-");
#endif		
	while (SPI_I2S_GetFlagStatus(ENC_SPI, SPI_I2S_FLAG_TXE) == RESET);

  	/* Send a Byte through the SPI peripheral */
  	SPI_I2S_SendData(ENC_SPI, txbyte);

	//DelayUs(10);  
	while (SPI_I2S_GetFlagStatus(ENC_SPI, SPI_I2S_FLAG_BSY) == SET);
	
	//DelayUs(10);
		// Wait for write to complete

  	/* Wait to receive a Byte */
  	while (SPI_I2S_GetFlagStatus(ENC_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  
  	/* Return the Byte read from the SPI bus */
  	rxbyte = (uint8_t)SPI_I2S_ReceiveData(ENC_SPI);

	DelayUs(100);
	DelayUs(300);
	
#ifdef DEBUG
	debug_printf("%X R-%X\n\r", txbyte, rxbyte);
#endif		

	//CSPASSIVE();

	return rxbyte;
}

//*****************************************************************************
uint8_t Enc28j60ReadOp(uint8_t op, uint8_t address)
{
	uint8_t rxbyte;

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: Enc28j60ReadOp\n\r");
#endif

   	CSACTIVE();
   	// issue read command
   	/*SPDR = op | (address & ADDR_MASK);
   	waitspi();
   	// read data
   	SPDR = 0x00;
   	waitspi();
   	// do dummy read if needed (for mac and mii, see datasheet page 29)
   	if(address & 0x80)
   	{
      SPDR = 0x00;
      waitspi();
   	}*/

	enc_sendbyte(op | (address & ADDR_MASK));
	rxbyte = enc_sendbyte(0);

	if(address & 0x80) {
    	rxbyte = enc_sendbyte(0);
   	}


   	// release CS
   	CSPASSIVE();
   	return rxbyte;
}

//*****************************************************************************
void Enc28j60WriteOp(uint8_t op, uint8_t address, uint8_t data)
{

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: Enc28j60WriteOp\n\r");
#endif
   /*CSACTIVE;
   // issue write command
   SPDR = op | (address & ADDR_MASK);
   waitspi();
   // write data
   SPDR = data;
   waitspi();
   CSPASSIVE;
*/
	CSACTIVE();
	enc_sendbyte(op | (address & ADDR_MASK));
	enc_sendbyte(data);
	CSPASSIVE();
}

//*****************************************************************************
void Enc28j60ReadBuffer(uint16_t len, uint8_t* data)
{

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: Enc28j60ReadBuffer\n\r");
#endif
   	CSACTIVE();
   	// issue read command
   	/*SPDR = ENC28J60_READ_BUF_MEM;
   	waitspi();
   	while(len)
   	{
      	len--;
      	// read data
      	SPDR = 0x00;
      	waitspi();
      	*data = SPDR;
      	data++;
   	}
   	*data='\0';
	*/

	enc_sendbyte(ENC28J60_READ_BUF_MEM);
	while(len) {

		len--;
      	// read data
      	*data = enc_sendbyte(0x00);
      	data++;
   	}

   	*data='\0';

   	CSPASSIVE();
}

//*****************************************************************************
void Enc28j60WriteBuffer(uint16_t len, uint8_t* data)
{

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: Enc28j60WriteBuffer\n\r");
#endif

   CSACTIVE();
   // issue write command
   /*SPDR = ENC28J60_WRITE_BUF_MEM;
   waitspi();
   while(len)
   {
      len--;
      // write data
      SPDR = *data;
      data++;
      waitspi();
   }
	*/


	enc_sendbyte(ENC28J60_WRITE_BUF_MEM);
   
   	while(len) {
    	len--;
      	// write data
      	enc_sendbyte( *data);
      	data++;
   	}

   CSPASSIVE();
}

//*****************************************************************************
void Enc28j60SetBank(uint8_t address)
{

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: Enc28j60SetBank %d\n\r", address);
#endif

   // set the bank (if needed)
   if((address & BANK_MASK) != Enc28j60Bank)
   {
      // set the bank
      Enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0));
      Enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK)>>5);
      Enc28j60Bank = (address & BANK_MASK);
   }
}

//*****************************************************************************
uint8_t Enc28j60Read(uint8_t address)
{

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: Enc28j60Read %d\n\r", address);
#endif

   // set the bank
   Enc28j60SetBank(address);
   // do the read
   return Enc28j60ReadOp(ENC28J60_READ_CTRL_REG, address);
}

//*****************************************************************************
void Enc28j60Write(uint8_t address, uint8_t data)
{

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: Enc28j60Write %d %d\n\r", address, data);
#endif

   // set the bank
   Enc28j60SetBank(address);
   // do the write
   Enc28j60WriteOp(ENC28J60_WRITE_CTRL_REG, address, data);
}

//*****************************************************************************
void Enc28j60PhyWrite(uint8_t address, uint16_t data)
{

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: Enc28j60PhyWrite %d %d\n\r", address, data);
#endif

   // set the PHY register address
   Enc28j60Write(MIREGADR, address);
   // write the PHY data
   Enc28j60Write(MIWRL, data);
   Enc28j60Write(MIWRH, data>>8);
   // wait until the PHY write completes
   while(Enc28j60Read(MISTAT) & MISTAT_BUSY)
   {
      DelayUs(15);
   }
}


//**************************************************************************************
// Flash the 2 RJ45 LEDs twice to show that the interface works
void InitPhy (void)
{

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: InitPhy\n\r");
#endif

	/* Magjack leds configuration, see enc28j60 datasheet, page 11 */
	// LEDA=green LEDB=yellow
	//
	// 0x880 is PHLCON LEDB=on, LEDA=on
	// enc28j60PhyWrite(PHLCON,0b0000 1000 1000 00 00);
	Enc28j60PhyWrite(PHLCON,0x880);
	DelayUs(500); //DelayMs(500);
	//
	// 0x990 is PHLCON LEDB=off, LEDA=off
	// enc28j60PhyWrite(PHLCON,0b0000 1001 1001 00 00);
	Enc28j60PhyWrite(PHLCON,0x990);
	DelayUs(500);	//DelayMs(500);
	//
	// 0x880 is PHLCON LEDB=on, LEDA=on
	// enc28j60PhyWrite(PHLCON,0b0000 1000 1000 00 00);
	Enc28j60PhyWrite(PHLCON,0x880);
	DelayUs(500);	//DelayMs(500);
	//
	// 0x990 is PHLCON LEDB=off, LEDA=off
	// enc28j60PhyWrite(PHLCON,0b0000 1001 1001 00 00);
	Enc28j60PhyWrite(PHLCON,0x990);
	DelayUs(500);	//DelayMs(500);
	//
   	// 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
   	// enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
   	Enc28j60PhyWrite(PHLCON,0x476);
	DelayUs(100);	//DelayMs(100);
}


//*****************************************************************************
void Enc28j60Init(uint8_t* macaddr)
{

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: Enc28j60Init %2X-%2X-%2X-%2X-%2X-%2X \n\r", macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
#endif
	
#ifdef EXT_ENC
  	RCC_APB1PeriphClockCmd(ENC_SPI_CLK, ENABLE);
#else
	RCC_APB2PeriphClockCmd(ENC_SPI_CLK, ENABLE);
#endif
	
  	/* Enable SCK, MOSI and MISO GPIO clocks */
	RCC_AHB1PeriphClockCmd(ENC_SPI_SCK_GPIO_CLK | ENC_SPI_CS_GPIO_CLK | ENC_SPI_MISO_GPIO_CLK, ENABLE);

	GPIO_PinAFConfig(ENC_SPI_SCK_GPIO_PORT, ENC_SPI_SCK_SOURCE, ENC_SPI_SCK_AF);
  	GPIO_PinAFConfig(ENC_SPI_MISO_GPIO_PORT, ENC_SPI_MISO_SOURCE, ENC_SPI_MISO_AF);
  	GPIO_PinAFConfig(ENC_SPI_MOSI_GPIO_PORT, ENC_SPI_MOSI_SOURCE, ENC_SPI_MOSI_AF);  

	/* SPI SCK pin configuration */
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  	GPIO_InitStructure.GPIO_Pin = ENC_SPI_SCK_PIN;	//NP2_ENC_SPI_SCK_PIN;
	GPIO_Init(ENC_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  	/* SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = ENC_SPI_MISO_PIN;	//NP2_ENC_SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(ENC_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

	/* SPI  MOSI pin configuration */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin =  ENC_SPI_MOSI_PIN;	//NP2_ENC_SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(ENC_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  	/* SPI configuration -------------------------------------------------------*/
  	SPI_I2S_DeInit(ENC_SPI);
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; // | SPI_NSSInternalSoft_Set;
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;	//SPI_BaudRatePrescaler_64;
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  	SPI_InitStructure.SPI_CRCPolynomial = 0;
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_Init(ENC_SPI, &SPI_InitStructure);

  	/* Enable SPI for ENC */
  	SPI_Cmd(ENC_SPI, ENABLE);

  	/* Configure GPIO PIN for Enc Chip select */
  	GPIO_InitStructure.GPIO_Pin = ENC_SPI_CS_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(ENC_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  	/* Deselect : Chip Select high */
  	GPIO_SetBits(ENC_SPI_CS_GPIO_PORT, ENC_SPI_CS_PIN);
	
	/* Configure GPIO PIN for Power */
  	GPIO_InitStructure.GPIO_Pin = NP2_ENC_PWR_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(NP2_ENC_PWR_GPIO_PORT, &GPIO_InitStructure);
  	
	/* Turn Power off and on */	
	GPIO_ResetBits(NP2_ENC_PWR_GPIO_PORT, NP2_ENC_PWR_PIN);	

	/* Configure GPIO PIN for Reset */
  	GPIO_InitStructure.GPIO_Pin = NP2_ENC_RST_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(NP2_ENC_RST_GPIO_PORT, &GPIO_InitStructure);
  	
	/* Turn Reset on */	
	GPIO_SetBits(NP2_ENC_RST_GPIO_PORT, NP2_ENC_RST_PIN);
	
  	DelayUs(100);  	

   // perform system reset
   Enc28j60WriteOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
   DelayUs(50);	//DelayMs(50);

   // check CLKRDY bit to see if reset is complete
   // The CLKRDY does not work. See Rev. B4 Silicon Errata point. Just wait.
   //while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY));
   // do bank 0 stuff
   // initialize receive buffer
   // 16-bit transfers, must write low byte first
   // set receive buffer start address
   NextPacketPtr = RXSTART_INIT;
   // Rx start
   Enc28j60Write(ERXSTL, RXSTART_INIT & 0xFF);
   Enc28j60Write(ERXSTH, RXSTART_INIT >> 8);
   // set receive pointer address
   Enc28j60Write(ERXRDPTL, RXSTART_INIT & 0xFF);
   Enc28j60Write(ERXRDPTH, RXSTART_INIT >> 8);
   // RX end
   Enc28j60Write(ERXNDL, RXSTOP_INIT & 0xFF);
   Enc28j60Write(ERXNDH, RXSTOP_INIT >> 8);
   // TX start
   Enc28j60Write(ETXSTL, TXSTART_INIT & 0xFF);
   Enc28j60Write(ETXSTH, TXSTART_INIT >> 8);
   // TX end
   Enc28j60Write(ETXNDL, TXSTOP_INIT & 0xFF);
   Enc28j60Write(ETXNDH, TXSTOP_INIT >> 8);
   // do bank 1 stuff, packet filter:
   // For broadcast packets we allow only ARP packtets
   // All other packets should be unicast only for our mac (MAADR)
   //
   // The pattern to match on is therefore
   // Type     ETH.DST
   // ARP      BROADCAST
   // 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
   // in binary these poitions are:11 0000 0011 1111
   // This is hex 303F->EPMM0=0x3f,EPMM1=0x30
   Enc28j60Write(ERXFCON, ERXFCON_UCEN | ERXFCON_CRCEN | ERXFCON_PMEN);
   Enc28j60Write(EPMM0, 0x3f);
   Enc28j60Write(EPMM1, 0x30);
   Enc28j60Write(EPMCSL, 0xf9);
   Enc28j60Write(EPMCSH, 0xf7);
   //
   //
   // do bank 2 stuff
   // enable MAC receive
   Enc28j60Write(MACON1, MACON1_MARXEN | MACON1_TXPAUS | MACON1_RXPAUS);
   // bring MAC out of reset
   Enc28j60Write(MACON2, 0x00);
   // enable automatic padding to 60bytes and CRC operations
   Enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN);
   // set inter-frame gap (non-back-to-back)
   Enc28j60Write(MAIPGL, 0x12);
   Enc28j60Write(MAIPGH, 0x0C);
   // set inter-frame gap (back-to-back)
   Enc28j60Write(MABBIPG, 0x12);
   // Set the maximum packet size which the controller will accept
   // Do not send packets longer than MAX_FRAMELEN:
   Enc28j60Write(MAMXFLL, MAX_FRAMELEN & 0xFF);	
   Enc28j60Write(MAMXFLH, MAX_FRAMELEN >> 8);
   // do bank 3 stuff
   // write MAC address
   // NOTE: MAC address in ENC28J60 is byte-backward
   Enc28j60Write(MAADR5, macaddr[0]);
   Enc28j60Write(MAADR4, macaddr[1]);
   Enc28j60Write(MAADR3, macaddr[2]);
   Enc28j60Write(MAADR2, macaddr[3]);
   Enc28j60Write(MAADR1, macaddr[4]);
   Enc28j60Write(MAADR0, macaddr[5]);
   // no loopback of transmitted frames
   Enc28j60PhyWrite(PHCON2, PHCON2_HDLDIS);
   // switch to bank 0
   Enc28j60SetBank(ECON1);
   // enable interrutps
   Enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE | EIE_PKTIE);
   // enable packet reception
   Enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
}

//*****************************************************************************
uint8_t Enc28j60getrev(void)     // read the revision of the chip
{

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: Enc28j60getrev\n\r");
#endif

	return(Enc28j60Read(EREVID));
}

//*****************************************************************************
void Enc28j60PacketSend(uint16_t len, uint8_t* packet)
{

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: Enc28j60PacketSend %d\n\r", len);
#endif

	// Set the write pointer to start of transmit buffer area
	Enc28j60Write(EWRPTL, TXSTART_INIT & 0xFF);
	Enc28j60Write(EWRPTH, TXSTART_INIT >> 8);
	// Set the TXND pointer to correspond to the packet size given
	Enc28j60Write(ETXNDL, (TXSTART_INIT + len) & 0xFF);
	Enc28j60Write(ETXNDH, (TXSTART_INIT + len) >> 8);
	// write per-packet control byte (0x00 means use macon3 settings)
	Enc28j60WriteOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);
	// copy the packet into the transmit buffer
	Enc28j60WriteBuffer(len, packet);
	// send the contents of the transmit buffer onto the network
	Enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
   // Reset the transmit logic problem. See Rev. B4 Silicon Errata point 12.
	if((Enc28j60Read(EIR) & EIR_TXERIF))
   {
      Enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRTS);
   }
}

//*****************************************************************************
// Gets a packet from the network receive buffer, if one is available.
// The packet will be headed by an ethernet header.
//      maxlen  The maximum acceptable length of a retrieved packet.
//      packet  Pointer where packet data should be stored.
// Returns: Packet length in bytes if a packet was retrieved, zero otherwise.
uint16_t Enc28j60PacketReceive(uint16_t maxlen, uint8_t* packet)
{
	uint16_t rxstat;
	uint16_t len;

#ifdef DEBUG
	debug_printf("ENCSPI DEBUG: Enc28j60PacketReceive %d\n\r", maxlen);
#endif

	// check if a packet has been received and buffered
	//if( !(Enc28j60Read(EIR) & EIR_PKTIF) ){
        // The above does not work. See Rev. B4 Silicon Errata point 6.
	if( Enc28j60Read(EPKTCNT) ==0 ) {
#ifdef DEBUG
		debug_printf("ENCSPI DEBUG: Enc28j60PacketReceive return\n\r");
#endif

		return(0);
   	}

	// Set the read pointer to the start of the received packet
	Enc28j60Write(ERDPTL, NextPacketPtr);
	Enc28j60Write(ERDPTH, NextPacketPtr >> 8);
	
   // read the next packet pointer
	NextPacketPtr  = Enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	NextPacketPtr |= Enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0) << 8;

	// read the packet length (see datasheet page 43)
	len  = Enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	len |= Enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0) << 8;
   len -= 4; //remove the CRC count
	
   // read the receive status (see datasheet page 43)
	rxstat  = Enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	rxstat |= Enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0) << 8;

	// limit retrieve length
   if (len>maxlen-1)
   {
      len=maxlen-1;
   }
   // check CRC and symbol errors (see datasheet page 44, table 7-3):
   // The ERXFCON.CRCEN is set by default. Normally we should not
   // need to check this.
   if ((rxstat & 0x80)==0)
   {
      // invalid
      len=0;
   }
   else
   {
      // copy the packet from the receive buffer
      Enc28j60ReadBuffer(len, packet);
   }
   
   // Move the RX read pointer to the start of the next received packet
	// This frees the memory we just read out
	Enc28j60Write(ERXRDPTL, NextPacketPtr);
	Enc28j60Write(ERXRDPTH, NextPacketPtr >> 8);
	
   // decrement the packet counter indicate we are done with this packet
	Enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
	return(len);
}


/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "netif/etharp.h"
#include "err.h"
#include "ethernetif.h"

#include "netconf.h"
#include "enc28j60def.h"
#include "enc28j60_lowlevel.h"
#include "FreeRTOS.h"
#include "task.h"

//#define DEBUG 1
#ifdef DEBUG
#include "debug_printf.h"
#endif

#include <string.h>


#define netifMTU                                (1500)
#define netifINTERFACE_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE*6 )
#define netifINTERFACE_TASK_PRIORITY		( configMAX_PRIORITIES - 1 )
#define netifGUARD_BLOCK_TIME			( 250 )
/* The time to block waiting for input. */
#define emacBLOCK_TIME_WAITING_FOR_INPUT	( ( portTickType ) 100 )

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'


static struct netif *s_pxNetIf = NULL;
xSemaphoreHandle s_xSemaphore = NULL;
          
static void ethernetif_input( void * pvParameters );
static void arp_timer(void *arg);


/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void low_level_init(struct netif *netif)
{
  /* set netif MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;
	
  /* set netif MAC hardware address */
  netif->hwaddr[0] =  MAC_ADDR0;
  netif->hwaddr[1] =  MAC_ADDR1;
  netif->hwaddr[2] =  MAC_ADDR2;
  netif->hwaddr[3] =  MAC_ADDR3;
  netif->hwaddr[4] =  MAC_ADDR4;
  netif->hwaddr[5] =  MAC_ADDR5;
  
  /* set netif maximum transfer unit */
  netif->mtu = 1500;

  /* Accept broadcast address and ARP traffic */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
  
  s_pxNetIf =netif;
 
  /* create binary semaphore used for informing ethernetif of frame reception */
  if (s_xSemaphore == NULL)
  {
    s_xSemaphore= xSemaphoreCreateCounting(20,0);
  }

  /* initialize MAC address in ethernet MAC */ 
  enc28j60Init(netif->hwaddr, 1);
  
  /* create the task that handles the ETH_MAC */
  xTaskCreate(ethernetif_input, (signed char*) "Eth_if", netifINTERFACE_TASK_STACK_SIZE, NULL,
              netifINTERFACE_TASK_PRIORITY,NULL);
  
  /* Enable MAC and DMA transmission and reception */
}


/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
  //static xSemaphoreHandle xTxSemaphore = NULL;
  struct pbuf *q;
  //uint32_t l = 0;
  //u8_t *buffer ;

#if ETH_PAD_SIZE
  pbuf_header(p, ETH_PAD_SIZE); /* drop the padding word */
#endif

	enc28j60BeginPacketSend(p->tot_len);

  	for(q = p; q != NULL; q = q->next) {
    	/* Send the data from the pbuf to the interface, one pbuf at a
    	   time. The size of the data in each pbuf is kept in the ->len
    	   variable. */
		enc28j60PacketSend(q->payload, q->len);
  	}

	enc28j60EndPacketSend();
  
  //LINK_STATS_INC(link.xmit);

  return ERR_OK;
}


	
  	

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf * low_level_input(struct netif *netif)
{
  struct pbuf *p, *q;
  u16_t len;
  //uint32_t l=0,i =0;
  //FrameTypeDef frame;
  //u8_t *buffer;
  
  p = NULL;
  
/* Obtain the size of the packet and put it into the "len"
     variable. */
  len = enc28j60BeginPacketReceive();
	if(!len) {
#ifdef DEBUG
	debug_printf("ETH DEBUG: low_level_input %d\n\r", len);
#endif

	return NULL;
	}
#if ETH_PAD_SIZE
  len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

  /* We allocate a pbuf chain of pbufs from the pool. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  
  if (p != NULL) {

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    /* We iterate over the pbuf chain until we have read the entire
     * packet into the pbuf. */
    for(q = p; q != NULL; q = q->next) {
      /* Read enough bytes to fill this pbuf in the chain. The
       * available data in the pbuf is given by the q->len
       * variable. */
      //read data into(q->payload, q->len);
			enc28j60PacketReceive(q->payload, q->len);
    }
			
    //acknowledge that packet has been read();
		enc28j60EndPacketReceive();

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

   // LINK_STATS_INC(link.recv);
  } else {
    //drop packet();
		enc28j60EndPacketReceive();
    //LINK_STATS_INC(link.memerr);
    //LINK_STATS_INC(link.drop);
  }
  
  return p;
}


/**
 * This function is the ethernetif_input task, it is processed when a packet 
 * is ready to be read from the interface. It uses the function low_level_input() 
 * that should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void ethernetif_input( void * pvParameters )
{
  struct pbuf *p;
  
  for( ;; )
  {
    //if (xSemaphoreTake( s_xSemaphore, emacBLOCK_TIME_WAITING_FOR_INPUT)==pdTRUE)
    //{
      p = low_level_input( s_pxNetIf );
	if (p != NULL) {
      if (ERR_OK != s_pxNetIf->input( p, s_pxNetIf))
      {
        pbuf_free(p);
        p=NULL;
      }
    }

	vTaskDelay(100);

  }
}  
      
/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;

  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);
  
  etharp_init();
  sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);

  return ERR_OK;
}


static void arp_timer(void *arg)
{
  etharp_tmr();
  sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);
}

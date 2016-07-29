/**
 * \file
 *
 * \brief SAM SPI Quick Start
 *
 * Copyright (c) 2012-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <internalClock.h>
#include <hmi.h>
#include <app_config.h>
#include <network.h>
uint16_t crcCalcul=0;
void configure_spi_slave(void);

//! [dev_inst]
struct spi_module spi_slave_instance;
//! [dev_inst]
//! [setup]

//! [configure_spi]
void configure_spi_slave(void)
{
//! [config]
	struct spi_config config_spi_slave;

	/* Configure, initialize and enable SERCOM SPI module */
	//Master mode enabled
	//MSB of the data is transmitted first
	//Transfer mode 0
	//MUX Setting D
	//Character size eight bits
	//Not enabled in sleep mode
	//Receiver enabled
	//Baudrate 100000
	//Default pinmux settings for all pads
	//GCLK generator 0
	spi_get_config_defaults(&config_spi_slave);
	
	//mode slave
	config_spi_slave.mode = SPI_MODE_SLAVE;

	config_spi_slave.mode_specific.slave.preload_enable = true;

	config_spi_slave.mode_specific.slave.frame_format = SPI_FRAME_FORMAT_SPI_FRAME;

	config_spi_slave.mux_setting = EXT1_SPI_SERCOM_MUX_SETTING;
						

	config_spi_slave.pinmux_pad0 = EXT1_SPI_SERCOM_PINMUX_PAD0;

	config_spi_slave.pinmux_pad1 = EXT1_SPI_SERCOM_PINMUX_PAD1;

	config_spi_slave.pinmux_pad2 = EXT1_SPI_SERCOM_PINMUX_PAD2;

	config_spi_slave.pinmux_pad3 = EXT1_SPI_SERCOM_PINMUX_PAD3;

	spi_init(&spi_slave_instance, EXT1_SPI_MODULE, &config_spi_slave);

	spi_enable(&spi_slave_instance);


}
bool receive (int nbWord,uint8_t dataSend[]){
	timeOut=0;
	uint16_t status;
	uint8_t indice;
	uint8_t data[1];
	uint16_t i;
	crcCalcul=0;
		for(i=0;i<nbWord;i++){
			while(spi_read_buffer_wait(&spi_slave_instance, data,1,0x000)!=STATUS_OK){
				if(timeOut>50){
					return false;
				}
			
			}
			if(i!=(nbWord-1)){
				spi_write(&spi_slave_instance,data[0]);
			}
			timeOut=0;
			crcCalcul+=data[0];
			dataSend[i]=data[0];
		}
		
		/*switch(status){
			case STATUS_OK:
				printf("ALL receive\r\n");
			break;
			case STATUS_ABORTED:
				printf("aborted\r\n");
			
			break;
			case STATUS_ERR_INVALID_ARG	:
				printf("error invalid arg\r\n");
			
			break;
			case STATUS_ERR_TIMEOUT:
				// printf("error timeout\r\n");
			
			break;
			case STATUS_ERR_DENIED:
				printf("error acess denied\r\n");
			
			break;
			case STATUS_ERR_OVERFLOW:
				printf("error acess denied\r\n");
			
			break;
			default:
				printf("error invalid status");
			break;*/
		

	
	return true;
	
}
//#ifdef MASTER_R21
uint8_t calculCRC(uint8_t data[]){
	uint16_t i;
	uint8_t crc=0;
	for(i=0;i<(DATASIZE*2);i++){
		crc=crc+data[i];			
	}
	
	return crc;
}
//#endif

bool crcManage(uint16_t crc){
	crc=crc & 0xff ;//8bit
	uint8_t crcSend[1];
	uint16_t status;
	status=spi_write(&spi_slave_instance,crc);
	//printf("write value crc : %d (status %d)\r\n",crc,status);
	//check CRC	
	if(receive(1,crcSend)){
		uint8_t crcReceive=crcSend[0];
		if(crc==crcReceive){
			printf("crc ok crc:%d, crcReceive:%d\r\n",crc,crcReceive);
			return true;
		}
		else{
			printf("ERROR CRC crc:%d, crcReceive:%d\r\n",crc,crcReceive);
			return false;
		}
	}
	else{
		printf("CRC not received\r\n");	
	}
}

void dataParse(uint8_t data[]){
	printf("receice \r\n");
	networkTx(data,DATASIZE*2);
	uint8_t crc[1];
	crc[0]=calculCRC(data);
	networkTx(crc,1);
	printf("end\r\n");	
	
	
}
int main(void){

	
	system_init();
	configureHMI();
	
	configurationNetwork();
	#ifndef MASTER_R21
		configure_spi_slave();
	#endif
	configureInternalClock();
	
	
	#ifdef MASTER_R21
		//printf("MASTER\r\n");;
	#else
		printf("boot\r\n");
		printf("SLAVE\r\n");;
	#endif
	uint8_t buffer_rx[DATASIZE*2];

	spi_write(&spi_slave_instance,0);
	uint16_t i ;
	for(i=0;i<DATASIZE*2;i++){	//a suppr
		buffer_rx[i]=i;
	}
     while(1) {
		#ifndef MASTER_R21//SPI management
		

			timeOut=0;
			if(receive(DATASIZE*2,buffer_rx)){//receive 1024 value *2
				//uint16_t crc=calculCRC(buffer_rx); //calcul the crc
				if(crcManage(crcCalcul)){ //send and check the crc
					dataParse(buffer_rx);
						
				}
			}
			else{
				//printf("ERROR TIMEOUT\r\n");
			}
		#else
			uint16_t indice=0;
			//while(!networkAvailable());
		
			
			while(indice<(DATASIZE*2)+1){
				while(networkAvailable()){

					if(indice<(DATASIZE*2)){
						buffer_rx[indice]=networkRead();
						indice++;
						
					}
					else{
						indice++;
						uint8_t crc = calculCRC(buffer_rx);
						uint8_t crcReceive=networkRead();
						if(crc==crcReceive){
							for(i=0;i<(DATASIZE*2);i++){
								printf("%c",buffer_rx[i]);
							}
							printf("%c",crc);
							//printf("crc ok \r\n");
							//send to HMI
						}
						else{
							//printf("ERROR crc NOK(%d!=%d)\r\n",crc,crcReceive);
						}
					
					}
				
				
				}
			}

		#endif

     }//end while true
	
}//end main

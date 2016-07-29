 #include "mbed.h"
  #include <internalClock.h>
 SPISlave device(PB22, PB02, PB23, PB03); // mosi, miso, sclk, ssel
 Serial hmi(USBTX,USBRX);
 uint16_t buffer[2060];

 int main() {
	 uint16_t indice =0;
	 hmi.baud(38400);
     hmi.printf("boot\r\n");
	 configureInternalClock();
     device.format(8,0);//8 bit mode 0
     //device.frequency(2343750);
    device.reply(0x00);              // Prime SPI with first reply
     while(1) {
		indice=0;
		timeOut=0;
		bool parity=false;
		while(!device.receive());
		while(timeOut<4){
			 while(device.receive()){
			
				if(indice<2060){
						if(parity==false){
							parity=true;
							buffer[indice]=device.read();	
							device.reply(buffer[indice]);			
							indice++;
							timeOut=0;
						}
						else{
							parity=false;
							device.reply(device.read());
						}
				}
				else{
					device.read();
				}
				
			 }
		}
		hmi.printf("receive:%d \r\n",indice);
		uint16_t value=0;
		for(uint16_t i=0;i<indice;i++){
			
			hmi.printf("%u:	%u\r\n",i,buffer[i]);
			if(i%2==0){
					value=0;
					value = (buffer[i]<<8)|value;
					
			}
			else{
					value = buffer[i]|value;
					
					hmi.printf("		%u:	%u\r\n",i,value);
					
			}				
		}
		hmi.printf("end\r\n");
         
     }
 }
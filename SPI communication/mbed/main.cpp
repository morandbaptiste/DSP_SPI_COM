 #include "mbed.h"
  #include <internalClock.h>
 SPISlave device(PB22, PB02, PB23, PB03); // mosi, miso, sclk, ssel
 Serial hmi(USBTX,USBRX);
 int main() {
     hmi.printf("boot\r\n");
	 configureInternalClock();
     device.format(8,0);
     device.frequency(2343750);
	int v;
    device.reply(0x00);              // Prime SPI with first reply
     while(1) {

         if(device.receive()) {
            v = device.read();   // Read byte from master
            hmi.printf("receive: %d\r\n",v);
            device.reply(v);         // Make this the next reply            
         }
     }
 }
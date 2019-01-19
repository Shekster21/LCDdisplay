// To display the string "Abhishek P" on a 16X2 LCD display connected to a 
//AT89C51 microcontroller with 12MHz clock.
// The bits D7-D0 are connected to port 2 bits P2.7 - P2.0.
// RS -> P3.0
// E -> P3.2
// R/W -> P3.1
#include<reg51.h>

void lcdInit(void);
void writeCommand(unsigned char );
void writeData(unsigned char );
void msdelay(unsigned char );
void lcdReady(void);



sbit rs = P3^0;
sbit e = P3^2;
sbit rw = P3^1;
sbit ready = P2^7;

void main(void){
	unsigned char myname[] = "Abhishek P";
	unsigned char count,addcount,address,tempcnt;
  lcdInit();
	while(1){
	for(addcount=0x00;addcount<0x1B;addcount++){
		if (addcount > 0x0f){
			address = 0x80;
			writeCommand(address);
			for(count = 0;count<=10;count++){
				tempcnt = count+(addcount-0x0f);
				if(tempcnt < 0x0A){
					writeData(myname[tempcnt]);
				}else{
					writeData(0x00);
				}
			msdelay(100);
		}
		}else{
			address = 0x8F - addcount;
			writeCommand(address);
		for(count = 0;count<=10;count++){
				writeData(myname[count]);
				msdelay(100);
		}
	}
	}
}
}
void lcdInit(void){
    writeCommand(0x38);
		writeCommand(0x0c);
		writeCommand(0x01);
		//writeCommand(0x8F);
	  return;
}
void writeCommand(unsigned char command){
    // sends the command to the command register of the lcd.
    lcdReady(); // check is LCD is ready to receive command
    P2 = command; // output command on port 2
    rs = 0; // rs = 0 selects command register in the LCD
    rw = 0; // rw = 0 writes data into the LCD
    e = 1;  // high pulse on E
    msdelay(1); // delay
    e = 0; // H-to-L transition.
		return;
}

void writeData(unsigned char databyte){
    // writes data into the dataregister of lcd display.
    lcdReady();
    P2 = databyte;// output data on port 2
    rs = 1; // rs = 1 selects data register in the LCD
    rw = 0; // rw = 0 writes data into lcd
    e = 1; // high pulse on E
    msdelay(1); // delay
    e = 0; // H-to-L transition
		return;
}

void msdelay(unsigned char delay){
    // max value of delay = 200ms. 
    unsigned int mcount = 4 * delay; // initailize max count
    unsigned int count;
    TMOD = 0x02; // timer 0 in mode 2
    TH0 = 0x06; // autoreload value to produce 0.25ms delay
    TR0 = 1; // start counter
    for(count = 0;count<mcount;count++){
        while(TF0 == 0);
        TF0 = 1; // clear overflow flag
    }
    TR0 = 0; //stop timer
		return;
}

void lcdReady(void){
    // checks if LCD is ready to receive data/commands
    P2 = 0xFF; // initialises port2 as input port
    rs = 0; // command register select
    rw = 1; // read command register
    while(ready == 1)			// loop while LCD is busy
		{
			e = 0;
			msdelay(2);
			e = 1;
		
		}
		rw = 0;
		return;
}
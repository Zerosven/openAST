
/*
 * openAST Arduino rewrite
 * open source AMB Simulated transponder
 *  Created: 2014-07-21 16:09:20 Rewrite in arduino 2018
 *   Author: condac
 *
 *    Notes: This is the old AMB system not compatible with the new current RC4 system

 Copyright (C) 2014  condac

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

 
char timerstop = 0b01010000;
char timerstart = 0b01010001;

boolean sendStatus = false;

// 2351957
// ....................pre1  pre2  d1    d2    d3    d4    d5    d6    d7    d8    d9    d10
char idMessage[12] = { 0XF9, 0X16, 0XE1, 0XCB, 0X12, 0X1C, 0XC9, 0XD6, 0XC3, 0XE0, 0XFF, 0X0F };
char Status1[12]   = { 0xF9, 0x16, 0xDA, 0xE7, 0x94, 0x77, 0xE9, 0x3C, 0x91, 0xD7, 0xC3, 0xCC };
char Status2[12]   = { 0xF9, 0x16, 0xEC, 0x50, 0x55, 0x92, 0xE2, 0x23, 0x61, 0xD4, 0xF0, 0x0C };
char Status3[12]   = { 0xF9, 0x16, 0x36, 0x58, 0x15, 0x1B, 0xC8, 0xC3, 0x62, 0x14, 0x3C, 0x00 };
char Status4[12]   = { 0xF9, 0x16, 0x0E, 0x29, 0xBA, 0xE0, 0x3E, 0xE3, 0x62, 0xDB, 0xC0, 0xC3 };
char Status5[12]   = { 0xF9, 0x16, 0x36, 0x55, 0x57, 0x09, 0xFB, 0x3F, 0x91, 0x27, 0x00, 0xF0 };
char Status6[12]   = { 0xF9, 0x16, 0x0E, 0xFE, 0xF0, 0x8A, 0x22, 0x3C, 0x52, 0x1B, 0x3F, 0xF3 };
char Status7[12]   = { 0xF9, 0x16, 0xD7, 0xA8, 0x10, 0x77, 0xD1, 0x23, 0xA2, 0xD7, 0xC3, 0x3C };


void setup() {
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  
  TCCR1 = 0b01010001;  // set COM1A0 PWM1A CS10  COM1A0 tells the timer to Toggle the value on OC1A pins (page 89 attiny25 datasheet)
			//                       CS10 set timer prescale to 1
			 //            PWM1A enable pwm output
			// The timer needs to count to 255 before it starts to reset after 4 counts instead, don't know why
  OCR1A = 0x02;
  OCR1C = 0x03;


}

void messages() {
  sendData(idMessage);
  delayMicroseconds(2000+random(1000));
  sendData(idMessage);
  delayMicroseconds(2000+random(1000));
  sendData(idMessage);
  delayMicroseconds(2000+random(1000));
}

void loop() {

  digitalWrite(2,LOW);
  while (true) { // this while is not needed
    messages();
    
    if (sendStatus) {

      sendData(Status1);
      delayMicroseconds(2000+random(1000));
      messages();
    
      sendData(Status2);
      delayMicroseconds(2000+random(1000));
      messages();
    
      sendData(Status3);
      delayMicroseconds(2000+random(1000));
      messages();
    
      sendData(Status4);
      delayMicroseconds(2000+random(1000));
      messages();
    
      sendData(Status5);
      delayMicroseconds(2000+random(1000));
      messages();
    
      sendData(Status6);
      delayMicroseconds(2000+random(1000));
      messages();
    
      sendData(Status7);
      delayMicroseconds(2000+random(1000));
    }
  }
  
  
  
  //delay(100);
}

void cycle_delay() {
  asm("nop");
  asm("nop");
  asm("nop");
}

void sendByte(char inData) {

  if( (inData & (1 << 7)) ) {
    TCCR1 = timerstop;
  }
  TCCR1 = timerstart; //  
  cycle_delay();

  if( (inData & (1 << 6)) ) {
    TCCR1 = timerstop;
  }
  TCCR1 = timerstart; //  
  cycle_delay();

  
  if( (inData & (1 << 5)) ) {
    TCCR1 = timerstop;
  }
  TCCR1 = timerstart; //  
  cycle_delay();

  
  if( (inData & (1 << 4)) ) {
    TCCR1 = timerstop;
  }
  TCCR1 = timerstart; //  
  cycle_delay();

  
  if( (inData & (1 << 3)) ) {
    TCCR1 = timerstop;
  }
  TCCR1 = timerstart; //  
  cycle_delay();

  
  if( (inData & (1 << 2)) ) {
    TCCR1 = timerstop;
  }
  TCCR1 = timerstart; 
  cycle_delay();

  
  if( (inData & (1 << 1)) ) {
    TCCR1 = timerstop;
  }
  TCCR1 = timerstart; 
  cycle_delay();

  
  if( (inData & (1 << 0)) ) {
    TCCR1 = timerstop;
  }
  TCCR1 = timerstart; 
  cycle_delay();

  // TODO: look if the last delay is to long with the return calls
 
}
void sendData(char dataArray[]) {
  digitalWrite(2,HIGH); // stop led while sending data
  
  TCCR1 = timerstart;


  noInterrupts();
  
  sendByte(0x00);
  sendByte(dataArray[0]);
  sendByte(dataArray[1]);
  sendByte(dataArray[2]);
  sendByte(dataArray[3]);
  sendByte(dataArray[4]);
  sendByte(dataArray[5]);
  sendByte(dataArray[6]);
  sendByte(dataArray[7]);
  sendByte(dataArray[8]);
  sendByte(dataArray[9]);
  sendByte(dataArray[10]);
  sendByte(dataArray[11]);
  
  TCCR1 = timerstop; //out  TCCR1,timerstop // Shut down radio
                     //ldi  slask, 0b0100_0000 // set COM1A0 (bit4) to 0 to disconnect timer pins
  TCCR1 = 0b01000000;//out  TCCR1,slask
  interrupts();

  
  
  digitalWrite(0,LOW);//cbi  PORTB, 0  // set both output to 0 to avoid current drain
  digitalWrite(1,LOW);//cbi  PORTB, 1  // set both output to 0 to avoid current drain
  
}

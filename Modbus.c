//==============================================================================
//
// Title:		Modbus.c
// Purpose:		A short description of the implementation.
//
// Created on:	22.02.2021 at 14:26:41 by user.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "toolbox.h"
#include "Modbus.h"
#include <userint.h>



#include "Global.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables
static unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
} ;

/* Table of CRC values for low–order byte */
static char auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
} ;



//==============================================================================
// Static functions
unsigned short CrcFunction(unsigned char* puchMsg, unsigned short usDataLen) {

	unsigned char uchCRCHi = 0xFF ; /* high byte of CRC initialized */
	unsigned char uchCRCLo = 0xFF ; /* low byte of CRC initialized */
	unsigned uIndex ; /* will index into CRC lookup table */
	while (usDataLen--) /* pass through message buffer */
	{
	uIndex = uchCRCHi ^ *puchMsg++ ; /* calculate the CRC */
	uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
	uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (unsigned short)(uchCRCHi << 8 | uchCRCLo) ;
}
uint16_t Swap16(uint16_t  data) {   	

	data=(unsigned short)((data >> 8) | (data << 8));

	return data;
}

uint8_t Swap8(uint8_t data) {
	data = (data >> 4) | (data << 4) ; 
	return data; 
}

//==============================================================================
// Global variables
//extern portNumber;


//==============================================================================
// Global functions
int MBS_Function10H(int portNumber, uint8_t slaveID, uint16_t startingAddress, int dataType, uint16_t registerNumber, void *inputData) {
	
	FlushInQ(portNumber);
	char readBuffer10h[256] = {0}; 
	SModbus10HQuery sendBuffer10h = {0}; 
	SModbus10HResponse* pFrame;
	float* pOutput;
	int messageLength = 7 ; // slaveID , function, address,bytecount 
	char tempBuffer[12];
	
	sendBuffer10h.slaveID = slaveID;
	sendBuffer10h.functionCode = 0x10;
	sendBuffer10h.startingAddress = Swap16(startingAddress);
	sendBuffer10h.registerNumber = Swap16(registerNumber);	
	uint16_t crcBytes;
	int i;
	int crcStartIndex = 0;
	if(dataType == Bits_32) {										 //32bit data yazmak için
		sendBuffer10h.byteCount= 2 * (int)registerNumber; 
	  
			for(i=0 ; i<(sendBuffer10h.byteCount/4) ; i++) {
				if(i!=0) {
					inputData = ((uint32_t*)inputData)+1; 
				}
		 		sendBuffer10h.dataAndCrc[4*i+0] = (*(uint32_t*)inputData >> 24);	 
				sendBuffer10h.dataAndCrc[4*i+1] = (*(uint32_t*)inputData >> 16);	
				sendBuffer10h.dataAndCrc[4*i+2] = (*(uint32_t*)inputData >> 8);	
				sendBuffer10h.dataAndCrc[4*i+3] = (*(uint32_t*)inputData);
				crcStartIndex = 4*i+4;
				messageLength += 4;	
		 	}
			 
			 
      	crcBytes = CrcFunction((unsigned char*)&sendBuffer10h,messageLength);
		sendBuffer10h.dataAndCrc[crcStartIndex]=(uint8_t)(crcBytes >> 8);
		sendBuffer10h.dataAndCrc[crcStartIndex+1]=(uint8_t)crcBytes;
		messageLength += 2;	
	  	ComWrt(portNumber, (char*)&sendBuffer10h,messageLength );
	}
	
	else if(dataType == Float) {
		
		sendBuffer10h.byteCount = 2 * (int)registerNumber ;	
	    for(i=0 ; i<(sendBuffer10h.byteCount/4) ; i++) {
			if(i!=0) {
					inputData = ((uint32_t*)inputData)+1; 
			}	
			sendBuffer10h.dataAndCrc[4*i+0] = ((uint8_t)*(uint32_t*)inputData >> 24);	 
			sendBuffer10h.dataAndCrc[4*i+1] = ((uint8_t)*(uint32_t*)inputData >> 16);	
			sendBuffer10h.dataAndCrc[4*i+2] = ((uint8_t)*(uint32_t*)inputData >> 8);	
			sendBuffer10h.dataAndCrc[4*i+3] = ((uint8_t)*(uint32_t*)inputData) ;
			crcStartIndex = 4*i+4;
			messageLength += 4;
		}	 
		crcBytes = CrcFunction((unsigned char*)&sendBuffer10h, 11 );		
		sendBuffer10h.dataAndCrc[crcStartIndex] = (uint8_t)(crcBytes >> 8);
		sendBuffer10h.dataAndCrc[crcStartIndex+1] = (uint8_t)crcBytes;		
		messageLength += 2;			
		pOutput=(float*)sendBuffer10h.dataAndCrc ; 
  		ComWrt(portNumber, (char*)&sendBuffer10h,messageLength );	
	}		
		
	else if(dataType == Bits_16) {
		sendBuffer10h.byteCount= 2*(int)registerNumber; 
	 
		for(i=0 ; i<(sendBuffer10h.byteCount/4) ; i++) {
			if(i!=0) ((uint16_t*)inputData)++;  
			sendBuffer10h.dataAndCrc[4*i+0] = (uint8_t)(*(uint16_t*)inputData >> 8);	 
			sendBuffer10h.dataAndCrc[4*i+1] = (uint8_t)(*(uint16_t*)inputData) ;
			crcStartIndex = 4*i+2;
			messageLength += 2; 
			
	    }
	
		crcBytes=CrcFunction((unsigned char*)&sendBuffer10h,messageLength ); 
		sendBuffer10h.dataAndCrc[crcStartIndex]=(uint8_t)(crcBytes >> 8);
		sendBuffer10h.dataAndCrc[crcStartIndex+1]=(uint8_t)crcBytes;	
		messageLength += 2;
		ComWrt(portNumber, (char*)&sendBuffer10h,messageLength );
		
    } else if (dataType == Char) { 
			sendBuffer10h.byteCount= 2 * (int)registerNumber; // char yazmak için 
		//	memset(tempBuffer, 0, 12);
			//strncat(tempBuffer,(char*)inputData,sendBuffer10h.byteCount);
		//	for (i=0; i < (sendBuffer10h.byteCount) ; i ++){
		//		tempBuffer[i] = *((char*)inputData+i);
		//	}
			for(i=0 ; i<(sendBuffer10h.byteCount) ; i++) {
				//	if(i!=0) ((char*)inputData)++;
					sendBuffer10h.dataAndCrc[i] = *((char*)inputData+i);
					crcStartIndex++;
					messageLength++; 
		 	}
      	crcBytes = CrcFunction((unsigned char*)&sendBuffer10h,messageLength);
		sendBuffer10h.dataAndCrc[crcStartIndex]=(uint8_t)(crcBytes >> 8);
		sendBuffer10h.dataAndCrc[crcStartIndex+1]=(uint8_t)crcBytes;
		messageLength += 2;	
	  	ComWrt(portNumber, (char*)&sendBuffer10h,messageLength ); 
	}
	else if(dataType == Bits_8) {
		sendBuffer10h.byteCount= 2 * (int)registerNumber;
		for(i=0 ; i<(sendBuffer10h.byteCount) ; i++) {
				//	if(i!=0) ((char*)inputData)++;
				memcpy(&sendBuffer10h.dataAndCrc[i],((uint8_t*)inputData+i),sizeof(uint8_t));
				crcStartIndex++;
				messageLength++;
				if(sendBuffer10h.byteCount-i <= 4) { 
					sendBuffer10h.dataAndCrc[i+0] = ((uint8_t)*(uint32_t*)inputData+i >> 24);	 
					sendBuffer10h.dataAndCrc[i+1] = ((uint8_t)*(uint32_t*)inputData+i >> 16);	
					sendBuffer10h.dataAndCrc[i+2] = ((uint8_t)*(uint32_t*)inputData+i >> 8);	
					sendBuffer10h.dataAndCrc[i+3] = ((uint8_t)*(uint32_t*)inputData+i) ;
					crcStartIndex = i+4;
					messageLength += 4;
					break ; 
				}
		}
		crcBytes = CrcFunction((unsigned char*)&sendBuffer10h,messageLength);
		sendBuffer10h.dataAndCrc[crcStartIndex]=(uint8_t)(crcBytes >> 8);
		sendBuffer10h.dataAndCrc[crcStartIndex+1]=(uint8_t)crcBytes;
		messageLength += 2;	
	  	ComWrt(portNumber, (char*)&sendBuffer10h,messageLength ); 
	}
		
	
	int bytesRead = ComRd (portNumber,readBuffer10h,8 ); 
	//frame = (sResponse_10H*)readBuffer10h;  
	
return bytesRead; }


int MBS_Function3H(int portNumber, uint8_t slaveID, uint16_t startingAddress, int dataType, uint16_t registerNumber, void* outputData) {
	float* pOutput;
	int i;
	char readBuffer[256] = {0};
	unsigned __int64 output64 = 0;
	
	
	SModbus3HQuery sendBuffer;
	SResponse3H4H* pResponse;
	sendBuffer.slaveID = slaveID;
	sendBuffer.functionCode = 3;
	sendBuffer.startingAddress = Swap16(startingAddress);

	sendBuffer.registerNumber = Swap16(registerNumber);
	sendBuffer.crc = Swap16(CrcFunction((unsigned char*)&sendBuffer, 6));
		
	pResponse = (SResponse3H4H*)readBuffer;
	
	uint16_t messageLength = 5 + (registerNumber*2); 
	FlushInQ (portNumber);
	int bytesWritten = ComWrt(portNumber, (char*)&sendBuffer, 8); 
	Delay(0.1);
	if (bytesWritten < 0) {
		return -1 ;
	}
	int bytesRead = ComRd (portNumber, readBuffer,messageLength);
	
	if(dataType == Float) {
		for(i=0 ; i < registerNumber/2 ; i++) {
			*((uint32_t*)outputData + i) =(pResponse->data[4*i] << 24)		| 
										  (pResponse->data[4*i + 1] << 16) 	|
										  (pResponse->data[4*i +2 ] << 8)	| 
										  (pResponse->data[4*i + 3]); 
		
		}
		
		pOutput = (float*)outputData;
	
	} else if(dataType == Bits_32) {
		for(i=0 ; i < registerNumber/2 ; i++) {
			*((uint32_t*)outputData + i) =  (pResponse->data[4*i+0] << 24)		| 
					  	   			        (pResponse->data[4*i + 1] << 16) 	|
					  	   			        (pResponse->data[4*i + 2] << 8)	    | 
					       			        (pResponse->data[4*i + 3]) ;
					
	
		}
		
	} else if(dataType == Bits_16) {
		  for(i = 0 ; i < registerNumber ; i++) {										
		  		*((uint16_t*)outputData + i) = (uint16_t)(pResponse->data[2*i] << 8) | pResponse->data[2*i + 1];
		   }
		
	} else if(dataType == Double) {
		for(i=0 ; i < registerNumber/4 ; i++) {
			output64 = ToBigEndian64(*(((unsigned __int64*)&pResponse->data) + i));
			*((double*)outputData + i) = *((double*)&output64);
		}
	}
return bytesRead ;
}   

int MBS_Function4H(int portNumber, uint8_t slaveID, uint16_t startingAddress, int dataType, uint16_t registerNumber,void *outputData) {
	
	float* pOutput;
	int i;
	char readBuffer[256] = {0};
	unsigned __int64 output64 = 0; 
	
	
	SModbus3HQuery sendBuffer;
	SResponse3H4H* pResponse;
	sendBuffer.slaveID = slaveID;
	sendBuffer.functionCode = 4;
	sendBuffer.startingAddress = Swap16(startingAddress);
	sendBuffer.registerNumber = Swap16(registerNumber);
	sendBuffer.crc = Swap16(CrcFunction((unsigned char*)&sendBuffer, 6));	
	pResponse = (SResponse3H4H*)readBuffer;	
	uint16_t messageLength = 5+ (registerNumber*2); 
	FlushInQ (portNumber);
	ComWrt(portNumber, (char*)&sendBuffer, 8); 
	Delay(0.1);
	int bytesRead = ComRd (portNumber, readBuffer,messageLength);
	
	if(dataType == Float) {
		for(i=0 ; i < registerNumber /2 ; i++) {
		*((uint32_t*)outputData + i) =(pResponse->data[4*i] << 24)		| 
							  	      (pResponse->data[4*i + 1] << 16) 	|
						 		      (pResponse->data[4*i +2 ] << 8)	| 
						  	          (pResponse->data[4*i + 3]); 
     
		}
		
		pOutput = (float*)outputData;
	
	} else if(dataType == Bits_32) {
		for(i=0 ; i < registerNumber/2 ; i++) {
				*((uint32_t*)outputData + i) =  (pResponse->data[4*i+0] << 24)		| 
												(pResponse->data[4*i + 1] << 16) 	|
												(pResponse->data[4*i + 2] << 8)	    | 
												(pResponse->data[4*i + 3]) ;
						
	
		}
		
	} else if(dataType == Bits_16) {
		  for(i = 0 ; i < registerNumber/2 ; i++) {										
		  		*((uint16_t*)outputData + i) = (uint16_t)(pResponse->data[2*i] << 8) | pResponse->data[2*i + 1];
		   }
		
	} else if(dataType == Double) {
		for(i=0 ; i < registerNumber/4 ; i++) {
			output64 = ToBigEndian64(*(((unsigned __int64*)&pResponse->data) + i));
			*((double*)outputData + i) = *((double*)&output64);
		}
	}
	
	return bytesRead ;	
}

int MBS_Function6H(int portNumber, uint8_t slaveID ,uint16_t startingAddress, int dummy, uint16_t registerNumber, void *inputData) {
	 
	char readBuffer[256] = {0}; 
	
	SModbus6HQuery sendBuffer = {0}; 

	sendBuffer.slaveID = slaveID;
	sendBuffer.functionCode = 0x06;
	sendBuffer.startingAddress = Swap16(startingAddress);
	
	sendBuffer.data[0] = (*(uint16_t*)inputData) >> 8;
	sendBuffer.data[1] = (*(uint16_t*)inputData);
	
	sendBuffer.crc=Swap16(CrcFunction((unsigned char*)&sendBuffer, 6 ));
	
	int bytesWritten = ComWrt(portNumber,(char*)&sendBuffer,8 ); 
	if (bytesWritten < 0 ) {
			return -1 ; 
	}
    int bytesRead = ComRd (portNumber,readBuffer,8 );  
	
	return bytesRead;
}

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?


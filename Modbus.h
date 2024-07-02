//==============================================================================
//
// Title:		Modbus.h
// Purpose:		A short description of the interface.
//
// Created on:	22.02.2021 at 14:26:41 by user.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __Modbus_H__
#define __Modbus_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"
		
#include <rs232.h>
#include <utility.h> 
#include "cvidef.h"
#include "stdint.h"
#include "stdbool.h"

//==============================================================================
// Constants
#define INT32							1 
#define FLOAT							0
#define INT16							2
#define DOUBLE							3
#define CHAR 							4
#define BITS_8							5		

//==============================================================================
// Types
typedef struct SRESPONSE3H4H {
	uint8_t slaveID;
	uint8_t functionCode;
	uint8_t byteCount;
	uint8_t data[251];
	uint8_t crc[2] 
} SResponse3H4H ; 

typedef struct SMODBUS3HQUERY { 
	uint8_t 	slaveID; 
	uint8_t 	functionCode;
	uint16_t 	startingAddress; 
	uint16_t 	registerNumber;
	uint16_t 	crc ; 
} SModbus3HQuery ; 


typedef struct SMODBUS10HQUERY { 
	uint8_t 	slaveID; 
	uint8_t 	functionCode; 
	uint16_t	startingAddress; 
	uint16_t	registerNumber; 
	uint8_t 	byteCount; 
	uint8_t 	dataAndCrc[248] ;
} SModbus10HQuery ; 

typedef struct SMODBUS10HRESPONSE {
	uint8_t 	slaveID ; 
	uint8_t 	functionCode; 
	uint16_t    startingAddress;
	uint16_t 	registerNumber;
	uint8_t		crc[2];
} SModbus10HResponse;

typedef struct SMODBUS6HQUERY {
	uint8_t slaveID ;
	uint8_t functionCode;
	uint16_t startingAddress;
	uint8_t data[2] ; 
	uint16_t crc ; 
}SModbus6HQuery ;

typedef enum DATATYPES{
	Float,
	Bits_32,
	Bits_16,
	Double,
	Char ,
	Bits_8
}DataTypes ;
	
	

//==============================================================================
// External variables
DataTypes typeData ;

//==============================================================================
// Global functions
int MBS_Function10H(int portNumber,  uint8_t slaveID,  uint16_t startingAddress, int dataType, uint16_t registerNumber, void *inputData);
int MBS_Function3H(int portNumber ,  uint8_t slaveID,  uint16_t startingAddress, int dataType, uint16_t registerNumber, void *outputData);
int MBS_Function4H(int portNumber ,  uint8_t slaveID,  uint16_t startingAddress, int dataType, uint16_t registerNumber, void *outputData);
int MBS_Function6H(int portNumber ,  uint8_t slaveID,  uint16_t startingAddress, int dummy   , uint16_t registerNumber, void *inputData) ;



#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Modbus_H__ */

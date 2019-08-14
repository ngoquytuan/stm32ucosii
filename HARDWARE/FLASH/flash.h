#ifndef __FLASH_H
#define __FLASH_H			    
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//This program is for learning use only and may not be used for any other purpose without the permission of the author.
//Mini STM32 development board
//W25X16 Drive function	   
//Punctual atom@ALIENTEK
//technology Forum:www.openedv.com
//Modified date:2010/6/13 
//°æ±¾£ºV1.0
//Copyright, piracy will be investigated.
//Copyright(C) Punctual atom 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	 
#define W25Q16 	0XEF14
 
#define SPI_FLASH_TYPE	W25Q16

#define	SPI_FLASH_CS PAout(2)  //Check FLASH					 
////////////////////////////////////////////////////////////////////////////

extern u8 SPI_FLASH_BUF[4096];
//W25X16 read and write
#define FLASH_ID 0XEF14
//Instruction list
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

void SPI_Flash_Init(void);
u16  SPI_Flash_ReadID(void);  	    //Read FLASH ID
u8	 SPI_Flash_ReadSR(void);        //Read Status register 
void SPI_FLASH_Write_SR(u8 sr);  	//Write status register
void SPI_FLASH_Write_Enable(void);  //Write enable 
void SPI_FLASH_Write_Disable(void);	//Write protection
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //Read flash
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//Write flash
void SPI_Flash_Erase_Chip(void);    	  //Whole erase
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//Sector erase
void SPI_Flash_Wait_Busy(void);           //Waiting for free
void SPI_Flash_PowerDown(void);           //Enter power-down mode
void SPI_Flash_WAKEUP(void);			  //wake
#endif

















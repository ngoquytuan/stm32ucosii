#include "flash.h" 
#include "spi.h"
#include "delay.h"   
//Mini STM32 development board
//W25X16 driver function
//Positive point atom @ALIENTEK
//2010/6/13
//V1.0

//4Kbytes is a Sector
//16 sectors are 1 block
//W25X16
// Capacity is 2M bytes, a total of 32 Block, 512 Sector

// Initialize the IO port of SPI FLASH
void SPI_Flash_Init(void)
{

}  

// Read the status register of SPI_FLASH
//BIT7 6 5 4 3 2 1 0
//SPR RV TB BP2 BP1 BP0 WEL BUSY
//SPR: default 0, status register protection bit, used with WP
//TB, BP2, BP1, BP0: FLASH area write protection settings
//WEL: Write enable lock
//BUSY: Busy flag bit (1, busy; 0, idle)
//Default: 0x00
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	SPI_FLASH_CS=0;                            //Enable device   
	stm32_spi_rw(W25X_ReadStatusReg);    //Send Read Status Register command    
	byte=stm32_spi_rw(0Xff);             //Read a byte  
	SPI_FLASH_CS=1;                            //Cancel chip selection     
	return byte;   
} 
// Write SPI_FLASH status register
//Only SPR, TB, BP2, BP1, BP0 (bit 7, 5, 4, 3, 2) can be written!!!
void SPI_FLASH_Write_SR(u8 sr)   
{   
	SPI_FLASH_CS=0;                            //Enable device   
	stm32_spi_rw(W25X_WriteStatusReg);   //Send Write Status Register command    
	stm32_spi_rw(sr);               //Write a byte  
	SPI_FLASH_CS=1;                            //Cancel chip selection     	      
}   
//SPI_FLASH write enable
// Set WEL  
void SPI_FLASH_Write_Enable(void)   
{
	SPI_FLASH_CS=0;                            //Enable device   
    stm32_spi_rw(W25X_WriteEnable);      //Send write enable  
	SPI_FLASH_CS=1;                            //Cancel chip selection     	      
} 
//SPI_FLASH write prohibition
// Clear WEL
void SPI_FLASH_Write_Disable(void)   
{  
	SPI_FLASH_CS=0;                            //Enable device   
    stm32_spi_rw(W25X_WriteDisable);     //Send write disable command    
	SPI_FLASH_CS=1;                            //Cancel chip selection     	      
} 			    
//Read chip ID W25X16 ID: 0XEF14
u16 SPI_Flash_ReadID(void)
{
	u16 Temp = 0;	  
	SPI_FLASH_CS=0;				    
	stm32_spi_rw(0x90);//Send Read ID command	    
	stm32_spi_rw(0x00); 	    
	stm32_spi_rw(0x00); 	    
	stm32_spi_rw(0x00); 	 			   
	Temp|=stm32_spi_rw(0xFF)<<8;  
	Temp|=stm32_spi_rw(0xFF);	 
	SPI_FLASH_CS=1;				    
	return Temp;
}   		    
// Read SPI FLASH
// Start reading the specified length of data at the specified address
//pBuffer: data storage area
//ReadAddr: the address to start reading (24bit)
//NumByteToRead: the number of bytes to read (maximum 65535)
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;    												    
	SPI_FLASH_CS=0;                            //Enable device   
    stm32_spi_rw(W25X_ReadData);         //Send read command   
    stm32_spi_rw((u8)((ReadAddr)>>16));  //Send a 24-bit address   
    stm32_spi_rw((u8)((ReadAddr)>>8));   
    stm32_spi_rw((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=stm32_spi_rw(0XFF);   //Cyclic reading  
    }
	SPI_FLASH_CS=1;                            //Cancel chip selection     	      
}  
//SPI writes less than 256 bytes of data in one page (0~65535)
// Start writing the maximum 256 bytes of data at the specified address
//pBuffer: data storage area
//WriteAddr: the address to start writing (24bit)
//NumByteToWrite: The number of bytes to write (maximum 256), which should not exceed the number of bytes remaining on the page!!!	 
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    SPI_FLASH_Write_Enable();                  //SET WEL 
	SPI_FLASH_CS=0;                            //Enable device   
    stm32_spi_rw(W25X_PageProgram);      //Send page write command   
    stm32_spi_rw((u8)((WriteAddr)>>16)); //Send a 24-bit address    
    stm32_spi_rw((u8)((WriteAddr)>>8));   
    stm32_spi_rw((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)stm32_spi_rw(pBuffer[i]);//Cyclic write number  
	SPI_FLASH_CS=1;                            //Cancel chip selection 
	SPI_Flash_Wait_Busy();					   //Waiting for writing to end
} 
//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 
//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)  		   
u8 SPI_FLASH_BUF[4096];
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=WriteAddr/4096;//扇区地址 0~511 for w25x16
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			SPI_Flash_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//写入整个扇区  

		}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};	 	 
}
//擦除整个芯片
//整片擦除时间:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//等待时间超长...
void SPI_Flash_Erase_Chip(void)   
{                                             
    SPI_FLASH_Write_Enable();                  //SET WEL 
    SPI_Flash_Wait_Busy();   
  	SPI_FLASH_CS=0;                            //Enable device   
    stm32_spi_rw(W25X_ChipErase);        //发送片擦除命令  
	SPI_FLASH_CS=1;                            //Cancel chip selection     	      
	SPI_Flash_Wait_Busy();   				   //等待芯片擦除结束
}   
//擦除一个扇区
//Dst_Addr:扇区地址 0~511 for w25x16
//擦除一个山区的最少时间:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   
	Dst_Addr*=4096;
    SPI_FLASH_Write_Enable();                  //SET WEL 	 
    SPI_Flash_Wait_Busy();   
  	SPI_FLASH_CS=0;                            //Enable device   
    stm32_spi_rw(W25X_SectorErase);      //发送扇区擦除指令 
    stm32_spi_rw((u8)((Dst_Addr)>>16));  //发送24bit地址    
    stm32_spi_rw((u8)((Dst_Addr)>>8));   
    stm32_spi_rw((u8)Dst_Addr);  
	SPI_FLASH_CS=1;                            //Cancel chip selection     	      
    SPI_Flash_Wait_Busy();   				   //等待擦除完成
}  
//等待空闲
void SPI_Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}  
//进入掉电模式
void SPI_Flash_PowerDown(void)   
{ 
  	SPI_FLASH_CS=0;                            //Enable device   
    stm32_spi_rw(W25X_PowerDown);        //发送掉电命令  
	SPI_FLASH_CS=1;                            //Cancel chip selection     	      
    delay_us(3);                               //等待TPD  
}   
//唤醒
void SPI_Flash_WAKEUP(void)   
{  
  	SPI_FLASH_CS=0;                            //Enable device   
    stm32_spi_rw(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	SPI_FLASH_CS=1;                            //Cancel chip selection     	      
    delay_us(3);                               //等待TRES1
}   



























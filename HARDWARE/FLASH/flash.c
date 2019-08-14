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
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)  		   
u8 SPI_FLASH_BUF[4096];
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=WriteAddr/4096;//������ַ 0~511 for w25x16
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			SPI_Flash_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//д����������  

		}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 	 
}
//��������оƬ
//��Ƭ����ʱ��:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//�ȴ�ʱ�䳬��...
void SPI_Flash_Erase_Chip(void)   
{                                             
    SPI_FLASH_Write_Enable();                  //SET WEL 
    SPI_Flash_Wait_Busy();   
  	SPI_FLASH_CS=0;                            //Enable device   
    stm32_spi_rw(W25X_ChipErase);        //����Ƭ��������  
	SPI_FLASH_CS=1;                            //Cancel chip selection     	      
	SPI_Flash_Wait_Busy();   				   //�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ 0~511 for w25x16
//����һ��ɽ��������ʱ��:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   
	Dst_Addr*=4096;
    SPI_FLASH_Write_Enable();                  //SET WEL 	 
    SPI_Flash_Wait_Busy();   
  	SPI_FLASH_CS=0;                            //Enable device   
    stm32_spi_rw(W25X_SectorErase);      //������������ָ�� 
    stm32_spi_rw((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    stm32_spi_rw((u8)((Dst_Addr)>>8));   
    stm32_spi_rw((u8)Dst_Addr);  
	SPI_FLASH_CS=1;                            //Cancel chip selection     	      
    SPI_Flash_Wait_Busy();   				   //�ȴ��������
}  
//�ȴ�����
void SPI_Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  
//�������ģʽ
void SPI_Flash_PowerDown(void)   
{ 
  	SPI_FLASH_CS=0;                            //Enable device   
    stm32_spi_rw(W25X_PowerDown);        //���͵�������  
	SPI_FLASH_CS=1;                            //Cancel chip selection     	      
    delay_us(3);                               //�ȴ�TPD  
}   
//����
void SPI_Flash_WAKEUP(void)   
{  
  	SPI_FLASH_CS=0;                            //Enable device   
    stm32_spi_rw(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	SPI_FLASH_CS=1;                            //Cancel chip selection     	      
    delay_us(3);                               //�ȴ�TRES1
}   



























#include "Flash.h"
/*һλ��ַ8�ֽ�  8λ��ַ 64�ֽ� */
void Flash_WriteData(uint32_t addr,uint64_t *data,uint16_t Size)
{
    //1������FLASH
	static uint8_t bank1[128]={0};
    HAL_FLASH_Unlock();

    //2������FLASH
    //��ʼ��FLASH_EraseInitTypeDef
	if(!bank1[(addr-0x08000000)/0x800])
	{
		bank1[(addr-0x08000000)/0x800]=1;
		FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.Page = (addr-0x08000000)/0x800;
    f.NbPages = 1;
		f.Banks=FLASH_BANK_1;
    //����PageError
    uint32_t PageError = 0;
    //���ò�������
    HAL_FLASHEx_Erase(&f, &PageError);
	}
    //3����FLASH��д
    uint16_t TempBuf = 0;
    for(uint16_t i = 0;i< Size ;i++)
    {
        TempBuf = *(data+i);
        HAL_FLASH_Program(TYPEPROGRAM_DOUBLEWORD , addr + i * 8, TempBuf);
    }

    //4����סFLASH
    HAL_FLASH_Lock();
}

// 0 -127 page  BANK1  
// 0 -127 page  BANK2
void flash_read(uint64_t address,uint8_t BANK, uint64_t *data)//��BNAK1 ���� BANK2 �� ĳһ��ַ�������� 64 bit
{
    uint64_t ret = HAL_OK;
		if(BANK==1)
			address+=FLASH_BASE+0xE00;
		if(BANK==2)
			address+=FLASH_BASE+0x40000;
		*data =  *((uint64_t*)address);		
 
}
uint32_t FLASH_ReadHalfWord(uint64_t faddr)
{
	return *(__IO uint16_t*)faddr; 
}
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void Flash_ReadData(uint32_t ReadAddr,uint64_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=FLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=8;//ƫ��2���ֽ�.	
	}
}

 

/*д��ʾ��һ������8���ֽ�*/
//				Flash_WriteData(PARA_ADDR+0+record_num*32, (uint64_t *)&t, 1);	
//				 Flash_ReadData(PARA_ADDR+0+(recive-1)*32,data_rec,4);		
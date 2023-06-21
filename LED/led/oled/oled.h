#ifndef __OLED_H_
#define __OLED_H_	

#include "stdint.h"
#include "i2c.h"




void WriteCmd(void);
//���豸д��������
void OLED_WR_CMD(uint8_t cmd);
//���豸д����
void OLED_WR_DATA(uint8_t data);
//��ʼ��oled��Ļ
void OLED_Init(void);
	//����
void OLED_Clear(void);
//����
void OLED_Clearrow(uint8_t i);
//����OLED��ʾ    
void OLED_Display_On(void);
//�ر�OLED��ʾ     
void OLED_Display_Off(void);
//���ù��
void OLED_Set_Pos(uint8_t x, uint8_t y);

void OLED_On(void);
	
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);

 //��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2);

//��ʾһ���ַ��Ŵ�
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size);

//��ʾ����
//hzk ��ȡģ����ó�������
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);

void OLED_ver_scroll(void);

#endif

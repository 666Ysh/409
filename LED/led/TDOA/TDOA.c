#include "TDOA.h"
#include "exti.h"
#include "usart.h"
#include "stdio.h"
#include "math.h"
#include "tim.h"
_TDOA TDOA_S;
float x1,y1;
extern int flag;
void TDOA_Init(_TDOA *TDOA_S)
{
	TDOA_S->L=90;
	TDOA_S->r31=0;
	
	TDOA_S->r21=0;
	TDOA_S->x31=0;
	TDOA_S->x21=0;
	TDOA_S->y31=0;
	TDOA_S->y21=0;
	TDOA_S->x1=0;
	TDOA_S->x2=0;
	TDOA_S-> x3=0;
	TDOA_S-> y1=0;
	TDOA_S->y2=0;
	TDOA_S->y3=0;
	TDOA_S->r1=0;
	TDOA_S->r2=0;
	TDOA_S->r3=0;
	TDOA_S->K1=0;
	TDOA_S->K2=0;
	TDOA_S->K3=0;
	TDOA_S->p1=0;
	TDOA_S->q1=0;
	TDOA_S->p2=0;
	TDOA_S->q2=0;
	TDOA_S->a=0;
	TDOA_S->b=0;
	TDOA_S->c=0;
	TDOA_S->Angle=0;
	
}
	


void TDOA_Get_distanceANDangle(_TDOA *TDOA_S)
{
	
	//�ԣ�x1,y1��Ϊ��վ
	//�ֱ�����ģ��������ʾ����
	TDOA_S->x1=90.0;
	TDOA_S->y1=90.0;
	TDOA_S->x2=-90.0;
	TDOA_S->y2=90.0;
	TDOA_S->x3=-90.0;
	TDOA_S->y3=-90.0;
	                    
	//	������Դ��ģ��2,1��ģ��3,1��ʱ���
	TDOA_S->r21=(MIT2_S.mit_count-MIT1_S.mit_count)*0.34;
	TDOA_S->r31=(MIT3_S.mit_count-MIT1_S.mit_count)*0.34;
	
	//�м����
	TDOA_S->x21=TDOA_S->x2-TDOA_S->x1;
	TDOA_S->x31=TDOA_S->x3-TDOA_S->x1;
	TDOA_S->y21=TDOA_S->y2-TDOA_S->y1;
	TDOA_S->y31=TDOA_S->y3-TDOA_S->y1;
	
	//�м����
	TDOA_S->K1=pow(TDOA_S->x1,2.0)+pow(TDOA_S->y1,2.0);//K1��ʾx1*x1+y1*y1
	TDOA_S->K2=pow(TDOA_S->x2,2.0)+pow(TDOA_S->y2,2.0);//K2��ʾx2*x2+y2*y2
	TDOA_S->K3=pow(TDOA_S->x3,2.0)+pow(TDOA_S->y3,2.0);//K3��ʾx3*x3+y3*y3
	
	//����δ֪��ƽ��������һ�����Է���
	//2x21x+2y21y=(K2-K1)-(r21*r21+2r21r1)
	//2x31x+2y31y=(K3-K1)-(r31*r31+2r31r1)

	//��Ԫ���xy
	//�м����p1,q1��p2,q2
	TDOA_S->p1=TDOA_S->y21*TDOA_S->r31*TDOA_S->r31-TDOA_S->y31*TDOA_S->r21*TDOA_S->r21+TDOA_S->y31*
	(TDOA_S->K2-TDOA_S->K1)-TDOA_S->y21*(TDOA_S->K3-TDOA_S->K1);
	TDOA_S->p1=TDOA_S->p1/(2.0*(TDOA_S->x21*TDOA_S->y31-TDOA_S->x31*TDOA_S->y21));
	
	TDOA_S->q1=(TDOA_S->y21*TDOA_S->r31-TDOA_S->y31*TDOA_S->r21)/(TDOA_S->x21*TDOA_S->y31-TDOA_S->x31*TDOA_S->y21);
	
	TDOA_S->p2=TDOA_S->x21*TDOA_S->r31*TDOA_S->r31-TDOA_S->x31*TDOA_S->r21*TDOA_S->r21+TDOA_S->x31*
	(TDOA_S->K2-TDOA_S->K1)-TDOA_S->x21*(TDOA_S->K3-TDOA_S->K1);
	TDOA_S->p2=TDOA_S->p2/(2.0*(TDOA_S->x31*TDOA_S->y21-TDOA_S->x21*TDOA_S->y31));

	TDOA_S->q2=(TDOA_S->x21*TDOA_S->r31-TDOA_S->x31*TDOA_S->r21)/(TDOA_S->x31*TDOA_S->y21-TDOA_S->x21*TDOA_S->y31);
	
	//r1�ش����õ�����a*r^2+b*r^2+c=0
	TDOA_S->a=TDOA_S->q1*TDOA_S->q1+TDOA_S->q2*TDOA_S->q2-1.0;
	TDOA_S->b=-2.0*(TDOA_S->q1*(TDOA_S->x1-TDOA_S->p1)+TDOA_S->q2*(TDOA_S->y1-TDOA_S->p2));
	TDOA_S->c=(TDOA_S->x1-TDOA_S->p1)*(TDOA_S->x1-TDOA_S->p1)+(TDOA_S->y1-TDOA_S->p2)*(TDOA_S->y1-TDOA_S->p2);
	
	//�Խ�������ƣ���ʵ������룬b^2-4ac>0,���
		if(TDOA_S->b*TDOA_S->b-4.0*TDOA_S->a*TDOA_S->c>0)
		{
			TDOA_S->r1=(-TDOA_S->b+sqrt(TDOA_S->b*TDOA_S->b-4.0*TDOA_S->a*TDOA_S->c))/(2.0*TDOA_S->a);
			TDOA_S->r2=(-TDOA_S->b-sqrt(TDOA_S->b*TDOA_S->b-4.0*TDOA_S->a*TDOA_S->c))/(2.0*TDOA_S->a);
		}
	
		//�Խ�����ж�
		if(TDOA_S->r1>0.0&& fabs(TDOA_S->r1) < 255)
		{
			TDOA_S->x=TDOA_S->p1+TDOA_S->q1*TDOA_S->r1;
			TDOA_S->y=TDOA_S->p2+TDOA_S->q2*TDOA_S->r1;
		}	
		
		else if(TDOA_S->r1<0.0&& fabs(TDOA_S->r1) < 255)
		{
			TDOA_S->x=TDOA_S->p1+TDOA_S->q1*TDOA_S->r2;
			TDOA_S->y=TDOA_S->p2+TDOA_S->q2*TDOA_S->r2;
		}
		else
			printf("�޽�\n");
		
		
		if (TDOA_S->x > 0 && TDOA_S->y > 0)
	{
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_5,GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_5,GPIO_PIN_RESET);
		HAL_Delay(100);
		TDOA_S->Angle = atan(TDOA_S->x/TDOA_S->y)*57.3;
		printf("��ƫ��%.2lf\n",TDOA_S-> Angle);
	}

	else if (TDOA_S->x < 0 && TDOA_S->y > 0)
	{
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_4,GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_4,GPIO_PIN_RESET);
		HAL_Delay(100);
		TDOA_S->Angle = atan(-TDOA_S->x/TDOA_S->y)*57.3;
		printf("��ƫ��%.2lf\n",TDOA_S-> Angle);
	}

	else if (TDOA_S->x < 0 && TDOA_S->y < 0)
	{
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_Delay(100);
		TDOA_S->Angle = acos(-TDOA_S->x / -TDOA_S->y)*57.3;
		printf("��ƫ��%.2lf\n", TDOA_S->Angle);

	}
	
	else if (TDOA_S->x > 0 && TDOA_S->y < 0)
	{
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_Delay(100);
		TDOA_S->Angle = acos(TDOA_S->x/-TDOA_S->y)*57.3;
		printf("��ƫ��%.2lf\n",TDOA_S-> Angle);
	}
		
		//Servo_Control(TDOA_S->Angle);
		
		                                      

	printf("x:%.2lf\r\n",TDOA_S->x);
	printf("y:%.2lf\r\n",TDOA_S->y);
	printf("r21:%.2lf\r\n",TDOA_S->r21);
	printf("r31:%.2lf\r\n",TDOA_S->r31);
	printf("t21:%.2lf\r\n",TDOA_S->r21/0.34);
	printf("t31:%.2lf\r\n",TDOA_S->r31/0.34);
}








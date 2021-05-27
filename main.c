#include "stm32f10x.h"
#include "IERG3810_DELAY.h"
#include "IERG3810_LED.h"
#include "IERG3810_clock.h"
#include "IERG3810_USART.h"
#include "IERG3810_TFTLCD.h"
#include "Global.h"
#define LED0_PWM_VAL TIM3->CCR2
  u32 ps2key = 0;	//store PS2 Keyboard data
  u32 ps2count = 0;	
	u32 timeout=20000;
	u8 counter=0x00;
	u8 last=0x00;
	int a=0;
	int c=0;
	int d=0;
	int first=0;
	int game=0;
	int second=0;
	int third=0;
	int restart=0;
	int start=0;
	int look=0;
	int choose=0;
	int survive=0;
	double speed=0;
	int wudi=0;
	int health=3;//count PS2 keyboard received bit
	int time0=0;
  int time1=80000;
	int x1=0;
	int x2=100;
	int x3=160;
	int y1=0;
	int y2=100;
	int y3=200;
	int xsup=30;
	int ysup=200;
void IERG3810_NVIC_SetPriorityGroup(u8 prigroup){
	u32 temp,temp1;
	temp1 = prigroup & 0x00000007;
	temp1 <<= 8;
	temp = SCB->AIRCR;
	temp &= 0x0000F8FF;
	temp |= 0x05FA0000;
	temp |= temp1;
	SCB->AIRCR = temp;
}
void IERG3810_TIM3_Init(u16 arr, u16 psc){
	RCC->APB1ENR|=1<<1;            //timer3 enable
	TIM3->ARR=arr;
	TIM3->PSC=psc;
	TIM3->DIER|=1<<0;
	TIM3->CR1|=0x01;
	NVIC->IP[29]=0x45;
	NVIC->ISER[0]|=(1<<29);
}
void IERG3810_TIM4_Init(u16 arr, u16 psc){
	RCC->APB1ENR|=1<<2;            //timer4 enable
	TIM4->ARR=arr;
	TIM4->PSC=psc;
	TIM4->DIER|=1<<0;
	TIM4->CR1|=0x01;
	NVIC->IP[30]=0x45;
	NVIC->ISER[0]|=(1<<30);
}
void DS0_turnOff(){
	GPIOB->BSRR=1<<5;
}
void DS0_turnOff2(){
	DS0_turnOff();
}
void IERG3810_SYSTICK_Init10ms(void){
	SysTick->CTRL=0;
	SysTick->LOAD=900000;
	SysTick->CTRL|=3;
}


void TIM3_IRQHandler(void){
	
	GPIOB->BRR=1<<5;
		GPIOB->BSRR=1<<5;
		GPIOB->BRR=1<<5;
		DS0_turnOff();
		GPIOB->BRR=1<<5;
		DS0_turnOff2();
}
void EXTI15_10_IRQHandler(void)
{	
	
	//design program here.
		Delay(10);
	if(ps2count>=1 &&ps2count<=8){
		ps2key=(GPIOC->IDR & (1<<10))?1:0;
		counter |=ps2key<<(ps2count-1);

	}

	ps2count++;
	EXTI->PR=1<<11;
	
	
}

void TIM4_IRQHandler(void){
	if(TIM4->SR&1<<0){
		GPIOE->ODR^=1<<5;
}
	TIM4->SR&=~(1<<0);
}
void IERG3810_TIM3_PwmInit(u16 arr, u16 psc){

	RCC->APB2ENR |=1<<3;
	GPIOB->CRL&=0xFF0FFFFF;
	GPIOB->CRL|=0x00B00000;
	RCC->APB2ENR |=1<<0;
	AFIO->MAPR&=0xFFFFF3FF;
	AFIO->MAPR|=1<<11;
	RCC->APB1ENR|=1<<1;
	TIM3->ARR=arr;
	TIM3->PSC=psc;
	TIM3->CCMR1|=7<<12;
	TIM3->CCMR1|=1<<11;
	TIM3->CCER|=1<<4;
	TIM3->CR1=0x0080;
	TIM3->CR1|=0x01;
}
void IERG3810_PS2key_ExitInit()
{
	RCC ->APB2ENR |=1<<4;
	GPIOC->CRH &= 0xFFFF0FFF;
	GPIOC->CRH |= 0x00008000;
	GPIOC->ODR |=1<<11;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[2] &= 0xFFFF0FFF;
	AFIO->EXTICR[2] |= 0x00002000;
	EXTI->IMR |= 1<<11;
	EXTI->FTSR |= 1<<11;
	NVIC->IP[40] = 0x65;  
	NVIC->ISER[1] |= (1<<8);
}
 


void initial(void){
	
	IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
	}
void gameover(){
	initial();
	while(restart==0){
		//initial();
		a=survive/4000000;
		Delay(100000);
		//IERG3810_TFTLCD_FillRectangle(0xffff,10,50,20,20);
		first=a/100; //first digit
		
		c=a-first*100;
		second=c/10;
		d=c-second*10;
		third=d;
		IERG3810_TFTLCD_ShowChar(60,180,57,0xffff,0x0000);//y
	IERG3810_TFTLCD_ShowChar(68,180,47,0xffff,0x0000);//o
	IERG3810_TFTLCD_ShowChar(76,180,53,0xffff,0x0000);//u
	IERG3810_TFTLCD_ShowChar(84,180,50,0xffff,0x0000);//r
	IERG3810_TFTLCD_ShowChar(92,180,0,0xffff,0x0000);//
	IERG3810_TFTLCD_ShowChar(100,180,51,0xffff,0x0000);//s
	IERG3810_TFTLCD_ShowChar(108,180,35,0xffff,0x0000);//c
	IERG3810_TFTLCD_ShowChar(116,180,47,0xffff,0x0000);//o
	IERG3810_TFTLCD_ShowChar(124,180,50,0xffff,0x0000);//r
	IERG3810_TFTLCD_ShowChar(132,180,37,0xffff,0x0000);//e
	IERG3810_TFTLCD_ShowChar(140,180,0,0xffff,0x0000);//
	IERG3810_TFTLCD_ShowChar(148,180,41,0xffff,0x0000);//i
	IERG3810_TFTLCD_ShowChar(156,180,51,0xffff,0x0000);//s
	IERG3810_TFTLCD_ShowChar(164,180,0,0xffff,0x0000);//

	IERG3810_TFTLCD_ShowChar(172,180,first+16,0x7E5,0x0000);
		IERG3810_TFTLCD_ShowChar(180,180,second+16,0x7E5,0x0000);
		IERG3810_TFTLCD_ShowChar(188,180,third+16,0x7E5,0x0000);    //score
		

	IERG3810_TFTLCD_ShowChar(50,140,48,0xffff,0x0000);//p
	IERG3810_TFTLCD_ShowChar(58,140,50,0xffff,0x0000);//r
	IERG3810_TFTLCD_ShowChar(66,140,37,0xffff,0x0000);//e
	IERG3810_TFTLCD_ShowChar(74,140,51,0xffff,0x0000);//s
	IERG3810_TFTLCD_ShowChar(82,140,51,0xffff,0x0000);//s
	IERG3810_TFTLCD_ShowChar(90,140,0,0xffff,0x0000);//
	IERG3810_TFTLCD_ShowChar(98,140,51,0xffff,0x0000);//s
	IERG3810_TFTLCD_ShowChar(106,140,0,0xffff,0x0000);//
	IERG3810_TFTLCD_ShowChar(114,140,52,0xffff,0x0000);//t
	IERG3810_TFTLCD_ShowChar(122,140,47,0xffff,0x0000);//o
	IERG3810_TFTLCD_ShowChar(130,140,0,0xffff,0x0000);//
	IERG3810_TFTLCD_ShowChar(138,140,50,0xffff,0x0000);//r
	IERG3810_TFTLCD_ShowChar(146,140,37,0xffff,0x0000);//e
	IERG3810_TFTLCD_ShowChar(154,140,51,0xffff,0x0000);//s
	IERG3810_TFTLCD_ShowChar(162,140,52,0xffff,0x0000);//t
	IERG3810_TFTLCD_ShowChar(170,140,33,0xffff,0x0000);//a
	IERG3810_TFTLCD_ShowChar(178,140,50,0xffff,0x0000);//r
	IERG3810_TFTLCD_ShowChar(186,140,52,0xffff,0x0000);//t
		
	
		
			if(ps2count >= 11)
		{
		if ((counter == 0x0000001B)&&last!=0xF0)
		{
		
		  //start=0;
			game=1;
			restart=1;
		}
		//if ((counter == 0x0000001C)&&last==0xF0)
		//{
			//start=1;
		//}
		 last=counter;
		counter=0x00;
		ps2key=0;
		ps2count=0;
		EXTI->PR=1<<11;	
		}
		timeout--;
  if(timeout==0){
		timeout=20000;
		ps2key=0;
		ps2count=0;
		counter=0x00;
}
	}
  
	
}
void drawlevelstick(int x, int y){
	
	IERG3810_TFTLCD_DrawDot(x,y+6,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x,y+7,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x,y+8,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x,y+9,0x0000);
	IERG3810_TFTLCD_DrawDot(x,y+10,0x0000);
	
	IERG3810_TFTLCD_DrawDot(x+1,y+6,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+1,y+7,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+1,y+8,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+1,y+9,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+1,y+10,0x0000);
	
	IERG3810_TFTLCD_DrawDot(x+2,y+6,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+2,y+7,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+2,y+8,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+2,y+9,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+2,y+10,0xbdf7);
	
		
	IERG3810_TFTLCD_DrawDot(x+3,y+6,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+3,y+7,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+3,y+8,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+3,y+9,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+3,y+10,0x0000);
	
	IERG3810_TFTLCD_DrawDot(x+4,y+6,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+4,y+7,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+4,y+8,0xbdf7);
	IERG3810_TFTLCD_DrawDot(x+4,y+9,0x0000);
	IERG3810_TFTLCD_DrawDot(x+4,y+10,0x0000);
	
	
}
void drawlevelsticks(int x,int y){
	int i=0;
	for(i=0;i<80;i=i+5){
		drawlevelstick(x+i,y);
	}
}
	
	
void drawlevels(void){
	IERG3810_TFTLCD_FillRectangle(0xffff,x1,80,y1,10);
	IERG3810_TFTLCD_FillRectangle(0x07e0,x2,80,y2,5);
	drawlevelsticks(x2,y2);
	IERG3810_TFTLCD_FillRectangle(0xffff,x3,80,y3,10);
}
void clearlevel(int x,int y){
     IERG3810_TFTLCD_FillRectangle(0x0000,x,80,y,10);//set to backgroundcolor
}
void clearlevels(void){
	clearlevel(x1,y1);//level1 position 
	clearlevel(x2,y2);//level2 position
	clearlevel(x3,y3);//level3 position
}
void clearsup(void){
	IERG3810_TFTLCD_FillRectangle(0x0000,xsup,16,ysup,16);//set to backgroundcolor
}
void drawsup(int x,int y){
IERG3810_TFTLCD_DrawDot(x,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+4,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+5,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+6,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+7,y,0x0000);                                    
IERG3810_TFTLCD_DrawDot(x+8,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+9,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+10,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+11,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+12,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+13,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y,0x0000);
	                                                  //line 1
IERG3810_TFTLCD_DrawDot(x,y+1,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+1,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+1,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+1,0x0000);
IERG3810_TFTLCD_DrawDot(x+4,y+1,0xFFFF);
IERG3810_TFTLCD_DrawDot(x+5,y+1,0xffe0);
IERG3810_TFTLCD_DrawDot(x+6,y+1,0xFFFF);
IERG3810_TFTLCD_DrawDot(x+7,y+1,0xBDF7);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+1,0xBDF7);
IERG3810_TFTLCD_DrawDot(x+9,y+1,0xFFFF);
IERG3810_TFTLCD_DrawDot(x+10,y+1,0xffe0);
IERG3810_TFTLCD_DrawDot(x+11,y+1,0xFFFF);
IERG3810_TFTLCD_DrawDot(x+12,y+1,0x0000);
IERG3810_TFTLCD_DrawDot(x+13,y+1,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y+1,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+1,0x0000);
                                                    //line2
IERG3810_TFTLCD_DrawDot(x,y+2,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+2,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+2,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+2,0x0000);
IERG3810_TFTLCD_DrawDot(x+4,y+2,0x0000);
IERG3810_TFTLCD_DrawDot(x+5,y+2,0xffe0);
IERG3810_TFTLCD_DrawDot(x+6,y+2,0xffe0);
IERG3810_TFTLCD_DrawDot(x+7,y+2,0xBDF7);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+2,0xBDF7);
IERG3810_TFTLCD_DrawDot(x+9,y+2,0xffe0);
IERG3810_TFTLCD_DrawDot(x+10,y+2,0xffe0);
IERG3810_TFTLCD_DrawDot(x+11,y+2,0xffe0);
IERG3810_TFTLCD_DrawDot(x+12,y+2,0xffe0);
IERG3810_TFTLCD_DrawDot(x+13,y+2,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y+2,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+2,0x0000);
                                                    //line3
IERG3810_TFTLCD_DrawDot(x,y+3,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+3,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+3,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+3,0x0000);
IERG3810_TFTLCD_DrawDot(x+4,y+3,0x0000);
IERG3810_TFTLCD_DrawDot(x+5,y+3,0xffe0);
IERG3810_TFTLCD_DrawDot(x+6,y+3,0xffe0);
IERG3810_TFTLCD_DrawDot(x+7,y+3,0x1111);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+3,0x1111);
IERG3810_TFTLCD_DrawDot(x+9,y+3,0xffe0);
IERG3810_TFTLCD_DrawDot(x+10,y+3,0xffe0);
IERG3810_TFTLCD_DrawDot(x+11,y+3,0x0000);
IERG3810_TFTLCD_DrawDot(x+12,y+3,0x0000);
IERG3810_TFTLCD_DrawDot(x+13,y+3,0xffe0);
IERG3810_TFTLCD_DrawDot(x+14,y+3,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+3,0x0000);
                                                    //line4
IERG3810_TFTLCD_DrawDot(x,y+4,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+4,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+4,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+4,0x0000);
IERG3810_TFTLCD_DrawDot(x+4,y+4,0x0000);
IERG3810_TFTLCD_DrawDot(x+5,y+4,0x0000);
IERG3810_TFTLCD_DrawDot(x+6,y+4,0xffe0);
IERG3810_TFTLCD_DrawDot(x+7,y+4,0x1111);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+4,0x1111);
IERG3810_TFTLCD_DrawDot(x+9,y+4,0xffe0);
IERG3810_TFTLCD_DrawDot(x+10,y+4,0x0000);
IERG3810_TFTLCD_DrawDot(x+11,y+4,0x0000);
IERG3810_TFTLCD_DrawDot(x+12,y+4,0x0000);
IERG3810_TFTLCD_DrawDot(x+13,y+4,0x1111);
IERG3810_TFTLCD_DrawDot(x+14,y+4,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+4,0x0000);
                                                    //line5																									
IERG3810_TFTLCD_DrawDot(x,y+5,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+5,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+5,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+5,0x0000);
IERG3810_TFTLCD_DrawDot(x+4,y+5,0x0000);
IERG3810_TFTLCD_DrawDot(x+5,y+5,0x0000);
IERG3810_TFTLCD_DrawDot(x+6,y+5,0xffe0);
IERG3810_TFTLCD_DrawDot(x+7,y+5,0x1111);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+5,0x1111);
IERG3810_TFTLCD_DrawDot(x+9,y+5,0xffe0);
IERG3810_TFTLCD_DrawDot(x+10,y+5,0x0000);
IERG3810_TFTLCD_DrawDot(x+11,y+5,0x0000);
IERG3810_TFTLCD_DrawDot(x+12,y+5,0x0000);
IERG3810_TFTLCD_DrawDot(x+13,y+5,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y+5,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+5,0x0000);
                                                    //line6	
IERG3810_TFTLCD_DrawDot(x,y+6,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+6,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+6,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+6,0x0000);
IERG3810_TFTLCD_DrawDot(x+4,y+6,0x0000);
IERG3810_TFTLCD_DrawDot(x+5,y+6,0x0000);
IERG3810_TFTLCD_DrawDot(x+6,y+6,0xf800);
IERG3810_TFTLCD_DrawDot(x+7,y+6,0xf800);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+6,0xf800);
IERG3810_TFTLCD_DrawDot(x+9,y+6,0xf800);
IERG3810_TFTLCD_DrawDot(x+10,y+6,0x0000);
IERG3810_TFTLCD_DrawDot(x+11,y+6,0x0000);
IERG3810_TFTLCD_DrawDot(x+12,y+6,0x0000);
IERG3810_TFTLCD_DrawDot(x+13,y+6,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y+6,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+6,0x0000);
                                                    //line7
IERG3810_TFTLCD_DrawDot(x,y+7,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+7,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+7,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+7,0x1111);
IERG3810_TFTLCD_DrawDot(x+4,y+7,0x1111);
IERG3810_TFTLCD_DrawDot(x+5,y+7,0x1111);
IERG3810_TFTLCD_DrawDot(x+6,y+7,0x1111);
IERG3810_TFTLCD_DrawDot(x+7,y+7,0xf81f);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+7,0xf81f);
IERG3810_TFTLCD_DrawDot(x+9,y+7,0x1111);
IERG3810_TFTLCD_DrawDot(x+10,y+7,0x1111);
IERG3810_TFTLCD_DrawDot(x+11,y+7,0x1111);
IERG3810_TFTLCD_DrawDot(x+12,y+7,0x1111);
IERG3810_TFTLCD_DrawDot(x+13,y+7,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y+7,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+7,0x0000);
																//line8

IERG3810_TFTLCD_DrawDot(x,y+8,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+8,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+8,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+8,0xffe0);
IERG3810_TFTLCD_DrawDot(x+4,y+8,0x1111);
IERG3810_TFTLCD_DrawDot(x+5,y+8,0x1111);
IERG3810_TFTLCD_DrawDot(x+6,y+8,0x1111);
IERG3810_TFTLCD_DrawDot(x+7,y+8,0xf81f);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+8,0xf81f);
IERG3810_TFTLCD_DrawDot(x+9,y+8,0x1111);
IERG3810_TFTLCD_DrawDot(x+10,y+8,0x1111);
IERG3810_TFTLCD_DrawDot(x+11,y+8,0x1111);
IERG3810_TFTLCD_DrawDot(x+12,y+8,0xffe0);
IERG3810_TFTLCD_DrawDot(x+13,y+8,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y+8,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+8,0x0000);
																//line9

IERG3810_TFTLCD_DrawDot(x,y+9,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+9,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+9,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+9,0xffe0);
IERG3810_TFTLCD_DrawDot(x+4,y+9,0xffe0);
IERG3810_TFTLCD_DrawDot(x+5,y+9,0x0000);
IERG3810_TFTLCD_DrawDot(x+6,y+9,0x1111);
IERG3810_TFTLCD_DrawDot(x+7,y+9,0x1111);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+9,0x1111);
IERG3810_TFTLCD_DrawDot(x+9,y+9,0x1111);
IERG3810_TFTLCD_DrawDot(x+10,y+9,0x0000);
IERG3810_TFTLCD_DrawDot(x+11,y+9,0xffe0);
IERG3810_TFTLCD_DrawDot(x+12,y+9,0xffe0);
IERG3810_TFTLCD_DrawDot(x+13,y+9,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y+9,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+9,0x0000);
																//line10

IERG3810_TFTLCD_DrawDot(x,y+10,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+10,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+10,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+10,0xffe0);
IERG3810_TFTLCD_DrawDot(x+4,y+10,0xffe0);
IERG3810_TFTLCD_DrawDot(x+5,y+10,0x0000);
IERG3810_TFTLCD_DrawDot(x+6,y+10,0xffe0);
IERG3810_TFTLCD_DrawDot(x+7,y+10,0x1111);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+10,0x1111);
IERG3810_TFTLCD_DrawDot(x+9,y+10,0xffe0);
IERG3810_TFTLCD_DrawDot(x+10,y+10,0x0000);
IERG3810_TFTLCD_DrawDot(x+11,y+10,0xffe0);
IERG3810_TFTLCD_DrawDot(x+12,y+10,0xffe0);
IERG3810_TFTLCD_DrawDot(x+13,y+10,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y+10,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+10,0x0000);
																//line11

IERG3810_TFTLCD_DrawDot(x,y+11,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+11,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+11,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+11,0xffe0);
IERG3810_TFTLCD_DrawDot(x+4,y+11,0xffe0);
IERG3810_TFTLCD_DrawDot(x+5,y+11,0x0000);
IERG3810_TFTLCD_DrawDot(x+6,y+11,0xffe0);
IERG3810_TFTLCD_DrawDot(x+7,y+11,0xffe0);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+11,0xffe0);
IERG3810_TFTLCD_DrawDot(x+9,y+11,0xffe0);
IERG3810_TFTLCD_DrawDot(x+10,y+11,0x0000);
IERG3810_TFTLCD_DrawDot(x+11,y+11,0xffe0);
IERG3810_TFTLCD_DrawDot(x+12,y+11,0xffe0);
IERG3810_TFTLCD_DrawDot(x+13,y+11,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y+11,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+11,0x0000);
																//line12
   
																										
IERG3810_TFTLCD_DrawDot(x,y+12,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+12,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+12,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+12,0xffe0);
IERG3810_TFTLCD_DrawDot(x+4,y+12,0xffe0);
IERG3810_TFTLCD_DrawDot(x+5,y+12,0xffe0);
IERG3810_TFTLCD_DrawDot(x+6,y+12,0xffe0);
IERG3810_TFTLCD_DrawDot(x+7,y+12,0xffe0);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+12,0xffe0);
IERG3810_TFTLCD_DrawDot(x+9,y+12,0xffe0);
IERG3810_TFTLCD_DrawDot(x+10,y+12,0xffe0);
IERG3810_TFTLCD_DrawDot(x+11,y+12,0xffe0);
IERG3810_TFTLCD_DrawDot(x+12,y+12,0xffe0);
IERG3810_TFTLCD_DrawDot(x+13,y+12,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y+12,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+12,0x0000);
                                                    //line13
																											
IERG3810_TFTLCD_DrawDot(x,y+13,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+13,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+13,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+13,0xffe0);
IERG3810_TFTLCD_DrawDot(x+4,y+13,0xF81F);
IERG3810_TFTLCD_DrawDot(x+5,y+13,0xffe0);
IERG3810_TFTLCD_DrawDot(x+6,y+13,0x0000);
IERG3810_TFTLCD_DrawDot(x+7,y+13,0x0000);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+13,0x0000);
IERG3810_TFTLCD_DrawDot(x+9,y+13,0x0000);
IERG3810_TFTLCD_DrawDot(x+10,y+13,0xffe0);
IERG3810_TFTLCD_DrawDot(x+11,y+13,0xF81F);
IERG3810_TFTLCD_DrawDot(x+12,y+13,0xffe0);
IERG3810_TFTLCD_DrawDot(x+13,y+13,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y+13,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+13,0x0000);
                                                    //line14
	
IERG3810_TFTLCD_DrawDot(x,y+14,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+14,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+14,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+14,0x0000);
IERG3810_TFTLCD_DrawDot(x+4,y+14,0xffe0);
IERG3810_TFTLCD_DrawDot(x+5,y+14,0x0000);
IERG3810_TFTLCD_DrawDot(x+6,y+14,0x0000);
IERG3810_TFTLCD_DrawDot(x+7,y+14,0x0000);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+14,0x0000);
IERG3810_TFTLCD_DrawDot(x+9,y+14,0x0000);
IERG3810_TFTLCD_DrawDot(x+10,y+14,0x0000);
IERG3810_TFTLCD_DrawDot(x+11,y+14,0xffe0);
IERG3810_TFTLCD_DrawDot(x+12,y+14,0x0000);
IERG3810_TFTLCD_DrawDot(x+13,y+14,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y+14,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+14,0x0000);
                                                    //line15
																										
IERG3810_TFTLCD_DrawDot(x,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+1,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+2,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+3,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+4,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+5,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+6,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+7,y+15,0x0000);                                    
IERG3810_TFTLCD_DrawDot(x+8,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+9,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+10,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+11,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+12,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+13,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+14,y+15,0x0000);
IERG3810_TFTLCD_DrawDot(x+15,y+15,0x0000);
                                                    //line16
																										
}
void levelmoving(){
	/*int t1=tmp1;
	
	int t2=tmp2;
	int t3=tmp3;
	int X1=x1+10;
	int X2=x2+70;
	int X3=x3+100;
	IERG3810_TFTLCD_FillRectangle(0xffff,X1,55,t1,10);
	IERG3810_TFTLCD_FillRectangle(0xffff,X2,55,t2,10);
	IERG3810_TFTLCD_FillRectangle(0xffff,X3,55,t3,10);
	;
	*///////a new method
	if(health<=0)
	{
		
		{
			gameover();
		}
	}
		
		
	if((ysup>11)&&(ysup<21)){
		{
			gameover();
		}
	}
	time0++;
	clearlevels();
	clearsup();
	y1=y1+5+speed;
	if(y1>302){
		y1=16;
		x1=time0%122;
	}
	y2=y2+5+speed;
	if(y2>302){
		y2=16;
		x2=time0%150;
	}
	y3=y3+5+speed;
	if(y3>302){
		y3=16;
		x3=time0%233;
	}
	drawlevels();
	int c=onboard();
	//if((ysup<=323)&&ysup>=304)
	if((ysup>=286)&&ysup<=288)
		{
		health--;
		GPIOE->BRR = 1<<5;  //DS1 on
		Delay(500000);
		GPIOE->BSRR = 1<<5;  //DS1 off		
		ysup=ysup-10-speed;
	}
	if(c==0){
		ysup=ysup-3;
		drawsup(xsup,ysup);//draw sup
	}
	if(c==1){
		ysup=ysup+5+speed;
	drawsup(xsup,ysup);//draw sup
	}
	
}
void settowudi(void){
	suptimer=0;
	wudi=1;
	DS0_on();
	
}
int onboard(void){
	if (((xsup+8>x1)&&(xsup+8<x1+80))&&((ysup<y1+5)&&(ysup>y1-5))){
		return 1;
		}
	else if (((xsup+8>x2)&&(xsup+8<x2+80))&&((ysup<y2+5)&&(ysup>y2-5))){
		if(wudi==0){
		health--;
		settowudi();
		
		GPIOE->BRR = 1<<5;  //DS1 on
		Delay(500000);
		GPIOE->BSRR = 1<<5;  //DS1 off		
		}
		return 1;
		
	
		//delay(100);
		}
	else if (((xsup+8>x3)&&(xsup+8<x3+80))&&((ysup<y3+5)&&(ysup>y3-5))){
		return 1;
		}
	else{
		return 0;
	}
}
void supmove(void){
	
		if(ps2count >= 11)
		{
		if ((counter == 0x0000001C)&&last!=0xF0)
		{
			clearsup();
		if(xsup<0){
			xsup=0;
		}
			if (xsup> 0)
			{
				xsup -= 20;
				//GPIOB->BRR = 1<<5;  //DS0 on
				//Delay(500000);
				//GPIOB->BSRR = 1<<5;  //DS0 off
			}
			drawsup(xsup,ysup);//draw sup
		}
		if ((counter == 0x0000001C)&&last==0xF0)
		{
			clearsup();
		 
			if (xsup> 16)
			{
				xsup -= 20;
				//GPIOB->BRR = 1<<5;  //DS0 on
				//Delay(500000);
				//GPIOB->BSRR = 1<<5;  //DS0 off
			}
			drawsup(xsup,ysup);//draw sup
		}
		
		else if((counter == 0x00000023)&&last!=0xF0)
		{
			clearsup();
			
			if (xsup < 224)
			{
				xsup += 20;	
				//GPIOE->BRR = 1<<5;  //DS1 on
				//Delay(500000);
				//GPIOE->BSRR = 1<<5;  //DS1 off			
			}
			
		
			
		}		
		else if((counter == 0x00000023)&&last==0xF0)
		{
			clearsup();
			
			if (xsup < 224)
			{
				xsup += 20;	
				//GPIOE->BRR = 1<<5;  //DS1 on
				//Delay(500000);
				//GPIOE->BSRR = 1<<5;  //DS1 off			
			}
			
		
			
		}		
		
		drawsup(xsup,ysup);
		last=counter;
		counter=0x00;
		ps2key=0;
		ps2count=0;
		EXTI->PR=1<<11;	
		}
		timeout--;
  if(timeout==0){
		timeout=20000;
		ps2key=0;
		ps2count=0;
		counter=0x00;
}
}
void clearhealth(void){
	IERG3810_TFTLCD_FillRectangle(0x0000,0,56,0,16);
}
void showhealth(void){
	clearhealth();
	IERG3810_TFTLCD_FillRectangle(0xffff,0,240,17,1);
	if(health==3){
		
		IERG3810_TFTLCD_ShowChinese(0,0, 2, 0xf800, 0x0000);
	  IERG3810_TFTLCD_ShowChinese(20,0, 2, 0xf800, 0x0000);
		IERG3810_TFTLCD_ShowChinese(40,0, 2, 0xf800, 0x0000);
	}
	if(health==2){
		IERG3810_TFTLCD_ShowChinese(0,0, 2, 0xf800, 0x0000);
	  IERG3810_TFTLCD_ShowChinese(20,0, 2, 0xf800, 0x0000);
	}
	if(health==1){
		IERG3810_TFTLCD_ShowChinese(0,0, 2, 0xf800, 0x0000);
	}
}
void showupsticks(void){
	int what=0;
	for(what=0;what<240;what=what+16){
	
	IERG3810_TFTLCD_ShowChinese(what,304, 3, 0xbdf7, 0x0000);
	
}
	}

void beginpage(void){
	int geng=0;
  showupsticks();
	for(geng=0;geng<240;geng=geng+16){
		drawsup(geng,150);
	}
	//IERG3810_TFTLCD_FillRectangle(0xffff,0,10,0,500);//set to backgroundcolor
	
	IERG3810_TFTLCD_ShowChar(78,240,51,0x7E5,0x0000); //s
	IERG3810_TFTLCD_ShowChar(86,240,53,0x7E5,0x0000); //u
	IERG3810_TFTLCD_ShowChar(94,240,48,0x7E5,0x0000); //p
	IERG3810_TFTLCD_ShowChar(102,240,0,0xffff,0x0000); //
	IERG3810_TFTLCD_ShowChar(110,240,36,0x7E5,0x0000); //d
	IERG3810_TFTLCD_ShowChar(118,240,50,0x7E5,0x0000); //r
	IERG3810_TFTLCD_ShowChar(126,240,47,0x7E5,0x0000); //o
	IERG3810_TFTLCD_ShowChar(134,240,48,0x7E5,0x0000); //p
	
	IERG3810_TFTLCD_ShowChar(10,100,17,0xffff, 0x0000);
  IERG3810_TFTLCD_ShowChar(18,100,17,0xFFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(26,100,21,0xFFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(34,100,21,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(42,100,16,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(50,100,23,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(58,100,23,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(66,100,16,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(74,100,17,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(82,100,18,0xfFFF, 0x0000);
	                                                         //luffy student id
	IERG3810_TFTLCD_ShowChar(98,100,58,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(106,100,40,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(114,100,33,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(122,100,46,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(130,100,39,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(138,100,00,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(146,100,58,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(154,100,40,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(162,100,33,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(170,100,46,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(178,100,57,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(186,100,33,0xfFFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(194,100,47,0xfFFF, 0x0000);
	                                                            //luffy name
																															
	IERG3810_TFTLCD_ShowChar(10,80,17,0xffff,0x0000);
  IERG3810_TFTLCD_ShowChar(18,80,17,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(26,80,21,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(34,80,21,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(42,80,16,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(50,80,23,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(58,80,23,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(66,80,18,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(74,80,21,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(82,80,19,0xffff,0x0000);
																										//sunwei id
	IERG3810_TFTLCD_ShowChar(98,80,51,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(106,80,53,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(114,80,46,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(122,80,0,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(130,80,55,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(138,80,37,0xffff,0x0000);
	IERG3810_TFTLCD_ShowChar(146,80,41,0xffff,0x0000);
                                                                  //sunwei name																														
	IERG3810_TFTLCD_ShowChar(50,30,48,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(58,30,50,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(66,30,37,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(74,30,51,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(82,30,51,0x0FFF, 0x0000);
	                                                               //press
	IERG3810_TFTLCD_ShowChar(98,30,51,0x0FFF, 0x0000);            //s
	IERG3810_TFTLCD_ShowChar(106,30,00,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(114,30,52,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(122,30,47,0x0FFF, 0x0000);  //to
	IERG3810_TFTLCD_ShowChar(130,30,00,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(138,30,51,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(146,30,52,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(154,30,33,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(162,30,50,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(170,30,52,0x0FFF, 0x0000);//start
	
	
	//IERG3810_TFTLCD_ShowChar(30,100,1,0x2342,0x8786);
	Delay(1000000);
	//DS1_on();
}

void choosedifficulty(void){
	IERG3810_TFTLCD_ShowChar(50,200,17,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(58,200,13,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(64,200,13,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(72,200,30,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(96,200,46,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(104,200,47,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(112,200,50,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(120,200,45,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(128,200,33,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(136,200,44,0xffff, 0x0000);
	//normal
	IERG3810_TFTLCD_ShowChar(50,150,18,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(58,150,13,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(64,150,13,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(72,150,30,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(96,150,40,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(104,150,33,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(112,150,50,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(120,150,36,0xffff, 0x0000);
	//hard
	IERG3810_TFTLCD_ShowChar(50,100,19,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(58,100,13,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(64,100,13,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(72,100,30,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(96,100,40,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(104,100,37,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(112,100,44,0xffff, 0x0000);
	IERG3810_TFTLCD_ShowChar(120,100,44,0xffff, 0x0000);
	//hard
}
void showinstruction(void){
	IERG3810_TFTLCD_ShowChar(40,200,55,0xffff,0x0000); //w
IERG3810_TFTLCD_ShowChar(48,200,33,0xffff,0x0000); //a
IERG3810_TFTLCD_ShowChar(56,200,52,0xffff,0x0000); //t
IERG3810_TFTLCD_ShowChar(64,200,35,0xffff,0x0000); //c
IERG3810_TFTLCD_ShowChar(72,200,40,0xffff,0x0000); //h
IERG3810_TFTLCD_ShowChar(80,200,00,0xffff,0x0000); //
IERG3810_TFTLCD_ShowChar(88,200,47,0xffff,0x0000); //o
IERG3810_TFTLCD_ShowChar(96,200,53,0xffff,0x0000); //u
IERG3810_TFTLCD_ShowChar(104,200,52,0xffff,0x0000); //t
IERG3810_TFTLCD_ShowChar(112,200,00,0xffff,0x0000); //
IERG3810_TFTLCD_ShowChar(120,200,38,0xffff,0x0000); //f
IERG3810_TFTLCD_ShowChar(128,200,47,0xffff,0x0000); //o
IERG3810_TFTLCD_ShowChar(136,200,50,0xffff,0x0000); //r
IERG3810_TFTLCD_ShowChar(144,200,00,0xffff,0x0000); //
IERG3810_TFTLCD_ShowChar(152,200,51,0xffff,0x0000); //s
IERG3810_TFTLCD_ShowChar(160,200,52,0xffff,0x0000); //t
IERG3810_TFTLCD_ShowChar(168,200,41,0xffff,0x0000); //i
IERG3810_TFTLCD_ShowChar(176,200,35,0xffff,0x0000); //c
IERG3810_TFTLCD_ShowChar(184,200,43,0xffff,0x0000); //k
IERG3810_TFTLCD_ShowChar(192,200,51,0xffff,0x0000); //s
IERG3810_TFTLCD_ShowChar(200,200,1,0xffff,0x0000); //!




IERG3810_TFTLCD_ShowChar(58,180,48,0xffff,0x0000); //p
IERG3810_TFTLCD_ShowChar(66,180,50,0xffff,0x0000); //r
IERG3810_TFTLCD_ShowChar(74,180,37,0xffff,0x0000); //e
IERG3810_TFTLCD_ShowChar(82,180,51,0xffff,0x0000); //s
IERG3810_TFTLCD_ShowChar(90,180,51,0xffff,0x0000); //s
IERG3810_TFTLCD_ShowChar(98,180,0,0xffff,0x0000); //

IERG3810_TFTLCD_ShowChar(106,180,33,0xffff,0x0000); //a
IERG3810_TFTLCD_ShowChar(114,180,0,0xffff,0x0000); //

IERG3810_TFTLCD_ShowChar(122,180,52,0xffff,0x0000); //t
IERG3810_TFTLCD_ShowChar(130,180,47,0xffff,0x0000); //o 
IERG3810_TFTLCD_ShowChar(138,180,0,0xffff,0x0000); //

IERG3810_TFTLCD_ShowChar(146,180,44,0xffff,0x0000); //l
IERG3810_TFTLCD_ShowChar(154,180,37,0xffff,0x0000); //e
IERG3810_TFTLCD_ShowChar(162,180,38,0xffff,0x0000); //f
IERG3810_TFTLCD_ShowChar(170,180,52,0xffff,0x0000); //t





IERG3810_TFTLCD_ShowChar(58,160,48,0xffff,0x0000); //p
IERG3810_TFTLCD_ShowChar(66,160,50,0xffff,0x0000); //r
IERG3810_TFTLCD_ShowChar(74,160,37,0xffff,0x0000); //e
IERG3810_TFTLCD_ShowChar(82,160,51,0xffff,0x0000); //s
IERG3810_TFTLCD_ShowChar(90,160,51,0xffff,0x0000); //s
IERG3810_TFTLCD_ShowChar(98,160,0,0xffff,0x0000); //

IERG3810_TFTLCD_ShowChar(106,160,36,0xffff,0x0000); //d
IERG3810_TFTLCD_ShowChar(114,160,0,0xffff,0x0000); //

IERG3810_TFTLCD_ShowChar(122,160,52,0xffff,0x0000); //t
IERG3810_TFTLCD_ShowChar(130,160,47,0xffff,0x0000); //o 
IERG3810_TFTLCD_ShowChar(138,160,0,0xffff,0x0000); //

IERG3810_TFTLCD_ShowChar(146,160,50,0xffff,0x0000); //r
IERG3810_TFTLCD_ShowChar(154,160,41,0xffff,0x0000); //i
IERG3810_TFTLCD_ShowChar(162,160,39,0xffff,0x0000); //g
IERG3810_TFTLCD_ShowChar(170,160,40,0xffff,0x0000); //h
IERG3810_TFTLCD_ShowChar(178,160,52,0xffff,0x0000); //t

IERG3810_TFTLCD_ShowChar(74,140,57,0xffff,0x0000); //y
IERG3810_TFTLCD_ShowChar(82,140,47,0xffff,0x0000); //o 
IERG3810_TFTLCD_ShowChar(90,140,53,0xffff,0x0000); //u 
IERG3810_TFTLCD_ShowChar(98,140,0,0xffff,0x0000); //

IERG3810_TFTLCD_ShowChar(106,140,40,0xffff,0x0000); //h 
IERG3810_TFTLCD_ShowChar(114,140,33,0xffff,0x0000); //a 
IERG3810_TFTLCD_ShowChar(122,140,54,0xffff,0x0000); //v 
IERG3810_TFTLCD_ShowChar(130,140,37,0xffff,0x0000); //e 
IERG3810_TFTLCD_ShowChar(138,140,0,0xffff,0x0000); //

IERG3810_TFTLCD_ShowChar(146,140,19,0xffff,0x0000); //3
IERG3810_TFTLCD_ShowChar(154,140,0,0xffff,0x0000); //
IERG3810_TFTLCD_ShowChinese(162,140, 2, 0xf800, 0x0000);

IERG3810_TFTLCD_ShowChar(50,30,48,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(58,30,50,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(66,30,37,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(74,30,51,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(82,30,51,0x0FFF, 0x0000);
	                                                               //press
	IERG3810_TFTLCD_ShowChar(98,30,35,0x0FFF, 0x0000);            //c
	IERG3810_TFTLCD_ShowChar(106,30,00,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(114,30,52,0x0FFF, 0x0000);
	IERG3810_TFTLCD_ShowChar(122,30,47,0x0FFF, 0x0000);  //to
	
	IERG3810_TFTLCD_ShowChar(138,30,35,0x0FFF, 0x0000);//c
	IERG3810_TFTLCD_ShowChar(146,30,47,0x0FFF, 0x0000);//o
	IERG3810_TFTLCD_ShowChar(154,30,46,0x0FFF, 0x0000);//n
	IERG3810_TFTLCD_ShowChar(162,30,52,0x0FFF, 0x0000);//t
	IERG3810_TFTLCD_ShowChar(170,30,41,0x0FFF, 0x0000);//i
	IERG3810_TFTLCD_ShowChar(178,30,46,0x0FFF, 0x0000);//n
	IERG3810_TFTLCD_ShowChar(186,30,53,0x0FFF, 0x0000);//u
	IERG3810_TFTLCD_ShowChar(194,30,37,0x0FFF, 0x0000);//e
}
void backto(){
	start=0;
	look=0;
	choose=0;
	restart=0;
	game=0;
	health=3;
	xsup=30;
	ysup=200;
	survive=0;
}
int main(void){
	
	Delay(10000);
	u8 dir=1;
	u16 led0pwmval=0;
	int j=0;
	IERG3810_clock_tree_init();
	IERG3810_LED_Init();
	IERG3810_NVIC_SetPriorityGroup(5);
	IERG3810_TFTLCD_Init();
	Delay(10000000);
	IERG3810_PS2key_ExitInit();
	
	//IERG3810_TIM3_Init(4999,7199);
  //IERG3810_TIM4_Init(4999,1799);
	IERG3810_SYSTICK_Init10ms();
	//u16 led0pwmval=0;
	//u16 LED0_PWM_VAL;
	Delay(10000000);
  initial();
	IERG3810_TIM3_PwmInit(9999,48);

	while(1){
		//int start=0;
		beginpage();
		choose=0;
		while(start==0){
			
		if(ps2count >= 11)
		{
		if ((counter == 0x0000001B)&&last!=0xF0)
		{
			start=1;
		
			
		}
		//if ((counter == 0x0000001C)&&last==0xF0)
		//{
			//start=1;
		//}
		 last=counter;
		counter=0x00;
		ps2key=0;
		ps2count=0;
		EXTI->PR=1<<11;	
		}
		timeout--;
  if(timeout==0){
		timeout=20000;
		ps2key=0;
		ps2count=0;
		counter=0x00;
}
			
		}
	  initial();
		Delay(100000);
		int look=0;
		while(look==0){
			showinstruction();
			if(ps2count >= 11)
		 {
			if ((counter == 0x00000021)&&last!=0xF0)
			{
			look=1;
		
			
			}
		//if ((counter == 0x0000001C)&&last==0xF0)
		//{
			//start=1;
		//}
			last=counter;
		counter=0x00;
		ps2key=0;
		ps2count=0;
		EXTI->PR=1<<11;	
		}
		timeout--;
  if(timeout==0){
		timeout=20000;
		ps2key=0;
		ps2count=0;
		counter=0x00;
}
	}
	choose=0;
   initial();
	while(choose==0){
			choosedifficulty();
	//watchinstruction;
			if(ps2count >= 11)
		{
		if ((counter == 0x00000016)&&last!=0xF0)
		{
			choose=1;
			//speed=0;
		
			
		}
		else if ((counter == 0x0000001E)&&last==0xF0)
		{
			//start=1;
			choose=1;
			speed=0.5;
		}
		else if ((counter == 0x00000026)&&last==0xF0)
		{
			//start=1;
			choose=1;
			speed=1;
		}
		
		 last=counter;
		counter=0x00;
		ps2key=0;
		ps2count=0;
		EXTI->PR=1<<11;	
		}
		timeout--;
  if(timeout==0){
		timeout=20000;
		ps2key=0;
		ps2count=0;
		counter=0x00;
}
		
		}
	initial();
	  while(game==0){
		
		j++;
		survive++;
		if(j%time1==0){
		showupsticks();
	  levelmoving();
		supmove();
		showhealth();
		
			if(suptimer==20)
			{
				wudi=0;
				DS0_off();
			}
		}
		
		
	}

	backto();
	initial();
	//DS1_on();
	}
	}

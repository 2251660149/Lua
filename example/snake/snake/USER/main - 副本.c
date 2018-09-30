#include "sys.h"
#include "delay.h" 	 
#include "lcd.h"  
#include "stdio.h"
#include "sdio_sdcard.h"   
#include "ff.h"  
#include "diskio.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "usrdef.h"
#include "string.h"
#include "key.h"
#define Countof(a) (sizeof(a)/sizeof(a[0]))
void display(u8 Hex);    
static int do_file_script(char *filename);
 
#define SIZE CountofUSART_RX_BUF)	 

typedef struct MenuItem MenuItem;
typedef struct Menu Menu;
	u8 menuid[3];
	u8 mcount;
struct Menu      
{    

	u16 x;u16 y;
	u16 H_SIZE;
	u16 Dis;
	u16 Bcolor;
	u16 FontSize;
	u16 FontBc;
	u16 scolor;
	MenuItem *Item;
	u8 Visible;
};   

struct MenuItem       
{     
		u8 Menucount;
    char *DisplayString;  
  	Menu *ChildMenu;  
    Menu *ParentMenu;  
	  void (*ToDo)();  
};   

MenuItem MainMenu[];    //??? 
MenuItem Test[]; 	    //????   
MenuItem Test2[];
Menu _MM1;
Menu _MM2;
Menu _MM3;
struct MenuItem *TestMenu;
  
void Run(struct MenuItem *Item);  

void Run(struct MenuItem *Item )   
{   
    (*(Item ->ToDo))(Item );   
}  
void Nothing(void)   
{} 
void MM2(struct MenuItem *Item)   
{
	if(do_file_script(Item->DisplayString)<0)
		return ;
} 
Menu _MM4;
MenuItem MainMenu[] =	 
{
	{2,"file",0,&_MM3,Nothing},    
  {2,"image",0,&_MM3,Nothing},  
}; 

MenuItem Test[]=   	
{   
  {4,"close",0,0,Nothing},   
	{4,"run",&_MM4,0,Nothing},  
	{4,"open",&_MM3,0,Nothing}, 
	{4,"other",0,0,Nothing},  
};  
MenuItem Test2[]=   	
{   
  {2,"hello",0,&_MM2,Nothing},   
	{2,"thanks",0,&_MM2,Nothing},  
};  
MenuItem Test3[]=   	
{   
  {2,"test.lua",0,&_MM2,MM2},   
	{2,"autorun.lua",0,&_MM2,MM2},  
};  
Menu _MM1=
{
	0,0,100,12,WHITE,16,GRAY,BLUE,MainMenu,1
};
Menu _MM2=
{
	0,35,100,12,WHITE,16,GRAY,BLUE,Test,1
};
Menu _MM3=
{
	0,0,100,12,WHITE,16,GRAY,BLUE,Test2,1
};
Menu _MM4=
{
	0,0,100,12,WHITE,16,GRAY,BLUE,Test3,1
};

void InitMenu(struct Menu *Menu)
{
	if(Menu->Visible==0) return;
 
  LCD_Color_Fill2(Menu->x,Menu->y,Menu->H_SIZE,Menu->Item->Menucount*(Menu->Dis+Menu->FontSize),Menu->Bcolor);
 
}
const unsigned char asc2_32[32]=

{0x00,0x00,0x00,0x00,0x7F,0xFC,0x3F,0xF8,0x3F,0xF8,0x1F,0xF0,0x1F,0xF0,0x0F,0xE0,0x0F,0xE0,0x07,0xC0,0x07,0xC0,0x03,0x80,0x03,0x80,0x01,0x00,0x01,0x00,0x00,0x00}
;
void LCD_Showcon(u16 x,u16 y,u16 color)
{  							  
  u8 temp,t1,t;
	u16 y0=y;
	for(t=0;t<32;t++)
	{   
		temp=asc2_32[t]; 	 	//����1206����
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,color);
			else LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//��������
			if((y-y0)==16)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}   

void Locate2(Menu *_menu)   
{   
	int t=0;
	if(_menu->Visible==1)
	{
		for(t=0;t<_menu->Item->Menucount;t++)
	{
			
		if(t==menuid[mcount])
		{
			BACK_COLOR=_menu->FontBc;
		
 LCD_Color_Fill2(_menu->x+(strlen(_menu->Item[t].DisplayString)*_menu->FontSize/2),_menu->y+t*(_menu->Dis+_menu->FontSize),_menu->H_SIZE-(strlen(_menu->Item[t].DisplayString)*_menu->FontSize/2),_menu->FontSize,_menu->FontBc);
	}
		else
		{
			BACK_COLOR=_menu->Bcolor;
 LCD_Color_Fill2(_menu->x+(strlen(_menu->Item[t].DisplayString)*_menu->FontSize/2),_menu->y+t*(_menu->Dis+_menu->FontSize),_menu->H_SIZE-(strlen(_menu->Item[t].DisplayString)*_menu->FontSize/2),_menu->FontSize,_menu->Bcolor);
		}
		if (_menu->Item[t].ChildMenu!=0)
		{
			LCD_Showcon(_menu->x+_menu->H_SIZE-_menu->FontSize,_menu->y+t*(_menu->Dis+_menu->FontSize),POINT_COLOR);
		//	LCD_ShowString(30,200,_menu->FontSize,_menu->Item[t].DisplayString,POINT_COLOR);
		}

		LCD_ShowString(_menu->x,_menu->y+t*(_menu->Dis+_menu->FontSize),_menu->FontSize,_menu->Item[t].DisplayString,POINT_COLOR);
	 
	}
 
 }
	else
	{
		LCD_Color_Fill2(_menu->x,_menu->y,_menu->H_SIZE,_menu->Item->Menucount*(_menu->Dis+_menu->FontSize),BACK_COLOR);
	}
//	 POINT_COLOR=BLACK;
	 BACK_COLOR=BLACK;
}  



static int _LCD_Clear(lua_State *L) 
{ 
  u16 color = luaL_checkint(L, 1);
	LCD_Fill(0,16,239,320,color);
  //LCD_Clear(color);
	return 0;
}

static int LCD_print(lua_State *L) 
{ 
	u8   x = luaL_checkint(L, 1);
  u8   y = luaL_checkint(L, 2);
  char * text = (char *)luaL_checkstring(L, 3);
  LCD_ShowString(x,y,16,text,GREEN);
	return 0;
}

static int _delay_ms(lua_State *L)
{
	int value;
	value= luaL_checkint( L, 1 );
	delay_ms(value);
	return 0;
}
static int GetKey(lua_State *L)
{
   // int num = lua_tonumber(L,1); /* the second argument: number */
    //lua_pushstring(L, "the first return");
    lua_pushnumber(L, KEY_Scan(0));
    return 1;            /* tell lua how many variables are returned */
}

const luaL_Reg lcd_map[]=
{
	{"PrintStr",LCD_print},
	{"Clear",_LCD_Clear},
	{NULL, NULL}
};
 static int register_lcd( lua_State *L )
{
	luaL_register( L, "LCD",lcd_map );
	USR_REG_NUMBER(L,"RED",RED);
	USR_REG_NUMBER(L,"GREEN",GREEN);
	USR_REG_NUMBER(L,"BLUE",BLUE);
	USR_REG_NUMBER(L,"YELLOW",YELLOW);
	USR_REG_NUMBER(L,"WHITE",WHITE);
	USR_REG_NUMBER(L,"BLACK",BLACK);
	return 0;
}
	lua_State *L;
	FILE *fp;
	long size;
   FATFS fs;

	char *buff;
	
	static int do_file_script(char *filename)
{
		

#if 0
	char *buff={"while 1 do LCD.Clear(LCD.BLACK); delay_ms(500);LCD.Clear(LCD.GREEN); delay_ms(500);LCD.Clear(LCD.RED); delay_ms(500);end"};
#else
f_mount( 0, &fs);//�����ļ�ϵͳ
	if( ( fp = fopen(filename, "r" ) ) == NULL )
	{
		LCD_ShowString(30,150,16,"CAN NOT OPEN THIS FILE",RED);
		delay_ms(300);
		LCD_ShowString(30,150,16,"CAN NOT OPEN THIS FILE",BLACK);
		return -1;
	}
	 
	fseek( fp, 0L, SEEK_END );
  size=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	if(size<=0)
	{	
		fclose( fp ); 
		return -1;
	}
	buff=malloc(size);
	fread(buff,size,1,fp);
	
	if(buff==NULL)
	{	
		LCD_ShowString(50,150,16,"buff  NULL",RED);
		return -1;
	}
 	LCD_ShowString(50,150,16,buff,RED);
 #endif
	L   = lua_open(); 
	luaopen_base(L);  
	register_lcd(L);
	
	lua_register(L,"GetKey",GetKey);
	lua_register(L,"delay_ms",_delay_ms);
	luaL_dostring(L, buff); 
	return 0;
}
  Menu *menu;
//void gameover()
//{
//	LCD_ShowString(90,70,16,"game over!",BLUE);		 //��ʾһ���ַ���,16����

//	LCD_ShowNum(224,165,0,1,16);//��ʾ����ֵ	
//}
//#define SNAKE_Max_Long 60//�ߵ���󳤶�
////�߽ṹ��
//struct Snake
//{
//	u8 X[SNAKE_Max_Long];
//	u8 Y[SNAKE_Max_Long];
//	u8 Long;//�ߵĳ���
//	u8 Life;//�ߵ����� 0���� 1����
//	u8 Direction;//���ƶ��ķ���
//}snake;

////ʳ��ṹ��
//struct Food
//{
//	u8 X;//ʳ�������
//	u8 Y;//ʳ��������
//	u8 Yes;//�ж��Ƿ�Ҫ����ʳ��ı��� 0��ʳ�� 1��Ҫ����ʳ��
//}food;

////��Ϸ�ȼ�����
//struct Game
//{
//	u16 Score;//����
//	u8 Life;//��Ϸ�ȼ�	
//}game;
////����Ϸ
//void play()
//{
//	u16 i;	
//	u8 key;
//	snake.Long=2;//�����ߵĳ���
//	snake.Life=0;//�߻�����
//	snake.Direction=1;//�ߵ���ʼ������Ϊ��
//	game.Score=0;//����Ϊ0
//	game.Life=4;//�ߵ�����ֵ
//	food.Yes=1;//������ʳ��
//	u8 pause=0;
//	snake.X[0]=0;snake.Y[0]=24;
//	snake.X[1]=12;snake.Y[1]=24;
//		while(1)
//	{
//				key=KEY_Scan(0);		//????
//		
//	  	if(key)
//		{			
//					switch(key)
//				{
//					case KEY0_PRES:snake.Direction=1;pause=0;break;//�����˶�
//					case KEY2_PRES:snake.Direction=2;pause=0;break;//�����˶�
//					case WKUP_PRES:snake.Direction=3;pause=0;break;//�����˶�
//					case KEY1_PRES:snake.Direction=4;pause=0;break;//�����˶�
//					case PAUSE:pause=1;break;//�����˶�
//				}
//		}
//		
//     if(pause==0)
//			{
//		//��ͣ
//		LCD_DrawRectangle(120-16,286,120+40,318);
//		Test_Show_CH_Font16(120,294,10,RED);
//		Test_Show_CH_Font16(120+14,294,11,RED);
//			if(food.Yes==1)//�����µ�ʳ��
//			{
//			  	food.X=12+rand()%(228/12)*12;
//					food.Y=12+rand()%(148/12)*12;
//					food.Yes=0;
//			}
//				if(food.Yes==0)//��ʳ���Ҫ��ʾ
//				{	
//					LCD_Fill(food.X,food.Y,food.X+10,food.Y+10,BLUE);
//				}
//				//ȡ����Ҫ���»����ߵĽ���
//				for(i=snake.Long-1;i>0;i--)
//				{
//					snake.X[i]=snake.X[i-1];
//					snake.Y[i]=snake.Y[i-1];
//				}
//	   
//				 switch(snake.Direction)
//				{
//					case 1:snake.X[0]+=12;break;//�����˶�
//					case 2:snake.X[0]-=12;break;//�����˶�
//					case 3:snake.Y[0]-=12;break;//�����˶�
//					case 4:snake.Y[0]+=12;break;//�����˶�
//				}
//					for(i=0;i<snake.Long;i++)//������	
//						LCD_Fill(snake.X[i],snake.Y[i],snake.X[i]+10,snake.Y[i]+10,RED);//��������
//						delay_ms(300);//��ʱ
//						LCD_Fill(snake.X[snake.Long-1],snake.Y[snake.Long-1],snake.X[snake.Long-1]+10,snake.Y[snake.Long-1]+10,WHITE);//��������		
//						//delay_ms(1000);
//					
//					//�ж��Ƿ�ײǽ
//					if(snake.X[0]==0||snake.X[0]>240||snake.Y[0]==0||snake.Y[0]>150)
//					//{
//// 				//		gameover();//��Ϸ����
//						snake.Life=1;//��������
//					//	break;
//				//	}
//					//���ߵ����峬��3�ں��ж����������ײ
//					for(i=3;i<snake.Long;i++)
//					{
//						
//						if(snake.X[i]==snake.X[0]&&snake.Y[i]==snake.Y[0])//�������һ����ֵ����ͷ������Ⱦ���Ϊ��������ײ
//					//	{
//// 							gameover();//��Ϸ����
//// 							snake.Life=1;//��������
//							game.Life-=1;
//					//		break;
//				//		}
//					}
//					if(snake.Life==1||game.Life==0)//���������ж��Ժ������������������ѭ�������¿�ʼ
//					{
//					gameover();
//					break;
//					}	
//					//�ж����Ƿ�Ե���ʳ��
//					if(snake.X[0]==food.X&&snake.Y[0]==food.Y)
//					{
//						LCD_Fill(food.X,food.Y,food.X+10,food.Y+10,WHITE);//�ѳԵ���ʳ������
//						snake.Long++;//�ߵ����峤һ��
//						game.Score+=10;
//						LCD_ShowNum(40,165,game.Score,3,16);//��ʾ�ɼ�	
//						food.Yes=1;//��Ҫ������ʾʳ��
//					}
//						LCD_ShowNum(224,165,game.Life,1,16);//��ʾ����ֵ	
//				}
//			else
//			{
//	//��ʼ
//		LCD_DrawRectangle(120-16,286,120+40,318);
//		Test_Show_CH_Font16(120,294,8,RED);
//		Test_Show_CH_Font16(120+14,294,9,RED);
//	

//			}
//	}	
//}

 int main(void)
{
	 
	u8 key;
	delay_init();
	LCD_Init();
	SD_Init();
	KEY_Init();	
 
//	  	POINT_COLOR =BLACK;
//// 	LCD_DrawRectangle(1,1,238,159);
//// 	LCD_DrawRectangle(2,2,237,158);
//// 	LCD_DrawRectangle(3,3,236,157);
//// 	LCD_DrawRectangle(4,4,235,156);
//	 LCD_DrawLine(0, 161, 240, 161);
//		//������ ��64 ��32 
//	POINT_COLOR = 0x0000;
//	//�ɼ�
//	Test_Show_CH_Font16(0,165,0,RED);
//	Test_Show_CH_Font16(16,165,1,RED);
//	LCD_ShowChar(32,165,':',16,1,RED);
//	LCD_ShowString(40,165,16,"  0",RED);	
//	//����
//	Test_Show_CH_Font16(184,165,2,RED);
//	Test_Show_CH_Font16(200,165,3,RED);
//	LCD_ShowChar(216,165,':',16,1,RED);
//	LCD_ShowString(224,165,16," ",RED);	


//	//��Ϸ����
//	Test_Show_CH_Font16(98,237,12,RED);//̰
//	Test_Show_CH_Font16(114,237,13,RED);//��
//	Test_Show_CH_Font16(130,237,14,RED);//��
	menu=&_MM2;
  InitMenu(menu);
  Locate2(menu);
 // play();
	
  while(1)
	{
		key=KEY_Scan(0);		//????
	   	if(key)
		{						   
	     switch(key)
			{				 
		case WKUP_PRES:
			         if(menu->Visible==0)
							 {  
								 menu->Visible=1;
								 menuid[mcount]=0;
									InitMenu(menu);
									Locate2(menu); 
							 }		 
			break;
				case KEY0_PRES:	
					if(menu->Visible==0) break;
						   	  menuid[mcount]++;   
	        if (menuid[mcount]>menu->Item->Menucount -1) menuid[mcount]=0;  
			Locate2(menu);
					break;
				case KEY1_PRES:	
        if(menu->Visible==0) break;
				if ((menu->Item+menuid[mcount])->ChildMenu!=0)   
	     {   
	        
			   menu =(menu->Item+menuid[mcount])->ChildMenu; 
				 if(menu->Visible==0)
				 {
					 menu->Visible=1;
				 }
				 menu->x=menu->Item->ParentMenu->x+menu->H_SIZE;
				 menu->y=menu->Item->ParentMenu->y+(menuid[mcount]*(menu->Dis+menu->FontSize));
				 
				 InitMenu(menu);
		     mcount++;   
	     	 menuid[mcount]=0;   
			   Locate2(menu);
			   menu->x-=(menu->Item->ParentMenu->x+menu->H_SIZE-menu->x);
				// menu->y-=menu->Item->ParentMenu->y+(menuid[mcount]*(menu->Dis+menu->FontSize));
		   }   
		  else   
		 {   
			      Run(menu->Item+menuid[mcount]);   
		 }	
					break;
				case KEY2_PRES:		 
				
			
			if (menu->Item->ParentMenu !=0)   
			{   
				if(menu->Visible==1)
			  {
					menu->Visible=0;
			
				}
					  InitMenu(menu);
				  Locate2(menu);
			  menu=(menu->Item+menuid[mcount])->ParentMenu;   
			
			  
         InitMenu(menu);   
				mcount--;   
		      Locate2(menu);
					
			 }   
			 else   
			 {   
         if(menu->Visible==1)
			   menu->Visible=0;
				InitMenu(menu);
				Locate2(menu);     
		   }	
					break;
			}
		}

	
  }

}


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
		temp=asc2_32[t]; 	 	//调用1206字体
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,color);
			else LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==16)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
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
	//LCD_Fill(0,16,239,320,color);
  LCD_Clear(color);
	return 0;
}
static int _LCD_Fill(lua_State *L) 
{ 
	int x1 = luaL_checkint(L, 1);
	int y1 = luaL_checkint(L, 2);
	int x2 = luaL_checkint(L, 3);
	int y2 = luaL_checkint(L, 4);
  u16 color = luaL_checkint(L, 5);
	LCD_Fill(x1,y1,x2,y2,color);
  //LCD_Clear(color);
	return 0;
}
static int _LCD_ShowNum(lua_State *L) 
{ 
	int x = luaL_checkint(L, 1);
	int y = luaL_checkint(L, 2);
	int num = luaL_checkint(L, 3);
	int len = luaL_checkint(L, 4);
  int size = luaL_checkint(L, 5);
	LCD_ShowNum(x,y,num,len,size);
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
static int Setrand(lua_State *L)
{
  int val =0;
  val = rand()%(228/12)*12;
  lua_pushnumber(L,val);
  return 1;
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
	 int key=0;
	
	 u8 gkey= KEY_Scan(0);
	  switch(gkey)
		{
			case WKUP_PRES:key=1;break;
			case KEY0_PRES:key=2;break;
			case KEY1_PRES:key=3;break;
			case KEY2_PRES:key=4;break;
			default:break;
			
		}
		if(gkey)
    lua_pushnumber(L,key);
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
	return 0;
}
#define Size 1024*4
	lua_State *L;
	FILE *fp;
	int size=Size;
   FATFS fs;
char buff[Size];
	char buff2[Size]=
{
 "BLUE=0x001F WHITE=0xFFFF RED=0xF800 LCD.Clear(WHITE)  Key_UP = 1 Key_Down = 3 Key_Left = 4 Key_Right = 2 key =0 Snake ={} Food ={} game ={} Snake.X={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} Snake.Y={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  i=0  Snake.Length=2  Snake.Life=0 Snake.Dir=1 game.Score=0 game.Life=4 Food.Yes=1 Food.X=0 Food.Y=0 Snake.X[0]=0 Snake.Y[0]=24 Snake.X[1]=12 Snake.Y[1]=24 function GameOver() while 1 do key = Getkey()   if key==Key_UP or key==Key_Down or key==Key_Left or key==Key_Right  then  i=0  Snake.Length=2  Snake.Life=0 Snake.Dir=1 game.Score=0 game.Life=4 Food.Yes=1 Food.X=0 Food.Y=0 Snake.X[0]=0 Snake.Y[0]=24 Snake.X[1]=12 Snake.Y[1]=24 LCD.Clear(WHITE) break else LCD_ShowNum(10,10,33,3,16) end end	end while 1 do key = Getkey()   if key==Key_UP then Snake.Dir=3	elseif key==Key_Down 	then Snake.Dir=4		elseif key==Key_Left then Snake.Dir=2	elseif key==Key_Right	then  Snake.Dir=1  end	if Food.Yes==1 then Food.X=12+rand() Food.Y=12+rand() Food.Yes=0 end if(Food.Yes==0) then LCD_Fill(Food.X,Food.Y,Food.X+10,Food.Y+10,BLUE) end for i=Snake.Length-1,1,-1 do Snake.X[i]=Snake.X[i-1] Snake.Y[i]=Snake.Y[i-1] end  if Snake.Dir==1 then Snake.X[0]=Snake.X[0]+12  elseif Snake.Dir==2  then  Snake.X[0]=Snake.X[0]-12  elseif Snake.Dir==3  then Snake.Y[0]=Snake.Y[0]-12 elseif Snake.Dir==4	 then	 Snake.Y[0]=Snake.Y[0]+12	  end  for i=0,Snake.Length-1,1 do LCD_Fill(Snake.X[i],Snake.Y[i],Snake.X[i]+10,Snake.Y[i]+10,RED)  end delay_ms(300) LCD_Fill(Snake.X[Snake.Length-1],Snake.Y[Snake.Length-1],Snake.X[Snake.Length-1]+10,Snake.Y[Snake.Length-1]+10,WHITE) if(Snake.X[0]==Food.X and Snake.Y[0]==Food.Y)  then LCD_Fill(Food.X,Food.Y,Food.X+10,Food.Y+10,WHITE) Snake.Length=Snake.Length+1  game.Score=game.Score+10  LCD_ShowNum(40,165,game.Score,3,16)	 Food.Yes=1 end LCD_ShowNum(224,165,game.Life,1,16) if(Snake.X[0]==0 or Snake.X[0]>240 or Snake.Y[0]==0 or Snake.Y[0]>320) then GameOver() end  end "
	//for i=1,Snake.Length do LCD_Fill(Snake.X[i],Snake.Y[i],Snake.X[i]+10,Snake.Y[i]+10,LCD.RED) end if Food.Yes==1 then Food.X=12+rand()%(228/12)*12	end " 
};
	//if(Snake.X[0]==0 or Snake.X[0]>240 or Snake.Y[0]==0 or Snake.Y[0]>150) then GameOver() end//  for i=1,Snake.Length do LCD_Fill(Snake.X[i],Snake.Y[i],Snake.X[i]+10,Snake.Y[i]+10,RED); end delay_ms(300)
// LCD_Fill(Snake.X[Snake.Length-1],Snake.Y[Snake.Length-1],Snake.X[Snake.Length-1]+10,Snake.Y[Snake.Length-1]+10,WHITE);
 //"BLUE=0x001F WHITE=0xFFFF RED=0xF800 LCD.Clear(WHITE)  Key_UP = 1 Key_Down = 3 Key_Left = 4 Key_Right = 2 key =0 Snake ={} Food ={} game ={} Snake.X={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} Snake.Y={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  i=0  Snake.Length=2  Snake.Life=0 Snake.Dir=1 game.Score=0 game.Life=4 Food.Yes=1 Snake.X[0]=0 Snake.Y[0]=24 Snake.X[1]=12 Snake.Y[1]=24 while 1 do key = Getkey()   if key==Key_UP then Snake.Dir=3	elseif key==Key_Down 	then Snake.Dir=4		elseif key==Key_Left then Snake.Dir=2	elseif key==Key_Right	then  Snake.Dir=1  end	if Food.Yes==1 then Food.X=12+rand()%(228/12)*12 Food.Y=12+rand()%(148/12)*12 Food.Yes=0 end if(Food.Yes==0) then LCD_Fill(Food.X,Food.Y,Food.X+10,Food.Y+10,BLUE) end for i=Snake.Length-1,1,-1 do Snake.X[i]=Snake.X[i-1] Snake.Y[i]=Snake.Y[i-1] end  if Snake.Dir==1 then Snake.X[0]=Snake.X[0]+12  elseif Snake.Dir==2  then  Snake.X[0]=Snake.X[0]-12  elseif Snake.Dir==3  then Snake.Y[0]=Snake.Y[0]-12 elseif Snake.Dir==4	 then	 Snake.Y[0]=Snake.Y[0]+12	  end  for i=0,Snake.Length,1 do LCD_Fill(Snake.X[i],Snake.Y[i],Snake.X[i]+10,Snake.Y[i]+10,RED)  end delay_ms(300) LCD_Fill(Snake.X[Snake.Length-1],Snake.Y[Snake.Length-1],Snake.X[Snake.Length-1]+10,Snake.Y[Snake.Length-1]+10,WHITE) if(Snake.X[0]==Food.X and Snake.Y[0]==Food.Y)  then LCD_Fill(Food.X,Food.Y,Food.X+10,Food.Y+10,WHITE) Snake.Length=Snake.Length+1  game.Score=game.Score+10  LCD_ShowNum(40,165,game.Score,3,16)	 Food.Yes=1 end LCD_ShowNum(224,165,game.Life,1,16) end 
	static int do_file_script(char *filename)
{


f_mount( 0, &fs);//挂载文件系统
	if( ( fp = fopen(filename, "r" ) ) == NULL )
	{ 
		LCD_ShowString(30,150,16,"CAN NOT OPEN THIS FILE",RED);
		delay_ms(300);
		LCD_ShowString(30,150,16,"CAN NOT OPEN THIS FILE",BLACK);
		return -1;
	}
	 
	fseek( fp, 0L, SEEK_END );
  //size=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	if(size<=0)
	{	
		fclose( fp ); 
		return -1;
	}
	//buff=malloc(size);
	fread(buff,size,1,fp);
	
	if(buff==NULL)
	{	
		LCD_ShowString(50,150,16,"buff  NULL",RED);
		return -1;
	}

	LCD_ShowString(50,100,16,"size =",RED);
	LCD_ShowNum(100,100,strlen(buff),4,16);
	 	LCD_ShowString(50,150,16,buff,RED);
	//while(1);
	L   = lua_open(); 
	luaopen_base(L);  
	register_lcd(L);
	lua_register(L,"LCD_print",LCD_print);
	lua_register(L,"LCD_Fill",_LCD_Fill);
	lua_register(L,"Getkey",GetKey);
	lua_register(L,"delay_ms",_delay_ms);
	lua_register(L,"rand",Setrand);
	lua_register(L,"LCD_ShowNum",_LCD_ShowNum);
	
	luaL_dostring(L, buff); 
	return 0;
}
  Menu *menu;
 int main(void)
{
	 
	u8 key;
	delay_init();
	LCD_Init();
	SD_Init();
	KEY_Init();	
	do_file_script("test.lua");
//	LCD_ShowString(50,100,16,"size =",RED);
//	LCD_ShowNum(100,100,strlen(buff2),4,16);
//	 	LCD_ShowString(50,150,16,buff2,RED);
//	//while(1);
//	L   = lua_open(); 
//	luaopen_base(L);  
//	register_lcd(L);
//	
//	lua_register(L,"GetKey",GetKey);
//	lua_register(L,"delay_ms",_delay_ms);
//	luaL_dostring(L, buff2); 
//	menu=&_MM2;
//  InitMenu(menu);
//  Locate2(menu);
//	
//  while(1)
//	{
//		key=KEY_Scan(0);		//????
//	   	if(key)
//		{						   
//	     switch(key)
//			{				 
//		case WKUP_PRES:
//			         if(menu->Visible==0)
//							 {  
//								 menu->Visible=1;
//								 menuid[mcount]=0;
//									InitMenu(menu);
//									Locate2(menu); 
//							 }		 
//			break;
//				case KEY0_PRES:	
//					if(menu->Visible==0) break;
//						   	  menuid[mcount]++;   
//	        if (menuid[mcount]>menu->Item->Menucount -1) menuid[mcount]=0;  
//			Locate2(menu);
//					break;
//				case KEY1_PRES:	
//        if(menu->Visible==0) break;
//				if ((menu->Item+menuid[mcount])->ChildMenu!=0)   
//	     {   
//	        
//			   menu =(menu->Item+menuid[mcount])->ChildMenu; 
//				 if(menu->Visible==0)
//				 {
//					 menu->Visible=1;
//				 }
//				 menu->x=menu->Item->ParentMenu->x+menu->H_SIZE;
//				 menu->y=menu->Item->ParentMenu->y+(menuid[mcount]*(menu->Dis+menu->FontSize));
//				 
//				 InitMenu(menu);
//		     mcount++;   
//	     	 menuid[mcount]=0;   
//			   Locate2(menu);
//			   menu->x-=(menu->Item->ParentMenu->x+menu->H_SIZE-menu->x);
//				// menu->y-=menu->Item->ParentMenu->y+(menuid[mcount]*(menu->Dis+menu->FontSize));
//		   }   
//		  else   
//		 {   
//			      Run(menu->Item+menuid[mcount]);   
//		 }	
//					break;
//				case KEY2_PRES:		 
//				
//			
//			if (menu->Item->ParentMenu !=0)   
//			{   
//				if(menu->Visible==1)
//			  {
//					menu->Visible=0;
//			
//				}
//					  InitMenu(menu);
//				  Locate2(menu);
//			  menu=(menu->Item+menuid[mcount])->ParentMenu;   
//			
//			  
//         InitMenu(menu);   
//				mcount--;   
//		      Locate2(menu);
//					
//			 }   
//			 else   
//			 {   
//         if(menu->Visible==1)
//			   menu->Visible=0;
//				InitMenu(menu);
//				Locate2(menu);     
//		   }	
//					break;
//			}
//		}

//	
//  }

}


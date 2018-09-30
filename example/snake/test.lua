BLUE=0x001F
BACKCOLOR=0x0000 
RED=0xF800 
LCD.Clear(BACKCOLOR)  
Key_UP = 1 Key_Down = 3 Key_Left = 4 Key_Right = 2 
key =0 Snake ={} Food ={} game ={} 
Snake.X={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} Snake.Y={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  
i=0  Snake.Length=2  Snake.Life=0 Snake.Dir=1 game.Score=0 game.Life=4 Food.Yes=1 Food.X=0 Food.Y=0 Snake.X[0]=0 Snake.Y[0]=24 Snake.X[1]=12 Snake.Y[1]=24 
function GameOver() 
while 1 do 
	key = Getkey()   
	if key==Key_UP or key==Key_Down or key==Key_Left or key==Key_Right  
	then  i=0  Snake.Length=2  Snake.Life=0 Snake.Dir=1 game.Score=0 game.Life=4 Food.Yes=1 Food.X=0 Food.Y=0 Snake.X[0]=0 Snake.Y[0]=24 Snake.X[1]=12 Snake.Y[1]=24 LCD.Clear(BACKCOLOR) break 
	else LCD_print(0,100,"Game is Over! press any key to  continue") 
    end 
end	
end 
while 1 do 
key = Getkey()   
if key==Key_UP then Snake.Dir=3	
elseif key==Key_Down then Snake.Dir=4 
elseif key==Key_Left then Snake.Dir=2	
elseif key==Key_Right	then  Snake.Dir=1  
end	
if Food.Yes==1 then Food.X=12+rand() Food.Y=12+rand() Food.Yes=0 end 
if(Food.Yes==0) then LCD_Fill(Food.X,Food.Y,Food.X+10,Food.Y+10,BLUE) end 
for i=Snake.Length-1,1,-1 do 
Snake.X[i]=Snake.X[i-1] Snake.Y[i]=Snake.Y[i-1] 
end  
if Snake.Dir==1 then 
Snake.X[0]=Snake.X[0]+12  
elseif Snake.Dir==2  then  
Snake.X[0]=Snake.X[0]-12  
elseif Snake.Dir==3  then 
Snake.Y[0]=Snake.Y[0]-12 
elseif Snake.Dir==4	 then	
Snake.Y[0]=Snake.Y[0]+12	
end  
for i=0,Snake.Length-1,1 do 
LCD_Fill(Snake.X[i],Snake.Y[i],Snake.X[i]+10,Snake.Y[i]+10,RED)  
end 
delay_ms(300) 
LCD_Fill(Snake.X[Snake.Length-1],Snake.Y[Snake.Length-1],Snake.X[Snake.Length-1]+10,Snake.Y[Snake.Length-1]+10,BACKCOLOR) 
if(Snake.X[0]==Food.X and Snake.Y[0]==Food.Y)  then 
LCD_Fill(Food.X,Food.Y,Food.X+10,Food.Y+10,BACKCOLOR) 
Snake.Length=Snake.Length+1  
game.Score=game.Score+10 
Food.Yes=1 
end 
LCD_ShowNum(40,165,game.Score,3,16)
LCD_ShowNum(224,165,game.Life,1,16) 
if(Snake.X[0]==0 or Snake.X[0]>240 or Snake.Y[0]==0 or Snake.Y[0]>320) then 
GameOver() 
end 

 end
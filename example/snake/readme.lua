BLUE=0x001F
BACKCOLOR=0x0000 
RED=0xF800 
LCD.Clear(BACKCOLOR) 
key = 0 
while 1 do 
key = Getkey() 
if key == 1 
then 
LCD.Clear(RED) 
elseif key == 2 
then 
LCD.Clear(BACKCOLOR) 
elseif key ==3 
then 
LCD.Clear(BACKCOLOR) 
elseif key == 4 
then 
LCD.Clear(BLUE) 
end
LCD_print(0,0," switch (key) {case 1: RED break;case 2: BACKCOLOR break;case 4: BLUE break; default:break;} ") 
end



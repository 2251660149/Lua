dio.mode(dio.OUTPUT,0);
while 1 do
   dio.write(dio.LOW,0);
   delay.ms(500);
   dio.write(dio.HIGH,0);
   delay.ms(500);
end
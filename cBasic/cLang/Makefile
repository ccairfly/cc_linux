CC = gcc
obj = main.o app_key.o apps.o app_audio.o \
app_batt.o app_led.o app_test.o app_factory.o
edit : $(obj)
	gcc	-o edit $(obj)
	
$(obj) : app_key.h app_util.h
main.o apps.o app_factory.o: apps.h 

.PHONY : clean
clean :
	-del edit.exe $(obj)
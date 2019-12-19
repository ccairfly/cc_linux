CC = gcc
obj = main.o app_key.o apps.o app_audio.o \
app_batt.o app_led.o app_test.o app_factory.o
edit : $(obj)
	gcc	-o edit $(obj)
main.o : app_util.h apps.h app_key.h
app_key.o : app_key.h
apps.o : apps.h app_util.h app_key.h
app_audio.o : app_util.h app_key.h
app_batt.o : app_util.h app_key.h
app_led.o : app_util.h app_key.h
app_test.o : app_util.h app_key.h
app_factory.o : apps.h app_util.h app_key.h
clean :
	del edit.exe $(obj)
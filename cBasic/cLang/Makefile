edit : main.o app_key.o apps.o app_audio.o \
app_batt.o app_led.o app_test.o app_factory.o
	gcc -o edit main.o app_key.o apps.o app_audio.o \
app_batt.o app_led.o app_test.o app_factory.o
main.o : main.c app_util.h apps.h app_key.h
	gcc -c main.c
app_key.o : app_key.c app_key.h
	gcc -c app_key.c
apps.o : apps.c apps.h app_util.h app_key.h
	gcc -c apps.c
app_audio.o : app_audio.c app_util.h app_key.h
	gcc -c app_audio.c
app_batt.o : app_batt.c app_util.h app_key.h
	gcc -c app_batt.c
app_led.o : app_led.c app_util.h app_key.h
	gcc -c app_led.c
app_test.o : app_test.c app_util.h app_key.h
	gcc -c app_test.c
app_factory.o : app_factory.c apps.h app_util.h app_key.h
	gcc -c app_factory.c
clean :
	del edit main.o app_key.o apps.o app_audio.o \
app_batt.o app_factory.o app_led.o app_test.o
all: connect hello hello_ext events complete_events

connect:
	gcc -Wall connect.c -lxcb -o connect

hello:
	gcc -Wall hello.c -lxcb -o hello

hello_ext:
	gcc -Wall hello_ext.c -lxcb -o hello_ext

events:
	gcc -Wall events.c -lxcb -o events

complete_events:
	gcc -std=c99 -Wall complete_events.c -lxcb -o cevents

clean:
	rm connect hello hello_ext events cevents

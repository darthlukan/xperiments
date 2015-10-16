all: connect hello hello_ext

connect:
	gcc -Wall connect.c -lxcb -o connect

hello:
	gcc -Wall hello.c -lxcb -o hello

hello_ext:
	gcc -Wall hello_ext.c -lxcb -o hello_ext

clean:
	rm connect hello hello_ext

DESTDIR=/usr/bin


all: tinywm

tinywm:
	gcc -std=c99 -Wall tinywm.c -lxcb -o tinywm

install: tinywm
	cp tinywm ${DESTDIR}/tinywm

uninstall:
	rm -f ${DESTDIR}/tinywm

clean:
	rm tinywm

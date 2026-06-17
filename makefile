# Règle principale : compile l'exécutable "steve" et nettoie les fichiers objets (.o) automatiquement
main: main.o menu.o platforme.o libisentlib.a
	gcc -Wall main.o menu.o platforme.o -o steve libisentlib.a -lm -lglut -lGL -lX11
	rm -f main.o menu.o platforme.o BmpLib.o ErreurLib.o ESLib.o GfxLib.o OutilsLib.o SocketLib.o ThreadLib.o TortueLib.o WavLib.o

main.o: main.c GfxLib.h BmpLib.h ESLib.h menu.h
	gcc -Wall -c main.c

menu.o: menu.c menu.h GfxLib.h BmpLib.h
	gcc -Wall -c menu.c

platforme.o: platforme.c platforme.h GfxLib.h BmpLib.h
	gcc -Wall -c platforme.c

# Compilation de la bibliothèque ISEN
libisentlib.a: BmpLib.o ErreurLib.o ESLib.o GfxLib.o OutilsLib.o SocketLib.o ThreadLib.o TortueLib.o WavLib.o
	ar r libisentlib.a BmpLib.o ErreurLib.o ESLib.o GfxLib.o OutilsLib.o SocketLib.o ThreadLib.o TortueLib.o WavLib.o
	ranlib libisentlib.a

BmpLib.o: BmpLib.c BmpLib.h OutilsLib.h
	gcc -Wall -O2 -c BmpLib.c

ESLib.o: ESLib.c ESLib.h ErreurLib.h
	gcc -Wall -O2 -c ESLib.c

ErreurLib.o: ErreurLib.c ErreurLib.h
	gcc -Wall -O2 -c ErreurLib.c

GfxLib.o: GfxLib.c GfxLib.h ESLib.h
	gcc -Wall -O2 -c GfxLib.c -I/usr/include/GL

OutilsLib.o: OutilsLib.c OutilsLib.h
	gcc -Wall -O2 -c OutilsLib.c

SocketLib.o: SocketLib.c SocketLib.h
	gcc -Wall -O2 -c SocketLib.c

ThreadLib.o: ThreadLib.c ThreadLib.h
	gcc -Wall -O2 -c ThreadLib.c

TortueLib.o: TortueLib.c TortueLib.h GfxLib.h
	gcc -Wall -O2 -c TortueLib.c

WavLib.o: WavLib.c WavLib.h OutilsLib.h
	gcc -Wall -O2 -c WavLib.c -Wno-unused-result

zip:
	zip libisentlib.zip *.[ch] *.bmp *.pdf makefile

# Nettoie absolument tout SAUF l'exécutable "steve"
clean:
	rm -f *~ *.o *.a

# Supprime tout, y compris l'exécutable
deepclean: clean
	rm -f steve

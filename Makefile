CFLAGS = -O2

sterne:	sterne.o
		$(CC) sterne.o `sdl-config --libs` -lGL -o sterne

sterne.o:	sterne.c
		$(CC) $(CFLAGS) -c sterne.c `sdl-config --cflags`

clean:
		rm -f *.o sterne

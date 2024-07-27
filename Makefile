CFLAGS = -O2

sterne:	sterne.o
		$(CC) $^ `sdl-config --libs` -lGL -o $@

sterne.o:	sterne.c
		$(CC) $^ $(CFLAGS) -c `sdl-config --cflags`

clean:
		rm -f *.o sterne

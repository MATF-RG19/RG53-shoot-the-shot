PROGRAM = shoot-the-shot
CC		= gcc
FLAGS	= -Wall
LIBS 	= -lGL -lGLU -lglut -lm

$(PROGRAM): main.o draw.o
			$(CC) $(FLAGS) -o $(PROGRAM) main.o draw.o $(LIBS)

main.o:	main.c draw.c draw.h
			$(CC) $(FLAGS) -c main.c -o main.o $(LIBS)

draw.o: draw.c draw.h
			$(CC) $(FLAGS) -c draw.c -o draw.o $(LIBS)
			
.PHONY:
		clean
		
clean:
		-rm *.o	$(PROGRAM)

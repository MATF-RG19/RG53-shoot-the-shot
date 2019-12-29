PROGRAM = shoot-the-shot
CC		= gcc
FLAGS	= -Wall
LIBS 	= -lGL -lGLU -lglut -lm

$(PROGRAM): main.o draw.o details.o
			$(CC) $(FLAGS) -o $(PROGRAM) main.o draw.o details.o $(LIBS)

main.o:	main.c draw.c draw.h details.c details.h
			$(CC) $(FLAGS) -c main.c -o main.o $(LIBS)

draw.o: draw.c draw.h
			$(CC) $(FLAGS) -c draw.c -o draw.o $(LIBS)
			
details.o: details.c details.h
			$(CC) $(FLAGS) -c details.c -o details.o $(LIBS)
			
.PHONY:
		clean
		
clean:
		-rm *.o	$(PROGRAM)

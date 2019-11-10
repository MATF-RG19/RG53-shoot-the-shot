PROGRAM = shoot-the-shot
CC		= gcc
FLAGS	= -Wall
LIBS 	= -lGL -lGLU -lglut

$(PROGRAM): main.o
			$(CC) $(FLAGS) -o $(PROGRAM) main.o $(LIBS)

main.o:	main.c
			$(CC) $(FLAGS) -c main.c -o main.o $(LIBS)
			
.PHONY:
		clean
		
clean:
		-rm *.o	$(PROGRAM)

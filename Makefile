PROGRAM = shoot-the-shot
CC		= gcc
FLAGS	= -Wall
LIBS 	= -lGL -lGLU -lglut -lm

$(PROGRAM): main.o draw.o details.o image.o
			$(CC) $(FLAGS) -o $(PROGRAM) main.o draw.o details.o image.o $(LIBS)

main.o:	main.c draw.c draw.h details.c details.h image.c image.h
			$(CC) $(FLAGS) -c main.c -o main.o $(LIBS)

draw.o: draw.c draw.h
			$(CC) $(FLAGS) -c draw.c -o draw.o $(LIBS)
			
details.o: details.c details.h
			$(CC) $(FLAGS) -c details.c -o details.o $(LIBS)
			
image.o: image.c image.h
			$(CC) $(FLAGS) -c image.c -o image.o $(LIBS)
			
.PHONY:
		clean
		
clean:
		-rm *.o	$(PROGRAM)

  
CC=g++
OPCOES_COMPILADOR=-I. -I/../SOIL -lGL -lGLU -lglut -lSOIL -lsfml-audio -unistd.h
TODOS_ARQUIVOS_PONTOH =
TODOS_ARQUIVOS_OBJ = pong.o 

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	g++ -o pong $^ $(OPCOES_COMPILADOR)

run: all
	./pong

clean:
	rm *.o pong


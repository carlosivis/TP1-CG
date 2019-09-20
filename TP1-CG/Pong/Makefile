  
CC=g++
OPCOES_COMPILADOR=-I. -I/../SOIL -lGL -lGLU -lglut -lSOIL -lsfml-audio
TODOS_ARQUIVOS_PONTOH =
TODOS_ARQUIVOS_OBJ = main.o 

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	g++ -o main $^ $(OPCOES_COMPILADOR)

run: all
	./main

clean:
	rm *.o main


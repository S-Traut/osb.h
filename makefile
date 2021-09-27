SRC := src
OBJ := obj
FLAGS=-g -Wall

SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

exec: osbh
	./osbh

osbx: $(OBJECTS)
	$(CC) $^ -o $@ $(FLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -I$(SRC) -c $< -o $@ $(FLAGS)

clean:
	rm -rf obj/
	rm libft

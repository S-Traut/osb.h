SRC := src
OBJ := obj
FLAGS=-g -Wall

NAME=osbh

SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

exec: $(NAME)
	@./$(NAME)

$(NAME): $(OBJ) $(OBJECTS)
	@$(CC) $(OBJECTS) -o $(NAME) $(FLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	@$(CC) -I$(SRC) -c $< -o $@ $(FLAGS)

$(OBJ):
	@mkdir $(OBJ)

clean:
	@rm -rf $(OBJ)
	@rm -f libft
	@rm -f $(NAME)

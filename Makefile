CC = gcc
CFLAGS = -I./includes -I/opt/homebrew/include -Wall -Wextra -Wno-deprecated-declarations
LDFLAGS = -L/opt/homebrew/lib -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio -lm

SRC = \
	src/main.c \
	src/init.c \
	src/raycasting.c \
	src/movement.c \
	src/menu.c \
	src/weapon.c \
	src/enemy.c


OBJ = $(SRC:.c=.o)
NAME = wolf3d

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

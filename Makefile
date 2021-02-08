NAME = play_me

CC = gcc

CFLAGS = -I/usr/include -O3 -I./mlx

# gcc -I/usr/include -O3 -I./mlx   -c -o main.o main.c
# gcc -o mlx-test main.o -L./mlx -lmlx -L/usr/include/../lib -lXext -lX11 -lm -lbsd

HEADER =	cub3D.h \

SRC =		main \

FIL = $(addsuffix .c, $(addprefix ./, $(SRC)))

OBJ = $(FIL:.c=.o)

BIN = $(addsuffix .o, $(SRC))


.PHONY: all clean fclean re norm

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)  -L./mlx -lmlx -L/usr/include/../lib -lXext -lX11 -lm -lbsd

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all


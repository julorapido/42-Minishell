OUT = minishell

SRC = ./parsing/*.c  *.c ./builtins/*.c ./env/*.c ./executor/*.c ./signals/*.c
SRC_OBJS = $(SRC.c=.o)

FLAGS = -Wall -Wextra -g
HEADER = -I. -I$(LIBFT_DIR)

LIBFT_DIR = Libft/
LIBFT = $(LIBFT_DIR)libft.a

all : $(LIBFT) $(OUT)

$(LIBFT):
	@echo "Making Libft.."
	@make -C $(LIBFT_DIR)

$(OUT): $(OUT_O)
	gcc $(FLAGS) $(SRC) $(HEADER) -lreadline -o $(OUT) $(LIBFT)

clean:
	rm -f $(SRC_OBJS)

fclean: clean
	rm -f $(OUT)

re: fclean all


NAME = ft_ls

CC = gcc
CFLAGS = -Wall -Werror -Wextra
SANITIZE = -g3 -fsanitize=address -fsanitize=leak

SRC = src/ft_ls.c src/parser.c src/output.c
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

INCLUDE = -I./inc
LIBFT = libft

.PHONY: all
all: $(NAME)
$(NAME): libft_submodule $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT) -lft -o $(NAME)

obj/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $(INCLUDE) -I$(LIBFT) $< -o $@

.PHONY: clean
clean:
	$(RM) -r obj
	make clean -C $(LIBFT)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)
	make fclean -C $(LIBFT)

.PHONY: re
re: fclean all

.PHONY: libft
libft_submodule:
	@if [ -z "$$(git config --file .gitmodules submodule.$(LIBFT).url)" ]; then \
		git submodule add https://github.com/AingeruAlvarezSanchez/Libft $(LIBFT); \
	fi
	git submodule update --init --recursive
	make ext -C $(LIBFT)

.PHONY: sanitize
sanitize: CFLAGS += $(SANITIZE)
sanitize: clean all

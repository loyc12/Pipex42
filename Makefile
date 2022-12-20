# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llord <llord@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/05 11:28:36 by llord             #+#    #+#              #
#    Updated: 2022/12/20 15:16:50 by llord            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



#------------------------------------------------------------------------------#
#                                  COLOURS                                     #
#------------------------------------------------------------------------------#

DEF_COLOR = \033[0;39m
MAGENTA = \033[0;95m
RED = \033[0;91m
YELLOW = \033[0;93m
GREEN = \033[0;92m
CYAN = \033[0;96m
BLUE = \033[0;94m
GRAY = \033[0;90m
WHITE = \033[0;97m

#------------------------------------------------------------------------------#
#                                  GENERICS                                    #
#------------------------------------------------------------------------------#

# Special variables
DEFAULT_GOAL: all
.DELETE_ON_ERROR: $(NAME)
.PHONY: all bonus clean fclean re run compare debug leaks longleaks

# Hide calls
export VERBOSE	=	TRUE
ifeq ($(VERBOSE),TRUE)
	HIDE =
else
	HIDE = @
endif


#------------------------------------------------------------------------------#
#                                 VARIABLES                                    #
#------------------------------------------------------------------------------#

# Compiler and flags
CC		=	gcc
CFLAGS	=	-Wall -Werror -Wextra
RM		=	rm -rf
INCLUDE =
LIBS	=

# Dir and file names
NAME	=	pipex
SRCDIR	=	src/
OBJDIR	=	bin/
FILES	=	pipex

SRCS	=	$(addprefix $(SRCDIR), $(addsuffix .c, $(FILES)))
OBJS	=	$(addprefix $(OBJDIR), $(addsuffix .o, $(FILES)))


#------------------------------------------------------------------------------#
#                                 TARGETS                                      #
#------------------------------------------------------------------------------#

all: $(NAME)

$(NAME): $(OBJS)
	$(HIDE)$(CC) $(CFLAGS) -o $@ $^ $(INCLUDE) $(LIBS)
	@echo "$(GREEN)Files compiled$(DEF_COLOR)"

$(OBJS): $(OBJDIR)%.o : $(SRCDIR)%.c
	@echo "$(YELLOW)Compiling: $< $(DEF_COLOR)"
	$(HIDE)$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

$(OBJDIR):
	$(HIDE)mkdir -p $(OBJDIR)

# Removes objects
clean:
	$(HIDE)$(RM) $(OBJS)
	@echo "$(MAGENTA)Object files cleaned$(DEF_COLOR)"

# Removes objects and executables
fclean: clean
	$(HIDE)$(RM) $(NAME)
	@echo "$(RED)Executable files cleaned$(DEF_COLOR)"

# Removes objects and executables and remakes
re: fclean all
	@echo "$(CYAN)Cleaned and rebuilt everything!$(DEF_COLOR)"

# Runs the resulting file
run: re
	@echo "$(BLUE)Starting the program...$(DEF_COLOR)"
	./pipex src.txt "grep e" "wc -l" dst.txt

compare: re
	@echo "$(RED)Starting the comparison...$(DEF_COLOR)"
	./pipex src.txt "grep a" "wc -w" pipex.txt
	< src.txt grep a | wc -w > real.txt

debug:
	@echo "$(RED)Compiled for debugging!$(DEF_COLOR)"
	gcc -g -Wall -Werror -Wextra pipex.h src/pipex.c

leaks: re
	@echo "$(RED)Starting the leak checking...$(DEF_COLOR)"
	leaks --atExit -- ./pipex src.txt "grep e" "wc -l" dst.txt

longleaks: re
	@echo "$(RED)Starting the complete leak checking...$(DEF_COLOR)"
	leaks --atExit -- ./pipex src.txt "grep e" "wc -l" dst.txt
	sleep 1s
	leaks --atExit -- ./pipex src.tx "grep e" "wc -l" dst.txt
	sleep 1s
	leaks --atExit -- ./pipex src.txt "grep e" "wc -l" ""
	sleep 1s
	leaks --atExit -- ./pipex src.txt "grep" "wc -l" dst.txt
	sleep 1s
	leaks --atExit -- ./pipex src.txt "grep e" "wc -la" dst.txt
	sleep 1s
	leaks --atExit -- ./pipex src.txt "grep e" "wc -l"
	sleep 1s
	leaks --atExit -- ./pipex src.txt "grep e" "wc -l" dst.txt dst.txt
#	sleep 1s
#	leaks --atExit -- ./pipex src.txt "gre e" "wc -l" dst.txt
#	sleep 1s
#	leaks --atExit -- ./pipex src.txt "gre e" "ww -l" dst.txt
#	sleep 1s
#	leaks --atExit -- ./pipex src.txt "grep e" "ww -l" dst.txt


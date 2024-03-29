# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llord <llord@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/05 11:28:36 by llord             #+#    #+#              #
#    Updated: 2022/12/22 14:15:11 by llord            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#------------------------------------------------------------------------------#
#                                   COLOURS                                    #
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
#                                   GENERICS                                   #
#------------------------------------------------------------------------------#

# Special variables
DEFAULT_GOAL: all
.DELETE_ON_ERROR: $(NAME)
.PHONY: all bonus clean fclean re run debug leaks tests

# Hide calls
export VERBOSE	= FALSE
ifeq ($(VERBOSE),TRUE)
	HIDE =
else
	HIDE = @
endif

#------------------------------------------------------------------------------#
#                                  VARIABLES                                   #
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
TSTDIR	=	tests/
FILES	=	pipex forkers pathers addons libft_imports

SRCS	=	$(addprefix $(SRCDIR), $(addsuffix .c, $(FILES)))
OBJS	=	$(addprefix $(OBJDIR), $(addsuffix .o, $(FILES)))

CMD		=	./pipex $(TSTDIR)src.txt "grep a" "cat" $(TSTDIR)dst.txt

#------------------------------------------------------------------------------#
#                                   TARGETS                                    #
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
run: all
	@echo "$(BLUE)Piping..$(DEF_COLOR)"
	$(HIDE)$(CMD)

debug: all
	@echo "$(RED)Compiled for debugging!$(DEF_COLOR)"
	$(HIDE)gcc -g -Wall -Werror -Wextra pipex.h src/pipex.c

leaks: all
	@echo "$(RED)Checking leaks...$(DEF_COLOR)"
	$(HIDE)valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes -s $(CMD)

tests: all
#	real pipe creates empty files when an error occurs, so long as dst path is non-empty (t1-t10)
	@echo "$(RED)Starting the comparison tests...$(DEF_COLOR)"
	@echo "\ntest 1"
	$(HIDE)./pipex $(TSTDIR)src.txt "grep e" "cat" $(TSTDIR)p1.txt
	$(HIDE)< $(TSTDIR)src.txt grep e | cat > $(TSTDIR)r1.txt
	@echo "\ntest 2"
	$(HIDE)./pipex "" "grep e" "cat" $(TSTDIR)p2.txt
	$(HIDE)< "" grep e | cat > $(TSTDIR)r2.txt
	@echo "\ntest 3"
	$(HIDE)./pipex nope.txt "grep e" "cat" $(TSTDIR)p3.txt
	$(HIDE)< nope.txt grep e | cat > $(TSTDIR)r3.txt
	@echo "\ntest 4"
	$(HIDE)./pipex $(TSTDIR)src.txt "" "cat" $(TSTDIR)p4.txt
	$(HIDE)< $(TSTDIR)src.txt "" | cat > $(TSTDIR)r4.txt
	@echo "\ntest 5"
	$(HIDE)./pipex $(TSTDIR)src.txt "grep" "cat" $(TSTDIR)p5.txt
	$(HIDE)< $(TSTDIR)src.txt grep | cat > $(TSTDIR)r5.txt
	@echo "\ntest 6"
	$(HIDE)./pipex $(TSTDIR)src.txt "gre e" "cat" $(TSTDIR)p6.txt
	$(HIDE)< $(TSTDIR)src.txt gre e | cat > $(TSTDIR)r6.txt
	@echo "\ntest 7"
	$(HIDE)./pipex $(TSTDIR)src.txt "grep e" "ca" $(TSTDIR)p7.txt
#	$(HIDE)< $(TSTDIR)src.txt grep e | ca > $(TSTDIR)r7.txt
	@echo "\ntest 8"
	$(HIDE)./pipex $(TSTDIR)src.txt "grep e" "cat -a" $(TSTDIR)p8.txt
#	$(HIDE)< $(TSTDIR)src.txt grep e | cat -a > $(TSTDIR)r8.txt
	@echo "\ntest 9"
	$(HIDE)./pipex $(TSTDIR)src.txt "grep e" "" $(TSTDIR)p9.txt
#	$(HIDE) < $(TSTDIR)src.txt grep e | "" > $(TSTDIR)r9.txt
	@echo "\ntest 10"
	$(HIDE)./pipex $(TSTDIR)src.txt "" "" $(TSTDIR)p10.txt
#	$(HIDE)< $(TSTDIR)src.txt "" | "" > $(TSTDIR)r10.txt
	@echo "\ntest 11"
	$(HIDE)./pipex "" "grep e" "cat" ""
#	$(HIDE)< "" grep e | cat > ""
	@echo "\ntest 12"
	$(HIDE)./pipex $(TSTDIR)src.txt "grep e" "cat" ""
#	$(HIDE)< $(TSTDIR)src.txt grep e | cat > ""
	@echo "\ntest 13"
	$(HIDE)./pipex "" "" "" ""
#	$(HIDE)< "" "" | "" > ""


#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    mkgen.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nbouteme <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/11/23 11:08:14 by nbouteme          #+#    #+#              #
#    Updated: 2015/12/08 12:51:41 by nbouteme         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

ifndef SRC
SRC := $(shell ls | grep \\.c$)
endif

LIBS =
DEPS =
$(foreach s,$(SRC), $(eval DEPS += $(shell gcc -MM $s -Ilibft/includes |\
										   tr -d \\\\ | sed s/$(basename $s).o:\ $s// |\
										   tr ' ' '\n' | grep '^libft' | sort | uniq |\
										   cut -d '/' -f 4 | sed s/\\.h//)))
OBUILDLIBS := $(DEPS)
.SUFFIXES:
CC = clang
CFLAGS = -Wall -Wextra -Werror $(OPTS)
OBJ = $(SRC:.c=.o)
ECHO = echo
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
ECHO = echo -e
endif
UNIQ := $(shell mktemp)
$(shell make -s -C libft OBUILDLIBS="$(OBUILDLIBS)" > $(UNIQ))
include $(UNIQ)
INCDIRS += -Ilibft/includes
$(eval LIBDIRS += $(addprefix -L,$(LDEP)))
$(eval LIBS += $(DEP))
all: 
	$(foreach dep,$(PDEP), $(if $(shell make -C $(dep) &> /dev/null), $(eval )))
	@$(MAKE) -s $(NAME)
%.o: %.c
	@$(CC) $(CFLAGS) $(INCDIRS) -c $^
	@$(ECHO) "\033[0;32m[✓] Built C object" $@
$(NAME): $(OBJ)
	@$(ECHO) "\033[0;34m--------------------------------"
	@$(CC) -o $(NAME) $(CFLAGS) $(OBJ) $(LIBDIRS) $(addprefix -l,$(LIBS)) $(INCDIRS)
	@$(ECHO) "\033[0;31m[✓] Linked C executable" $(NAME)
clean:
	@/bin/rm -rf $(OBJ)
	@$(ECHO) "\033[0;33m[✓] Removed object files" $(OBJ)
fclean: clean
	@/bin/rm -rf $(NAME)
	@$(ECHO) "\033[0;33m[✓] Removed executable" $(NAME)
re: fclean
	@$(MAKE) all
.PHONY: clean fclean re cleandeps fcleandeps $(PHONY)

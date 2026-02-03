# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lvicino <lvicino@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/28 16:25:07 by lvicino           #+#    #+#              #
#    Updated: 2025/11/11 17:01:20 by lvicino          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= webserv
BUILD_DIR		= build
SRC_DIR			= src

# Libraries
CONF_LIB		= src/conf/conf.a
SERVER_LIB		= src/server/server.a
CGI_LIB			= src/cgi/cgi.a

# Sub-make commands
MAKE_CONF		= $(MAKE) -C src/conf
MAKE_SERVER		= $(MAKE) -C src/server
MAKE_CGI		= $(MAKE) -C src/cgi

# Main source
MAIN_SRC		= $(SRC_DIR)/main.cpp
MAIN_OBJ		= $(MAIN_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
MAIN_DEP		= $(MAIN_OBJ:.o=.d)

# Compiler
CC				= c++
CFLAGS			= -Wall -Werror -Wextra -MMD -MP -std=c++98
INCLUDE			= -I src/conf/include \
				  -I src/server/include \
				  -I src/server/include/Utils \
				  -I src/cgi/include

# Flags
MAKEFLAGS		+= --no-print-directory
DIR_DUP			= mkdir -p $(@D)

# Phony targets
.PHONY: all clean fclean re

# Default target
all: $(NAME)

# Link main + libraries
$(NAME): $(MAIN_OBJ) $(CONF_LIB) $(SERVER_LIB) $(CGI_LIB)
	$(CC) $(MAIN_OBJ) $(CONF_LIB) $(SERVER_LIB) $(CGI_LIB) -o $(NAME)

# Compile main
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(DIR_DUP)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Build directory (order-only)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Force rebuild of libraries before linking (order-only)
$(CONF_LIB) $(SERVER_LIB) $(CGI_LIB): | libs
libs:
	$(MAKE_CONF)
	$(MAKE_SERVER)
	$(MAKE_CGI)

# Include dependencies
-include $(MAIN_DEP)

# Clean object files
clean:
	if [ -d "$(BUILD_DIR)" ]; then rm -rf $(BUILD_DIR); fi
	$(MAKE_CONF) clean
	$(MAKE_SERVER) clean
	$(MAKE_CGI) clean

# Full clean
fclean: clean
	if [ -f "$(NAME)" ]; then rm -f $(NAME); fi
	$(MAKE_CONF) fclean
	$(MAKE_SERVER) fclean
	$(MAKE_CGI) fclean

# Rebuild
re: fclean all

# Silence clean/fclean
.SILENT: clean fclean

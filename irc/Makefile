SRCS_DIR	= src
OBJS_DIR	= obj

PORT		= 2002
IP			= 127.0.0.1
SRCS		= $(shell find $(SRCS_DIR) -name '*.cpp')
OBJS		= $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
DEPS		= $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.d)
NAME		= ircserv
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -I./include
LDFLAGS		=

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(DEPS)
	rm -rf $(OBJS)
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

run: all
	@echo ircserv executing by makefile...
	@./$(NAME) $(PORT) 123

-include $(DEPS)

.PHONY: all clean fclean re run
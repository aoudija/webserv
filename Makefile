NAME = webserv
CPP = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
SRC_FILES = main.cpp server/server.cpp response/response.cpp\
			request/request.cpp client/client.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)
DEP  = server.hpp

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CPP) $(FLAGS) $(OBJ_FILES) -o $(NAME)

%.o: %.cpp $(DEP)
	$(CPP) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean re clean
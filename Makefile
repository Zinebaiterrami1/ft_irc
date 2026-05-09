NAME = ircserv
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

# Utilise la fonction wildcard pour lister les fichiers
SRC = $(wildcard src/*.cpp) $(wildcard commands/*.cpp) main.cpp
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

NAME = computer

FILES_SRC = Number.cpp Rational.cpp Matrix.cpp program.cpp utils.cpp Symbol.cpp Complex.cpp Expression.cpp Computer.cpp Function.cpp Polynomial.cpp
OBJ = $(FILES_SRC:.cpp=.o)
HEAD = Number.hpp Rational.hpp Matrix.hpp utils.hpp Symbol.hpp Complex.hpp Expression.hpp Computer.hpp Function.hpp Polynomial.hpp

all: $(NAME)

$(NAME): $(OBJ)
	clang++ -std=c++11 -Wall -Wextra -Werror $(OBJ)  -o $(NAME)

$(OBJ): %o : %cpp $(HEAD)
	clang++ -std=c++11 -Wall -Wextra -Werror -c $< -o $@
clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean $(NAME)
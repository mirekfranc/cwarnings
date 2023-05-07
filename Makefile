
CXXFLAGS = -O3 -Wall -Wextra -Weffc++ -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wrestrict -Wnull-dereference -Wold-style-cast -Wuseless-cast -Wdouble-promotion -Wshadow -Wformat=2 -std=c++20 -pedantic
CXX=g++
FILES = cwarnings

all: $(FILES)

cwarnings:

clean:
	-rm -rf $(FILES)

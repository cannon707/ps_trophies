CFLAGS ?= -std=c++98 -Wall -Wextra -Iinclude

ALL = bin/graph_trophy

all: $(ALL)

clean:
	rm -f obj/* bin/*

# --------------------
# Object files

obj/trophy.o: src/trophy.cpp include/trophy.hpp
	g++ $(CFLAGS) -c -o obj/trophy.o src/trophy.cpp

obj/graph_trophy.o: src/graph_trophy.cpp include/trophy.hpp
	g++ $(CFLAGS) -c -o obj/graph_trophy.o src/graph_trophy.cpp

# ---------------------
# Executables

bin/graph_trophy: obj/graph_trophy.o obj/trophy.o
	g++ $(CFLAGS) -o bin/graph_trophy obj/graph_trophy.o obj/trophy.o

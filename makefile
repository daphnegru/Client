CFLAGS:=-c -Wall -Weffc++ -g -std=c++11  -Iinclude -pthread
LDFLAGS:=-lboost_system -pthread


all: StompBookClubClient
	g++ -o bin/StompBookClubClient bin/user.o bin/connectionHandler.o bin/Book.o bin/keyboard.o bin/mainclient.o bin/readsocket.o $(LDFLAGS)

StompBookClubClient: bin/user.o bin/connectionHandler.o bin/Book.o bin/keyboard.o bin/mainclient.o bin/readsocket.o

bin/user.o: src/user.cpp
	g++ $(CFLAGS) -o bin/user.o src/user.cpp

bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/Book.o: src/Book.cpp
	g++ $(CFLAGS) -o bin/Book.o src/Book.cpp

bin/keyboard.o: src/keyboard.cpp
	g++ $(CFLAGS) -o bin/keyboard.o src/keyboard.cpp

bin/mainclient.o: src/mainclient.cpp
	g++ $(CFLAGS) -o bin/mainclient.o src/mainclient.cpp

bin/readsocket.o: src/readsocket.cpp
	g++ $(CFLAGS) -o bin/readsocket.o src/readsocket.cpp

.PHONY: clean
clean:
	rm -f bin/*

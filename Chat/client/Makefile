.PHONY:all
all:windows client client_window

client_window:chat_client.c windows.cc client_window.cc
	g++ $^ -o $@ -lpthread -ljsoncpp -lboost_filesystem -lboost_system -lncursesw -std=c++11

windows:windows.cc
	g++ $^ -o $@ -lpthread -lncursesw -std=c++11 -DWIN

client:chat_client.c
	g++ $^ -o $@ -lpthread -ljsoncpp -lboost_filesystem -lboost_system -DCHAT

.PHONY:clean
clean:
	rm -f client windows client_window

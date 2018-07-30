server:chat_server.c server_main.c
	g++ $^ -o $@ -std=c++11 -lpthread -lboost_filesystem -lboost_system -ljsoncpp

.PHONY:clean
clean:
	rm -f server

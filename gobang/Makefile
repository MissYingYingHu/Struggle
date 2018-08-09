ROOT=$(shell pwd)
INCLUDE=-I$(ROOT)/include/
LIB=-L$(ROOT)/mysql/
LIB_NAME=-lmysqlclient

.PHONY:all

all:server client 

# insert:db.c
# 	gcc -o $@ $^ $(LIB)  $(LIB_NAME) $(INCLUDE)

server:server.c  db.c 
	gcc -o $@ $^ -lpthread $(LIB)  $(LIB_NAME) $(INCLUDE)

client:client.c
	gcc -o $@ $^

.PHONY:clean

clean:
	rm -f server client select insert

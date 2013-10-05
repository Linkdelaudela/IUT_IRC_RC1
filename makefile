#--------------------------------------------------------------
# Makefile generated with cmak version 0.9.6 (4/2006).
# Date: 5/10/2012 8:13:5
#--------------------------------------------------------------

PREFIX  = /usr/local
CFLAGS  = -Wall -g
LDFLAGS = -lpthread -lboost_system

CC = g++
RM = rm -f
INSTALL_PROG = install -m 755 -s

EXE = IRC_server

OBJS = channel.o main.o tcp_connection.o tcp_message.o tcp_server.o user.o

ALL : $(EXE)

channel.o : channel.cpp
	$(CC) -c channel.cpp $(CFLAGS) -o channel.o

main.o : main.cpp
	$(CC) -c main.cpp $(CFLAGS) -o main.o

tcp_connection.o : tcp_connection.cpp
	$(CC) -c tcp_connection.cpp $(CFLAGS) -o tcp_connection.o

tcp_message.o : tcp_message.cpp
	$(CC) -c tcp_message.cpp $(CFLAGS) -o tcp_message.o

tcp_server.o : tcp_server.cpp
	$(CC) -c tcp_server.cpp $(CFLAGS) -o tcp_server.o

user.o : user.cpp
	$(CC) -c user.cpp $(CFLAGS) -o user.o

$(EXE) : $(OBJS)
	$(CC) $(OBJS) -g -o $(EXE) $(LDFLAGS)

install : $(EXE)
	$(INSTALL_PROG) $(EXE) $(PREFIX)/bin

uninstall :
	$(RM) $(PREFIX)/bin/$(EXE)

clean :
	$(RM) $(OBJS) $(EXE)

include Make.defines
//CC = gcc

//LIBS =  -lsocket\
//	/home/courses/cse533/Stevens/unpv13e_solaris2.10/libunp.a

//FLAGS =  -g -O2 
//CFLAGS = ${FLAGS} -I/home/courses/cse533/Stevens/unpv13e_solaris2.10/lib

all: server.o client.o app.o odr.o aodr.o get_hw_addrs.o
	${CC} -o server server.o app.o  ${LIBS}
	${CC} -o  client client.o app.o  ${LIBS} 
	${CC} -o odr    odr.o aodr.o get_hw_addrs.o ${LIBS} 
server.o: server.c
	${CC} ${CFLAGS} -c server.c

client.o: client.c
	${CC} ${CFLAGS} -c    client.c

app.o: app.c
	${CC} ${CFLAGS} -c app.c
odr.o: odr.c
	${CC} ${CFLAGS} -c odr.c

aodr.o: aodr.c
	${CC} ${CFLAGS} -c aodr.c

get_hw_addrs.o: get_hw_addrs.c
	${CC} ${CFLAGS} -c get_hw_addrs.c

clean:
	rm -f server server.o 
	rm -f client client.o
	rm -f app    app.o
	rm -f odr    odr.o
	rm -f aodr   aodr.o
	rm -f get_hw_addrs.o

CC=gcc
CCFLAGS=-g -o
LDFLAGS=-L /usr/lib/mysql -l mysqlclient
ODIR=../obj
EDIR=../exe
SRC=../src


all: $(ODIR)/msg_hub_srv.o $(EDIR)/msg_hub_srv


$(EDIR)/msg_hub_srv: $(ODIR)/msg_hub_srv.o 
	gcc $(CCFLAGS) $@ $^ $(LDFLAGS)
#	gcc -o ../exe/msg_hub_srv msg_hub_srv.o -l mysqlclient

$(ODIR)/msg_hub_srv.o: $(SRC)/msg_hub_srv.c
	$(CC) $(CCFLAGS) $@ -c $^

#	gcc -g -c msg_hub_srv.c

clean:
	ls -l $(ODIR)/*.o
	rm -v -f $(ODIR)/*.o
	ls -l $(EDIR)/*
	rm -v -f $(EDIR)/*

list:
	ls -l $(SRC)/*.c
	ls -l $(ODIR)/*.o
	ls -l $(EDIR)/*


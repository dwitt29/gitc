CC=gcc
CCFLAGS=-g -o
LDFLAGS=-L /usr/lib/mysql -l mysqlclient
ODIR=../obj
EDIR=../exe
SRC=../src

#ODIR=../../share/src
#all: df_mon_clt.o $(ODIR)/open_logfile.o $(ODIR)/close_logfile.o $(ODIR)/write_logfile.o $(ODIR)/now.o
#	gcc -o ../exe/df_mon_clt df_mon_clt.o $(ODIR)/open_logfile.o $(ODIR)/close_logfile.o $(ODIR)/write_logfile.o $(ODIR)/now.o

all: $(ODIR)/df_mon_clt.o $(EDIR)/df_mon_clt

$(EDIR)/df_mon_clt: $(ODIR)/df_mon_clt.o 
	gcc $(CCFLAGS) $@ $^ $(LDFLAGS)

#	gcc -o ../exe/df_mon_clt df_mon_clt.o
#all: df_mon_clt.o 
#	gcc -o ../exe/df_mon_clt df_mon_clt.o 

$(ODIR)/df_mon_clt.o: $(SRC)/df_mon_clt.c
	$(CC) $(CCFLAGS) $@ -c $^
#	gcc $(CCFLAGS) $@ $^ $(LDFLAGS)
#	gcc -g -c df_mon_clt.c

clean:
	ls -l $(ODIR)/*.o
	rm -v -f $(ODIR)/*.o
	ls -l $(EDIR)/*
	rm -v -f $(EDIR)/*

list:
	ls -l $(SRC)/*.c
	ls -l $(ODIR)/*.o
	ls -l $(EDIR)/*


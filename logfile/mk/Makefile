CC=gcc
CCFLAGS=-g -c -o
ODIR=../obj
SRC=../src

all: $(ODIR)/open_logfile.o $(ODIR)/close_logfile.o $(ODIR)/write_logfile.o $(ODIR)/now.o

$(ODIR)/open_logfile.o: $(SRC)/open_logfile.c
#	$(CC) $(CCFLAGS) -o $@ -c $^
	$(CC) $(CCFLAGS) $@ $^

$(ODIR)/close_logfile.o: $(SRC)/close_logfile.c
	$(CC) $(CCFLAGS) $@ $^ 

$(ODIR)/write_logfile.o: $(SRC)/write_logfile.c
	$(CC) $(CCFLAGS) $@ $^ 

$(ODIR)/now.o: $(SRC)/now.c
	$(CC) $(CCFLAGS) $@ $^ 

clean:
	ls -l $(ODIR)/*.o 
	rm -v -f $(ODIR)/*.o 

list:
	ls -l $(SRC)/*.c 
	ls -l $(ODIR)/*.o 

CC = clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
EXEC = huff
EXEC2 = dehuff
BRTEST = brtest
BWTEST = bwtest
NODETEST = nodetest
PQTEST = pqtest
HEADERS = bitreader.h bitwriter.h node.h pq.h

all: $(EXEC) $(EXEC2) $(BRTEST) $(BWTEST) $(NODETEST) $(PQTEST)

$(EXEC): $(EXEC).o bitreader.o bitwriter.o node.o pq.o
	$(CC) $^ $(CFLAGS) -o $@

$(EXEC2): $(EXEC2).o bitreader.o bitwriter.o node.o pq.o
	$(CC) $^ $(CFLAGS) -o $@

$(BRTEST): $(BRTEST).o bitreader.o
	$(CC) $^ $(CFLAGS) -o $@

$(BWTEST): $(BWTEST).o bitwriter.o
	$(CC) $^ $(CFLAGS) -o $@

$(NODETEST): $(NODETEST).o node.o
	$(CC) $^ $(CFLAGS) -o $@

$(PQTEST): $(PQTEST).o pq.o node.o
	$(CC) $^ $(CFLAGS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXEC) $(EXEC2) $(BRTEST) $(BWTEST) $(NODETEST) $(PQTEST) *.o

format:
	clang-format -i -style=file *.[ch]

.PHONY: all clean format

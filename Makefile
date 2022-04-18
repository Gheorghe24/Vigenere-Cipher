.phony: clean

CC = gcc -Wall

OUTPUT_DIR = outputs
EXEC = tema2
OFILES = tema2.o TreeMap.o Cipher.o

all: tema2

tema2: $(OFILES)
	$(CC) $(OFILES) -o $(EXEC)

$@.o: $@.c $@.h
	$(CC) -c $@.c

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(EXEC) $(OFILES) $(OUTPUT_DIR)/*.out $(OUTPUT_DIR)/*.dot


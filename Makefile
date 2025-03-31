CC = gcc
CFLAGS = -Wall -g

# Zdefiniuj pliki źródłowe
SRC = main.c output.c graph.c partition.c utils.c
OBJ = $(SRC:.c=.o)

# Plik wykonywalny
EXEC = program

# Reguła kompilacji
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Reguła kompilacji obiektów
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Reguła czyszczenia
clean:
	rm -f $(OBJ) $(EXEC)

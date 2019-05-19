CC=gcc
CFLAGS = -c -Wall -O3 -I.
LDFLAGS = -lm -lpthread

EXECUTABLE = ompchat
SOURCES = main.c client.c common.c server.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = client.h common.h server.h


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

$%.o: %.c
	$(CC) $(CFLAGS) $@

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
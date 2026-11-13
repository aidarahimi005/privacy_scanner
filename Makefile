CC = gcc

CFLAGS = -Wall -Wextra -std=c11

INCLUDES = -Iinclude

SOURCES = src/main.c src/scanner.c src/patterns.c src/report.c

TARGET = privacy_scanner

all:
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe sanitized.txt report.json
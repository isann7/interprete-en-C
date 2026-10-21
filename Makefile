TARGET = interprete

SRCS = $(wildcard *.c)

all:
	gcc $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

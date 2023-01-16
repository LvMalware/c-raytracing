OUTPUT=raycast
SOURCES=main.c src/*.c
INCLUDE=-Ilib
CFLAGS=-Wall -Ofast $(INCLUDE) -o $(OUTPUT) -lm

all:
	$(CC) $(SOURCES) $(CFLAGS)

clean:
	find . -iname '*~' -delete
	rm -rf $(OUTPUT)

run: clean all
	./$(OUTPUT)

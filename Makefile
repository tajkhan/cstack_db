all: build


build:
	gcc -o db src/db.c

clean:
	rm db src/*.o

run: build
	./db

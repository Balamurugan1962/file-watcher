run: compile
	./build/fw

compile: clean
	mkdir build
	clang src/main.c -o build/fw


clean:
	rm -rf build

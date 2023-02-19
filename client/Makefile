kbdtool: main.cc Makefile
	g++ -g -o $@ main.cc $$(pkg-config --libs --cflags hidapi-hidraw stb)

clean:
	rm -f kbdtool


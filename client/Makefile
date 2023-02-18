kbdtool: main.cc Makefile
	g++ -g -o $@ main.cc $$(pkg-config --libs --cflags hidapi-hidraw netpbm)

clean:
	rm -f kbdtool


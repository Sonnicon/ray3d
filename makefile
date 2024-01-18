SOURCES := $(wildcard src/*.c lib/*/*.c)
OBJECTS := $(addprefix objs/,$(patsubst %.c,%.o,$(SOURCES)))

build all: $(OBJECTS)
	gcc -o ray3d $^ `sdl2-config --cflags --libs` -lm

objs/%.o: %.c
	mkdir -p objs/$(dir $<)
	gcc -g -o objs/$(<:.c=.o) -c $< -Isrc `sdl2-config --cflags --libs`-lm

run: build
	./ray3d

clean:
	rm -f ./ray3d
	rm -rf ./objs

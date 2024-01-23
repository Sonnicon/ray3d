SOURCES := $(wildcard src/*.c lib/*/*.c)
HEADERS := $(wildcard src/*.h lib/*/*.h)
OBJECTS := $(addprefix objs/,$(patsubst %.c,%.o,$(SOURCES)))

build all: $(OBJECTS)
	gcc -o ray3d $^ `sdl2-config --cflags --libs` -lSDL2_image -lm

objs/%.o: %.c $(HEADERS)
	mkdir -p objs/$(dir $<)
	gcc -g -o objs/$(<:.c=.o) -c $< -Isrc `sdl2-config --cflags --libs` -lSDL2_image -lm

run: build
	./ray3d

gdb: build
	gdb ./ray3d

clean:
	rm -f ./ray3d
	rm -rf ./objs

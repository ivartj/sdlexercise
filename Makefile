PROGNAME=sdlapp
CFLAGS=`sdl-config --cflags` `pkg-config --cflags cairo`
LDFLAGS=`sdl-config --libs` `pkg-config --libs cairo`

SRCS=$(wildcard *.c)
OBJS=$(subst .c,.o,$(SRCS))

all: $(PROGNAME)

$(PROGNAME): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $?

clean:
	rm -f *.o $(PROGNAME)

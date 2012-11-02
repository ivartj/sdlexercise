PROGNAME=sdlapp
CFLAGS=`sdl-config --cflags` `pkg-config --cflags gl glu`
LDFLAGS=`sdl-config --libs` `pkg-config --libs gl glu`

SRCS=*.c
OBJS=$(SRCS:.c=.o)

all: $(PROGNAME)

$(PROGNAME): $(OBJS)
	$(CC) -o $(PROGNAME) $(LDFLAGS) $(OBJS)

$(OBJS): $(SRCS)
	$(CC) -c $(CFLAGS) $(SRCS)

clean:
	rm -f $(OBJS) $(PROGNAME)

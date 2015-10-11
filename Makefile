CFLAGS = -Wall -Wextra -Werror -fpic -ldl -D_GNU_SOURCE
LDFLAGS = -shared
all: libprefmpd.so

lib%.so: %.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $<

%.o: src/%.c
	${CC} ${CFLAGS} -o $@ -c $<

.PHONY: all clean

clean:
	-rm -f libprefmpd.so

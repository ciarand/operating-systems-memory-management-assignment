CXX_FLAGS=-Wall -Wextra -g
CXX=gcc
BIN=$(CXX) $(CXX_FLAGS)

uncrustify:
	uncrustify --replace --no-backup -c style.cfg *.c

clean:
	rm -f sender recv a.out core.*

.PHONY: clean uncrustify

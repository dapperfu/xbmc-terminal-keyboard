XTK_EXE = xbmc-terminal-keyboard
CFLAGS = -lncurses -lpthread -g

all : $(XTK_EXE)

update:
	install -m755 $(XTK_EXE) /usr/bin

install:
	install -m755 $(XTK_EXE) /usr/bin

uninstall:
	-rm -f /usr/bin/$(XTK_EXE)

$(XTK_EXE) : $(XTK_EXE).o
	$(CXX) $(CFLAGS) -o $@ $^

$(XTK_EXE).o : $(XTK_EXE).cpp
	$(CXX) -g -c $(XTK_EXE).cpp

clean :
	-rm $(XTK_EXE) $(XTK_EXE).o

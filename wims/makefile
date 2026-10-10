
CXX = g++
CXXFLAGS = -O2 -Wall -Wextra -std=c++17
wims: wims.o console_func.o
	$(CXX) $(CXXFLAGS) $^ -o  $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	rm -f wims *.o

PREFIX = /usr/local/bin

install: wims
	install -m 755 wims $(PREFIX)

uninstall:
	rm -f $(Prefix)/wims

# vim: set noexpandtab :

CXX      := g++
LD       := g++
CXXFLAGS := -W -Wall -Wextra -Weffc++ -Werror -pedantic -std=c++0x -O2 -g #-Waggregate-return

OZWDIR   := /usr/include/openzwave
PREFIX   := /usr/local
BIN      := zwave-sunrise

all: $(BIN)

$(BIN): sunrise.o config.o
	$(LD) sunrise.o config.o -o $(BIN) -lopenzwave -lpthread -ludev

clean:
	rm -f $(BIN) sunrise.o config.o

sunrise.o: sunrise.cpp config.h
	$(CXX) $(CXXFLAGS) -I /usr/include -I $(OZWDIR) -I $(OZWDIR)/value_classes -I $(OZWDIR)/platform -c sunrise.cpp -o sunrise.o

config.o: config.cpp config.h
	$(CXX) $(CXXFLAGS) -I /usr/include -I $(OZWDIR) -I $(OZWDIR)/value_classes -I $(OZWDIR)/platform -c config.cpp -o config.o

install:
	install $(BIN) $(PREFIX)/bin

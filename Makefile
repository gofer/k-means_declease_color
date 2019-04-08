TARGET=debug.out
SRCS=main.cc
OBJS=$(addsuffix .o, $(basename $(SRCS)))

CXXFLAGS=-O3 -Wall -I.
LDFLAGS=

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $^

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ main.cc

clean:
	rm -f *.o

distclean:
	make clean; rm -f $(TARGET)

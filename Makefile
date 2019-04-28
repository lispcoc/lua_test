CXXFLAGS=-I. -g -Wall -Werror -std=c++1z -I/usr/include/lua5.2
INCS=
OBJS=main.o lua.o
LIBS = -L/usr/lib/ -llua5.2 -lm -lstdc++
TARGET=test
CXX = g++

%.o: %.c $(INCS)
	$(CXX) $(CXXFLAGS) $(LIBS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf $(TARGET) *.o

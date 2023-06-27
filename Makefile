CXX = g++
CXXFLAGS = -std=c++11
INCLUDES = -Isrc/include -I/usr/local/include/boost_1_82_0
LIBS = -l pcap

SRCS = $(shell find src -name '*.cpp') 
TARGET = netreplay

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $^ $(LIBS)

clean:
	$(RM) $(TARGET)

test-libpcap: libtesting/libpcap_test.cpp
	$(CXX) $(CXXFLAGS) -o libpcap_test $^ $(LIBS)
	./libpcap_test

TARGET       = scld

SRCDIR       = src
OBJDIR       = obj
BINDIR       = bin

TARGET       := $(BINDIR)/$(TARGET)

CXX          = g++

CXXFLAGS     = -Wall -Wextra -std=c++20 -O3 -march=native -I./$(SRCDIR)

LDFLAGS      = -ltbb -lboost_system -lssl -lcrypto -lpthread
WINLDFLAGS   = -l:libtbb12.dll.a -l:libboost_filesystem-mt.dll.a -l:libssl.dll.a -l:libcrypto.dll.a -lws2_32 -lwsock32

CXX_SOURCES  = $(wildcard $(SRCDIR)/*.cpp)

CXX_OBJECTS  = $(CXX_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

.PHONY: all
all: remove $(TARGET) clean

$(TARGET): $(CXX_OBJECTS) $(BINDIR)
	$(CXX) $(CXX_OBJECTS) $(LDFLAGS) -o $@

windows: remove build_win clean

build_win: $(CXX_OBJECTS) $(BINDIR)
	$(CXX) $(CXX_OBJECTS) $(WINLDFLAGS) -o $(TARGET).exe

$(CXX_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@

$(BINDIR):
	mkdir -p $@

clean:
	$(RM) -rf $(CXX_OBJECTS) $(OBJDIR)

remove:
	$(RM) -rf $(CXX_OBJECTS) $(OBJDIR) $(TARGET).exe $(TARGET) $(BINDIR)

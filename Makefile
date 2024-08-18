TARGET       = scld

SRCDIR       = src
OBJDIR       = obj
BINDIR       = bin

CFLAGS       = -Wall -O2 -I./$(SRCDIR)
CXXFLAGS     = -Wall -O2 -fopenmp -I./$(SRCDIR)

LDFLAGS      = -lcurl -fopenmp

C_SOURCES    = $(wildcard $(SRCDIR)/*.c)
CXX_SOURCES  = $(wildcard $(SRCDIR)/*.cpp)

C_INCLUDES   = $(wildcard $(SRCDIR)/*.h)
CXX_INCLUDES = $(wildcard $(SRCDIR)/*.hpp)

C_OBJECTS    = $(C_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
CXX_OBJECTS  = $(CXX_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

.PHONY: all
all: remove $(BINDIR)/$(TARGET) clean

$(BINDIR)/$(TARGET): $(C_OBJECTS) $(CXX_OBJECTS) $(BINDIR)
	g++ $(CXX_OBJECTS) $(C_OBJECTS)  $(LDFLAGS) -o $@

windows: remove build_win clean

build_win: $(C_OBJECTS) $(CXX_OBJECTS) $(BINDIR)
	g++ $(CXX_OBJECTS) $(C_OBJECTS)  $(LDFLAGS) -o $(BINDIR)/$(TARGET).exe

$(CXX_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

$(C_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(OBJDIR)
	g++ $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@

$(BINDIR):
	mkdir -p $@

clean:
	rm -rf $(C_OBJECTS) $(CXX_OBJECTS) $(OBJDIR)
	
remove:
	rm -rf $(C_OBJECTS) $(CXX_OBJECTS) $(OBJDIR) $(BINDIR)

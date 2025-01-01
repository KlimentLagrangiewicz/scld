TARGET       = scld

SRCDIR       = src
OBJDIR       = obj
BINDIR       = bin

TARGET       := $(BINDIR)/$(TARGET)

CFLAGS       = -Wall -O2 -I./$(SRCDIR)
CXXFLAGS     = -Wall -O2 -fopenmp -I./$(SRCDIR)

LDFLAGS      = -lcurl -fopenmp

C_SOURCES    = $(wildcard $(SRCDIR)/*.c)
CXX_SOURCES  = $(wildcard $(SRCDIR)/*.cpp)

C_OBJECTS    = $(C_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
CXX_OBJECTS  = $(CXX_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

.PHONY: all
all: remove $(TARGET) clean

$(TARGET): $(C_OBJECTS) $(CXX_OBJECTS) $(BINDIR)
	g++ $(CXX_OBJECTS) $(C_OBJECTS) $(LDFLAGS) -o $@

windows: remove build_win clean

build_win: $(C_OBJECTS) $(CXX_OBJECTS) $(BINDIR)
	g++ $(CXX_OBJECTS) $(C_OBJECTS) $(LDFLAGS) -o $(TARGET).exe

$(CXX_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

$(C_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(OBJDIR)
	g++ $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@

$(BINDIR):
	mkdir -p $@

clean:
	$(RM) -rf $(C_OBJECTS) $(CXX_OBJECTS) $(OBJDIR)
	
remove:
	$(RM) -rf $(C_OBJECTS) $(CXX_OBJECTS) $(OBJDIR) $(TARGET).exe $(TARGET) $(BINDIR)

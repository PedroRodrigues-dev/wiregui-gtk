TARGETDIR=bin
TARGET=$(TARGETDIR)/wiregui-gtk
CXX=g++ -std=c++17
CCFLAGS=-Wall -g -MMD
LD=g++
GTKFLAGS=`pkg-config gtkmm-3.0 --cflags --libs`
SRCDIR=src
OBJDIR=obj

SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

$(TARGET): $(OBJS) | $(TARGETDIR)
	$(LD) -o $(TARGET) $(OBJS) $(GTKFLAGS)
	@rm -f $(OBJDIR)/*.o $(OBJDIR)/*.d

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) -c $(CCFLAGS) $(GTKFLAGS) $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGETDIR):
	mkdir -p $(TARGETDIR)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJDIR)/*.o $(OBJDIR)/*.d

.PHONY: all
all: $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

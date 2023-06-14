TARGETDIR=bin
TARGETFILE=wiregui-gtk
DISTFILE=wiregui-gtk.tar
INSTALLERFILE=installer.sh
TARGET=$(TARGETDIR)/${TARGETFILE}
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
	rm -rf $(TARGETDIR) $(OBJDIR) $(DISTFILE)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: dist
dist: $(TARGET)
	cp $(TARGET) .
	rm -f $(DISTFILE)
	tar -cvf $(DISTFILE) $(TARGETFILE) $(INSTALLERFILE)
	rm -f $(TARGETFILE)

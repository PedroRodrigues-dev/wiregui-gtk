VERSION=1_0_0
ARCH=amd64
TARGETFILE=wiregui-gtk

TARGETDIR=bin
SRCDIR=src
OBJDIR=obj
PACKAGERAWNAME=package

TARGET=$(TARGETDIR)/$(TARGETFILE)
PACKAGEDIR=$(PACKAGERAWNAME)/usr/bin
PACKAGENAME=$(TARGETFILE)_$(VERSION)_$(ARCH)

CXX=g++ -std=c++17
CCFLAGS=-Wall -g -MMD
LD=g++
GTKFLAGS=`pkg-config gtkmm-3.0 --cflags --libs`

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
	rm -rf $(TARGETDIR) $(OBJDIR) $(PACKAGEDIR) *.deb

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: deb
deb: $(TARGET)
	mkdir -p $(PACKAGEDIR)
	cp $(TARGET) $(PACKAGEDIR)/$(TARGETFILE)
	dpkg-deb --build $(PACKAGERAWNAME)
	mv $(PACKAGERAWNAME).deb $(PACKAGENAME).deb

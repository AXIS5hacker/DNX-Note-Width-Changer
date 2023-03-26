.SUFFIXES:
.SUFFIXES: .cpp .o

# General variables
PACKAGE	    = dynamix-chart-width-control
PREFIX	    ?= /usr/local
DISTDIR	    = .
BINDIR	    = $(PREFIX)/bin
LIBDIR	    = $(PREFIX)/lib/$(PACKAGE)
SHAREDIR    = $(PREFIX)/share/$(PACKAGE)
DOCDIR	    = $(PREFIX)/share/doc/$(PACKAGE)
MANDIR	    = $(PREFIX)/share/man/man1

INSTALL	    ?= $(shell which install)
CXX	    ?= $(shell which g++)

# Default target
all: build

chart_store.o: chart_store.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -c $(DISTDIR)/$< -o $@

width.o: width.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -c $(DISTDIR)/$< -o $@

main.o: main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -c $(DISTDIR)/$< -o $@

$(PACKAGE): width.o main.o chart_store.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

build: $(PACKAGE)

# Clean target
clean:
	rm -f *.o $(PACKAGE) *~

# Install generic target
install: $(PACKAGE)
	$(INSTALL) -d $(INSTALL_OPTIONS) \
		$(DESTDIR)$(BINDIR) \
		$(DESTDIR)$(DOCDIR)
	$(INSTALL) $(PACKAGE) $(DESTDIR)$(BINDIR)
	$(INSTALL) ../README.md $(DESTDIR)$(DOCDIR)

# Uninstall target
uninstall:
	rm -rf $(DESTDIR)$(BINDIR)/$(PACKAGE)
	rm -rf $(DESTDIR)$(DOCDIR)

# variables
CXX = g++
PROGNAME = classview
VERSION = 1.1.2
BINDIR = bin
SRCDIR = src

# compiler options
BASE_CFLAGS = -Wall -pipe `gtkmm-config --cflags` -D_REENTRANT
CFLAGS = $(BASE_CFLAGS) -Winline -DDEBUG -g -fno-default-inline
#CFLAGS = $(BASE_CFLAGS) -DNDEBUG -O3 -finline-functions
LD_FLAGS = `gtkmm-config --libs`

# profiling compiler options (comment out other CFLAGS vars)
#CFLAGS = -D_REENTRANT -DDEBUG -pg `gtkmm-config --cflags`


# targets
all: nobin mainsrc
	mkdir $(BINDIR)
	mv $(SRCDIR)/$(PROGNAME) $(BINDIR)

nobin:
	rm -rf $(BINDIR)

mainsrc:
	$(MAKE) -C $(SRCDIR) CXX="$(CXX)" PROGNAME="$(PROGNAME)" CFLAGS="$(CFLAGS)" LD_FLAGS="$(LD_FLAGS)"

fromdos:
	perl -pi -e 's/\r\n/\n/' AUTHORS BUGS COPYING ChangeLog FAQ INSTALL REQUIREMENTS TODO
	$(MAKE) -C $(SRCDIR) $@

todos:
	perl -pi -e 's/\n/\r\n/' AUTHORS BUGS COPYING ChangeLog FAQ INSTALL REQUIREMENTS TODO
	$(MAKE) -C $(SRCDIR) $@

strip: all
	strip -sv $(BINDIR)/$(PROGNAME)

clean: nobin
	$(MAKE) -C $(SRCDIR) PROGNAME="$(PROGNAME)" $@

install: strip
	@echo "not done"

package: clean
	cp -R ../$(PROGNAME)/ ../$(PROGNAME)-$(VERSION)/
	mv ../$(PROGNAME)-$(VERSION)/ ./
	tar -cvf $(PROGNAME)-$(VERSION).tar $(PROGNAME)-$(VERSION)/
	gzip $(PROGNAME)-$(VERSION).tar
	rm -rf $(PROGNAME)-$(VERSION)/

# variables
OBJDIR = obj
OBJECTS = $(OBJDIR)/main.o \
          $(OBJDIR)/Config.o \
          $(OBJDIR)/FileSystem.o \
          $(OBJDIR)/Parser.o \
          $(OBJDIR)/MainWindow.o \
          $(OBJDIR)/AboutDialog.o \
          $(OBJDIR)/PreferencesDialog.o \
          $(OBJDIR)/MessageBox.o \
          $(OBJDIR)/FileSelection.o \
          $(OBJDIR)/TreeItem.o
INCLUDE = -I. -I./linux
LIB = -L.


# targets
all: $(PROGNAME)

$(PROGNAME): $(OBJDIR) $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LIB) $(LD_FLAGS)

$(OBJDIR):
	mkdir $(OBJDIR)

fromdos:
	perl -pi -e 's/\r\n/\n/' *.cpp *.h linux/*.cpp linux/*.h

todos:
	perl -pi -e 's/\n/\r\n/' *.cpp *.h linux/*.cpp linux/*.h

strip: all
	strip -sv $(PROGNAME)

clean:
	-rm -rf $(OBJDIR) *.o $(PROGNAME) core a.out *.log tags


# dependancies
$(OBJDIR)/main.o: main.cpp main.h Parser.h MainWindow.h linux/MainWindow.h linux/TreeItem.h FileSystem.h Config.h linux/MessageBox.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) -c $<

$(OBJDIR)/Config.o: Config.cpp Config.h FileSystem.h shared.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) -c $<

$(OBJDIR)/FileSystem.o: FileSystem.cpp FileSystem.h Config.h linux/MessageBox.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) -c $<

$(OBJDIR)/Parser.o: Parser.cpp Parser.h Config.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) -c $<

$(OBJDIR)/MainWindow.o: linux/MainWindow.cpp linux/MainWindow.h linux/PreferencesDialog.h linux/AboutDialog.h linux/MessageBox.h linux/FileSelection.h linux/TreeItem.h FileSystem.h Config.h main.h shared.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) -c $<

$(OBJDIR)/AboutDialog.o: linux/AboutDialog.cpp linux/AboutDialog.h shared.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) -c $<

$(OBJDIR)/PreferencesDialog.o: linux/PreferencesDialog.cpp linux/PreferencesDialog.h Config.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) -c $<

$(OBJDIR)/MessageBox.o: linux/MessageBox.cpp linux/MessageBox.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) -c $<

$(OBJDIR)/FileSelection.o: linux/FileSelection.cpp linux/FileSelection.h Config.h FileSystem.h main.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) -c $<

$(OBJDIR)/TreeItem.o: linux/TreeItem.cpp linux/TreeItem.h FileSystem.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) -c $<

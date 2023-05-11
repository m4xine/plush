CXX		:=clang++
CLANGFORMAT	:=clang-format
CXXFLAGS	:=-std=c++17 -Wall -fno-exceptions 
LDFLAGS		:=
BINARY		:=plush
SOURCEDIR	:=plush
INCLUDEDIR	:=plush
BUILDDIR	:=build
HEADERS		:=$(shell find $(SOURCEDIR) -name '*.h')
SOURCES		:=$(shell find $(SOURCEDIR) -name '*.cpp')
OBJECTS		:=$(patsubst $(SOURCEDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))

debug:		CXXFLAGS+=-DDEBUG -g
debug:		build
release:	CXXFLAGS+=-O2
release:	build

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -I$(INCLUDEDIR) -c $< -o $@

$(BUILDDIR)/$(BINARY): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) -o $(BUILDDIR)/$(BINARY)

build: $(BUILDDIR)/$(BINARY)

format:
	$(CLANGFORMAT) -i -style=file $(HEADERS) $(SOURCES)

clean:
	@rm -r $(BUILDDIR)

install: release
	@cp $(BUILDDIR)/$(BINARY) $(DESTDIR)/usr/local/bin/$(BINARY)

.PHONY: format debug release install clean

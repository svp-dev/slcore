dist_pkginclude_DATA = \
	tinyxml/tinyxml.h

SOURCES = $(INCLUDES) \
	tinyxml/tinyxml.cpp \
	tinyxml/tinyxmlerror.cpp \
	tinyxml/tinyxmlparser.cpp

lib_LIBRARIES = libtinyxml.a libtinyxml_g.a

libtinyxml_a_SOURCES = $(SOURCES)
libtinyxml_g_a_SOURCES = $(SOURCES)
libtinyxml_g_a_CXXFLAGS = -g -DDEBUG


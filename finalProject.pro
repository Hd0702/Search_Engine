TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += /usr/include/boost
LIBS += -L/usr/include/boost -lboost_serialization -lboost_system -lboost_filesystem
QMAKE_CXXFLAGS += -lboost_serialization
QMAKE_CXXFLAGS += -lboost_filesystem

SOURCES += main.cpp \
    avltests.cpp \
    reader.cpp \
    porter2_stemmer.cpp \
    avlindex.cpp \
    queryprocessor.cpp \
    hashindex.cpp \
    userinterface.cpp \
    hashtest.cpp

HEADERS += \
    catch.hpp \
    dstree.h \
    reader.h \
    porter2_stemmer.h \
    hashmap.h \
    indexinterface.h \
    avlindex.h \
    queryprocessor.h \
    hashindex.h \
    userinterface.h

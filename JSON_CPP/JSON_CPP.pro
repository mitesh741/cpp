TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
INCLUDEPATH += /home/mitesh/Downloads/jsoncpp-master/include/


LIBS +=   -L/home/mitesh/Downloads/jsoncpp-master/build/debug/src/lib_json -ljsoncpp

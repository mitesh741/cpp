TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
Bitmap.cpp \
Mandelbrot.cpp \
ZoomList.cpp

HEADERS += Bitmap.h \
BitmapFileHeader.h \
BitmapInfoHeader.h \
Mandelbrot.h \
Zoom.h \
ZoomList.h

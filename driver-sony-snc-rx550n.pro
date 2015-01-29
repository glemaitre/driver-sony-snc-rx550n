QT       += core network
QT       -= gui

TARGET = untitled
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += ./src/sonysncrx550n.cpp \
           ./src/main.cpp 

HEADERS += ./src/sonysncrx550n.h
             
# Support c++11
QMAKE_CXXFLAGS += -std=c++11

# Configuration via pkg-config
#CONFIG += link_pkgconfig

# Add the library needed
#PKGCONFIG += opencv eigen3

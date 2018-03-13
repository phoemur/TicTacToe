TEMPLATE = app
TARGET = tictactoe
INCLUDEPATH += .

DEFINES += QT_DEPRECATED_WARNINGS

# Input
HEADERS += MainWindow.h
FORMS += MainWindow.ui
SOURCES += MainWindow.cpp tictactoe.cpp
RESOURCES += icons/icons.qrc

# Custom config
QT += widgets
QMAKE_CXXFLAGS += -std=c++14
CONFIG += release
#CONFIG += debug
QMAKE_POST_LINK=$(STRIP) $(TARGET)
target.path = /usr/local/bin/
INSTALLS += target

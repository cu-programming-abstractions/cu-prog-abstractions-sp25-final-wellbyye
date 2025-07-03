TEMPLATE = app
QT -= gui
CONFIG += console c++17 silent
CONFIG -= app_bundle
SOURCES += src/main.cpp \
           src/generator.cpp \
           src/solver.cpp
HEADERS += src/cell.h \
           src/generator.h \
           src/solver.h

OTHER_FILES += \
    README.md \
    BITMASK_BFS_GUIDE.md
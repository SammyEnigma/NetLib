CONFIG -= qt

TEMPLATE = dll
TARGET = CoreLib

INCLUDEPATH += ./headers

HEADERS += \
./headers/Base64.h \
./headers/Character.h \
./headers/Node.h \
./headers/List.h \
./headers/StringEx.h

SOURCES += \
./sources/Base64.c \
./sources/Character.c \
./sources/List.c \
./sources/StringEx.c

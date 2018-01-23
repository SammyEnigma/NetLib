CONFIG -= QT

TEMPLATE = dll
TARGET = CoreLib

win32
{
    DEFINES += QT_DEPRECATED_WARNINGS
    DEFINES += _CRT_SECURE_NO_WARNINGS
    LIBS += -lws2_32
}

HEADERS += \
    headers/ABNFMessage.h \
    headers/Array.h \
    headers/Base64.h \
    headers/Base64C.h \
    headers/BinaryTree.h \
    headers/Buffer.h \
    headers/Character.h \
    headers/CharacterC.h \
    headers/Configuration.h \
    headers/CoreLib.h \
    headers/DataConnection.h \
    headers/DateTime.h \
    headers/Directory.h \
    headers/EncryptionTypes.h \
    headers/File.h \
    headers/GenericString.h \
    headers/KeyValue.h \
    headers/List.h \
    headers/ListC.h \
    headers/Logger.h \
    headers/Mail.h \
    headers/Map.h \
    headers/Mime.h \
    headers/Mutex.h \
    headers/Network.h \
    headers/NetworkDefs.h \
    headers/Node.h \
    headers/ProcessLock.h \
    headers/Queue.h \
    headers/SignalHandler.h \
    headers/Stack.h \
    headers/StringExC.h \
    headers/TcpClient.h \
    headers/TcpServer.h \
    headers/TreeNode.h \
    headers/Variant.h

SOURCES += \
    sources/ABNFMessage.cpp \
    sources/Base64.cpp \
    sources/Buffer.cpp \
    sources/Character.cpp \
    sources/Configuration.cpp \
    sources/DataConnection.cpp \
    sources/DateTime.cpp \
    sources/Directory.cpp \
    sources/dllmain.cpp \
    sources/File.cpp \
    sources/GenericString.cpp \
    sources/Logger.cpp \
    sources/Mail.cpp \
    sources/Mutex.cpp \
    sources/Network.cpp \
    sources/ProcessLock.cpp \
    sources/SignalHandler.cpp \
    sources/TcpClient.cpp \
    sources/TcpServer.cpp \
    sources/Variant.cpp \
    sources/Base64C.c \
    sources/CharacterC.c \
    sources/CoreLib.c \
    sources/ListC.c \
    sources/StringExC.c

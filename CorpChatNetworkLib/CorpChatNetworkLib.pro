QT += gui
QT += network
TEMPLATE = lib
DEFINES += CORPCHATNETWORKLIB_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += $$PWD/include

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
SOURCES += \
    src/connection.cpp \
    src/package.cpp \
    src/packagejsonserializer.cpp

HEADERS += \
    include/CorpChatNetworkLib_global.h \
    include/DocumentSerializer.h \
    include/IConnection.h \
    include/IPackageSerializer.h \
    include/connection.h \
    include/imageserializer.h \
    include/package.h \
    include/packagejsonserializer.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

QT += quick
QT += network
CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        src/client.cpp \
	src/contactchoosemodel.cpp \
        src/contactslist.cpp \
        src/contactsmodel.cpp \
        src/messageslist.cpp \
        src/messagesmodel.cpp \
		src/userdata.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    CorpChatClient_en_150.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += $$PWD/qml
QML2_IMPORT_PATH += $$PWD/qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CorpChatNetworkLib/release/ -lCorpChatNetworkLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CorpChatNetworkLib/debug/ -lCorpChatNetworkLib
else:unix: LIBS += -L$$OUT_PWD/../CorpChatNetworkLib/ -lCorpChatNetworkLib

INCLUDEPATH += $$PWD/../CorpChatNetworkLib
DEPENDPATH += $$PWD/../CorpChatNetworkLib

HEADERS += \
    include/client.h \
    include/contactchoosemodel.h \
    include/contactslist.h \
    include/contactsmodel.h \
    include/globals.h \
    include/messageslist.h \
    include/messagesmodel.h \
    include/userdata.h

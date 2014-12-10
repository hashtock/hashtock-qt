TEMPLATE = app

QT += qml quick widgets webkitwidgets network sql

SOURCES += main.cpp \
    qmlnetfactory.cpp \
    webkitcookiejar.cpp \
    user.cpp \
    statemaster.cpp \
    tag.cpp \
    hashtockapi.cpp \
    orderapi.cpp \
    order.cpp \
    messenger.cpp \
    qmlmessenger.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    qmlnetfactory.h \
    webkitcookiejar.h \
    user.h \
    statemaster.h \
    tag.h \
    hashtockapi.h \
    orderapi.h \
    order.h \
    messenger.h \
    qmlmessenger.h

OTHER_FILES += \
    components/qmldir

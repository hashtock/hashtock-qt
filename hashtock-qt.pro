TEMPLATE = app

QT += qml quick widgets network sql

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
    qmlmessenger.cpp \
    androidcookiejar.cpp \
    gaenetworkaccessmanager.cpp

RESOURCES += qml.qrc \
    android_res.qrc


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
    qmlmessenger.h \
    androidcookiejar.h \
    gaenetworkaccessmanager.h

OTHER_FILES += \
    components/qmldir

DISTFILES += \
    android/AndroidManifest.xml \
    android/src/org/hashtock/auth/Auth.java
#    android/src/org/hashtock/auth/Auth.java \
#    android/src/org/hashtock/auth/AppInfo.java \

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

# Additional import path used to resolve QML modules in Qt Creator's code model
android {
    message("* Using settings for Android.")
    QT += androidextras
    QML_IMPORT_PATH = platform_qml/android/
    QML2_IMPORT_PATH = platform_qml/android/
    OTHER_FILES += platform_qml/android/platform/qmldir
}

linux:!android {
    message("* Using settings for Unix/Linux.")
    OTHER_FILES += platform_qml/desktop/platform/qmldir
    QML_IMPORT_PATH = platform_qml/desktop/
    QML2_IMPORT_PATH = platform_qml/desktop/
}


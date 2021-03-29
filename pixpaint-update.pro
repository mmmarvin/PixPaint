TARGET = Update
TEMPLATE = app
#CONFIG += console
CONFIG -= app_bundle
CONFIG += no_keywords
CONFIG += c++17
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

HEADERS += \
    src/version.h \
    src/http/sslrequest.h \
    src/http/updater.h \
    src/3rdparty/jsonxx/jsonxx.h \
    updater/updaterwindow.h \
    updater/updater_version.h \
    src/http/http_helper.h \
    src/version_specific_headers.h \
    src/os_specific_functions.h

SOURCES += \
    src/version.cpp \
    src/http/sslrequest.cpp \
    src/http/sslrequest.inl \
    src/http/updater.cpp \
    src/3rdparty/jsonxx/jsonxx.cc \
    updater/updaterwindow.cpp \
    updater/pixpaint_updater.cpp \
    updater/updater_version.cpp \
    src/http/http_helper.cpp \
    src/version_specific_headers.cpp \
    src/os_specific_functions.cpp

LIBS += \
  -lssl -lcrypto


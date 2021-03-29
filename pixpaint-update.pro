TARGET = Update
TEMPLATE = app
#CONFIG += console
CONFIG -= app_bundle
CONFIG += no_keywords
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# CONFIGS
#CONFIG += CXX_14                    # Use c++14, else use c++17

CONFIG(debug, debug|release) {
}
CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

CXX_14 {
  CONFIG += c++14
  LIBS += -lboost_filesystem
} else {
  CONFIG += c++17
}

HEADERS += \
    updater/updater_version.h \
    updater/updaterwindow.h \
    src/3rdparty/jsonxx/jsonxx.h \
    src/http/http_helper.h \
    src/http/sslrequest.h \
    src/http/updater.h \
    src/debug_log.h \
    src/os_specific_functions.h \
    src/version.h \
    src/version_specific_headers.h

SOURCES += \
    updater/pixpaint_updater.cpp \
    updater/updater_version.cpp \
    updater/updaterwindow.cpp \
    src/http/http_helper.cpp \
    src/http/sslrequest.cpp \
    src/http/sslrequest.inl \
    src/http/updater.cpp \
    src/3rdparty/jsonxx/jsonxx.cc \
    src/debug_log.cpp \
    src/debug_log.inl \
    src/os_specific_functions.cpp \
    src/version.cpp \
    src/version_specific_headers.cpp

QMAKE_CXXFLAGS += \
    -Wpedantic -Werror=return-type -Werror=uninitialized -Werror=strict-overflow=5 #-Wswitch-default

LIBS += \
    -lssl -lcrypto


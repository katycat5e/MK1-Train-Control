INCLUDEPATH += ../..

QT += testlib
QT -= gui

CONFIG += qt

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    tst_blocklinks.cpp \
    ../../TrackModel/tracklayout.cpp


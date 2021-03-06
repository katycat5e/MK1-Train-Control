INCLUDEPATH += ../..

QT += testlib
QT += gui
QT += core gui serialport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    tst_testspeedregulator.cpp \
    ../mocs/TrainMoc.cpp \
    ../mocs/TrainControllerMoc.cpp \
    ../mocs/SpeedRegulatorMoc.cpp \
    ../mocs/CTCModeMoc.cpp \
    ../mocs/BeaconDecoderMoc.cpp \
    ../../TrackModel/trackmodel_types.cpp

HEADERS += \
    ../mocs/TrainMoc.h \
    ../mocs/TrainControllerMoc.h \
    ../mocs/SpeedRegulatorMoc.h \
    ../mocs/BeaconDecoderMoc.h \
    ../mocs/CTCModeMoc.h \
    ../../TrackModel/trackmodel_types.h

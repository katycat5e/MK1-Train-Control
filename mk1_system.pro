QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 file_copies

COPIES += layoutFiles

layoutFiles.files = $$files(*.csv)
layoutFiles.path = $$OUT_PWD

RC_ICONS = mk1_icon.ico
ICON = mk1_icon.icns

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CTCOffice/ctcoffice/CTCBlockDisplay.cpp \
    CTCOffice/ctcoffice/CTCBlockStatus.cpp \
    CTCOffice/ctcoffice/CTCDispatch.cpp \
    CTCOffice/ctcoffice/CTCHashmap.cpp \
    CTCOffice/ctcoffice/CTCMode.cpp \
    CTCOffice/ctcoffice/Files.cpp \
    CTCOffice/ctcoffice/ctc_main.cpp \
    CTCOffice/ctcoffice/maintenance.cpp \
    CTCOffice/ctcoffice/mainwindow.cpp \
    CTCOffice/ctcoffice/schedule.cpp \
    CTCOffice/ctcoffice/throughput.cpp \
    HWTrackController/HWTrackController.cpp \
    HWTrackController/HardwarePLC.cpp \
    HWTrackController/hwplcui.cpp \
    HWTrackController/initHWTrack.cpp \
    HWTrackController/Region.cpp \
    HWTrainController/SpeedRegulator.cpp \
    HWTrainController/TrainController.cpp \
    HWTrainController/BeaconDecoder.cpp \
    SWTrackController/BlockCntrl.cpp \
    SWTrackController/CTCSignals.cpp \
    SWTrackController/TrackController.cpp \
    SWTrackController/swtrackcntrlwindow.cpp \
    SWTrackController/trackcntrl_main.cpp \
    SWTrackController/tracklogic.cpp \
    SWTrackController/waysidecontainer.cpp \
    SWTrainController/SWTC.cpp \
    SWTrainController/SWTrainController_main.cpp \
    SWTrainController/TrainCGUI.cpp \
    SerialPort.cpp \
    TrackModel/layoutdialog.cpp \
    TrackModel/routemapview.cpp \
    TrackModel/routingtestdialog.cpp \
    TrackModel/ticketsdialog.cpp \
    TrackModel/tracklayout.cpp \
    TrackModel/trackmodel_main.cpp \
    TrackModel/trackmodel_status.cpp \
    TrackModel/trackmodel_types.cpp \
    TrackModel/trackmodeldisplay.cpp \
    TrackModel/trackrouter.cpp \
    TrainModel/Train.cpp \
    TrainModel/TrainModelControls.cpp \
    TrainModel/TrainModelMath.cpp \
    TrainModel/TrainModelUI.cpp \
    TrainModel/TrainModelUpdateBlock.cpp \
    binaryindicator.cpp \
    serialportdialog.cpp \
    system_main.cpp \
    systemsettingsdialog.cpp \
    ticketsystem.cpp \
    timetracker.cpp \
    weatherstation.cpp

HEADERS += \
    CTCOffice/ctcoffice/CTCBlockDisplay.h \
    CTCOffice/ctcoffice/CTCBlockStatus.h \
    CTCOffice/ctcoffice/CTCDispatch.h \
    CTCOffice/ctcoffice/CTCHashmap.h \
    CTCOffice/ctcoffice/CTCMode.h \
    CTCOffice/ctcoffice/Files.h \
    CTCOffice/ctcoffice/ctc_main.h \
    CTCOffice/ctcoffice/maintenance.h \
    CTCOffice/ctcoffice/mainwindow.h \
    CTCOffice/ctcoffice/schedule.h \
    CTCOffice/ctcoffice/throughput.h \
    HWTrackController/HWTrackController.hpp \
    HWTrackController/HWTrackController_main.h \
    HWTrackController/HardwarePLC.h \
    HWTrackController/Region.hpp \
    HWTrackController/hwplcui.h \
    HWTrainController/HWTrainController_main.h \
    HWTrainController/SpeedRegulator.h \
    HWTrainController/TrainController.h \
    HWTrainController/BeaconDecoder.h \
    SWTrackController/BlockCntrl.h \
    SWTrackController/CTCSignals.h \
    SWTrackController/TrackController.h \
    SWTrackController/swtrackcntrlwindow.h \
    SWTrackController/trackcntrl_main.h \
    SWTrackController/tracklogic.h \
    SWTrackController/waysidecontainer.h \
    SWTrainController/SWTC.h \
    SWTrainController/SWTrainController_main.h \
    SWTrainController/TrainCGUI.h \
    SerialPort.hpp \
    TrackModel/layoutdialog.h \
    TrackModel/routemapview.h \
    TrackModel/routingtestdialog.h \
    TrackModel/station_tracking.h \
    TrackModel/ticketsdialog.h \
    TrackModel/tracklayout.hpp \
    TrackModel/trackmodel_controller.hpp \
    TrackModel/trackmodel_main.hpp \
    TrackModel/trackmodel_status.h \
    TrackModel/trackmodel_train.hpp \
    TrackModel/trackmodel_types.hpp \
    TrackModel/trackmodeldisplay.h \
    TrackModel/trackrouter.h \
    TrainModel/Train.h \
    TrainModel/TrainModelControls.h \
    TrainModel/TrainModelMath.h \
    TrainModel/TrainModelUI.h \
    TrainModel/TrainModelUpdateBlock.h \
    binaryindicator.h \
    hw_sw_interfaces.h \
    serialportdialog.h \
    mk1_util.hpp \
    system_main.h \
    systemsettingsdialog.h \
    ticketsystem.h \
    timetracker.h \
    weatherstation.h

FORMS += \
    CTCOffice/ctcoffice/Files.ui \
    CTCOffice/ctcoffice/maintenance.ui \
    CTCOffice/ctcoffice/mainwindow.ui \
    CTCOffice/ctcoffice/throughput.ui \
    SWTrackController/swtrackcntrlwindow.ui \
    CTCOffice/ctcoffice/schedule.ui \
    HWTrackController/hwplcui.ui \
    SWTrainController/SWTrainController.ui \
    TrackModel/blockgeodialog.ui \
    TrackModel/layoutdialog.ui \
    TrackModel/routingtestdialog.ui \
    TrackModel/ticketsdialog.ui \
    TrainModel/TrainModelUI.ui \
    serialportdialog.ui \
    TrackModel/trackmodeldisplay.ui \
    systemsettingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    blue_line.csv \
    purple_line.csv \
    red_line.csv \
    green_line.csv

QT       += core gui serialport datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
    QT += charts

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SerialPort.cpp \
    define_parameter.cpp \
    gcode_decoder.cpp \
    main.cpp \
    mainwindow.cpp \
    packet_handler.cpp \
    qcustomplot.cpp \
    receivethread.cpp \
    temperature_control.cpp \
    temperature_controlpanel.cpp

HEADERS += \
    SerialPort.h \
    define_parameter.h \
    gcode_decoder.h \
    mainwindow.h \
    packet_handler.h \
    qcustomplot.h \
    receivethread.h \
    temperature_control.h \
    temperature_controlpanel.h

FORMS += \
    mainwindow.ui \
    temperature_controlpanel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    ../../DeCuongLV/week10/GUI/Temperature_Control2/GUI/GUI.pro



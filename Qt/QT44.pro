QT       += core gui serialport #add frameworks

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = openSerial
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    konfiguration.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    konfiguration.h \
    mainwindow.h

FORMS += \
    konfiguration.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

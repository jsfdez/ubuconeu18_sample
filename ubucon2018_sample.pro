TEMPLATE = app

QT += \
    core \
    quick \
    network

CONFIG += c++14

TARGET = ubucon2018_sample

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    model.cpp \
    listmodel.cpp

HEADERS += \
    model.h \
    listmodel.h

RESOURCES += res.qrc

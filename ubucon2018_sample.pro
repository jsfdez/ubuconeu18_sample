TEMPLATE = app

QT += \
    core \
    widgets \
    network

CONFIG += c++14

TARGET = ubucon2018_sample

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    model.cpp

HEADERS += \
    model.h

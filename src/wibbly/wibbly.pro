QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wibbly
CONFIG += no_batch
CONFIG += c++11
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += VS_TARGET_CPU_X86

include(../shared/shared.pri)
INCLUDEPATH += ../shared

#include(../../vapoursynth/vapoursynth.pri)
INCLUDEPATH += ../../vapoursynth/include
LIBS += -L"../../vapoursynth/msvc_project/x64/Release" -lVSScript

INCLUDEPATH += ../../
INCLUDEPATH += ../../python/include

SOURCES += \
	Wibbly.cpp \
	WibblyJob.cpp \
	WibblyWindow.cpp \
	
HEADERS += \
	WibblyJob.h \ 
	WibblyWindow.h

RESOURCES += \
    ../shared/res/resources.qrc
	

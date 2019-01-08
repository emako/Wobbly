QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wobbly
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
	FrameLabel.cpp \
	ImportWindow.cpp \
	OverlayLabel.cpp \
	PresetTextEdit.cpp \
	SpinBox.cpp \
	TableWidget.cpp \
	Wobbly.cpp \
	Wobbly.cpp \
	WobblyWindow.cpp \
	
HEADERS += \
	FrameLabel.h \ 
	ImportWindow.h \ 
	OverlayLabel.h \ 
	PresetTextEdit.h \ 
	SpinBox.h \ 
	TableWidget.h \ 
	WobblyWindow.h \ 

RESOURCES += \
    ../shared/res/resources.qrc
	

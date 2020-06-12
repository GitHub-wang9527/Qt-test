QT       += core gui  widgets

CONFIG += c++11
TARGET = TTT
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS


# code file
SOURCES += \
    main.cpp

HEADERS += \
    CheckboxCombo.h \
    DesktopImage.h \
    EventFWidget.h \
    GridScrollArea.h \
    PixLabel.h \
    PropertyButton.h \
    TTS.h \
    Watch.h

FORMS +=

RESOURCES += \
    TTT.qrc



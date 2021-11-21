QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


VPATH += ./src  ./include
SOURCES += \
    about.cpp \
    codeeditor.cpp \
    highlighter.cpp \
    preferences.cpp \
    main.cpp \
    mainwindow.cpp \
    textbox.cpp \

HEADERS += \
    about.h \
    codeeditor.h \
    highlighter.h \
    mainwindow.h \
    preferences.h \
    textbox.h \

FORMS += \
    about.ui \
    mainwindow.ui \
    preferences.ui \
    textbox.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

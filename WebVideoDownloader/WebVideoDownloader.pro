QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++0x

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ExeProcess.cpp \
    MfgInfo.cpp \
    QUtilityBox.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    ExeProcess.h \
    MainWindow.h \
    MfgInfo.h \
    QUtilityBox.h

FORMS += \
    MainWindow.ui \
    MfgInfo.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_FILE = icon.rc

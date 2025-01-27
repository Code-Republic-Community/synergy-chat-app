QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chat_widget.cpp \
    login.cpp \
    main.cpp \
    main_page_window.cpp \
    mainwindow.cpp \
    my_profile.cpp \
    navigationPrevOrNext.cpp \
    registration.cpp \
    scroll_widget.cpp \
    settings.cpp \
    v_chat_widget.cpp \
    welcomepg.cpp

HEADERS += \
    chat_widget.h \
    login.h \
    main_page_window.h \
    mainwindow.h \
    my_profile.h \
    navigationPrevOrNext.h \
    registration.h \
    scroll_widget.h \
    settings.h \
    v_chat_widget.h \
    welcomepg.h

TRANSLATIONS += \
    DeltaSynergy_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    logos.qrc

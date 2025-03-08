QT += core gui network websockets multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = Synergy

RC_ICONS = $$PWD/resources/icon.ico

INCLUDEPATH += header

SOURCES += \
    source/chat_widget.cpp \
    source/chatmessagewidget.cpp \
    source/globals.cpp \
    source/httpclient.cpp \
    source/loadingoverlay.cpp \
    source/loadingwidget.cpp \
    source/login.cpp \
    source/main.cpp \
    source/main_page_window.cpp \
    source/mainwindow.cpp \
    source/my_profile.cpp \
    source/navigationPrevOrNext.cpp \
    source/otherprofile.cpp \
    source/registration.cpp \
    source/scroll_widget.cpp \
    source/settings.cpp \
    source/translator.cpp \
    source/v_chat_widget.cpp \
    source/verification.cpp \
    source/websocketclient.cpp \
    source/welcomepg.cpp

HEADERS += \
    header/chat_widget.h \
    header/chatmessagewidget.h \
    header/globals.h \
    header/httpclient.h \
    header/loadingoverlay.h \
    header/loadingwidget.h \
    header/login.h \
    header/main_page_window.h \
    header/mainwindow.h \
    header/my_profile.h \
    header/navigationPrevOrNext.h \
    header/otherprofile.h \
    header/registration.h \
    header/scroll_widget.h \
    header/settings.h \
    header/translator.h \
    header/v_chat_widget.h \
    header/verification.h \
    header/websocketclient.h \
    header/welcomepg.h

TRANSLATIONS += \
    translations/Translator_en_US.ts \
    translations/Translator_hy_AM.ts \
    translations/Translator_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/logos.qrc \
    resources/ringtones.qrc \
    resources/styles.qrc

DISTFILES +=

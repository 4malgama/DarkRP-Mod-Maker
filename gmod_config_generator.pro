QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = build
RC_ICONS = resources/icon.ico

SOURCES += \
    _main.cpp \
    adminmodel.cpp \
    control.cpp \
    logs/logs.cpp \
    main.cpp \
    client.cpp \
    modules/player_models.cpp \
    player_model_viewer/playermodelviewer.cpp \
    settings/settings.cpp \
    viewer/viewer.cpp

HEADERS += \
    adminmodel.h \
    classes.h \
    client.h \
    help_functions.h \
    logs/logs.h \
    player_model_viewer/playermodelviewer.h \
    settings/settings.h \
    types.h \
    viewer/viewer.h

FORMS += \
    client.ui \
    logs/logs.ui \
    player_model_viewer/playermodelviewer.ui \
    settings/settings.ui \
    viewer/viewer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc

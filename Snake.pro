QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aisnake.cpp \
    controlcenter.cpp \
    food.cpp \
    ggpage.cpp \
    main.cpp \
    modechoosepage.cpp \
    pausepage.cpp \
    snake.cpp \
    subwidget.cpp \
    widget.cpp \
    winnerpage.cpp

HEADERS += \
    aisnake.h \
    constants.h \
    controlcenter.h \
    food.h \
    ggpage.h \
    modechoosepage.h \
    pausepage.h \
    snake.h \
    subwidget.h \
    widget.h \
    winnerpage.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

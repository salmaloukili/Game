QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20
QMAKE_CXXFLAGS += -fconcepts-diagnostics-depth=200
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller/gamecontroller.cpp \
    main.cpp \
    model/behaviors/attack.cpp \
    model/behaviors/behavior.cpp \
    model/behaviors/concrete/attack/counterattackbehavior.cpp \
    model/behaviors/concrete/attack/genericattackbehavior.cpp \
    model/behaviors/concrete/health/generichealingbehavior.cpp \
    model/behaviors/concrete/health/generichealthbehavior.cpp \
    model/behaviors/concrete/movement/genericmovebehavior.cpp \
    model/behaviors/concrete/movement/genericwalkablebehavior.cpp \
    model/behaviors/concrete/movement/healonstepbehavior.cpp \
    model/behaviors/concrete/movement/newlevelonstep.cpp \
    model/behaviors/concrete/movement/poisononstepbehavior.cpp \
    model/behaviors/concrete/movement/randommovementbehavior.cpp \
    model/behaviors/concrete/poison/genericpoisonablebehavior.cpp \
    model/behaviors/concrete/health/poisononkilledbehavior.cpp \
    model/behaviors/concrete/poison/genericpoisoningbehavior.cpp \
    model/behaviors/health.cpp \
    model/behaviors/movement.cpp \
    model/behaviors/poison.cpp \
    model/gameobject.cpp \
    model/gameobjectmodel.cpp \
    model/modelfactory.cpp \
    model/noise/perlinnoise.cpp \
    view/gamepixmapitem.cpp \
    view/gameview.cpp \
    view/gamewindow.cpp \
    view/renderer/colorrenderer.cpp \
    view/renderer/textrenderer.cpp \
    view/renderer/renderer.cpp \
    view/renderer/spriterenderer.cpp

HEADERS += \
    controller/gamecontroller.h \
    model/behaviors/attack.h \
    model/behaviors/behavior.h \
    model/behaviors/concrete/attack/counterattackbehavior.h \
    model/behaviors/concrete/attack/genericattackbehavior.h \
    model/behaviors/concrete/health/generichealingbehavior.h \
    model/behaviors/concrete/health/generichealthbehavior.h \
    model/behaviors/concrete/movement/genericmovebehavior.h \
    model/behaviors/concrete/movement/genericwalkablebehavior.h \
    model/behaviors/concrete/movement/healonstepbehavior.h \
    model/behaviors/concrete/movement/newlevelonstep.h \
    model/behaviors/concrete/movement/poisononstepbehavior.h \
    model/behaviors/concrete/movement/randommovementbehavior.h \
    model/behaviors/concrete/poison/genericpoisonablebehavior.h \
    model/behaviors/concrete/health/poisononkilledbehavior.h \
    model/behaviors/concrete/poison/genericpoisoningbehavior.h \
    model/behaviors/health.h \
    model/behaviors/movement.h \
    model/behaviors/poison.h \
    model/gameobject.h \
    model/gameobjectmodel.h \
    model/gameobjectsettings.h \
    model/modelfactory.h \
    model/noise/perlinnoise.h \
    node.h \
    publicenums.h \
    view/gamepixmapitem.h \
    view/gameview.h \
    view/gamewindow.h \
    view/renderer/colorrenderer.h \
    view/renderer/textrenderer.h \
    view/renderer/renderer.h \
    view/renderer/spriterenderer.h

FORMS += \
    gamewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += qdarkstyle/dark/darkstyle.qrc
#RESOURCES += qdarkstyle/light/lightstyle.qrc
RESOURCES += Resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/ -lworl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/ -lworld

INCLUDEPATH += $$PWD/../worldsource
DEPENDPATH += $$PWD/../worldsource


unix:!macx: LIBS += -L$$PWD/../worldlib/ -lworld

#INCLUDEPATH += $$PWD/../worldlib
#DEPENDPATH += $$PWD/../worldlib

DISTFILES += \
    README.md \
    docs/ARCH.md \
    docs/NOTES.md

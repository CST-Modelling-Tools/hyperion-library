QT -= gui

TEMPLATE = lib
DEFINES += HYPERIONLIBRARY_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = hyperion-library

INCLUDEPATH += $$PWD/include

INCLUDEPATH += "C:\Users\m.blanco\Dropbox\OpenSource\eigen-3.3.7"
#INCLUDEPATH += "C:\Users\Clothilde\Documents\eigen-3.3.7"
#INCLUDEPATH += C:\Users\m.constantinou\Downloads\eigen-3.3.7\eigen-3.3.7

SOURCES += \
    atmosphere.cpp \
    auxfunction.cpp \
    boundaries.cpp \
    dniashrae.cpp \
    heliostat.cpp \
    heliostatfield.cpp \
    heliostatpowerperunitarea.cpp \
    location.cpp \
    receiver.cpp

HEADERS += \
    include/HyperionLibrary_global.h \
    include/atmosphere.h \
    include/auxfunction.h \
    include/boundaries.h \
    include/dniashrae.h \
    include/heliostat.h \
    include/heliostatfield.h \
    include/heliostatpowerperunitarea.h \
    include/receiver.h \
    include/location.h \
    include/mathconstants.h

 DESTDIR = $$PWD/lib

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

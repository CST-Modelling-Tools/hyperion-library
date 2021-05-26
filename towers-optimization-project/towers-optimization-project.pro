TEMPLATE = app
QT += core
QT -= gui

CONFIG += c++1z console
CONFIG -= app_bundle

QMAKE_CXXFLAGS += /std:c++17

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../library/library.pri)

EIGENDIR = "C:\Users\m.blanco\Dropbox\OpenSource\eigen-3.3.7"
OPTLIB_INCLUDE_1 = "C:\Users\m.blanco\Dropbox\OpenSource\nlopt\build"
OPTLIB_INCLUDE_2 = "C:\Users\m.blanco\Dropbox\OpenSource\nlopt\src\api"
OPTLIBDIR_RELEASE = "C:\Users\m.blanco\Dropbox\OpenSource\nlopt\build\Release"
OPTLIBDIR_DEBUG = "C:\Users\m.blanco\Dropbox\OpenSource\nlopt\build\Debug"
OPTLIBDIR = "C:\Users\m.blanco\Dropbox\OpenSource\HyperionLibrary\library\lib"
#EIGENDIR += C:\Users\m.constantinou\Downloads\eigen-3.3.7\eigen-3.3.7


INCLUDEPATH += $$EIGENDIR
INCLUDEPATH += $$OPTLIBDIR

HEADERS += \


SOURCES += \
    main.cpp \

win32:CONFIG(release, debug|release): LIBS += -L$$OPTLIBDIR -llibnlopt-0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OPTLIBDIR -llibnlopt-0d

DEPENDPATH += $$OPTLIBDIR

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OPTLIBDIR/liblibnlopt-0.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OPTLIBDIR/liblibnlopt-0d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OPTLIBDIR/libnlopt-0.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OPTLIBDIR/libnlopt-0d.lib

#/*INCLUDEPATH += $$EIGENDIR
#INCLUDEPATH += $$OPTLIB_INCLUDE_1
#INCLUDEPATH += $$OPTLIB_INCLUDE_2

#HEADERS += \


#SOURCES += \
 #   main.cpp \

#win32:CONFIG(release, debug|release): LIBS += -L$$OPTLIBDIR -llibnlopt-0
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OPTLIBDIR -llibnlopt-0d

#DEPENDPATH += $$OPTLIBDIR

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OPTLIBDIR/liblibnlopt-0.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OPTLIBDIR/liblibnlopt-0d.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OPTLIBDIR/libnlopt-0.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OPTLIBDIR/libnlopt-0d.lib
#CONFIG(release, debug|release): LIBS += -L$$OPTLIBDIR_RELEASE -lnlopt
#else:CONFIG(debug, debug|release): LIBS += -L$$OPTLIBDIR_DEBUG -lnlopt


#INCLUDEPATH += $$PWD/../library/include
#DEPENDPATH += $$PWD/../library/include

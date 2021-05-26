QT += testlib

include(../library/library.pri)
#INCLUDEPATH += C:\Users\m.constantinou\Downloads\eigen-3.3.7\eigen-3.3.7
INCLUDEPATH += "C:\Users\m.blanco\Dropbox\OpenSource\eigen-3.3.7"
#INCLUDEPATH += "C:\Users\Clothilde\Documents\eigen-3.3.7"


HEADERS += \
    tests/atmosphere_test.h \
    tests/auxfunction_test.h \
    tests/boundaries_test.h \
    tests/dniashrae_test.h \
    tests/location_test.h \
    tests/receiver_test.h

SOURCES += \
    main.cpp \
    tests/atmosphere_test.cpp \
    tests/auxfunction_test.cpp \
    tests/boundaries_test.cpp \
    tests/dniashrae_test.cpp \
    tests/location_test.cpp \
    tests/receiver_test.cpp

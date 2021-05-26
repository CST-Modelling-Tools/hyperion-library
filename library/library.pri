LIBTARGET = hyperion-library
BASEDIR   = $${PWD}
DESTDIR = $$PWD/lib
INCLUDEPATH *= $${BASEDIR}/include
LIBS += -L$${DESTDIR} -lhyperion-library

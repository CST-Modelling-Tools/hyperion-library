#include "boundaries_test.h"
#include "boundaries.h"

using namespace hypl;

void boundaries_test::constructors_accessors()
{
    Boundaries bnd1;

    QCOMPARE(bnd1.xmin(),-500);
    QCOMPARE(bnd1.xmax(),500);
    QCOMPARE(bnd1.ymin(),-100);
    QCOMPARE(bnd1.ymax(),700);

    double xmin=-400;
    double xmax=400;
    double ymin=0;
    double ymax=600;

    Boundaries bnd2(xmin, xmax, ymin, ymax);

    QCOMPARE(bnd2.xmin(), xmin);
    QCOMPARE(bnd2.xmax(), xmax);
    QCOMPARE(bnd2.ymin(), ymin);
    QCOMPARE(bnd2.ymax(), ymax);
}

void boundaries_test::mutators()
{
    double xmin=-300;
    double xmax=300;
    double ymin=100;
    double ymax=500;

    Boundaries bnd1;;

    bnd1.set_xmin(xmin);
    bnd1.set_xmax(xmax);
    bnd1.set_ymin(ymin);
    bnd1.set_ymax(ymax);

    QCOMPARE(bnd1.xmin(), xmin);
    QCOMPARE(bnd1.xmax(), xmax);
    QCOMPARE(bnd1.ymin(), ymin);
    QCOMPARE(bnd1.ymax(), ymax);
}

void boundaries_test::public_functions()
{



// TO BE CONTINUED...
/*
    void Read(const QJsonObject& json);
    void Write(QJsonObject& json) const;
 */
}

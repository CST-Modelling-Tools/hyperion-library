#pragma once

#include <QtTest>  // http://doc.qt.io/qt-5/qttest-index.html
#include "auxfunction.h"


class Auxfunction_test : public QObject
{
    Q_OBJECT

public:
//    boundaries_test();


private slots:
//    void initTestCase(); // called before the first test function
//    void init(); // called before every test function
//    void cleanup(); // called after every test function
//    void cleanupTestCase(); // called after the last test function

    void constructors_accessors();
    void mutators();
    void public_functions();
//   void constants();
//    void components();

//    void assignments();
//    void comparisons();
//    void functions();
//    void conversions();

//    void operators();
//    void products();
//    void minmax();

//    void skip();
//    void length_data();
//    void length();
};


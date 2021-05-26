#include "tests/atmosphere_test.h"
#include "tests/receiver_test.h"
#include "tests/boundaries_test.h"
#include "tests/location_test.h"
#include "tests/auxfunction_test.h"
#include "tests/dniashrae_test.h"
#include <iostream>


int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    QTest::qExec(new atmosphere_test, argc, argv);
    std::cout << std::endl;
    QTest::qExec(new receiver_test, argc, argv);
    std::cout << std::endl;
    QTest::qExec(new boundaries_test, argc, argv);
    std::cout << std::endl;
    QTest::qExec(new Location_test, argc, argv);
    std::cout << std::endl;
    QTest::qExec(new Auxfunction_test, argc, argv);
    std::cout << std::endl;
    QTest::qExec(new dniashrae_test, argc, argv);
    //std::cout<< mathconstants::earth_rotational_speed;
    std::cout<<mathconstants::degree<<std::endl;
    return 0;
}

#include "receiver_test.h"
#include "receiver.h"

using namespace hypl;
#include <Eigen/Dense>

void receiver_test::constructors_accessors()
{
    Eigen::Vector3d v(1,1,1);

    Receiver rcv(v);
    QCOMPARE(rcv.aiming_point(),v);
    QCOMPARE(rcv.power(), 0.0);
}

void receiver_test::mutators()
{
    double power = 150.50;
    Eigen::Vector3d v(1,1,1);

    Receiver rcv1;

    rcv1.set_power(power);
    rcv1.set_aiming_point(v);

    QCOMPARE(rcv1.power(), power);
    QCOMPARE(rcv1.aiming_point(), v);
}

void receiver_test::public_functions()
{
    double power = 150.50;
    double addition = 100;

    Receiver rcv1;

    rcv1.set_power(power);
    QCOMPARE(rcv1.power(), power);

    rcv1.AddPower(addition);
    QCOMPARE(rcv1.power(), 250.5);
}



// TO BE CONTINUED...
/*
        void Read(const QJsonObject& json);
        void Write(QJsonObject& json) const;

 */

#include "atmosphere_test.h"
#include "atmosphere.h"

using namespace hypl;


void atmosphere_test::constructors_accessors()
{
    Atmosphere atm1;

    QCOMPARE(atm1.io(), 1110.0);
    QCOMPARE(atm1.beta(), 0.11);
    QCOMPARE(atm1.attenuation_model(), Atmosphere::AttenuationModel::VB);

    double io = 1000.0;
    double beta = 0.10;
    Atmosphere::AttenuationModel attenuation_model = Atmosphere::AttenuationModel::LH;

    Atmosphere atm2(io, beta, attenuation_model);

    QCOMPARE(atm2.io(), io);
    QCOMPARE(atm2.beta(), beta);
    QCOMPARE(atm2.attenuation_model(), attenuation_model);
}

void atmosphere_test::mutators()
{
    double io = 700.0;
    double beta = 0.09;
    Atmosphere::AttenuationModel attenuation_model = Atmosphere::AttenuationModel::SW;

    Atmosphere atm1;

    atm1.set_io(io);
    atm1.set_beta(beta);
    atm1.set_attenuation_model(attenuation_model);

    QCOMPARE(atm1.io(), io);
    QCOMPARE(atm1.beta(), beta);
    QCOMPARE(atm1.attenuation_model(), attenuation_model);
}

void atmosphere_test::public_functions()
{
    double io = 700.0;
    double beta = 0.09;
    Atmosphere::AttenuationModel attenuation_model = Atmosphere::AttenuationModel::SW;

    Atmosphere atm1(io, beta, attenuation_model);

    QCOMPARE(atm1.io(), io);
    QCOMPARE(atm1.beta(), beta);
    QCOMPARE(atm1.attenuation_model(), attenuation_model);

    //atm1.DefaultValues();

    //double default_io = 1110.0;
    //double default_beta = 0.11;
    //Atmosphere::AttenuationModel default_attenuation_model = Atmosphere::AttenuationModel::VB;

    //QCOMPARE(atm1.io(), default_io);
    //QCOMPARE(atm1.beta(), default_beta);
    //QCOMPARE(atm1.attenuation_model(), default_attenuation_model);


    atm1.set_attenuation_model(Atmosphere::AttenuationModel::SW);
    double slant_range = 1000.0;
    beta = atm1.beta();
    double attenuation = exp(-(0.000010696 + 0.0009196 * beta) * slant_range);
    QCOMPARE(atm1.Attenuation(slant_range), attenuation);


// TO BE CONTINUED...
/*
    double Attenuation(double slant_range ) const;
    void Read(const QJsonObject& json);
    void Write(QJsonObject& json) const;
 */
}

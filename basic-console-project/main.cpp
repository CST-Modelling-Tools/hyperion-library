#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

#include <iostream>

#include "heliostat.h"
#include "heliostatfield.h"


int main(int argc, char *argv[])
{
    // application
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Hyperion Basic Console Project");
    QCoreApplication::setApplicationVersion("0.1");

    std::cout << "----- BASIC CONSOLE PROJECT NEW -----" << std::endl << std::endl;

    std::cout << "Loading HeliostatField Specs ..." <<std::endl;

    hypl::HeliostatField myHeliostatField;
    myHeliostatField.LoadHeliostatField("pepe", hypl::HeliostatField::Json);
    std::cout << "Latitute: " << myHeliostatField.location().LatitudeDegree() << std::endl;
    std::cout << std::endl;
    std::cout << "Atmosphere -----" << std::endl;
    std::cout << "   Io = " << myHeliostatField.atmosphere().io() << std::endl;
    std::cout << "   beta = " << myHeliostatField.atmosphere().beta() << std::endl;
    std::cout << "   Attenuation Model = " << myHeliostatField.atmosphere().attenuation_model() << std::endl;
    std::cout << std::endl;
    std::cout << "Boundaries -----" << std::endl;
    std::cout << "   xmin = " << myHeliostatField.boundaries().xmin() << std::endl;
    std::cout << "   xmax = " << myHeliostatField.boundaries().xmin() << std::endl;
    std::cout << "   ymin = " << myHeliostatField.boundaries().ymin() << std::endl;
    std::cout << "   ymax = " << myHeliostatField.boundaries().ymax() << std::endl;
    std::cout << "Number of rows: " << myHeliostatField.nrows() << std::endl;
    std::cout << "Number of columns: " << myHeliostatField.ncolumns() << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Calculating annual efficiencies..." <<std::endl;
    std::cout << std::endl;

    QVector<double> annual_ideal_efficiencies_vector = myHeliostatField.AnnualIdealEfficiencies();
    const QVector<hypl::Heliostat*>& heliostats = myHeliostatField.heliostats();

    //Preparing to write the heliostat efficiencies to a file
    QFile output_file("IdealHeliostatEfficiencies_x24c.dat");
    output_file.open(QIODevice::WriteOnly);
    QTextStream out(&output_file);

    for(int index=0; index < heliostats.size(); index++ )
    {
        out << heliostats[index]->center().x() << " ";
        out << heliostats[index]->center().y() << " ";
        out << heliostats[index]->center().z() << " ";
        out << annual_ideal_efficiencies_vector[index] << endl;
    }
    output_file.close();

    std::cout << "DONE" <<std::endl;


    return app.exec();
}

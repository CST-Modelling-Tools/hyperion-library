#include <QCoreApplication>
#include <QFile>

#include <iostream>
#include "mathconstants.h"
#include "atmosphere.h"
#include "heliostatfield.h"
#include "receiver.h"
#include "location.h"
#include "nlopt.hpp"
#include <algorithm>

int count = 0;

double myfunc(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data);

int main(int argc, char *argv[])
{
    // application
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Hyperion Towers Optimization Project");
    QCoreApplication::setApplicationVersion("0.1");

    std::cout << "Towers Optimization" << std::endl << std::endl;

    std::cout << "Loading simulation..." <<std::endl;
    //hypl::Simulation& mySimulation = hypl::Simulation::getInstance();
    //mySimulation.LoadSimulation("pepe", hypl::Simulation::Json);
    //std::cout << "Simulation loaded succesfully" <<std::endl;
    //std::cout << "Latitute: " << mySimulation.location().latitude()/mathconstants::degree<< std::endl;

    std::cout << "DONE, now let's optimize! " <<std::endl;

    //Creating heliostat field
    int nrows = 50;
    int ncolumns = 50;
   // hypl::HeliostatField myHeliostatField(mySimulation, nrows, ncolumns );

    nlopt::opt opt(nlopt::LN_COBYLA, 2); /* algorithm and dimensionality */
    std::vector<double> lb(2);
    lb[0] = -HUGE_VAL; lb[1] = -1000.;
    opt.set_lower_bounds(lb);  /* lower bounds */
    std::vector<double> ub(2);
    ub[0] = HUGE_VAL; ub[1] = 1000.;
    opt.set_upper_bounds(ub);  /* upper bounds */
    //opt.set_max_objective(myfunc, &myHeliostatField);
    opt.set_xtol_rel(1e-4);
    std::vector<double> x(2);
    x[0] = 0.; x[1] = 0.;

    double maxf;
    try{
        nlopt::result result = opt.optimize(x, maxf);
        std::cout << "found minimum at f(" << x[0] << "," << x[1] << ") = "
             << maxf << std::endl;
    }
    catch(std::exception &e) {
        std::cout << "nlopt failed: " << e.what() << std::endl;
    }

    printf("found minimum after %d evaluations\n", count);


    return app.exec();
}

double myfunc(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data)
{
   hypl::HeliostatField* myHeliostatField = reinterpret_cast<hypl::HeliostatField*>(my_func_data);

   //const hypl::Simulation& simulation = hypl::Simulation::getInstance();

   ++count;
   if (!grad.empty()) {
       grad[0] = 0;
       grad[1] = 0;
   }
   // Change receiver position
   //QVector<hypl::Receiver*> receivers = simulation.receivers();

   Eigen::Vector3d receiver(x[0],x[1],100.);
  // receivers[0]->set_aiming_point(receiver);
  // receivers[0]->set_power(0.0);

    myHeliostatField->update();

   //Calculate the average efficiency
   QVector<double> annual_ideal_efficiencies_vector = myHeliostatField->AnnualIdealEfficiencies();

   std::sort(annual_ideal_efficiencies_vector.begin(), annual_ideal_efficiencies_vector.end(), std::greater<double>());

   int nHelios = annual_ideal_efficiencies_vector.size() / 2;
   assert(nHelios <= annual_ideal_efficiencies_vector.size());
   double mean_efficiency = 0.;
   for(int index=0; index < nHelios; index++ )
   {
        mean_efficiency += annual_ideal_efficiencies_vector.at(index);
   }

  // std::cout << " receiver pos: " << receivers[0]->aiming_point().transpose() << std::endl;
   std::cout << " average efficiency: " << mean_efficiency/nHelios << std::endl;

   return mean_efficiency/nHelios;
}

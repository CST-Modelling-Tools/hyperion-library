#ifndef AUXFUNCTION_H
#define AUXFUNCTION_H

#include "HyperionLibrary_global.h"

//Eigen includes
#include <Eigen/Dense>

#include <QDebug>
#include <QVector>

#include "location.h"
#include "atmosphere.h"

namespace hypl
{
    namespace auxfunction
    {
        int HYPERIONLIBRARY_EXPORT DayNumber(int day_index);
        double HYPERIONLIBRARY_EXPORT SolarDeclinationByDayNumber(int day_number);
        double HYPERIONLIBRARY_EXPORT SolarDeclinationByIndex(int day_index);
        double HYPERIONLIBRARY_EXPORT DniDailyEnergy(double declination, const Location &location, const Atmosphere &atmosphere);
        double HYPERIONLIBRARY_EXPORT DniYearlyEnergy(const Location& location, const Atmosphere& atmosphere, int delta_days);
    }

}
#endif // AUXFUNCTION_H

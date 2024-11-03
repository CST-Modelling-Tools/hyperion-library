#ifndef IDEALEFFICIENCYMAP_H
#define IDEALEFFICIENCYMAP_H

#include <vector>

#include "export.h"
#include "environment.h"
#include "boundaries.h"
#include "receiver.h"
#include "heliostat.h"

/*
template class DLL_API std::allocator<hypl::Receiver>;
template class DLL_API std::_Vector_val<std::_Simple_types<hypl::Receiver>>;
template class DLL_API std::_Compressed_pair<std::allocator<hypl::Receiver>,std::_Vector_val<std::_Simple_types<hypl::Receiver>>,true>;
template class DLL_API std::vector<hypl::Receiver>;

template class DLL_API std::allocator<hypl::Heliostat*>;
template class DLL_API std::_Vector_val<std::_Simple_types<hypl::Heliostat*>>;
template class DLL_API std::_Compressed_pair<std::allocator<hypl::Heliostat*>,std::_Vector_val<std::_Simple_types<hypl::Heliostat*>>,true>;
template class DLL_API std::vector<hypl::Heliostat*>;
*/

namespace hypl
{
    class HYPERIONLIBRARY_API IdealEfficiencyMap
    {
    public:

        IdealEfficiencyMap(Environment environment, Boundaries boundaries, std::vector<Receiver>& receivers, int nrows, int ncolumns);

        //Accessors
        Environment const& environment() const {return m_environment;}
        Boundaries const& boundaries() const {return m_boundaries;}
        std::vector<Receiver> const& receivers() const {return m_receiver;}
        int const& nrows() const {return m_nrows;}
        int const& ncolumns() const {return m_ncolumns;}
        std::vector<Heliostat> const& heliostats() {return m_heliostat;}

        //Mutators
        void update();
        void regenerate();
        void set_environment(Environment environment) {m_environment = environment; update();}
        void set_boundaries(Boundaries boundaries) {m_boundaries = boundaries; regenerate();}
        void set_receivers(std::vector<Receiver> receivers) {m_receiver = receivers; update();}
        void set_receivers_radius(double radius);
        void set_nrows(int nrows) {m_nrows = nrows; regenerate();}
        void set_ncolumns(int ncolumns) {m_ncolumns = ncolumns; regenerate();}
        void set_dimensions(int nrows, int ncolumns) {m_nrows = nrows; m_ncolumns = ncolumns; regenerate();}

        //Public functions
        void EvaluateAnnualEfficiencies(hypl::Heliostat::IdealEfficiencyType ideal_efficiency_type, double delta_t);

    private:
        void ProcessDay(int const& day_number, Heliostat::IdealEfficiencyType const& ideal_efficiency_type, 
                        double const& delta_hour_angle, double const& weight, double& direct_insolation);
        double StartingHourAngle(double const& wo, double const& delta_hour_angle);

    private:
        Environment m_environment;
        Boundaries m_boundaries;
        std::vector<Receiver>& m_receiver;
        int m_nrows;
        int m_ncolumns;
        std::vector<Heliostat> m_heliostat;
    };
}
#endif IDEALEFFICIENCYMAP_H
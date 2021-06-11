#ifndef IDEALEFFICIENCYMAP_H
#define IDEALEFFICIENCYMAP_H

#include "export.h"

#include <vector>

#include "environment.h"
#include "boundaries.h"
#include "receiver.h"
#include "heliostat.h"

template class DLL_API std::allocator<hypl::Receiver>;
template class DLL_API std::_Vector_val<std::_Simple_types<hypl::Receiver>>;
template class DLL_API std::_Compressed_pair<std::allocator<hypl::Receiver>,std::_Vector_val<std::_Simple_types<hypl::Receiver>>,true>;
template class DLL_API std::vector<hypl::Receiver>;

template class DLL_API std::allocator<hypl::Heliostat*>;
template class DLL_API std::_Vector_val<std::_Simple_types<hypl::Heliostat*>>;
template class DLL_API std::_Compressed_pair<std::allocator<hypl::Heliostat*>,std::_Vector_val<std::_Simple_types<hypl::Heliostat*>>,true>;
template class DLL_API std::vector<hypl::Heliostat*>;

namespace hypl
{
    class DLL_API IdealEfficiencyMap
    {
    public:

        IdealEfficiencyMap() {}
        IdealEfficiencyMap(Environment environment, Boundaries boundaries, std::vector<Receiver> receivers, int nrows, int ncolumns);
        ~IdealEfficiencyMap();

        //Accessors
        const Environment& environment() const {return m_environment;}
        const Boundaries& boundaries() const {return m_boundaries;}
        const std::vector<Receiver>& receivers() const {return m_receivers;}
        const int& nrows() const {return m_nrows;}
        const int& ncolumns() const {return m_ncolumns;}
        std::vector<Heliostat*>& heliostats() {return m_heliostats;}

        //Mutators
        void update();
        void regenerate();
        void set_environment(Environment environment) {m_environment = environment; update();}
        void set_boundaries(Boundaries boundaries) {m_boundaries = boundaries; regenerate();}
        void set_receivers(std::vector<Receiver> receivers) {m_receivers = receivers; update();}
        void set_nrows(int nrows) {m_nrows = nrows; regenerate();}
        void set_ncolumns(int ncolumns) {m_ncolumns = ncolumns; regenerate();}
        void set_dimensions(int nrows, int ncolumns) {m_nrows = nrows; m_ncolumns = ncolumns; regenerate();}

        //Public functions
        void EvaluateAnnualEfficiencies();

    private:
        Environment m_environment;
        Boundaries m_boundaries;
        std::vector<Receiver> m_receivers;
        int m_nrows;
        int m_ncolumns;
        std::vector<Heliostat*> m_heliostats;
    };
}
#endif IDEALEFFICIENCYMAP_H
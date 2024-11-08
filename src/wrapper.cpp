#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "boundaries.h"
#include "vec3d.h"
#include "vec2d.h"
#include "heliostat.h"
#include "environment.h"
#include "atmosphere.h"
#include "location.h"
#include "processheliostatfunctor.h"
#include "idealefficiencymap.h"
#include "auxfunction.h"
#include "receiver.h"

namespace py = pybind11;

PYBIND11_MODULE(hyperionlibrary_python_wrapper, m) {
    m.doc() = "Python bindings for the hyperion library";



    // Inside the PYBIND11_MODULE definition
    py::class_<hypl::Boundaries>(m, "Boundaries")
        .def(py::init<>())  // Default constructor
        .def(py::init<double, double, double, double>(), 
            py::arg("xmin"), py::arg("xmax"), py::arg("ymin"), py::arg("ymax"),
            "Initialize boundaries with specified limits")
        .def("xmin", &hypl::Boundaries::xmin, "Get the minimum x boundary")
        .def("xmax", &hypl::Boundaries::xmax, "Get the maximum x boundary")
        .def("ymin", &hypl::Boundaries::ymin, "Get the minimum y boundary")
        .def("ymax", &hypl::Boundaries::ymax, "Get the maximum y boundary")
        .def("set_xmin", &hypl::Boundaries::set_xmin, py::arg("xmin"), "Set the minimum x boundary")
        .def("set_xmax", &hypl::Boundaries::set_xmax, py::arg("xmax"), "Set the maximum x boundary")
        .def("set_ymin", &hypl::Boundaries::set_ymin, py::arg("ymin"), "Set the minimum y boundary")
        .def("set_ymax", &hypl::Boundaries::set_ymax, py::arg("ymax"), "Set the maximum y boundary")
        .def("width", &hypl::Boundaries::width, "Get the width of the boundaries")
        .def("height", &hypl::Boundaries::height, "Get the height of the boundaries");


    // Location class bindings
    py::class_<hypl::Location>(m, "Location")
        .def(py::init<>())  // Default constructor
        .def(py::init<double>())  // Constructor with latitude parameter
        .def("latitude", &hypl::Location::latitude, "Get latitude in radians")
        .def("cos_latitude", &hypl::Location::cos_latitude, "Get cosine of latitude")
        .def("sin_latitude", &hypl::Location::sin_latitude, "Get sine of latitude")
        .def("set_latitude_degree", &hypl::Location::set_latitude_degree, "Set latitude in degrees")
        .def("LatitudeDegree", &hypl::Location::LatitudeDegree, "Get latitude in degrees")
        .def("HourAngleLimit", &hypl::Location::HourAngleLimit, "Get hour angle limit for a given declination")
        .def("SolarVector", &hypl::Location::SolarVector, "Calculate solar vector based on hour angle and declination");

    // Atmosphere class bindings
    py::class_<hypl::Atmosphere>(m, "Atmosphere")
        .def(py::init<>())  // Default constructor
        .def(py::init<double, double, hypl::Atmosphere::TransmittanceModel>(),  // Constructor with parameters
             py::arg("io"), py::arg("beta"), py::arg("transmittance_model"))
        .def("io", &hypl::Atmosphere::io, "Get solar constant I0")
        .def("beta", &hypl::Atmosphere::beta, "Get atmospheric extinction coefficient")
        .def("transmittance_model", &hypl::Atmosphere::transmittance_model, "Get transmittance model type")
        .def("set_io", &hypl::Atmosphere::set_io, "Set solar constant I0")
        .def("set_beta", &hypl::Atmosphere::set_beta, "Set atmospheric extinction coefficient")
        .def("set_transmittance_model", &hypl::Atmosphere::set_transmittance_model, "Set transmittance model")
        .def("Transmittance", &hypl::Atmosphere::Transmittance, "Calculate transmittance over a given slant range")
        .def("DniFromSz", &hypl::Atmosphere::DniFromSz, "Calculate Direct Normal Irradiance (DNI) based on solar zenith angle")
        .def("TransmittanceModelName", &hypl::Atmosphere::TransmittanceModelName, "Get the name of the transmittance model");

    // Enum binding for Atmosphere::TransmittanceModel
    py::enum_<hypl::Atmosphere::TransmittanceModel>(m, "TransmittanceModel")
        .value("SW", hypl::Atmosphere::TransmittanceModel::SW, "Simple transmittance model")
        .value("VB", hypl::Atmosphere::TransmittanceModel::VB, "Vignola and Bird transmittance model")
        .value("LH", hypl::Atmosphere::TransmittanceModel::LH, "Leckner transmittance model")
        .export_values();

    // Environment class bindings
    py::class_<hypl::Environment>(m, "Environment")
        .def(py::init<>())  // Default constructor
        .def(py::init<hypl::Location, hypl::Atmosphere>(),  // Constructor with Location and Atmosphere
             py::arg("location"), py::arg("atmosphere"))
        .def("location", &hypl::Environment::location, "Get the location of the environment")
        .def("atmosphere", &hypl::Environment::atmosphere, "Get the atmosphere of the environment")
        .def("sun_subtended_angle", &hypl::Environment::sun_subtended_angle, py::return_value_policy::reference, 
             "Get the vector of sun subtended angles")
        .def("declination", &hypl::Environment::declination, py::return_value_policy::reference, 
             "Get the vector of declination values")
        .def("set_location", &hypl::Environment::set_location, "Set the location of the environment")
        .def("set_atmosphere", &hypl::Environment::set_atmosphere, "Set the atmosphere of the environment")
        .def("set_location_and_atmosphere", &hypl::Environment::set_location_and_atmosphere, 
             "Set both the location and atmosphere of the environment")
        .def("update_environment", &hypl::Environment::update_environment, "Update the environment parameters");

    // Heliostat class bindings
    py::class_<hypl::Heliostat::TrackingInfo>(m, "TrackingInfo")
        .def_readwrite("ideal_efficiency", &hypl::Heliostat::TrackingInfo::ideal_efficiency)
        .def_readwrite("aiming_at_receiver_id", &hypl::Heliostat::TrackingInfo::aiming_at_receiver_id);

    py::class_<hypl::Heliostat>(m, "Heliostat")
        .def(py::init<hypl::Environment&, std::vector<hypl::Receiver>&, hypl::vec3d>(), 
            py::arg("environment"), py::arg("receivers"), py::arg("center"))
        .def("environment", &hypl::Heliostat::environment, py::return_value_policy::reference, "Get the environment reference")
        .def("receivers", &hypl::Heliostat::receivers, py::return_value_policy::reference, "Get the receivers")
        .def("center", &hypl::Heliostat::center, py::return_value_policy::reference, "Get the center position of the heliostat")
        .def("transmittance", &hypl::Heliostat::transmittance, py::return_value_policy::reference, "Get transmittance values")
        .def("slant_range", &hypl::Heliostat::slant_range, py::return_value_policy::reference, "Get slant range values")
        .def("reflected_unit_vector", &hypl::Heliostat::reflected_unit_vector, py::return_value_policy::reference, "Get reflected unit vectors")
        .def("set_center", &hypl::Heliostat::set_center, "Set the center position of the heliostat")
        .def("update", &hypl::Heliostat::update, "Update the heliostat parameters")
        .def("Track", &hypl::Heliostat::Track, py::arg("sun_vector"), py::arg("sun_subtended_angle"), py::arg("ideal_efficiency_type"),
            "Track the sun vector and calculate efficiency")
        .def("Spillage", &hypl::Heliostat::Spillage, py::arg("receiver_id"), py::arg("sun_subtended_angle"), "Calculate spillage")
        .def("ReceiverShadowing", &hypl::Heliostat::ReceiverShadowing, py::arg("receiver_id"), py::arg("sun_vector"), 
            "Calculate receiver shadowing")
        .def_readwrite("m_annual_ideal_efficiency", &hypl::Heliostat::m_annual_ideal_efficiency);

    // Bind IdealEfficiencyType enum directly to the module
    py::enum_<hypl::Heliostat::IdealEfficiencyType>(m, "IdealEfficiencyType")
        .value("CosineOnly", hypl::Heliostat::IdealEfficiencyType::CosineOnly, "Efficiency considering only cosine loss")
        .value("CosineAndTransmittance", hypl::Heliostat::IdealEfficiencyType::CosineAndTransmittance, 
            "Efficiency considering cosine and transmittance losses")
        .value("AllFactors", hypl::Heliostat::IdealEfficiencyType::AllFactors, "Efficiency considering all factors")
        .export_values();

    // IdealEfficiencyMap class bindings
    py::class_<hypl::IdealEfficiencyMap>(m, "IdealEfficiencyMap")
        .def(py::init<hypl::Environment, hypl::Boundaries, std::vector<hypl::Receiver>, int, int>(),
            py::arg("environment"), py::arg("boundaries"), py::arg("receivers"), py::arg("nrows"), py::arg("ncolumns"))
        .def("environment", &hypl::IdealEfficiencyMap::environment, py::return_value_policy::reference, "Get the environment")
        .def("boundaries", &hypl::IdealEfficiencyMap::boundaries, py::return_value_policy::reference, "Get the boundaries")
        .def("receivers", &hypl::IdealEfficiencyMap::receivers, py::return_value_policy::reference, "Get the receivers")
        .def("nrows", &hypl::IdealEfficiencyMap::nrows, "Get the number of rows")
        .def("ncolumns", &hypl::IdealEfficiencyMap::ncolumns, "Get the number of columns")
        .def("heliostats", &hypl::IdealEfficiencyMap::heliostats, py::return_value_policy::reference, "Get the heliostats vector")
        .def("update", &hypl::IdealEfficiencyMap::update, "Update the efficiency map")
        .def("regenerate", &hypl::IdealEfficiencyMap::regenerate, "Regenerate the efficiency map")
        .def("set_environment", &hypl::IdealEfficiencyMap::set_environment, "Set a new environment and update the map")
        .def("set_boundaries", &hypl::IdealEfficiencyMap::set_boundaries, "Set new boundaries and regenerate the map")
        .def("set_receivers", &hypl::IdealEfficiencyMap::set_receivers, "Set a new list of receivers and update the map")
        .def("set_receivers_radius", &hypl::IdealEfficiencyMap::set_receivers_radius, py::arg("radius"), "Set the radius for all receivers")
        .def("set_nrows", &hypl::IdealEfficiencyMap::set_nrows, py::arg("nrows"), "Set the number of rows and regenerate the map")
        .def("set_ncolumns", &hypl::IdealEfficiencyMap::set_ncolumns, py::arg("ncolumns"), "Set the number of columns and regenerate the map")
        .def("set_dimensions", &hypl::IdealEfficiencyMap::set_dimensions, py::arg("nrows"), py::arg("ncolumns"), "Set both rows and columns, then regenerate")
        .def("EvaluateAnnualEfficiencies", &hypl::IdealEfficiencyMap::EvaluateAnnualEfficiencies, 
            py::arg("ideal_efficiency_type"), py::arg("delta_t"), "Evaluate annual efficiencies for the heliostats");

    // Receiver class bindings
    py::class_<hypl::Receiver>(m, "Receiver")
        .def(py::init<>())  // Default constructor
        .def(py::init<const hypl::vec3d&>(), py::arg("aiming_point"))  // Constructor with aiming point
        .def(py::init<const hypl::vec3d&, double>(), py::arg("aiming_point"), py::arg("radius"))  // Constructor with aiming point and radius
        .def("aiming_point", &hypl::Receiver::aiming_point, py::return_value_policy::reference, "Get the aiming point of the receiver")
        .def("power", &hypl::Receiver::power, "Get the power of the receiver")
        .def("radius", &hypl::Receiver::radius, "Get the radius of the receiver")
        .def("set_aiming_point", &hypl::Receiver::set_aiming_point, py::arg("aiming_point"), "Set the aiming point of the receiver")
        .def("set_power", &hypl::Receiver::set_power, py::arg("power"), "Set the power of the receiver")
        .def("set_radius", &hypl::Receiver::set_radius, py::arg("radius"), "Set the radius of the receiver")
        .def("AddPower", &hypl::Receiver::AddPower, py::arg("power_increase"), "Increase the power of the receiver by the specified amount");

    // Bindings for vec2d
    py::class_<hypl::vec2d>(m, "vec2d")
        .def(py::init<>())
        .def(py::init<double, double>())
        .def_readwrite("x", &hypl::vec2d::x)
        .def_readwrite("y", &hypl::vec2d::y)
        .def("norm", &hypl::vec2d::norm)
        .def("normalized", &hypl::vec2d::normalized);

    // Standalone functions for vec2d
    m.def("dot", [](const hypl::vec2d& a, const hypl::vec2d& b) {
        return hypl::dot(a, b);
    }, "Compute the dot product of two vec2d vectors");

    m.def("cross", [](const hypl::vec2d& a, const hypl::vec2d& b) {
        return hypl::cross(a, b);
    }, "Compute the cross product of two vec2d vectors");

    // Bindings for vec3d
    py::class_<hypl::vec3d>(m, "vec3d")
        .def(py::init<>())
        .def(py::init<double, double, double>())
        .def_readwrite("x", &hypl::vec3d::x)
        .def_readwrite("y", &hypl::vec3d::y)
        .def_readwrite("z", &hypl::vec3d::z)
        .def("norm", &hypl::vec3d::norm)
        .def("normalized", &hypl::vec3d::normalized);

    // Standalone functions for vec3d
    m.def("dot", [](const hypl::vec3d& a, const hypl::vec3d& b) {
        return hypl::dot(a, b);
    }, "Compute the dot product of two vec3d vectors");

    m.def("cross", [](const hypl::vec3d& a, const hypl::vec3d& b) {
        return hypl::cross(a, b);
    }, "Compute the cross product of two vec3d vectors");
}
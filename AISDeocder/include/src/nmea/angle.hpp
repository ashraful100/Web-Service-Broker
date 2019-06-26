#ifndef src__NMEA__ANGLE__HPP
#define src__NMEA__ANGLE__HPP

#include <string>
#include <src/geo/angle.hpp>

namespace src
{
namespace nmea
{
geo::latitude parse_latitude(const std::string & s);
std::string to_string(const geo::latitude & v);

geo::longitude parse_longitude(const std::string & s);
std::string to_string(const geo::longitude & v);
}
}

#endif

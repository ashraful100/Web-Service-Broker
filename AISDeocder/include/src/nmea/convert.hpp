#ifndef src__NMEA__CONVERT__HPP
#define src__NMEA__CONVERT__HPP

#include <src/nmea/constants.hpp>
#include <src/geo/angle.hpp>
#include <src/utils/optional.hpp>

namespace src
{
namespace nmea
{
direction convert_hemisphere(const geo::latitude & p) noexcept;
direction convert_hemisphere(const geo::longitude & p) noexcept;
geo::latitude::hemisphere convert_hemisphere_lat(direction t);
geo::longitude::hemisphere convert_hemisphere_lon(direction t);

geo::latitude correct_hemisphere(const geo::latitude & v, const direction & d);

utils::optional<geo::latitude> correct_hemisphere(
	const utils::optional<geo::latitude> & v, const utils::optional<direction> & d);

geo::longitude correct_hemisphere(const geo::longitude & v, const direction & d);

utils::optional<geo::longitude> correct_hemisphere(
	const utils::optional<geo::longitude> & v, const utils::optional<direction> & d);
}
}

#endif

#ifndef src__NMEA__SPLIT__HPP
#define src__NMEA__SPLIT__HPP

#include <string>
#include <vector>

namespace src
{
namespace nmea
{
/// @cond DEV
namespace detail
{
std::vector<std::string> parse_fields(
	const std::string & s, const std::string::size_type start_pos = 1u);
}
/// @endcond
}
}

#endif

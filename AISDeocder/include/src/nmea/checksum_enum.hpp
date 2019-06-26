#ifndef src__NMEA__CHECKSUM_ENUM__HPP
#define src__NMEA__CHECKSUM_ENUM__HPP

namespace src
{
namespace nmea
{
/// This enumeration defines options about strategies of handling checksums.
enum class checksum_handling {
	check, ///< Perform checksum check.
	ignore ///< Ignore the checksum.
};
}
}

#endif

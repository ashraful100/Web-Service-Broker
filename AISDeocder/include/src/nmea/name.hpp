#ifndef src__NMEA__NAME__HPP
#define src__NMEA__NAME__HPP

#include <string>
#include <src/nmea/sentence_id.hpp>
#include <src/nmea/constants.hpp>
#include <src/nmea/talker_id.hpp>

namespace src
{
namespace nmea
{
/// @{
/// Returns a printable / human readable name for the specified argument.
///
/// @param[in] t The argument to return the name for.
/// @return The human readable name.

std::string to_name(sentence_id t);
std::string to_name(talker t);
std::string to_name(selection_mode t);
std::string to_name(status t);
std::string to_name(route t);
std::string to_name(mode_indicator t);
std::string to_name(quality t);
std::string to_name(reference t);
std::string to_name(side t);
std::string to_name(type_of_point t);

std::string to_name(unit::distance t);
std::string to_name(unit::temperature t);
std::string to_name(unit::velocity t);
/// @}
}
}

#endif

#include "checksum.hpp"

namespace src
{
namespace nmea
{
checksum_error::checksum_error(uint8_t exp, uint8_t act)
	: expected(exp)
	, actual(act)
{
	snprintf(
		text_, sizeof(text_), "checksum error (actual:%02X, expected:%02X)", actual, expected);
}

/// @cond DEV
namespace detail
{
static constexpr char hex_digit(unsigned int t) noexcept
{
	return "0123456789ABCDEF"[t & 0xf];
}
}
/// @endcond

/// Returns the specified checksum as string.
///
/// @param[in] sum The checksum to render as string.
/// @return The checksum as string. This string is always two characters.
///
std::string checksum_to_string(uint8_t sum)
{
	char buf[3]{detail::hex_digit(sum >> 4), detail::hex_digit(sum), '\0'};
	return buf;
}
}
}

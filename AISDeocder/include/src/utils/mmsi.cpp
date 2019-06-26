#include "mmsi.hpp"

namespace src
{
namespace utils
{

bool mmsi::operator==(const mmsi & other) const
{
	// not comparing (this == &other) because the comparison
	// of the value is trivial.
	return value_ == other.value_;
}

bool mmsi::operator!=(const mmsi & other) const
{
	return !(*this == other);
}

/// This function is for convenience. It tries to find
/// out about the type of MMSI, and returns the appropriate
/// MID. If there is no MID, `initial_value` will return.
mmsi::value_type mmsi::mid() const
{
	if (is_regular())
		return regular_mid();
	if (is_group())
		return group_mid();
	if (is_coastal())
		return coastal_mid();
	if (is_auxiliary())
		return auxiliary_mid();
	if (is_sar_aircraft())
		return sar_mid();
	if (is_mob())
		return mob_mid();
	return initial_value;
}

mmsi::value_type mmsi::regular_mid() const
{
	if (!is_regular())
		return initial_value;
	return (value_ / 1000000) % 1000;
}

mmsi::value_type mmsi::regular_id() const
{
	if (!is_regular())
		return initial_value;
	return value_ % 1000000;
}

mmsi::value_type mmsi::group_mid() const
{
	if (!is_group())
		return initial_value;
	return (value_ / 100000) % 1000;
}

mmsi::value_type mmsi::group_id() const
{
	if (!is_group())
		return initial_value;
	return value_ % 100000;
}

mmsi::value_type mmsi::coastal_mid() const
{
	if (!is_coastal())
		return initial_value;
	return (value_ / 10000) % 1000;
}

mmsi::value_type mmsi::coastal_id() const
{
	if (!is_coastal())
		return initial_value;
	return value_ % 10000;
}

mmsi::value_type mmsi::auxiliary_mid() const
{
	if (!is_auxiliary())
		return initial_value;
	return (value_ / 10000) % 1000;
}

mmsi::value_type mmsi::auxiliary_id() const
{
	if (!is_auxiliary())
		return initial_value;
	return value_ % 10000;
}

mmsi::value_type mmsi::mob_mid() const
{
	if (!is_mob())
		return initial_value;
	return (value_ / 1000) % 1000;
}

mmsi::value_type mmsi::mob_id() const
{
	if (!is_mob())
		return initial_value;
	return value_ % 1000;
}

mmsi::value_type mmsi::sar_mid() const
{
	if (!is_sar_aircraft())
		return initial_value;
	return (value_ / 1000) % 1000;
}

/// True if MIDxxxxxx
bool mmsi::is_regular() const
{
	const value_type m = (value_ / 1000000) % 1000;
	return (m >= 200) && (m < 900);
}

/// True if 0MIDxxxxx
bool mmsi::is_group() const
{
	const value_type p = value_ / 100000000;
	const value_type m = (value_ / 100000) % 1000;
	return (p == 0) && (m >= 100) && !is_auxiliary() && !is_ais_aids();
}

/// True if 00MIDxxxx
bool mmsi::is_coastal() const
{
	const value_type p = value_ / 10000000;
	return p == 0;
}

/// True if 00MID0000 and same MID.
bool mmsi::is_all_coastal_for(value_type mid) const
{
	const value_type p = value_ / 10000000;
	const value_type m = (value_ / 10000) % 1000;
	const value_type i = value_ % 10000;
	return (p == 0) && (m == mid) && (mid >= 100) && (i == 0);
}

/// True if 009990000
bool mmsi::is_all_coastal() const
{
	const value_type p = value_ / 10000000;
	const value_type m = (value_ / 10000) % 1000;
	const value_type i = value_ % 10000;
	return (p == 0) && (m == 999) && (i == 0);
}

/// True if 98MIDxxxx
bool mmsi::is_auxiliary() const
{
	const value_type p = value_ / 10000000;
	return p == 98;
}

/// True if 99MIDxxxx
bool mmsi::is_ais_aids() const
{
	const value_type p = value_ / 10000000;
	return p == 99;
}

/// True if 111MIDxxx
bool mmsi::is_sar_aircraft() const
{
	const value_type p = (value_ / 1000000) % 1000;
	return p == 111;
}

/// True if 970MIDxxx
bool mmsi::is_sart() const
{
	const value_type p = (value_ / 1000000) % 1000;
	return p == 970;
}

/// True if 972MIDxxx
bool mmsi::is_mob() const
{
	const value_type p = (value_ / 1000000) % 1000;
	return p == 972;
}

/// True if 974MIDxxx
bool mmsi::is_epirb_ais() const
{
	const value_type p = (value_ / 1000000) % 1000;
	return p == 974;
}
}
}

#include "dbk.hpp"
#include <src/nmea/io.hpp>

namespace src
{
namespace nmea
{
constexpr const char * dbk::TAG;

dbk::dbk()
	: sentence(ID, TAG, talker_id::integrated_instrumentation)
{
}

dbk::dbk(talker talk, fields::const_iterator first, fields::const_iterator last)
	: sentence(ID, TAG, talk)
{
	if (std::distance(first, last) != 6)
		throw std::invalid_argument{"invalid number of fields in dbk"};

	read(*(first + 0), depth_feet_);
	read(*(first + 1), depth_feet_unit_);
	read(*(first + 2), depth_meter_);
	read(*(first + 3), depth_meter_unit_);
	read(*(first + 4), depth_fathom_);
	read(*(first + 5), depth_fathom_unit_);
}

void dbk::set_depth_feet(double t) noexcept
{
	depth_feet_ = t;
	depth_feet_unit_ = unit::distance::feet;
}

void dbk::set_depth_meter(double t) noexcept
{
	depth_meter_ = t;
	depth_meter_unit_ = unit::distance::meter;
}

void dbk::set_depth_fathom(double t) noexcept
{
	depth_fathom_ = t;
	depth_fathom_unit_ = unit::distance::fathom;
}

void dbk::append_data_to(std::string & s) const
{
	append(s, to_string(depth_feet_));
	append(s, to_string(depth_feet_unit_));
	append(s, to_string(depth_meter_));
	append(s, to_string(depth_meter_unit_));
	append(s, to_string(depth_fathom_));
	append(s, to_string(depth_fathom_unit_));
}
}
}

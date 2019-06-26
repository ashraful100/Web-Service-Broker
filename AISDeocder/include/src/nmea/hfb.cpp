#include "hfb.hpp"
#include <src/nmea/io.hpp>
#include <src/utils/unique.hpp>

namespace src
{
namespace nmea
{
constexpr const char * hfb::TAG;

hfb::hfb()
	: sentence(ID, TAG, talker_id::global_positioning_system)
{
}

hfb::hfb(talker talk, fields::const_iterator first, fields::const_iterator last)
	: sentence(ID, TAG, talk)
{
	if (std::distance(first, last) != 4)
		throw std::invalid_argument{"invalid number of fields in hfb"};

	read(*(first + 0), distance_head_foot_);
	read(*(first + 1), distance_head_foot_unit_);
	read(*(first + 2), distance_head_bottom_);
	read(*(first + 3), distance_head_bottom_unit_);
}

void hfb::append_data_to(std::string & s) const
{
	append(s, to_string(distance_head_foot_));
	append(s, to_string(distance_head_foot_unit_));
	append(s, to_string(distance_head_bottom_));
	append(s, to_string(distance_head_bottom_unit_));
}
}
}

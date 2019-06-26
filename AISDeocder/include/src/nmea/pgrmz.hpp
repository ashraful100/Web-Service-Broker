#ifndef src__NMEA__PGRMZ__HPP
#define src__NMEA__PGRMZ__HPP

#include <src/nmea/sentence.hpp>
#include <src/utils/optional.hpp>

namespace src
{
namespace nmea
{
/// @brief PGRMZ - Garmin Altitude Information
///
/// @code
///        1   2 3
///        |   | |
/// $PGRMZ,x.x,f,h*hh<CR><LF>
/// @endcode
///
/// Field Number:
///
/// 1. Altitude
/// 2. f=feet
/// 3. Fix Type
///    - 1 = no fix
///    - 2 = 2D fix
///    - 3 = 3D fix
///
/// Example: <tt>$PGRMZ,1494,f,*10</tt>
///
class pgrmz : public sentence
{
	friend class detail::factory;

public:
	constexpr static const sentence_id ID = sentence_id::PGRMZ;
	constexpr static const char * TAG = "PGRMZ";

	enum class fix_type : char {
		no_fix, ///< NMEA representation: '1'
		d2fix, ///< NMEA representation: '2'
		d3fix ///< NMEA representation: '3'
	};

	pgrmz();
	pgrmz(const pgrmz &) = default;
	pgrmz & operator=(const pgrmz &) = default;
	pgrmz(pgrmz &&) = default;
	pgrmz & operator=(pgrmz &&) = default;

protected:
	pgrmz(talker talk, fields::const_iterator first, fields::const_iterator last);
	virtual void append_data_to(std::string &) const override;

private:
	double altitude_ = 0.0;
	unit::distance altitude_unit_ = unit::distance::feet;
	utils::optional<fix_type> fix_;

public:
	decltype(altitude_) get_altitude() const { return altitude_; }
	decltype(altitude_unit_) get_altitude_unit() const { return altitude_unit_; }
	decltype(fix_) get_fix() const { return fix_; }

	void set_altitude(double t) noexcept
	{
		altitude_ = t;
		altitude_unit_ = unit::distance::feet;
	}
	void set_fix(fix_type t) noexcept { fix_ = t; }
};

std::string to_string(pgrmz::fix_type value);
}
}

#endif

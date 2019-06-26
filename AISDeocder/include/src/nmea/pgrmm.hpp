#ifndef src__NMEA__PGRMM__HPP
#define src__NMEA__PGRMM__HPP

#include <src/nmea/sentence.hpp>
#include <src/utils/optional.hpp>

namespace src
{
namespace nmea
{
/// @brief PGRMM - Garmin Map Datum
///
/// @code
///        1
///        |
/// $PGRMM,c--c*hh<CR><LF>
/// @endcode
///
/// Field Number:
///
/// 1. Map datum, variable length
///
/// Example: <tt>$PGRMM,WGS 84*06</tt>
///
class pgrmm : public sentence
{
	friend class detail::factory;

public:
	constexpr static const sentence_id ID = sentence_id::PGRMM;
	constexpr static const char * TAG = "PGRMM";

	pgrmm();
	pgrmm(const pgrmm &) = default;
	pgrmm & operator=(const pgrmm &) = default;
	pgrmm(pgrmm &&) = default;
	pgrmm & operator=(pgrmm &&) = default;

protected:
	pgrmm(talker talk, fields::const_iterator first, fields::const_iterator last);
	virtual void append_data_to(std::string &) const override;

private:
	std::string map_datum_;

public:
	decltype(map_datum_) get_map_datum() const { return map_datum_; }

	void set_map_datum(const std::string & t) noexcept;
};
}
}

#endif

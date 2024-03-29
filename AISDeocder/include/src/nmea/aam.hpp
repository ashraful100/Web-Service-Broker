#ifndef src__NMEA__AAM__HPP
#define src__NMEA__AAM__HPP

#include <src/nmea/sentence.hpp>
#include <src/nmea/waypoint.hpp>
#include <src/utils/optional.hpp>

namespace src
{
namespace nmea
{
/// @brief AAM - Waypoint Arrival Alarm
///
/// This sentence is generated by some units to indicate the status of arrival (entering
/// the arrival circle, or passing the perpendicular of the course line) at the destination
/// waypoint.
///
/// @code
///        1 2 3   4 5
///        | | |   | |
/// $--AAM,A,A,x.x,N,c--c*hh<CR><LF>
/// @endcode
///
/// Field Number:
/// 1. Status
///    - A = arrival circle entered
///    - V = arrival circle not entered
/// 2. Status
///    - A = perpendicular passed at waypoint
///    - V = perpendicular not passed
/// 3. Arrival circle radius
/// 4. Units of radius
///    - N = nautical miles
/// 5. Waypoint ID
///
/// Example: <tt>$GPAAM,A,A,0.10,N,WPTNME*43</tt>
///
/// `WPTNME` is the waypoint name.
///
class aam : public sentence
{
	friend class detail::factory;

public:
	constexpr static const sentence_id ID = sentence_id::AAM;
	constexpr static const char * TAG = "AAM";

	aam();
	aam(const aam &) = default;
	aam & operator=(const aam &) = default;
	aam(aam &&) = default;
	aam & operator=(aam &&) = default;

protected:
	aam(talker talk, fields::const_iterator first, fields::const_iterator last);
	virtual void append_data_to(std::string &) const override;

private:
	utils::optional<status> arrival_circle_entered_;
	utils::optional<status> perpendicualar_passed_;
	utils::optional<double> arrival_circle_radius_;
	utils::optional<unit::distance> arrival_circle_radius_unit_;
	utils::optional<waypoint> waypoint_id_;

	void check() const;

public:
	decltype(arrival_circle_entered_) get_arrival_circle_entered() const
	{
		return arrival_circle_entered_;
	}
	decltype(perpendicualar_passed_) get_perpendicualar_passed() const
	{
		return perpendicualar_passed_;
	}
	decltype(arrival_circle_radius_) get_arrival_circle_radius() const
	{
		return arrival_circle_radius_;
	}
	decltype(arrival_circle_radius_unit_) get_arrival_circle_radius_unit() const
	{
		return arrival_circle_radius_unit_;
	}
	decltype(waypoint_id_) get_waypoint_id() const { return waypoint_id_; }

	void set_arrival_circle_entered(status s);
	void set_perpendicular_passed(status s);
	void set_arrival_circle_radius(double t);
	void set_waypoint_id(const waypoint & id) { waypoint_id_ = id; }
};
}
}

#endif

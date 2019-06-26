#ifndef src__GEO__CPA__HPP
#define src__GEO__CPA__HPP

#include <chrono>
#include <tuple>
#include <src/geo/position.hpp>

namespace src
{
namespace geo
{

/// @brief This struct contains data about a vessel, needed to
/// compute CPA and TCPA.
struct vessel {
	position pos; ///< Current position of vessel.
	double sog; ///< Speed over ground in knots.
	double cog; ///< Course over ground in degrees.
};

std::tuple<position, position, std::chrono::seconds, bool> cpa(
	const vessel & vessel1, const vessel & vessel2);
}
}

#endif

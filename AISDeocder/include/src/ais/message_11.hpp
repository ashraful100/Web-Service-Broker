#ifndef src__AIS__MESSAGE_11__HPP
#define src__AIS__MESSAGE_11__HPP

#include <src/ais/message_04.hpp>
#include <src/geo/angle.hpp>

namespace src
{
namespace ais
{
/// @brief UTC/Date Response
class message_11 : public message_04
{
	friend class detail::factory;

public:
	constexpr static const message_id ID = message_id::utc_and_date_response;

	message_11();
	message_11(const message_11 &) = default;
	message_11 & operator=(const message_11 &) = default;
	message_11(message_11 &&) = default;
	message_11 & operator=(message_11 &&) = default;

protected:
	message_11(const raw & bits);
};
}
}

#endif

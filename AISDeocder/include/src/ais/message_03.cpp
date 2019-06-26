#include "message_03.hpp"

namespace src
{
namespace ais
{
message_03::message_03()
	: message_01(ID)
{
}

message_03::message_03(const raw & bits)
	: message_01(ID)
{
	if (bits.size() != SIZE_BITS)
		throw std::invalid_argument{"invalid number of bits in ais/message_03"};
	read_data(bits);
}
}
}

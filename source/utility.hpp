
#include <cmath>

#define _(__a__) \
	(static_cast<unsigned int>(__a__))
#define __(__a__) \
	(static_cast<int>(__a__))

#define $(__a__) \
	(static_cast<float>(__a__))

namespace e3
{
namespace utility
{

static inline constexpr float degress_to_radians(float degress)
{
	return degress * M_PI / 180;
}

}
}

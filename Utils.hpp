#ifndef UTILS_HPP
#define UTILS_HPP

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
#include <random>
typedef std::mt19937 mt19937;
#else
#include <boost/random.hpp>
typedef boost::random::mt19937 mt19937;
#endif


namespace Utils
{

	extern mt19937 mersenne;


	inline int randomInt() {
		return (mersenne() & 0x7FFFFFFF);
	}

	inline void seedRandom(int seed)
	{
		mersenne.seed(seed);
	}

	//Returns random float in [min, max)
	inline float randomFloat(float min, float max)
	{
		return (randomInt() / float(0x7FFFFFFF))*(max-min)+min;
	}

	//Returns random between [min, max]
	inline int randomInt(int min, int max) {
		if (min >= max) return min;
		return (randomInt()%((max-min)+1)) + min;
	}


	inline bool randomBool(int prob)
	{
		return randomInt(0, 99) < prob;
	}
	inline float toRad(float deg)
	{
		return deg/180.0*M_PI;
	}

	inline float toDeg(float rad)
	{
		return rad*180.0/M_PI;
	}

}

#endif // UTILS_HPP

/*!
 * @version $Id: infinimapplugin.h 40 2005-12-29 01:14:28Z mwolf $
 */
#ifndef LWPP_MATH_H
#define LWPP_MATH_H

#include <lwpp/debug.h>
#include <sstream>
#include <iomanip>

#ifndef PI
 #define PI     (3.14159265358979324)
 #define PI_f     (3.14159265358979324f)
#endif
#ifndef TWOPI
 #define TWOPI  (2.0 * PI)
#endif
#ifndef FOURPI
 #define FOURPI  (4.0 * PI)
#endif

#ifndef HALFPI
 #define HALFPI (0.5 * PI)
#endif

#define _doublemagicroundeps	      (.5-1.4e-11) //almost .5f = .5f - 1e^(number of exp bit)
#if (defined(_WIN32) & !defined(_WIN64))
#define FAST_INT
#endif

namespace lwpp {

	//! Fast rounding of a double to an integer
	inline int Round2Int(double val)
	{
		#ifdef _MACOS
			return (int) val;
		#endif
		#ifdef FAST_INT
		#define _doublemagic			double (6755399441055744.0)	//2^52 * 1.5,  uses limited precision to floor
			val		= val + _doublemagic;
			return ((long*)&val)[0];
		#else
			return int (val+_doublemagicroundeps);
		#endif
	}
	//! Fast double to integer conversion
	inline int Double2Int(double val) 
	{
		#ifdef _MACOS
			return (int) val;
		#endif
		#ifdef FAST_INT
			return (val<0) ?  Round2Int(val+_doublemagicroundeps) :	 Round2Int(val-_doublemagicroundeps);
		#else
			return (int)val;
		#endif
	}
/*
	template<class Type>
	Type BiLerp(double wx, double wy, Type d00, Type d10, Type d01, Type d11)
	{
		double iwx = 1.0 - wy;
		double iwy = 1.0 - wy;

		return (Type) (iwx * iwy * d00 +
										wx * iwy * d10 +
									 iwx *  wy * d01 +
										wx *  wy * d11);
	}
	
	template<>
	unsigned char BiLerp<unsigned char>(double wx, double wy, unsigned char d00, unsigned char d10, unsigned char d01, unsigned char d11)
	{
		double iwx = 1.0 - wy;
		double iwy = 1.0 - wy;

		return (unsigned char) (iwx * iwy * d00 +
										wx * iwy * d10 +
									 iwx *  wy * d01 +
										wx *  wy * d11);
	}
*/
	//! Clamp a value between min and max
	/*!
	 *
	 */
	template<class Type>
	Type Clamp(const Type value, const Type min = 0, const Type max = 1)
	{
		return (value < min) ? min : (value > max) ? max : value;
	}

	template<class Type>
	Type SmoothStep(Type value, const Type min, const Type max)
	{
		if (value >= max) return 1;
		value = (value - min)/(max-min);
		return (value * value * (3 - 2 * value));
	}

	template<class Type>
	Type SmoothStep(const Type value)
	{
		if (value >= 1) return 1;
		return (value * value * (3 - 2 * value));
	}

	template<class Type>
	Type Step(const Type a, const Type x)
	{
		return static_cast<Type>(x >= a);
	}

	template<class Type>
	int Mod(const Type a, const Type b)
	{
		Type n = (int)(a/b);
		int r = a - n*b;
		if (r < 0) r += (int)b;
		return r;
	}

	//! Degrees to radians conversion.
	template <class Type>
	constexpr Type Radians(const Type deg)
	{
		return deg * static_cast<Type>(0.017453292519943295769236907684886);
	}
	//! Radians to degree conversion.
	template <class Type>
	constexpr Type Degrees(const Type rad)
	{
		return rad * static_cast<Type>(57.2957795130823208767981548141052);
	}

	//! Maximum
	template <class Type>
	Type Max(Type a, Type b)
	{
		return (a > b ? a : b);
	}

	template <class Type>
	Type Max(const Type v[3])
	{
		return Max(v[0], Max(v[1], v[2]));
	}

	//! Minimum
	template <class Type>
	Type Min(const Type a, const Type b)
	{
		return (a < b ? a : b);
	}

	template <class Type>
	Type Min(const Type v[3])
	{
		return Min(v[0], Min(v[1], v[2]));
	}

	//! Minimum
	template <class Type>
	Type Sqr(const Type a)
	{
		return a*a;
	}

	template <class Type>
	Type log2(const Type a)
	{
		return log(a) / log((Type)2);
	}

	template <class Type>
	Type Range(const Type x, const Type min, const Type max) 
	{
		return (x * (max - min) + min);
	}

	template <class Type>
	Type Bounds(const Type x, const Type min, const Type max)
	{
		return Max(Min(x, max), min);
	}

	//! return true if the value is within the range, including the bounds
	template <class Type>
	bool inRange(const Type x, const Type min, const Type max) 
	{
		return (x >= min) && (x <= max);
	}

	template <class Type>
	Type Normalize(const Type x, const Type min, const Type max) 
	{
		return (x - min) / (max - min);
	}

	template <class Type>
	int Floor(const Type x) {
		return static_cast<int>(std::floor(x));
		//return (static_cast<int> (x)) - (x < 0 && (x != static_cast<int>(x)));
	}

	template <class Type>
	int Ceil(const Type x) 
	{
		return static_cast<int>(std::ceil(x));
		//return ((int)x) + (x > 0 && (x != (int) x));
	}

	template <class Type>
	Type Frac(const Type x)
	{
		Type i;
		return std::modf(x, &i);
	}

	template <class Type>
	int Round(const double r) 
	{
		return (r > 0.0) ? Floor(r + 0.5) : Ceil(r - 0.5);
	}

	inline int Pow2(const int n) {
		return 1 << n;
	}

	template <class T> T Lerp (const double f, const T a, const T b)
	{
		const double g = 1.0 - f;
		return T( a * g + b * f  );
	}

	template <class T> T Bias(const T val, const T bias)
	{
		return (pow(val, log(bias) / log(0.5)));
	}

	template <class T> T Gain(const T val, const T gain)
	{
		if (val < 0.5)
			return Bias(1.0 - val, 2.0 * gain) * 0.5;
		else
			return 1.0 - Bias(1.0 - val, 2.0 - 2.0 * gain) * 0.5;
	}

	template <class T> void Swap (T &a, T &b)
	{
		T temp(a);
		a = b;
		b = temp;
	}

	/*!
	 * Support function for doing cylindrical mapping
	 * @todo insert atan2(x,y) function
	 */
	inline double xztoh (const double x, const double z)
	{
		return atan2(x , z);
	}

	/*!
	 * Support function for doing spherical mapping
	 */
	inline void xyztohp (const double x, const double y, const double z, double &h, double &p)
	{
		h = xztoh(x, z);
		p = atan2(y, sqrt (Sqr(x) + Sqr(z)));
	}

	template <class T> T SignOf (const T a) 
	{
		return (a > static_cast<T>(0)) ? static_cast<T>(1) : static_cast<T>(-1);
	}

	// Colour
	template <typename T>
	T Colour2Luma(T *rgb)
	{
		//return  (rgb[0] * 0.2126 + rgb[1] * 0.7152 + rgb[2] * 0.0722);
		return  (rgb[0] + rgb[1] + rgb[2]) / 3.0f;
	}

  class LengthUnits
  {
  public:
    virtual std::string operator() (double value) = 0;
  }; 

  class SIUnits : LengthUnits
  {
    std::ostringstream stream;
    const char* unitStr[5] = { "nm", "mm", "m", "km", "Mm" };
  public:
    virtual std::string operator() (double value) override
    {
      std::ostringstream().swap(stream);
      int unit = 2;
      if (fabs(value) > std::numeric_limits<double>::epsilon())
      {
        while ((fabs(value) < 1.0) && unit)
        {
          value *= 1000.0;
          --unit;
        }
        while ((fabs(value) >= 1000.0) && (unit < 4))
        {
          value /= 1000.0;
          ++unit;
        }
      }
      stream << std::fixed << std::setprecision(((unit != 2) ? 3 : 4));
      stream << value << " " << unitStr[unit];
      return stream.str();
    }
  };

  class ImperialUnits : LengthUnits
  {
    std::ostringstream stream;
  public:
    virtual std::string operator() (double value) override
    {
      std::ostringstream().swap(stream);

      auto imp = value * 3.281;
      auto feet = lwpp::Floor(imp);
      auto rem = imp - feet;
      auto inches = rem * 12;

      stream << std::fixed;
      if (feet != 0.0)
        stream << feet << "\' ";
      stream << inches << "\" "; 
      
      //std::setprecision((unit != 2 ? 3 : 4));
      //stream << value << " " << unitStr[unit];
      return stream.str();
    }
  };

} // end namespace lwpp

#endif // LWPP_MATH_H

#ifndef Point3_H
#define Point3_H

#include <lwpp/Vector3d.h>

#ifdef _DEBUG
#ifdef _MSWIN
#pragma warning (push,1)
#include <Windows.h>
#include <ostream>
#include <sstream>
#include <string>
#pragma warning (pop)
#pragma warning(disable : 4996)
#pragma warning(disable : 4530)
#else // OSX
#include <ostream>
#include <sstream>
#include <string>
#endif // _MSWIN
#endif // _DEBUG

namespace lwpp
{
	/*!
	 * A Point3 describes a location in 3D space as denoted by x,y,z
	 */
	//! @ingroup Helper
	template <typename T>
	class Point3 {
		public:
			double x, y, z; //!< Storage for the vector components, uses &x for LW compatibility
		public:
			//! Default Constructor
			/*!
			 *@param n value to set all components to.
			 */
			explicit Point3 (T n = 0.0) : x(n), y(n), z(n) {}
			//! Constructor with all three components
			/*!
			 * @param _x first component
			 * @param _y second component
			 * @param _z third component
			 */
			Point3 (T _x, T _y, T _z) :  x(_x), y(_y), z(_z) {}
			//! Construct with an array of doubles.
			/*!
			 * @param n array of 3 doubles to initialize to.
			 */
			Point3 (const T n[3]) :  x(n[0]), y(n[1]), z(n[2]) {}
			//! Construct from a vector, basically describing a point in a unit sphere
			explicit Point3 (const Vector3<T> &v) :  x(v.x), y(v.y), z(v.z) {}
			T *asLWVector() {return &x;}

			//! Add a Vector3d
			Point3 operator+(const Vector3<T> &v) const 
			{
				return Point3(x + v.x, y + v.y, z + v.z);
			}
			Point3 &operator+=(const Vector3<T> &v)
			{
				x += v.x;
				y += v.y;
				z += v.z;
				return *this;
			}
			//! Subtract a Vector3d
			Point3 operator-(const Vector3<T> &v) const 
			{
				return Point3(x - v.x, y - v.y, z - v.z);
			}
			Point3 &operator-=(const Vector3<T> &v)
			{
				x -= v.x;
				y -= v.y;
				z -= v.z;
				return *this;
			}

			//! Subtract a Point3
			Vector3<T> operator-(const Point3 &p) const 
			{
				return Vector3<T>(x - p.x, y - p.y, z - p.z);
			}

			Point3 &operator*=(const T d)  
			{
				x *= d; y *= d; z*= d;
				return *this;
			}

			Point3 &operator*=(const Vector3<T> &v)  
			{
				x *= v.x;
				y *= v.y;
				z *= v.z;
				return *this;
			}

			Point3 operator*(const T d) const 
			{
				return Point3(x * d, y * d, z * d);
			}

			Point3 operator/(const T d) const 
			{
				assert (1 != 0);
				double id = 1.0 / d;
				return (*this * id);
			}

			Point3 &operator/=(const T d)  
			{
				assert (1 != 0);
				double id = 1.0 / d;
				*this *= id;
				return *this;
			}
			Point3 &operator/=(const Vector3<T>&v)  
			{
				x /= v.x;
				y /= v.y;
				z /= v.z;
				return *this;
			}
			inline T Magnitude() const {return sqrt(Sqr(x) + Sqr(y) + Sqr(z));}

	};
	typedef Point3<double> Point3d;
	typedef Point3<float> Point3f;
	/*!
	 * Compute the distance between two points
	 * @relates Point3
	 */
	template <typename T>
	inline T Distance(const Point3<T> &p1, const Point3<T> &p2)
	{
		return (p1 - p2).Magnitude();
	}

	template<typename T>
	inline Vector3<T>::Vector3 (const Point3<T> &p) :  x(p.x), y(p.y), z(p.z) {}
}


#ifdef _DEBUG
	template<typename T>
	inline std::ostream &operator<<(std::ostream &os, const lwpp::Vector3<T> &t)
	{
		//t.m->Print(os);
		os << " <" << t.x << "," << t.y << "," << t.z << "> ";
		return os;
	}

	template<typename T>
	inline std::ostream &operator<<(std::ostream &os, const lwpp::Point3<T> &t)
	{
		//t.m->Print(os);
		os << " <" << t.x << "," << t.y << "," << t.z << "> ";
		return os;
	}

#endif // _DEBUG

#endif // Point3_H

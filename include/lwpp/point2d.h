#ifndef Point2_H
#define Point2_H

#include <lwpp/Vector2d.h>

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
	class Point2 {
		public:
				double x, y; //!< Storage for the vector components, uses &x for LW compatibility
		public:
				//! Default Constructor
				/*!
				 *@param n value to set all components to.
				 */
				explicit Point2 (T n = 0.0) : x(n), y(n) {}
				//! Constructor with all three components
				/*!
				 * @param _x first component
				 * @param _y second component
				 */
				Point2 (T _x, T _y) :  x(_x), y(_y) {}
				//! Construct with an array of doubles.
				/*!
				 * @param n array of 3 doubles to initialize to.
				 */
				Point2 (const T n[2]) :  x(n[0]), y(n[1])) {}
				//! Construct from a vector, basically describing a point in a unit sphere
				explicit Point2 (const Vector2<T> &v) :  x(v.x), y(v.y) {}
				
				//! Compare a Point2
				inline bool operator== (const Point2<T> &b) const
				{
						return (x == b.x && y == b.y );
				}
				inline bool operator!= (const Point2<T> &b) const
				{
						return !(*this==b);
				}
				
				//! Add a Vector3d
				Point2 operator+(const Vector2<T> &v) const
				{
						return Point3(x + v.x, y + v.y);
				}
				Point2 &operator+=(const Vector2<T> &v)
				{
						x += v.x;
						y += v.y;
						return *this;
				}
				//! Subtract a Vector3d
				Point2 operator-(const Vector2<T> &v) const
				{
						return Point2(x - v.x, y - v.y);
				}
				Point2 &operator-=(const Vector2<T> &v)
				{
						x -= v.x;
						y -= v.y;
						return *this;
				}
				
				//! Add a Point3
				Point2 operator+(const Point2<T> &b) const
				{
						return Point2<T>(x + b.x, y + b.y);
				}
				Point2 &operator+=(const Point2<T> &b)
				{
						x += b.x;
						y += b.y;
						return *this;
				}
				//! Subtract a Point3
				Vector2<T> operator-(const Point2 &p) const
				{
						return Vector3<T>(x - p.x, y - p.y);
				}
				Point2 &operator-=(const Point2 &b)
				{
						x -= b.x;
						y -= b.y;
						return *this;
				}
				Point2 &operator*=(const T d)
				{
						x *= d; y *= d;
						return *this;
				}
				
				Point2 &operator*=(const Vector2<T> &v)
				{
						x *= v.x;
						y *= v.y;
						return *this;
				}
				
				Point2 operator*(const T d) const
				{
						return Point3(x * d, y * d);
				}
				//! Mutual component multiplication
				Point2 operator*(const Point2 &p) const
				{
						return Point3(x*p.x, y*p.y);
				}
				
				Point2 operator/(const T d) const
				{
						assert (1 != 0);
						double id = 1.0 / d;
						return (*this * id);
				}
				
				Point2 &operator/=(const T d)
				{
						assert (1 != 0);
						double id = 1.0 / d;
						*this *= id;
						return *this;
				}
				Point2 &operator/=(const Vector2<T>&v)
				{
						x /= v.x;
						y /= v.y;
						return *this;
				}
				inline T& operator[](unsigned int index)
				{
						if(index > 1)
								throw std::out_of_range("Index supplied to Point3::operator[] is out of range.");
								
				switch(index)
						{
				case 0:	return x;
				case 1:	return y;
						}
				}
				inline T Magnitude() const {return sqrt(Sqr(x) + Sqr(y));}
				
	};
	typedef Point2<double> Point2d;
	typedef Point2<float> Point2f;
	/*!
	 * Compute the distance between two points
	 * @relates Point3
	 */
	template <typename T>
	inline T Distance(const Point2<T> &p1, const Point2<T> &p2)
	{
		return (p1 - p2).Magnitude();
	}
		
	template<typename T>
	inline Vector2<T>::Vector2 (const Point2<T> &p) :  x(p.x), y(p.y){}
}


#ifdef _DEBUG
template<typename T>
inline std::ostream &operator<<(std::ostream &os, const lwpp::Vector2<T> &t)
{
		//t.m->Print(os);
		os << " <" << t.x << "," << t.y << "> ";
		return os;
	}
		
	template<typename T>
	inline std::ostream &operator<<(std::ostream &os, const lwpp::Point2<T> &t)
	{
		//t.m->Print(os);
		os << " <" << t.x << "," << t.y << "> ";
		return os;
	}
		
#endif // _DEBUG
		
#endif // Point2_H

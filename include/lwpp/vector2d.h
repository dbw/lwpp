/*! @file 
 * @brief Base classes for vectors and vector math
 * @version $Id: infinimapplugin.h 40 2005-12-29 01:14:28Z mwolf $
 */
#ifndef Vector2_H
#define Vector2_H

#include "lwpp/math.h"

namespace lwpp
{

#ifndef PI
 #define PI     (3.14159265358979324)
#endif
#ifndef HALFPI
 #define HALFPI (PI*0.5)
#endif
#ifndef TWOPI
 #define TWOPI (PI*2.0)
#endif
/* trying to force a commit*/
	template <typename T>	class Point2;

	//! Base Class for Vector, will later on be a template, currently uses doubles only
	//! @ingroup Helper
	template <typename T>
	class Vector2 {
		public:
			T x, y; //!< Storage for the vector components, use &x for LW compatibility
		public:
			//! Default Constructor
			/*!
			 *@param n value to set all components to.
			 */
			explicit Vector2 (T n = 0.0) 
				: x(n), y(n) {}
			//! Constructor with all three components
			/*!
			 * @param _x first component
			 * @param _y second component
			 * @param _z third component
			 */
			Vector2 (T _x, T _y) 
				: x(_x), y(_y) {};
			//! Construct with an array of doubles.
			/*!
			 *@param n array of 3 doubles to initialize to.
			 */
			explicit Vector2 (const T n[2]) 
				: x(n[0]), y(n[1]) {}
			explicit Vector2 (const Point2<T> &p);
			/*
			//! Construct from radial angles
			Vector3 (T theta, T phi)
				: x( -sin(theta) * cos(phi) ),
					y( -sin(theta) * sin(phi) ),
					z( cos(theta) )
			{}
			*/

			//! Return the Length/Magnitude of the Vector3
			inline T Magnitude() const {return sqrt(Sqr(x) + Sqr(y));}
			//! Normalize the Vector2
			inline Vector2 &Normalize();

			T &X() {return x;}
			T &Y() {return y;}

			inline void Translate (Vector2 dir, T distance) {
				x += dir.x * distance;
				y += dir.y * distance;
			}
			/// operator ///
			//! Compare a Vector2
			inline bool operator== (Vector2 b) const
						{
										return (x == b.x && y == b.y );
						}
						inline bool operator!= (Vector2 b) const
						{
										return !(*this==b);
						}
			//! Add a Vector2 
			inline Vector2& operator+= (Vector2 b) {
				x += b.x;
				y += b.y;			
				return *this;
			}
			//! Add a Vector2
			inline Vector2 operator+ (Vector2 b)
			{
				return Vector2 (x + b.x, y + b.y);
			}
			//! Subtract a Vector2
			inline Vector2& operator-= (Vector3 b) {
				x -= b.x;
				y -= b.y;
				return *this;
			}
			//! Subtract a Vector3
			inline Vector2 operator- (Vector2 b)
			{
				return Vector2 (x - b.x, y - b.y);
			}
			inline Vector2 operator- (const Vector2& b) const
			{
				return Vector2(x - b.x, y - b.y);
			}

			inline Vector2& operator*= (Vector2 b) {
				x *= b.x;
				y *= b.y;
				return *this;
			}
			inline Vector2 operator* (Vector2 b) 
			{
				return Vector2 (x * b.x, y * b.y);
			}

			inline Vector2& operator*= (T b)
			{
				x *= b;
				y *= b;
				return *this;
			}
			inline Vector2 operator* (double b)
			{
				return Vector2 (x * b, y * b);
			}

			inline Vector2& operator/= (Vector2 b)
			{
				x /= b.x;
				y /= b.y;
				return *this;
			}
			inline Vector2 operator/ (Vector2 b)
			{
				return Vector2(x / b.x, y / b.y);
			}
			inline Vector2& operator/= (T b) {
				T ib = 1.0 / b;
				x *= ib;
				y *= ib;
				return *this;
			}
			inline Vector2 operator/ (T b)
			{
				T ib = 1.0 / b;
				return Vector3 (x * ib, y * ib);
			}
			inline T& operator[](unsigned int index)
			{
				if(index > 1) 
					throw std::out_of_range("Index supplied to Vector2::operator[] is out of range.");

				switch(index)
				{
				case 0:	return x;
				case 1:	return y;
				}
			}

			inline void Abs()
			{
				x = x > 0 ? x : -x;
				y = y > 0 ? y : -y;
			}
	};

	template <typename T>
	inline Vector2<T> &Vector2<T>::Normalize()
	{
		*this /= Magnitude();
		return *this;
	}

	typedef Vector2<double> Vector2d;
	typedef Vector2<float> Vector2f;

} // end namespace lwpp
#endif // Vector3_H
/*! @file 
 * @brief Base classes for vectors and vector math
 * @version $Id: infinimapplugin.h 40 2005-12-29 01:14:28Z mwolf $
 */
#ifndef Vector3_H
#define Vector3_H

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
	template <typename T>	class Point3;

	//! Base Class for Vector, will later on be a template, currently uses doubles only
	//! @ingroup Helper
	template <typename T>
	class Vector3 {
		public:
			mutable T x, y, z; //!< Storage for the vector components, use &x for LW compatibility
		public:
			//! Default Constructor
			/*!
			 *@param n value to set all components to.
			 */
			explicit Vector3 (T n = 0.0) 
				: x(n), y(n), z(n) {}
			//! Constructor with all three components
			/*!
			 * @param _x first component
			 * @param _y second component
			 * @param _z third component
			 */
			Vector3 (T _x, T _y, T _z) 
				: x(_x), y(_y), z(_z) {};
			//! Construct with an array of doubles.
			/*!
			 *@param n array of 3 doubles to initialize to.
			 */
			explicit Vector3 (const T n[3]) 
				: x(n[0]), y(n[1]), z(n[2]) {}
			template <typename S>
			explicit Vector3(const Vector3<S> &v)
				: x(v.x), y(v.y), z(v.z) {}
			explicit Vector3 (const Point3<T> &p);
			//! Construct from radial angles
			Vector3 (T theta, T phi)
				: x( -sin(theta) * cos(phi) ),
					y( -sin(theta) * sin(phi) ),
					z( cos(theta) )
			{}

			inline T *asLWVector() const {return &x;}
			//! Get the Dot product
			inline T Dot(Vector3 a) const;
			inline Vector3 Cross(Vector3 a) const;
			//! Return the Length/Magnitude of the Vector3
			inline T Magnitude() const {return sqrt(Sqr(x) + Sqr(y) + Sqr(z));}
			//! Normalize the Vector3
			inline Vector3 &Normalize();
			inline Vector3 getNormalized();

			T &X() {return x;}
			T &Y() {return y;}
			T &Z() {return z;}

			inline void Translate (const Vector3 dir, const T distance) {
				x += dir.x * distance;
				y += dir.y * distance;
				z += dir.z * distance;
			}
			/// operator ///
			//! Compare a Vector3 
			inline bool operator== (const Vector3 b) const
			{
							return (x == b.x && y == b.y && z == b.z );
			}
			inline bool operator!= (const Vector3 b) const
			{
							return !(*this==b);
			}
			//! Add a Vector3 
			inline Vector3& operator+= (const Vector3 b) {
				x += b.x;
				y += b.y;
				z += b.z;
				return *this;
			}
			inline Vector3& operator+= (const T v)
			{
				x += v;
				y += v;
				z += v;
				return *this;
			}
			//! Add a Vector3
			inline Vector3 operator+ (const Vector3& b) const
			{
				return Vector3 (x + b.x, y + b.y, z + b.z);
			}
			inline Vector3 operator+ (const T b) const
			{
				return Vector3(x + b, y + b, z + b);
			}

			//! Subtract a Vector3
			inline Vector3& operator-= (const Vector3& b) {
				x -= b.x;
				y -= b.y;
				z -= b.z;
				return *this;
			}
			//! Subtract a scalar
			inline Vector3& operator-= (const T v)
			{
				x -= v;
				y -= v;
				z -= v;
				return *this;
			}
			//! Subtract a Vector3
			inline Vector3 operator- (const Vector3& b) const
			{
				return Vector3 (x - b.x, y - b.y, z - b.z);
			}

			inline Vector3 operator- (const T b) const
			{
				return Vector3(x - b, y - b, z - b);
			}

			inline Vector3& operator*= (const Vector3 &b) {
				x *= b.x;
				y *= b.y;
				z *= b.z;
				return *this;
			}
			inline Vector3 operator* (const Vector3 &b) const
			{
				return Vector3 (x * b.x, y * b.y, z * b.z);
			}

			inline Vector3& operator*= (const T b)
			{
				x *= b;
				y *= b;
				z *= b;
				return *this;
			}
			inline Vector3 operator* (const double b) const
			{
				return Vector3 (x * b, y * b, z * b);
			}

			inline Vector3& operator/= (const Vector3 &b)
			{
				x /= b.x;
				y /= b.y;
				z /= b.z;
				return *this;
			}
			inline Vector3 operator/ (const Vector3 &b) const
			{
				return Vector3 (x / b.x, y / b.y, z / b.z);
			}
			inline Vector3& operator/= (const T b) {
				T ib = 1.0 / b;
				x *= ib;
				y *= ib;
				z *= ib;
				return *this;
			}
			inline Vector3 operator/ (const T b) const
			{
				T ib = 1.0 / b;
				return Vector3 (x * ib, y * ib, z * ib);
			}
			inline T& operator[](const unsigned int index) const
			{
				assert(index < 3);
				
				if(index > 2) 
					throw std::out_of_range("Index supplied to Vector3::operator[] is out of range.");

				switch(index)
				{
				case 0:	return x;
				case 1:	return y;
				case 2:	return z;
				default: return x;
				}
			}

			inline T SphericalTheta() const
			{
				return acos(-y);
			}
			inline T SphericalPhi() const
			{
				float p = atan2(x, z);
				return (p < 0.0) ? p + TWOPI : p;
			}			


			/*
				template <class Real>
	Vector<3, Real> axis_angle_to_hpb(const Vector<3, Real>& axis, Real angle)
	{
		Vector<3, Real> hpb;

		Real s = sin(angle);
		Real c = cos(angle);
		Real t = 1 - c;

		hpb[0] = atan2(axis[0] * axis[2] * t + axis[1] * s, 1 - (axis[0] * axis[0] + axis[1] * axis[1]) * t);

		Real sp = axis[0] * s - axis[1] * axis[2] * t;
		if(sp < -1) sp = -1;
		else if(sp > 1) sp = 1;
		hpb[1] = asin(sp);

		hpb[2] = atan2(axis[0] * axis[1] * t + axis[2] * s, 1 - (axis[0] * axis[0] + axis[2] * axis[2]) * t);

		return hpb;
	}

			*/

			inline Vector3 toEuler(const T bank)
			{
				Vector3<T> euler;
				euler.x = HALFPI - atan2(z, x);
				euler.y = HALFPI - acos(-y);
				euler.z = bank;
				return euler;
			}

			inline void Abs()
			{
				x = x > 0 ? x : -x;
				y = y > 0 ? y : -y;
				z = z > 0 ? z : -z;
			}
      inline void doClamp(const T min = 0, const T max = 1)
			{				
				x = lwpp::Clamp(x, min, max);
				y = lwpp::Clamp(y, min, max);
				z = lwpp::Clamp(z, min, max);
			}
			inline Vector3 Clamp(const T min = 0, const T max = 1)
			{
				x = lwpp::Clamp(x, min, max);
				y = lwpp::Clamp(y, min, max);
				z = lwpp::Clamp(z, min, max);
				return *this;
			}

	};


	//! Cross product of two Vector3
	/*!
	 * @param a first Vector3
	 * @param b second Vector3
	 * @return the cross product
	 */
	template <typename T>
	inline Vector3<T> Cross(const Vector3<T> a, const Vector3<T> b) 
	{
		return Vector3<T>(a.y * b.z - a.z * b.y,
											a.z * b.x - a.x * b.z,
											a.x * b.y - a.y * b.x);
	}

	template <typename T>
	inline T Vector3<T>::Dot(const Vector3<T> a) const
	{
		return (x*a.x + y*a.y + z*a.z);
	}

	template <typename T>
	inline T Dot(const Vector3<T> a, const Vector3<T> b)
	{
		return (a.x*b.x + a.y * b.y + a.z * b.z);
	}

	template <typename T>
	inline Vector3<T> Vector3<T>::Cross(const Vector3<T> a) const
	{
		return Vector3<T>( y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x );
	}

	template <typename T>
	inline Vector3<T> &Vector3<T>::Normalize()
	{
		auto m = Magnitude();
		if (m != 0.0) *this /= m;
		return *this;
	}

	template <typename T>
	inline Vector3<T> Vector3<T>::getNormalized()
	{
		Vector3<T> v(*this);
		v.Normalize();
		return v;
	}

	//! Create a coordinate system out of one vector
	template <typename T>
	inline void CoordinateSystem(const Vector3<T> &v1, Vector3<T> *v2, Vector3<T> *v3)
	{
		if (abs(v1.x) > abs(v1.y))
		{
			T invLen = 1.0 / sqrt(v1.x * v1.x + v1.z * v1.z);
			*v2 = Vector3<T>(-v1.z * invLen, 0.0, v1.x * invLen);
		}
		else
		{
			T invLen = 1.0 / sqrt(v1.y * v1.y + v1.z * v1.z);
			*v2 = Vector3<T>(0.0, v1.z * invLen, -v1.y * invLen);
		}
		*v3 = Cross(v1, *v2);
	}
	//! Create a coordinate system out of two vectors, one being the normal, the other the viewing ray
	template <typename T>
	inline void CoordinateSystem(const Vector3<T> &view, const Vector3<T> &normal, Vector3<T> *right, Vector3<T> *forward)
	{
		if (view.Dot(normal) < .1)
		{
			CoordinateSystem(normal, right, forward);
		}
		else
		{
			*right = Cross(view, normal);
			*forward = Cross(*right, normal);
		}
	}

	template <typename T>
	inline Vector3<T> Reflect(const Vector3<T> &I, const Vector3<T> &N)
	{
		return I - N * 2 * Dot(N, I);
	}

	typedef Vector3<double> Vector3d;
	typedef Vector3<float> Vector3f;

} // end namespace lwpp
#endif // Vector3_H
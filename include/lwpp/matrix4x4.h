#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <lwpp/Point3d.h>
#include <limits>
#include <lwmath.h>

namespace lwpp
{
	//! @ingroup Helper
	template <typename T>
	class Matrix4x4
	{
	public:
		T m[4][4];
		//! Set identity matrix
		void SetIdentity() {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
						m[i][j] = 0.0;

			for (int i = 0; i < 4; i++)
				m[i][i] = 1.0;
		}		

		//! Blank Constructor, sets the identity matrix
		Matrix4x4 () {
			SetIdentity();
		}
		//! Construct from 3 coordinate vectors (usually passed from LW)
		Matrix4x4 (const Vector3<T> &right, const Vector3<T> &up, const Vector3<T> &forward)
		{
		
			m[0][0] = right.x;
			m[1][0] = right.y;
			m[2][0] = right.z;
			m[3][0] = 0.0;

			m[0][1] = up.x;
			m[1][1] = up.y;
			m[2][1] = up.z;
			m[3][1] = 0.0;

			m[0][2] = forward.x;
			m[1][2] = forward.y;
			m[2][2] = forward.z;
			m[3][2] = 0.0;

			m[0][3] = m[1][3] = m[2][3] = 0.0;
			m[3][3] = 1.0;
		}

		Matrix4x4 (Vector3<T> &right, Vector3<T> &up, Vector3<T> &forward, Point3<T> &pos)
		{
			m[0][0] = right.x;
			m[0][1] = right.y;
			m[0][2] = right.z;
			m[0][3] = 0.0;

			m[1][0] = up.x;
			m[1][1] = up.y;
			m[1][2] = up.z;
			m[1][3] = 0.0;

			m[2][0] = forward.x;
			m[2][1] = forward.y;
			m[2][2] = forward.z;
			m[2][3] = 0.0;

			m[3][0] = pos.x;
			m[3][1] = pos.y;
			m[3][2] = pos.z;
			m[3][3] = 1.0;
		}

		//! Construct from one axis and an angle
		Matrix4x4(const T angle, Vector3<T> axis)
		{

			T const a = angle;
			T const c = cos(a);
			T const s = sin(a);

			axis.Normalize();
			auto temp = axis * (T(1) - c);

			m[0][0] = c + temp[0] * axis[0];
			m[0][1] = temp[0] * axis[1] + s * axis[2];
			m[0][2] = temp[0] * axis[2] - s * axis[1];

			m[1][0] = temp[1] * axis[0] - s * axis[2];
			m[1][1] = c + temp[1] * axis[1];
			m[1][2] = temp[1] * axis[2] + s * axis[0];

			m[2][0] = temp[2] * axis[0] + s * axis[1];
			m[2][1] = temp[2] * axis[1] - s * axis[0];
			m[2][2] = c + temp[2] * axis[2];

			m[3][0] = 0.0;
			m[3][1] = 0.0;
			m[3][2] = 0.0;

			m[0][3] = 0.0;
			m[1][3] = 0.0;
			m[2][3] = 0.0;
			m[3][3] = 1.0;
		}

		//! Construct from 3 coordinate vectors (usually passed from LW)
		Matrix4x4 (const T v[9]) {

			m[0][0] = v[0];
			m[0][1] = v[1];
			m[0][2] = v[2];
      m[0][3] = 0.0;

			m[1][0] = v[3];
			m[1][1] = v[4];
			m[1][2] = v[5];
      m[1][3] = 0.0;

			m[2][0] = v[6];
			m[2][1] = v[7];
			m[2][2] = v[8];
      m[2][3] = 0.0;

      m[0][3] = m[1][3] = m[2][3] = 0.0;
			m[3][3] = 1.0;

/*
			m[0][0] = v[0];
			m[1][0] = v[1];
			m[2][0] = v[2];

			m[0][1] = v[3];
			m[1][1] = v[4];
			m[2][1] = v[5];

			m[0][2] = v[6];
			m[1][2] = v[7];
			m[2][2] = v[8];
      */
		}

		Matrix4x4(const T v[4][4])
		{
			for (int i = 0; i < 4; ++i)
			{
				m[i][0] = v[i][0];
				m[i][1] = v[i][1];
				m[i][2] = v[i][2];
				m[i][3] = v[i][3];
			}
		}

		Matrix4x4 (const Matrix4x4 &v)
		{
			for (int i = 0; i < 4; ++i)
			{
				m[i][0] = v.m[i][0];
				m[i][1] = v.m[i][1];
				m[i][2] = v.m[i][2];
				m[i][3] = v.m[i][3];
			}
		}

		Matrix4x4 &operator=(const Matrix4x4 &v)
		{
			if (this != &v)
			{
				for (int i = 0; i < 4; ++i)
				{
					m[i][0] = v.m[i][0];
					m[i][1] = v.m[i][1];
					m[i][2] = v.m[i][2];
					m[i][3] = v.m[i][3];
				}
			}
			return *this;
		}

        void asLWMatrix(T mat[4][4]) const {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    mat[i][j] = m[i][j];
        }

		inline void RotateP(T angle)
		{
			T s = sin(angle);
			T c = cos(angle);
			m[1][1] = c;
			// the following two have been swapped
			m[1][2] = s;
			m[2][1] = -s;
			m[2][2] = c;
		}
		inline void RotateH(T angle)
		{
			T s = sin(angle);
			T c = cos(angle);
			m[0][0] = c;
			m[2][0] = s;
			m[0][2] = -s;
			m[2][2] = c;
		}
		inline void RotateB(T angle)
		{
			T s = sin(angle);
			T c = cos(angle);
			m[0][0] = c;
			m[1][0] = -s;
			m[0][1] = s;
			m[1][1] = c;
		}

		inline void SetRotationHPB(T heading, T pitch, T bank)
		{
			T sinH = sin(heading);
			T cosH = cos(heading);
			T sinP = sin(pitch);
			T cosP = cos(pitch);
			T sinB = sin(bank);
			T cosB = cos(bank);
			m[0][0] = cosH * cosB - sinH * sinP * sinB;
			m[1][0] = -cosP * sinB;
			m[2][0] = sinH * cosB + cosH * sinP * sinB;

			m[0][1] = cosH * sinB + sinH * sinP * cosB;
			m[1][1] = cosP * cosB;
			m[2][1] = sinH * sinB + cosH * sinP * cosB;

			m[0][2] = -sinH * cosP;
			m[1][2] = sinP;
			m[2][2] = cosH * cosP;
		}

		inline void SetRotationBHP(T heading, T pitch, T bank)
		{
			T sinH = sin(heading);
			T cosH = cos(heading);
			T sinP = sin(pitch);
			T cosP = cos(pitch);
			T sinB = sin(bank);
			T cosB = cos(bank);
			m[0][0] = cosB * cosH;
			m[1][0] = -sinB * cosH;
			m[2][0] = sinH;

			m[0][1] = sinB * cosP + cosB * sinH * sinP;
			m[1][1] = cosB * cosP - sinH * sinB * sinP;
			m[2][1] = -cosH * sinP;

			m[0][2] = sinB * sinP - cosB * sinH * cosP;
			m[1][2] = cosB * sinP + sinH * sinB * cosP;
			m[2][2] = cosH * cosP;
		}

		Matrix4x4 (T m00, T m01, T m02, T m03,
							 T m10, T m11, T m12, T m13,
							 T m20, T m21, T m22, T m23,
							 T m30, T m31, T m32, T m33)
		{
			m[0][0] = m00;	m[0][1] = m01;	m[0][2] = m02; m[0][3] = m03;
			m[1][0] = m10;	m[1][1] = m11;	m[1][2] = m12; m[1][3] = m13;
			m[2][0] = m20;	m[2][1] = m21;	m[2][2] = m22; m[2][3] = m23;
			m[3][0] = m30;	m[3][1] = m31;	m[3][2] = m32; m[3][3] = m33;
		}
		/*
		For a homogeneous geometrical transformation matrix, you can get the roll, pitch and yaw angles, following the TRPY convention, using the following formulas:

		roll (rotation around z) : atan2(xy, xx)
		pitch (rotation around y) : -arcsin(xz)
		yaw (rotation around x) : atan2(yz,zz)

		where the matrix is defined in the form:

		[
		xx, yx, zx, px;
		xy, yy, zy, py;
		xz, yz, zz, pz;
		0, 0, 0, 1
		]

		Do use the full atan2 function so that you can get values from full trigonometric circle (ie. don't just use atan).
		*/

		Matrix4x4 (T v[4][4])
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m[i][j] = v[i][j];
				}
			}
		}
    
		//! Compute the product of two matrices
		inline Matrix4x4 Mul(const Matrix4x4 &m1)
		{
			T r[4][4];
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					r[i][j] = m[i][0] * m1.m[0][j] +
										m[i][1] * m1.m[1][j] +
										m[i][2] * m1.m[2][j] +
										m[i][3] * m1.m[3][j];
				}
			}
			return Matrix4x4(r);
		}

		//! Compute the product of two matrices
		inline Matrix4x4 operator+(const Matrix4x4 &m1)	const
		{
			T r[4][4];
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					r[i][j] = m[i][j] + m1.m[i][j];
				}
			}
			return Matrix4x4(r);
		}
		inline Matrix4x4 operator-(const Matrix4x4 &m1)	const
		{
			T r[4][4];
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					r[i][j] = m[i][j] - m1.m[i][j];
				}
			}
			return Matrix4x4(r);
		}

		inline bool operator==(const Matrix4x4& m1) const
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					if( m[i][j] != m1.m[i][j])
						return false;
				}
			}
			return true;
		}
		inline bool operator!=(const Matrix4x4& m1) const
		{
			return !(*this == m1);
		}

		//! Compute the product of two matrices
		inline Matrix4x4 operator*(const T v)	 const
		{
			T r[4][4];
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					r[i][j] = m[i][j] * v;
				}
			}
			return Matrix4x4(r);
		}

		//! Gauss-Jordan elimination with partial pivoting
		Matrix4x4 &Inverse() 
		{
			int indxc[4], indxr[4];
			int ipiv[4] = { 0, 0, 0, 0 };
			Matrix4x4 minv(*this);
			//memcpy(minv, m, 4*4*sizeof(double));
			int j;
			for (int i = 0; i < 4; i++) 
			{
				int irow = -1, icol = -1;
				T big = 0.;
				// Choose pivot
				for (j = 0; j < 4; j++) 
				{
					if (ipiv[j] != 1) 
					{
						for (int k = 0; k < 4; k++) 
						{
							if (ipiv[k] == 0) 
							{
								if (abs(minv.m[j][k]) >= big) 
								{
									big = abs(minv.m[j][k]);
									irow = j;
									icol = k;
								}
							}
							else if (ipiv[k] > 1) return *this; // Error("Singular matrix in MatrixInvert");
						}
					}	
				}
				++ipiv[icol];
				// Swap rows _irow_ and _icol_ for pivot
				if (irow != icol) 
				{
					for (int k = 0; k < 4; ++k) Swap(minv.m[irow][k], minv.m[icol][k]);
				}
				indxr[i] = irow;
				indxc[i] = icol;
				if (minv.m[icol][icol] == 0.)	return *this; // Error("Singular matrix in MatrixInvert");
				// Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
				T pivinv = 1.f / minv.m[icol][icol];
				minv.m[icol][icol] = 1.f;
				for (j = 0; j < 4; j++) minv.m[icol][j] *= pivinv;
				// Subtract this row from others to zero out their columns
				for (j = 0; j < 4; j++) 
				{
					if (j != icol) 
					{
						T save = minv.m[j][icol];
						minv.m[j][icol] = 0;
						for (int k = 0; k < 4; k++)
							minv.m[j][k] -= minv.m[icol][k]*save;
					}
				}
			}
			// Swap columns to reflect permutation
			for (j = 3; j >= 0; j--) 
			{
				if (indxr[j] != indxc[j]) 
				{
					for (int k = 0; k < 4; k++)
						Swap(minv.m[k][indxr[j]], minv.m[k][indxc[j]]);
				}
			}
			*this = minv;
			return *this;
		}
		/*!
		* Transform a Point3d with a Matrix4x4
		* @relates Point3d
		*/
		inline Point3<T> operator()(const Point3<T> &v) const
		{
			T xp = v.x*m[0][0] + v.y*m[1][0] + v.z*m[2][0] + m[3][0];
			T yp = v.x*m[0][1] + v.y*m[1][1] + v.z*m[2][1] + m[3][1];
			T zp = v.x*m[0][2] + v.y*m[1][2] + v.z*m[2][2] + m[3][2];

			T wp = v.x*m[0][3] + v.y*m[1][3] + v.z*m[2][3] + m[3][3];

			if (wp > std::numeric_limits<T>::denorm_min())
			{
				return Point3<T>(xp, yp, zp) / wp;
			}
			
			return Point3<T>(xp, yp, zp);
		}

		/*!
		* Quickly Transform a Point3d with a Matrix4x4 (only using 3x3
		* @relates Point3d
		*/
		inline Point3<T> &Mul3x3 (Point3<T> &v) const
		{
			T xp = v.x*m[0][0] + v.y*m[1][0] + v.z*m[2][0];
			T yp = v.x*m[0][1] + v.y*m[1][1] + v.z*m[2][1];
			T zp = v.x*m[0][2] + v.y*m[1][2] + v.z*m[2][2];
			v.x = xp;
			v.y = yp;
			v.z = zp;
			return v;
		}
		/*!
		* Transform a Point3d with a Matrix4x4
		* @relates Point3d
		*/
		inline T operator()(Point3<T> *v) const
		{
			T x = v->x;
			T y = v->y;
			T z = v->z;
			v->x		 = x*m[0][0] + y*m[1][0] + z*m[2][0] + m[3][0];
			v->y		 = x*m[0][1] + y*m[1][1] + z*m[2][1] + m[3][1];
			v->z		 = x*m[0][2] + y*m[1][2] + z*m[2][2] + m[3][2];
			T w			 = x*m[0][3] + y*m[1][3] + z*m[2][3] + m[3][3];
			//if (w > std::numeric_limits<T>::denorm_min())
			if (w != 0.0)
			{
				*v /= w;
			}
			return w;
		}
		/*!
		* Transform a Vector3d with a Matrix4x4
		* @relates Vector3d
		*/
		inline Vector3<T> transform(const Vector3<T> &v) const
		{
			return Vector3<T>( v.x*m[0][0] + v.y*m[1][0] + v.z*m[2][0],
												 v.x*m[0][1] + v.y*m[1][1] + v.z*m[2][1],
												 v.x*m[0][2] + v.y*m[1][2] + v.z*m[2][2]);
		}
		inline Vector3<T> operator()(const Vector3<T> &v) const
		{
			return transform(v);
		}
		inline Point3<T> transform(const Point3<T>& v) const
		{
			return Point3<T>(v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
							  v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
							  v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2]);
		}

		/** this conversion uses conventions as described on page:
*   https://www.euclideanspace.com/maths/geometry/rotations/euler/index.htm
*   Coordinate System: right hand
*   Positive angle: right hand
*   Order of euler angles: heading first, then attitude, then bank
*   matrix row column ordering:
*   [m00 m01 m02]
*   [m10 m11 m12]
*   [m20 m21 m22]*/
		/*
		public final void rotate(matrix  m) {
			// Assuming the angles are in radians.
			if (m.m10 > 0.998) { // singularity at north pole
				heading = Math.atan2(m.m02, m.m22);
				attitude = Math.PI / 2;
				bank = 0;
				return;
			}
			if (m.m10 < -0.998) { // singularity at south pole
				heading = Math.atan2(m.m02, m.m22);
				attitude = -Math.PI / 2;
				bank = 0;
				return;
			}
			heading = Math.atan2(-m.m20, m.m00);
			bank = Math.atan2(-m.m12, m.m11);
			attitude = Math.asin(m.m10);
		}*/

		Vector3<T> toEuler() const
		{
			Vector3<T> ret;
			/*
			if (m[1][2] > 0.999)
			{
				ret.x = atan2(m[0][2], m[2][2]);
				ret.y = -HALFPI;
				ret.z = 0;
			}
			else if (m[1][2] < -0.999)
			{
				ret.x = -atan2(m[0][2], m[2][2]);
				ret.y = HALFPI;
				ret.z = 0;			
			}
			else
			{
				ret.x = atan2(-m[2][0], m[0][0]);
				ret.y = -asin(m[1][2]);
				ret.z = atan2(-m[1][2], m[1][1]);
			}
			*/
			if (m[1][2] > 0.999)
			{
				ret.x = atan2(m[0][1], m[0][0]);
				ret.y = -HALFPI;
				ret.z = 0;
			}
			else if (m[1][2] < -0.999)
			{
				ret.x = -atan2(m[0][1], m[0][0]);
				ret.y = HALFPI;
				ret.z = 0;
			}
			else
			{
				ret.x = atan2(-m[0][0], m[2][0]);
				ret.y = -asin(m[1][0]);
				ret.z = atan2(-m[1][0], m[1][1]);
			}
			return ret;
		}

		Vector3<T> toEuler2()	const
		{			
			auto theta1 = atan2(m[1][2], m[2][2]);
			auto c2 = sqrt(lwpp::Sqr(m[0][0]) + lwpp::Sqr(m[0][1]));
			auto theta2 = atan2(-m[0][2], c2);
			auto s1 = sin(theta1);
			auto c1 = cos(theta1);
			auto theta3 = atan2(s1 * m[2][0] - c1 * m[1][0], c1 * m[1][1] - s1 * m[2][1]);
			return Vector3<T>(theta1, theta2, theta3);
		}
		
		/*
		Vector3<T> toEuler()
		{
			//assert(isRotationMatrix(R));
			Vector3<T> ret;
			float sy = sqrt(lwpp::Sqr(m[0][0]) + lwpp::Sqr(m[1][0]));

			bool singular = sy < 1e-6; // If

			if (!singular)
			{
				ret.x = -atan2(m[2][1], m[2][2]);
				ret.z = atan2(-m[2][0], sy);
				ret.y = atan2(m[1][0], m[0][0]);
			}
			else
			{
				ret.x = -atan2(-m[1][2], m[1][1]);
				ret.z = atan2(-m[2][0], sy);
				ret.y = 0;
			}
			return ret;
		}
		*/
	};                 

	typedef Vector3<double> Vector3d;
	typedef Vector3<float> Vector3f;

	typedef Matrix4x4<double> Matrix4x4d;
	typedef Matrix4x4<float> Matrix4x4f;
}                                 
#endif // MATRIX4X4_H

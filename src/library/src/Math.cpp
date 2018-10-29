#include "library/Math.h"

namespace library
{
	namespace math
	{

		//-------------------------------------------------------------------------
		// Vector2
		//-------------------------------------------------------------------------

		const Vector<2> Vector<2>::Zero(0.f);
		const Vector<2> Vector<2>::One(1.f);

		//-------------------------------------------------------------------------
		// Vector3
		//-------------------------------------------------------------------------

		const Vector<3> Vector<3>::Zero(0.f);
		const Vector<3> Vector<3>::One(1.f);

		const Vector<3> Vector<3>::Forward(0.f, 0.f, -1.f);
		const Vector<3> Vector<3>::Backward(0.f, 0.f, 1.f);
		const Vector<3> Vector<3>::Up(0.f, 1.f, 0.f);
		const Vector<3> Vector<3>::Down(0.f, -1.f, 0.f);
		const Vector<3> Vector<3>::Right(1.f, 0.f, 0.f);
		const Vector<3> Vector<3>::Left(-1.f, 0.f, 0.f);

		//-------------------------------------------------------------------------
		// Vector4
		//-------------------------------------------------------------------------

		const Vector<4> Vector<4>::Zero(0.f);
		const Vector<4> Vector<4>::One(1.f);

		//-------------------------------------------------------------------------
		// Matrix3
		//-------------------------------------------------------------------------

		const Matrix<3> Matrix<3>::Zero(0.f);
		const Matrix<3> Matrix<3>::Identity(
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			0.f, 0.f, 1.f
		);

		//-------------------------------------------------------------------------
		// Matrix4
		//-------------------------------------------------------------------------

		const Matrix<4> Matrix<4>::Zero(0.f);
		const Matrix<4> Matrix<4>::Identity(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		);

	} // namespace math
} // namespace library


#pragma once

namespace library::math
{
//-------------------------------------------------------------------------
// Matrix<3>
//-------------------------------------------------------------------------

constexpr Matrix<3>::Matrix(
	const float _11, const float _12, const float _13,
	const float _21, const float _22, const float _23,
	const float _31, const float _32, const float _33
)
	: _11(_11), _12(_12), _13(_13)
	, _21(_21), _22(_22), _23(_23)
	, _31(_31), _32(_32), _33(_33)
{}

constexpr Matrix<3>::Matrix(const float v)
	: Matrix(
		v, v, v,
		v, v, v,
		v, v, v
	)
{}

constexpr Matrix<3>::Matrix(const Row& r1, const Row& r2, const Row& r3)
	: Matrix(
		r1.x, r1.y, r1.z,
		r2.x, r2.y, r2.z,
		r3.x, r3.y, r3.z
	)
{}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Matrix<4>
//-------------------------------------------------------------------------

constexpr Matrix<4>::Matrix(
	const float _11, const float _12, const float _13, const float _14,
	const float _21, const float _22, const float _23, const float _24,
	const float _31, const float _32, const float _33, const float _34,
	const float _41, const float _42, const float _43, const float _44
)
	: _11(_11), _12(_12), _13(_13), _14(_14)
	, _21(_21), _22(_22), _23(_23), _24(_24)
	, _31(_31), _32(_32), _33(_33), _34(_34)
	, _41(_41), _42(_42), _43(_43), _44(_44)
{}

constexpr Matrix<4>::Matrix(const float v)
	: Matrix(
		v, v, v, v,
		v, v, v, v,
		v, v, v, v,
		v, v, v, v
	)
{}

constexpr Matrix<4>::Matrix(const Row& r1, const Row& r2, const Row& r3, const Row& r4)
	: Matrix(
		r1.x, r1.y, r1.z, r1.w,
		r2.x, r2.y, r2.z, r2.w,
		r3.x, r3.y, r3.z, r3.w,
		r4.x, r4.y, r4.z, r4.w
	)
{}

//-------------------------------------------------------------------------

constexpr Matrix<4> Matrix<4>::Scaling(const float x, const float y, const float z)
{
	return Matrix(
		x, 0.f, 0.f, 0.f,
		0.f, y, 0.f, 0.f,
		0.f, 0.f, z, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

constexpr Matrix<4> Matrix<4>::Scaling(const Vector3& vector)
{
	return Matrix(
		vector.x, 0.f, 0.f, 0.f,
		0.f, vector.y, 0.f, 0.f,
		0.f, 0.f, vector.z, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

//-------------------------------------------------------------------------
// arithmetic operators
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// add
//-------------------------------------------------------------------------

template<unsigned Size>
Matrix<Size> operator+(const Matrix<Size>& lhs, const Matrix<Size>& rhs)
{
	return Matrix<Size>(dx::MATRIX(lhs) + dx::MATRIX(rhs));
}

template<unsigned Size>
Matrix<Size>& operator += (Matrix<Size>& lhs, const Matrix<Size>& rhs)
{
	return lhs = dx::MATRIX(lhs) + dx::MATRIX(rhs);
}

template<unsigned Size>
Matrix<Size> operator+(const Matrix<Size>& lhs, const float v)
{
	return lhs + Matrix<Size>(v);
}

template<unsigned Size>
Matrix<Size>& operator += (Matrix<Size>& lhs, const float v)
{
	return lhs = lhs + Matrix<Size>(v);
}

//-------------------------------------------------------------------------
// negate
//-------------------------------------------------------------------------

template <unsigned Size>
Matrix<Size>& operator-(const Matrix<Size>& mat)
{
	return Matrix<Size>(-dx::MATRIX(mat));
}

//-------------------------------------------------------------------------
// subtract
//-------------------------------------------------------------------------

template <unsigned Size>
Matrix<Size>& operator-(const Matrix<Size>& lhs, const Matrix<Size>& rhs)
{
	return Matrix<Size>(dx::MATRIX(lhs) - dx::MATRIX(rhs));
}

template <unsigned Size>
Matrix<Size>& operator-=(Matrix<Size>& lhs, const Matrix<Size>& rhs)
{
	return lhs = dx::MATRIX(lhs) - dx::MATRIX(rhs);
}

template<unsigned Size>
Matrix<Size> operator-(const Matrix<Size>& lhs, const float v)
{
	return lhs - Matrix<Size>(v);
}

template<unsigned Size>
Matrix<Size>& operator -= (Matrix<Size>& lhs, const float v)
{
	return lhs = lhs - Matrix<Size>(v);
}

//-------------------------------------------------------------------------
// multiply
//-------------------------------------------------------------------------

template <unsigned Size>
Matrix<Size> operator*(const Matrix<Size>& lhs, const Matrix<Size>& rhs)
{
	return Matrix<Size>(dx::MATRIX(lhs) * dx::MATRIX(rhs));
}

template <unsigned Size>
Matrix<Size>& operator*=(Matrix<Size>& lhs, const Matrix<Size>& rhs)
{
	return lhs = dx::MATRIX(lhs) * dx::MATRIX(rhs);
}

template<unsigned Size>
Matrix<Size> operator*(const Matrix<Size>& lhs, const float v)
{
	return Matrix<Size>(dx::MATRIX(lhs) * rhs);
}

template<unsigned Size>
Matrix<Size>& operator *= (Matrix<Size>& lhs, const float v)
{
	return lhs = dx::MATRIX(lhs) * rhs;
}

//-------------------------------------------------------------------------
// Vector multiply
//-------------------------------------------------------------------------

template<typename T, unsigned Size>
VectorType<float, Size> operator*(const VectorType<T, Size>& vec, const Matrix<Size>& mat)
{
	const auto dxMatT = dx::XMMatrixTranspose(dx::MATRIX(mat));
	const auto dxVec = dx::VECTOR(vec);

	VectorType<float, Size> result;
	
	for (unsigned i = 0; i < Size; i++)
	{
		result.r[i] = dx::VectorDot<Size>(dxVec, dxMatT.r[i]);
	}

	return result;
}

template<typename T, unsigned Size>
VectorType<float, Size> operator*(const Matrix<Size>& mat, const VectorType<T, Size>& vec)
{
	const auto dxMat = dx::MATRIX(mat);
	const auto dxVec = dx::VECTOR(vec);

	VectorType<float, Size> result;

	for (unsigned i = 0; i < Size; i++)
	{
		result.r[i] = dx::VectorDot<Size>(dxVec, dxMat.r[i]);
	}

	return result;
}

template<unsigned Size>
VectorType<float, Size>& operator*=(VectorType<float, Size>& vec, const Matrix<Size>& mat)
{
	const auto dxMatT = dx::XMMatrixTranspose(dx::MATRIX(mat));
	const auto dxVec = dx::VECTOR(temp);

	for (unsigned i = 0; i < Size; i++)
	{
		vec.r[i] = dx::VectorDot<Size>(dxVec, dxMatT.r[i]);
	}

	return vec;
}

//-------------------------------------------------------------------------
// divide
//-------------------------------------------------------------------------

template <unsigned Size>
Matrix<Size> operator/(const Matrix<Size>& lhs, const float rhs)
{
	return Matrix<Size>(dx::MATRIX(lhs) / rhs);
}

template <unsigned Size>
Matrix<Size>& operator/=(Matrix<Size>& lhs, const float rhs)
{
	return lhs = dx::MATRIX(lhs) / rhs;
}
} // namespace library::math

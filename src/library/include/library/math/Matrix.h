#pragma once
#include "library/Math/Vector.h"

namespace library::math
{
struct Quaternion;

//-------------------------------------------------------------------------
// MatrixDef
//-------------------------------------------------------------------------

template <unsigned Size>
struct MatrixDef
{
	static constexpr unsigned Order = Size;
	static constexpr unsigned RowCount = Size;
	static constexpr unsigned ColCount = Size;

	using Row = Vector<Size>;
	using Col = Vector<Size>;
};

//-------------------------------------------------------------------------
// Matrix3
//-------------------------------------------------------------------------

template <>
struct Matrix<3> : public MatrixDef<3>
{
	union
	{
		struct
		{
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
		};

		std::array<Row, RowCount> _rows;
		std::array<float, RowCount * ColCount> _data;
	};

	//-------------------------------------------------------------------------

	// clang-format off
	constexpr Matrix(
		const float _11, const float _12, const float _13,
		const float _21, const float _22, const float _23,
		const float _31, const float _32, const float _33)
		: _11(_11), _12(_12), _13(_13)
		, _21(_21), _22(_22), _23(_23)
		, _31(_31), _32(_32), _33(_33)
	{}
	// clang-format on

	constexpr Matrix(const Row& r1, const Row& r2, const Row& r3) : _rows{r1, r2, r3} {}

	explicit constexpr Matrix(const float v = 0.f) : Matrix(v, v, v, v, v, v, v, v, v) {}

	// clang-format off
	explicit constexpr Matrix(const XMMatrix3& xmMatrix3)
		: _11(xmMatrix3._11), _12(xmMatrix3._12),_13(xmMatrix3._13)
		, _21(xmMatrix3._21), _22(xmMatrix3._22),_23(xmMatrix3._23)
		, _31(xmMatrix3._31), _32(xmMatrix3._32),_33(xmMatrix3._33)
	{}
	// clang-format on

	explicit constexpr Matrix(const XMMatrix& xmMatrix)
		: Matrix(Row(xmMatrix.r[0]), Row(xmMatrix.r[1]), Row(xmMatrix.r[2]))
	{}

	Matrix& operator=(const XMMatrix& xmMatrix);

	//-------------------------------------------------------------------------

	static const Matrix Zero;
	static const Matrix Identity;

	//-------------------------------------------------------------------------

	const Row& operator[](const unsigned rowIdx) const;
	Row& operator[](const unsigned rowIdx);

	const float& operator()(const unsigned rowIdx, const unsigned colIdx) const;
	float& operator()(const unsigned rowIdx, const unsigned colIdx);

	//-------------------------------------------------------------------------

	const Row& GetRow(const unsigned rowIdx) const;
	void SetRow(const unsigned rowIdx, const Row& row);

	Col GetCol(const unsigned colIdx) const;
	void SetCol(const unsigned colIdx, const Col& col);

	//-------------------------------------------------------------------------

	explicit operator XMMatrix() const;
	explicit operator const XMMatrix3&() const;
	explicit operator const float*() const { return _data.data(); }

	std::string ToString() const;

	//-------------------------------------------------------------------------

	Matrix Transpose() const;
};

//-------------------------------------------------------------------------
// Matrix4
//-------------------------------------------------------------------------

template <>
struct Matrix<4> : public MatrixDef<4>
{
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};

		std::array<Row, RowCount> _rows;
		std::array<float, RowCount * ColCount> _data;
	};

	//-------------------------------------------------------------------------
	// constructors
	//-------------------------------------------------------------------------

	explicit constexpr Matrix(const float value = 0.f)
		: _rows{
			  Row(value),
			  Row(value),
			  Row(value),
			  Row(value),
		  }
	{}

	// clang-format off
	explicit constexpr Matrix(
		const float _11, const float _12, const float _13, const float _14,
		const float _21, const float _22, const float _23, const float _24,
		const float _31, const float _32, const float _33, const float _34,
		const float _41, const float _42, const float _43, const float _44)
		: _11(_11), _12(_12), _13(_13), _14(_14)
		, _21(_21), _22(_22), _23(_23), _24(_24)
		, _31(_31), _32(_32), _33(_33), _34(_34)
		, _41(_41), _42(_42), _43(_43), _44(_44)
	{}
	// clang-format on

	explicit constexpr Matrix(const Row& r1, const Row& r2, const Row& r3, const Row& r4)
		: _rows{r1, r2, r3, r4}
	{}

	// clang-format off
	explicit constexpr Matrix(const XMMatrix4& xmMatrix4)
		: _11(xmMatrix4._11), _12(xmMatrix4._12), _13(xmMatrix4._13), _14(xmMatrix4._14)
		, _21(xmMatrix4._21), _22(xmMatrix4._22), _23(xmMatrix4._23), _24(xmMatrix4._24)
		, _31(xmMatrix4._31), _32(xmMatrix4._32), _33(xmMatrix4._33), _34(xmMatrix4._34)
		, _41(xmMatrix4._41), _42(xmMatrix4._42), _43(xmMatrix4._43), _44(xmMatrix4._44)
	{}
	// clang-format on

	explicit constexpr Matrix(const XMMatrix& xmMatrix)
		: Matrix(Row(xmMatrix.r[0]), Row(xmMatrix.r[1]), Row(xmMatrix.r[2]), Row(xmMatrix.r[3]))
	{}

	Matrix& operator=(const XMMatrix& xmMatrix);

	//-------------------------------------------------------------------------

	static const Matrix Zero;
	static const Matrix Identity;

	//-------------------------------------------------------------------------

	const Row& operator[](const unsigned rowIdx) const;
	Row& operator[](const unsigned rowIdx);

	const float& operator()(const unsigned rowIdx, const unsigned colIdx) const;
	float& operator()(const unsigned rowIdx, const unsigned colIdx);

	//-------------------------------------------------------------------------

	const Row& GetRow(const unsigned rowIdx) const;
	void SetRow(const unsigned rowIdx, const Row& row);

	Col GetCol(const unsigned colIdx) const;
	void SetCol(const unsigned colIdx, const Col& col);

	//-------------------------------------------------------------------------

	explicit operator XMMatrix() const;
	explicit operator const XMMatrix4&() const;
	explicit operator const float*() const { return _data.data(); }

	std::string ToString() const;

	//-------------------------------------------------------------------------

	static Matrix LookAtLH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir);
	static Matrix LookAtRH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir);

	static Matrix LookToLH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir);
	static Matrix LookToRH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir);

	//-------------------------------------------------------------------------

	static Matrix Translation(const Vector3& vector);
	static Matrix Translation(const float x, const float y, const float z);

	//-------------------------------------------------------------------------

	static Matrix PerspectiveFovLH(
		const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ);
	static Matrix PerspectiveFovRH(
		const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ);

	//-------------------------------------------------------------------------

	static Matrix OrthographicLH(const float width, const float height, const float nearZ, const float farZ);
	static Matrix OrthographicRH(const float width, const float height, const float nearZ, const float farZ);

	//-------------------------------------------------------------------------

	static Matrix RotationAxis(const Vector3& axis, const float angle);

	static Matrix RotationPitchYawRoll(const float pitch, const float yaw, const float roll);
	static Matrix RotationPitchYawRoll(const Vector3& vector);

	static Matrix RotationQuaternion(const Quaternion& quaternion);

	static Matrix RotationX(const float angle);
	static Matrix RotationY(const float angle);
	static Matrix RotationZ(const float angle);

	static Matrix AffineTransformation(
		const Vector3& scale,
		const Vector4& rotationOrigin,
		const Vector4& rotationQuat,
		const Vector3& translation);

	//-------------------------------------------------------------------------

	// static Matrix Scaling(const float scale);
	static Matrix Scaling(const float x, const float y, const float z);
	static Matrix Scaling(const Vector3& vector);

	//-------------------------------------------------------------------------

	float GetDeterminant() const;
	Matrix Inverse() const;

	Matrix Transpose() const;

	Vector3 GetForward() const;
	Matrix& SetForward(const Vector3& vector);

	const Vector3& GetUp() const;
	Matrix& SetUp(const Vector3& vector);

	const Vector3& GetRight() const;
	Matrix& SetRight(const Vector3& vector);

	const Vector3& GetTranslation() const;
	Matrix& SetTranslation(const Vector3& vector);
};

//-------------------------------------------------------------------------
// explicit instantiation
//-------------------------------------------------------------------------

extern template struct Matrix<3>;
extern template struct Matrix<4>;

//-------------------------------------------------------------------------
// arithmetic operators
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// add
//-------------------------------------------------------------------------

template <unsigned Size>
inline Matrix<Size>& operator+(const Matrix<Size>& lhs, const Matrix<Size>& rhs)
{
	return Matrix<Size>(XMMatrix(lhs) + XMMatrix(rhs));
}

template <unsigned Size>
inline Matrix<Size>& operator+=(Matrix<Size>& lhs, const Matrix<Size>& rhs)
{
	lhs = Matrix<Size>(XMMatrix(lhs) + XMMatrix(rhs));
	return lhs;
}

//-------------------------------------------------------------------------
// negate
//-------------------------------------------------------------------------

template <unsigned Size>
inline Matrix<Size>& operator-(const Matrix<Size>& mat)
{
	return Matrix<Size>(-XMMatrix(mat));
}

//-------------------------------------------------------------------------
// subtract
//-------------------------------------------------------------------------

template <unsigned Size>
inline Matrix<Size>& operator-(const Matrix<Size>& lhs, const Matrix<Size>& rhs)
{
	return Matrix<Size>(XMMatrix(lhs) - XMMatrix(rhs));
}

template <unsigned Size>
inline Matrix<Size>& operator-=(Matrix<Size>& lhs, const Matrix<Size>& rhs)
{
	lhs = Matrix<Size>(XMMatrix(lhs) - XMMatrix(rhs));
	return lhs;
}

//-------------------------------------------------------------------------
// multiply
//-------------------------------------------------------------------------

template <unsigned Size>
inline Matrix<Size> operator*(const Matrix<Size>& lhs, const Matrix<Size>& rhs)
{
	return Matrix<Size>(XMMatrix(lhs) * XMMatrix(rhs));
}

template <unsigned Size>
inline Matrix<Size>& operator*=(Matrix<Size>& lhs, const Matrix<Size>& rhs)
{
	lhs = Matrix<Size>(XMMatrix(lhs) * XMMatrix(rhs));
	return lhs;
}

//-------------------------------------------------------------------------

template <unsigned Size>
inline Matrix<Size> operator*(const Matrix<Size>& lhs, const float rhs)
{
	return Matrix<Size>(XMMatrix(lhs) * rhs);
}

template <unsigned Size>
inline Matrix<Size>& operator*=(Matrix<Size>& lhs, const float rhs)
{
	lhs = Matrix<Size>(XMMatrix(lhs) * rhs);
	return lhs;
}

//-------------------------------------------------------------------------
// divide
//-------------------------------------------------------------------------

template <unsigned Size>
inline Matrix<Size> operator/(const Matrix<Size>& lhs, const float rhs)
{
	return Matrix<Size>(XMMatrix(lhs) / rhs);
}

template <unsigned Size>
inline Matrix<Size>& operator/=(Matrix<Size>& lhs, const float rhs)
{
	lhs = Matrix<Size>(XMMatrix(lhs) / rhs);
	return lhs;
}
} // namespace library::math

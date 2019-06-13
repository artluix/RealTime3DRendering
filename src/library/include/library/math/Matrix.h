#pragma once
#include "library/Math/Vector.h"

namespace library::math
{
struct Quaternion;

//-------------------------------------------------------------------------
// MatrixDef<Size>
//-------------------------------------------------------------------------

template <unsigned Size>
struct MatrixDef
{
	static constexpr unsigned Order = Size;
	static constexpr unsigned RowCount = Size;
	static constexpr unsigned ColCount = Size;

	using Row = VectorType<float, Size>;
	using Col = VectorType<float, Size>;
};

//-------------------------------------------------------------------------
// Matrix<3>
//-------------------------------------------------------------------------

template <>
struct Matrix<3> : MatrixDef<3>
{
	union
	{
		struct
		{
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
		};

		dx::Matrix<float, 3> _dxMat;
		Row _rows[RowCount];
	};

	//-------------------------------------------------------------------------

	constexpr Matrix(
		const float _11, const float _12, const float _13,
		const float _21, const float _22, const float _23,
		const float _31, const float _32, const float _33
	);
	explicit constexpr Matrix(const float v = 0.f);
	constexpr Matrix(const Row& r1, const Row& r2, const Row& r3);

	//-------------------------------------------------------------------------

	explicit Matrix(const dx::MATRIX& dxMAT);
	Matrix& operator=(const dx::MATRIX& dxMAT);
	explicit operator dx::MATRIX() const;
	explicit operator const float*() const { return &_11; }

	//-------------------------------------------------------------------------

	static const Matrix Zero;
	static const Matrix Identity;

	//-------------------------------------------------------------------------

	const Row& operator[](const unsigned rowIdx) const { return _rows[rowIdx]; }
	Row& operator[](const unsigned rowIdx) { return _rows[rowIdx]; }

	const float& operator()(const unsigned rowIdx, const unsigned colIdx) const { return _rows[rowIdx][colIdx]; }
	float& operator()(const unsigned rowIdx, const unsigned colIdx) { return _rows[rowIdx][colIdx]; }

	//-------------------------------------------------------------------------

	const Row& GetRow(const unsigned rowIdx) const { return _rows[rowIdx]; }
	void SetRow(const unsigned rowIdx, const Row& row) { _rows[rowIdx] = row; }

	Col GetCol(const unsigned colIdx) const;
	void SetCol(const unsigned colIdx, const Col& col);

	//-------------------------------------------------------------------------

	std::string ToString() const;

	//-------------------------------------------------------------------------

	Matrix Transpose() const;
};

//-------------------------------------------------------------------------
// Matrix<4>
//-------------------------------------------------------------------------

template <>
struct Matrix<4> : MatrixDef<4>
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

		dx::Matrix<float, 4> _dxMat;
		Row _rows[RowCount];
	};

	//-------------------------------------------------------------------------
	// constructors
	//-------------------------------------------------------------------------

	constexpr Matrix(
		const float _11, const float _12, const float _13, const float _14,
		const float _21, const float _22, const float _23, const float _24,
		const float _31, const float _32, const float _33, const float _34,
		const float _41, const float _42, const float _43, const float _44
	);
	explicit constexpr Matrix(const float v = 0.f);
	constexpr Matrix(const Row& r1, const Row& r2, const Row& r3, const Row& r4);

	//-------------------------------------------------------------------------

	explicit Matrix(const dx::MATRIX& dxMAT);
	Matrix& operator=(const dx::MATRIX& dxMAT);
	explicit operator dx::MATRIX() const;
	explicit operator const float*() const { return &_11; }

	//-------------------------------------------------------------------------

	static const Matrix Zero;
	static const Matrix Identity;

	//-------------------------------------------------------------------------

	const Row& operator[](const unsigned rowIdx) const { return _rows[rowIdx]; }
	Row& operator[](const unsigned rowIdx) { return _rows[rowIdx]; }

	const float& operator()(const unsigned rowIdx, const unsigned colIdx) const { return _rows[rowIdx][colIdx]; }
	float& operator()(const unsigned rowIdx, const unsigned colIdx) { return _rows[rowIdx][colIdx]; }

	//-------------------------------------------------------------------------

	const Row& GetRow(const unsigned rowIdx) const { return _rows[rowIdx]; }
	void SetRow(const unsigned rowIdx, const Row& row) { _rows[rowIdx] = row; }

	Col GetCol(const unsigned colIdx) const;
	void SetCol(const unsigned colIdx, const Col& col);

	//-------------------------------------------------------------------------

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
		const float fovAngleY,
		const float aspectRatio,
		const float nearZ,
		const float farZ);
	static Matrix PerspectiveFovRH(
		const float fovAngleY,
		const float aspectRatio,
		const float nearZ,
		const float farZ);

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
		const Quaternion& rotationQuat,
		const Vector3& translation);

	//-------------------------------------------------------------------------

	static constexpr Matrix Scaling(const float x, const float y, const float z);
	static constexpr Matrix Scaling(const Vector3& vector);

	//-------------------------------------------------------------------------

	float GetDeterminant() const;

	Matrix4 Inverse() const;

	Matrix4 Transpose() const;

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
// extern template instantiation
//-------------------------------------------------------------------------

extern template struct Matrix<3>;
extern template struct Matrix<4>;

} // namespace library::math

#include "library/Math/Matrix-inl.h"

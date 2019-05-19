#include "StdAfx.h"
#include "library/Math/Matrix.h"

#include "library/Math/Quaternion.h"

#include <iomanip>
#include <sstream>

namespace library::math
{
//-------------------------------------------------------------------------
// Matrix3
//-------------------------------------------------------------------------

	constexpr Matrix<3> Matrix<3>::Zero = Matrix<3>(0.f);
	// clang-format off
	constexpr Matrix<3> Matrix<3>::Identity = Matrix<3>(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f);
	// clang-format on

//-------------------------------------------------------------------------

Matrix<3>& Matrix<3>::operator=(const XMMatrix& xmMatrix)
{
	DirectX::XMStoreFloat3x3(reinterpret_cast<XMMatrix3*>(this), xmMatrix);
	return *this;
}

//-------------------------------------------------------------------------

std::string Matrix<3>::ToString() const
{
	std::ostringstream oss;
	// clang-format off
	oss << std::setprecision(4) <<
		'/' << _11 << ", " << _12 << ", " << _13 << '\\\n' <<
		'|' << _21 << ", " << _22 << ", " << _23 << '|\n' <<
		'\\' << _11 << ", " << _12 << ", " << _13 << '/\n';
	return oss.str();
	// clang-format on
}

Matrix<3>::operator XMMatrix() const
{
	return DirectX::XMLoadFloat3x3(reinterpret_cast<const XMMatrix3*>(this));
}

Matrix<3> Matrix<3>::Transpose() const
{
	return Matrix(DirectX::XMMatrixTranspose(XMMatrix(*this)));
}

//-------------------------------------------------------------------------

const Matrix<3>::Row& Matrix<3>::GetRow(const unsigned rowIdx) const
{
	return _rows[rowIdx];
}

void Matrix<3>::SetRow(const unsigned rowIdx, const Row& row)
{
	_rows[rowIdx] = row;
}

Matrix<3>::Col Matrix<3>::GetCol(const unsigned colIdx) const
{
	return Col(_rows[0][colIdx], _rows[1][colIdx], _rows[2][colIdx]);
}

void Matrix<3>::SetCol(const unsigned colIdx, const Col& col)
{
	for (unsigned i = 0; i < RowCount; i++)
	{
		_rows[i][colIdx] = col[i];
	}
}

//-------------------------------------------------------------------------

Matrix<3>::operator const XMMatrix3&() const
{
	return reinterpret_cast<const XMMatrix3&>(*this);
}

Matrix<3>::Row& Matrix<3>::operator[](const unsigned rowIdx)
{
	return _rows[rowIdx];
}

const Matrix<3>::Row& Matrix<3>::operator[](const unsigned rowIdx) const
{
	return _rows[rowIdx];
}

const float& Matrix<3>::operator()(const unsigned rowIdx, const unsigned colIdx) const
{
	return _rows[rowIdx][colIdx];
}

float& Matrix<3>::operator()(const unsigned rowIdx, const unsigned colIdx)
{
	return _rows[rowIdx][colIdx];
}

//-------------------------------------------------------------------------

// Vector3 operator * (const Vector3& vector, const Matrix3& matrix)
//{
//	return Vector3::Matrix(DirectX::XMVector3Transform(vector, matrix));
//}

// Vector3& operator *= (Vector3& vector, const Matrix3& matrix)
//{
//	vector = Vector3::Matrix(DirectX::XMVector3Transform(vector, matrix));
//	return vector;
//}

//-------------------------------------------------------------------------
// Matrix4
//-------------------------------------------------------------------------

	constexpr Matrix<4> Matrix<4>::Zero = Matrix<4>(0.f);
	// clang-format off
	constexpr Matrix<4> Matrix<4>::Identity = Matrix<4>(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);
	// clang-format on

//-------------------------------------------------------------------------

Matrix<4>& Matrix<4>::operator=(const XMMatrix& xmMatrix)
{
	DirectX::XMStoreFloat4x4(reinterpret_cast<XMMatrix4*>(this), xmMatrix);
	return *this;
}

//-------------------------------------------------------------------------

Matrix<4> Matrix<4>::LookAtLH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir)
{
	return Matrix(DirectX::XMMatrixLookAtLH(XMVector(eyePos), XMVector(focusPos), XMVector(upDir)));
}

Matrix<4> Matrix<4>::LookAtRH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir)
{
	return Matrix(DirectX::XMMatrixLookAtRH(XMVector(eyePos), XMVector(focusPos), XMVector(upDir)));
}

Matrix<4> Matrix<4>::LookToLH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir)
{
	return Matrix(DirectX::XMMatrixLookToLH(XMVector(eyePos), XMVector(eyeDir), XMVector(upDir)));
}

Matrix<4> Matrix<4>::LookToRH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir)
{
	return Matrix(DirectX::XMMatrixLookToRH(XMVector(eyePos), XMVector(eyeDir), XMVector(upDir)));
}

//-------------------------------------------------------------------------

Matrix<4> Matrix<4>::Translation(const Vector3& vector)
{
	return Matrix(DirectX::XMMatrixTranslationFromVector(XMVector(vector)));
}

Matrix<4> Matrix<4>::Translation(const float x, const float y, const float z)
{
	return Matrix(DirectX::XMMatrixTranslation(x, y, z));
}

//-------------------------------------------------------------------------

Matrix<4> Matrix<4>::PerspectiveFovLH(
	const float fovAngleY,
	const float aspectRatio,
	const float nearZ,
	const float farZ)
{
	return Matrix(DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ));
}

Matrix<4> Matrix<4>::PerspectiveFovRH(
	const float fovAngleY,
	const float aspectRatio,
	const float nearZ,
	const float farZ)
{
	return Matrix(DirectX::XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, nearZ, farZ));
}

//-------------------------------------------------------------------------

Matrix<4> Matrix<4>::OrthographicLH(
	const float viewWidth,
	const float viewHeight,
	const float nearZ,
	const float farZ)
{
	return Matrix(DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ));
}

Matrix<4> Matrix<4>::OrthographicRH(
	const float viewWidth,
	const float viewHeight,
	const float nearZ,
	const float farZ)
{
	return Matrix(DirectX::XMMatrixOrthographicRH(viewWidth, viewHeight, nearZ, farZ));
}

//-------------------------------------------------------------------------

Matrix<4> Matrix<4>::RotationAxis(const Vector3& axis, const float angle)
{
	return Matrix(DirectX::XMMatrixRotationAxis(XMVector(axis), angle));
}

Matrix<4> Matrix<4>::RotationPitchYawRoll(const float roll, const float pitch, const float yaw)
{
	return Matrix(DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
}

Matrix<4> Matrix<4>::RotationPitchYawRoll(const Vector3& vector)
{
	return Matrix(DirectX::XMMatrixRotationRollPitchYawFromVector(XMVector(vector)));
}

Matrix<4> Matrix<4>::RotationQuaternion(const Quaternion& quaternion)
{
	return Matrix(DirectX::XMMatrixRotationQuaternion(XMVector(quaternion)));
}

Matrix<4> Matrix<4>::RotationX(const float angle)
{
	return Matrix(DirectX::XMMatrixRotationX(angle));
}

Matrix<4> Matrix<4>::RotationY(const float angle)
{
	return Matrix(DirectX::XMMatrixRotationY(angle));
}

Matrix<4> Matrix<4>::RotationZ(const float angle)
{
	return Matrix(DirectX::XMMatrixRotationZ(angle));
}

Matrix<4> Matrix<4>::AffineTransformation(
	const Vector3& scale,
	const Vector4& rotationOrigin,
	const Vector4& rotationQuat,
	const Vector3& translation)
{
	return Matrix(DirectX::XMMatrixAffineTransformation(
		XMVector(scale),
		XMVector(rotationOrigin),
		XMVector(rotationQuat),
		XMVector(translation)));
}

//-------------------------------------------------------------------------

// Matrix<4> Matrix<4>::Scaling(const float scale)
//{
//	return Scaling(scale, scale, scale);
//}

Matrix<4> Matrix<4>::Scaling(const float x, const float y, const float z)
{
	return Matrix(DirectX::XMMatrixScaling(x, y, z));
}

Matrix<4> Matrix<4>::Scaling(const Vector3& vector)
{
	return Matrix(DirectX::XMMatrixScalingFromVector(XMVector(vector)));
}

//-------------------------------------------------------------------------

std::string Matrix<4>::ToString() const
{
	std::ostringstream oss;
	// clang-format off
	oss << std::setprecision(4) <<
		'/' << _11 << ", " << _12 << ", " << _13 << ", " << _14 << '\\\n' <<
		'|' << _21 << ", " << _22 << ", " << _23 << ", " << _24 << '|\n' <<
		'|' << _31 << ", " << _32 << ", " << _33 << ", " << _34 << '|\n' <<
		'\\' << _11 << ", " << _12 << ", " << _13 << ", " << _14 << '/\n';
	return oss.str();
	// clang-format on
}

//-------------------------------------------------------------------------

float Matrix<4>::GetDeterminant() const
{
	return DirectX::XMVectorGetX(DirectX::XMMatrixDeterminant(XMMatrix(*this)));
}

Matrix<4> Matrix<4>::Inverse() const
{
	return Matrix(DirectX::XMMatrixInverse(nullptr, XMMatrix(*this)));
}

Matrix<4> Matrix<4>::Transpose() const
{
	return Matrix(DirectX::XMMatrixTranspose(XMMatrix(*this)));
}

//-------------------------------------------------------------------------

Vector3 Matrix<4>::GetForward() const
{
	return -_rows[2].xyz;
}

Matrix<4>& Matrix<4>::SetForward(const Vector3& vector)
{
	_rows[2].xyz = -vector;
	return *this;
}

//-------------------------------------------------------------------------

const Vector3& Matrix<4>::GetUp() const
{
	return _rows[1].xyz;
}

Matrix<4>& Matrix<4>::SetUp(const Vector3& vector)
{
	_rows[1].xyz = vector;
	return *this;
}

//-------------------------------------------------------------------------

const Vector3& Matrix<4>::GetRight() const
{
	return _rows[0].xyz;
}

Matrix<4>& Matrix<4>::SetRight(const Vector3& vector)
{
	_rows[0].xyz = vector;
	return *this;
}

//-------------------------------------------------------------------------

const Vector3& Matrix<4>::GetTranslation() const
{
	return _rows[3].xyz;
}

Matrix<4>& Matrix<4>::SetTranslation(const Vector3& vector)
{
	_rows[3].xyz = vector;
	return *this;
}

//-------------------------------------------------------------------------

const Matrix<4>::Row& Matrix<4>::GetRow(const unsigned rowIdx) const
{
	return _rows[rowIdx];
}

void Matrix<4>::SetRow(const unsigned rowIdx, const Row& row)
{
	_rows[rowIdx] = row;
}

Matrix<4>::Col Matrix<4>::GetCol(const unsigned colIdx) const
{
	return Col(_rows[0][colIdx], _rows[1][colIdx], _rows[2][colIdx], _rows[3][colIdx]);
}

void Matrix<4>::SetCol(const unsigned colIdx, const Col& col)
{
	for (unsigned i = 0; i < RowCount; i++)
	{
		_rows[i][colIdx] = col[i];
	}
}

//-------------------------------------------------------------------------

Matrix<4>::operator XMMatrix() const
{
	return DirectX::XMLoadFloat4x4(reinterpret_cast<const XMMatrix4*>(this));
}

Matrix<4>::operator const XMMatrix4&() const
{
	return reinterpret_cast<const XMMatrix4&>(*this);
}

Matrix4::Row& Matrix<4>::operator[](const unsigned rowIdx)
{
	return _rows[rowIdx];
}

const Matrix4::Row& Matrix<4>::operator[](const unsigned rowIdx) const
{
	return _rows[rowIdx];
}

const float& Matrix<4>::operator()(const unsigned rowIdx, const unsigned colIdx) const
{
	return _rows[rowIdx][colIdx];
}

float& Matrix<4>::operator()(const unsigned rowIdx, const unsigned colIdx)
{
	return _rows[rowIdx][colIdx];
}

//-------------------------------------------------------------------------

/*Vector4 operator * (const Vector4& vector, const Matrix4& matrix)
{
	return Vector4::Matrix(DirectX::XMVector4Transform(vector, matrix));
}
*/
/*Vector4& operator *= (Vector4& vector, const Matrix4& matrix)
{
	vector = Vector4::Matrix(DirectX::XMVector4Transform(vector, matrix));
	return vector;
}*/
} // namespace library::math

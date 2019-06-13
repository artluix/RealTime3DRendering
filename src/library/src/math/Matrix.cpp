#include "StdAfx.h"
#include "library/Math/Matrix.h"

#include "library/Math/Quaternion.h"

namespace library::math
{
//-------------------------------------------------------------------------
// Matrix3
//-------------------------------------------------------------------------

const Matrix<3> Matrix<3>::Zero{ 0.f };
const Matrix<3> Matrix<3>::Identity{
	1.f, 0.f, 0.f,
	0.f, 1.f, 0.f,
	0.f, 0.f, 1.f
};

//-------------------------------------------------------------------------

Matrix<3>::Matrix(const dx::MATRIX& dxMAT)
{
	dx::StaticMatrix<float, 3>::Store(dxMAT, _dxMat);
}

Matrix<3>& Matrix<3>::operator=(const dx::MATRIX& dxMAT)
{
	dx::StaticMatrix<float, 3>::Store(dxMAT, _dxMat);
	return *this;
}

Matrix<3>::operator dx::MATRIX() const
{
	return dx::StaticMatrix<float, 3>::Load(_dxMat);
}

//-------------------------------------------------------------------------

std::string Matrix<3>::ToString() const
{
	std::ostringstream oss;
	oss << std::setprecision(4) <<
		'/' << _11 << ", " << _12 << ", " << _13 << '\\\n' <<
		'|' << _21 << ", " << _22 << ", " << _23 << '|\n' <<
		'\\' << _11 << ", " << _12 << ", " << _13 << '/\n';
	return oss.str();
}

//-------------------------------------------------------------------------

Matrix<3> Matrix<3>::Transpose() const
{
	return Matrix(DirectX::XMMatrixTranspose(dx::MATRIX(*this)));
}

//-------------------------------------------------------------------------

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
// Matrix4
//-------------------------------------------------------------------------

const Matrix<4> Matrix<4>::Zero{ 0.f };
const Matrix<4> Matrix<4>::Identity {
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
};

//-------------------------------------------------------------------------

Matrix<4>::Matrix(const dx::MATRIX& dxMAT)
{
	dx::StaticMatrix<float, 4>::Store(dxMAT, _dxMat);
}

Matrix<4>& Matrix<4>::operator=(const dx::MATRIX& dxMAT)
{
	dx::StaticMatrix<float, 4>::Store(dxMAT, _dxMat);
	return *this;
}

Matrix<4>::operator dx::MATRIX() const
{
	return dx::StaticMatrix<float, 4>::Load(_dxMat);
}

//-------------------------------------------------------------------------

std::string Matrix<4>::ToString() const
{
	std::ostringstream oss;
	oss << std::setprecision(4) <<
		'/' << _11 << ", " << _12 << ", " << _13 << ", " << _14 << '\\\n' <<
		'|' << _21 << ", " << _22 << ", " << _23 << ", " << _24 << '|\n' <<
		'|' << _31 << ", " << _32 << ", " << _33 << ", " << _34 << '|\n' <<
		'\\' << _11 << ", " << _12 << ", " << _13 << ", " << _14 << '/\n';
	return oss.str();
}

//-------------------------------------------------------------------------

Matrix<4> Matrix<4>::LookAtLH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir)
{
	return Matrix(DirectX::XMMatrixLookAtLH(dx::VECTOR(eyePos), dx::VECTOR(focusPos), dx::VECTOR(upDir)));
}

Matrix<4> Matrix<4>::LookAtRH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir)
{
	return Matrix(DirectX::XMMatrixLookAtRH(dx::VECTOR(eyePos), dx::VECTOR(focusPos), dx::VECTOR(upDir)));
}

Matrix<4> Matrix<4>::LookToLH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir)
{
	return Matrix(DirectX::XMMatrixLookToLH(dx::VECTOR(eyePos), dx::VECTOR(eyeDir), dx::VECTOR(upDir)));
}

Matrix<4> Matrix<4>::LookToRH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir)
{
	return Matrix(DirectX::XMMatrixLookToRH(dx::VECTOR(eyePos), dx::VECTOR(eyeDir), dx::VECTOR(upDir)));
}

//-------------------------------------------------------------------------

Matrix<4> Matrix<4>::Translation(const Vector3& vector)
{
	return Matrix(DirectX::XMMatrixTranslationFromVector(dx::VECTOR(vector)));
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
	return Matrix(DirectX::XMMatrixRotationAxis(dx::VECTOR(axis), angle));
}

Matrix<4> Matrix<4>::RotationPitchYawRoll(const float roll, const float pitch, const float yaw)
{
	return Matrix(DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
}

Matrix<4> Matrix<4>::RotationPitchYawRoll(const Vector3& vector)
{
	return Matrix(DirectX::XMMatrixRotationRollPitchYawFromVector(dx::VECTOR(vector)));
}

Matrix<4> Matrix<4>::RotationQuaternion(const Quaternion& quaternion)
{
	return Matrix(DirectX::XMMatrixRotationQuaternion(dx::VECTOR(quaternion)));
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
	const Quaternion& rotationQuat,
	const Vector3& translation)
{
	return Matrix(DirectX::XMMatrixAffineTransformation(
		dx::VECTOR(scale),
		dx::VECTOR(rotationOrigin),
		dx::VECTOR(rotationQuat),
		dx::VECTOR(translation)));
}

//-------------------------------------------------------------------------

float Matrix<4>::GetDeterminant() const
{
	return DirectX::XMVectorGetX(DirectX::XMMatrixDeterminant(dx::MATRIX(*this)));
}

Matrix<4> Matrix<4>::Inverse() const
{
	return Matrix(DirectX::XMMatrixInverse(nullptr, dx::MATRIX(*this)));
}

Matrix<4> Matrix<4>::Transpose() const
{
	return Matrix(DirectX::XMMatrixTranspose(dx::MATRIX(*this)));
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
// template instantiation
//-------------------------------------------------------------------------

template struct Matrix<3>;
template struct Matrix<4>;

} // namespace library::math

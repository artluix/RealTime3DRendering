#include "StdAfx.h"
#include "library/math/Matrix.h"

namespace library::math
{

	//-------------------------------------------------------------------------
	// Matrix3
	//-------------------------------------------------------------------------

	XMMatrix Matrix<3>::Load(const Matrix& mat)
	{
		return mat.Load();
	}

	Matrix<3> Matrix<3>::Store(const XMMatrix& matrix)
	{
		DirectX::XMFLOAT3X3 xmFloat3x3;
		DirectX::XMStoreFloat3x3(&xmFloat3x3, matrix);
		return Matrix(xmFloat3x3);
	}

	//-------------------------------------------------------------------------

	constexpr Matrix<3> Matrix<3>::Zero = Matrix<3>(0.f);
	constexpr Matrix<3> Matrix<3>::Identity = Matrix<3>(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f
	);

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

	XMMatrix Matrix<3>::Load() const
	{
		DirectX::XMFLOAT3X3 xmFloat3x3(*this);
		return DirectX::XMLoadFloat3x3(&xmFloat3x3);
	}

	Matrix<3> Matrix<3>::Transpose() const
	{
		return Store(DirectX::XMMatrixTranspose(Load()));
	}

	//-------------------------------------------------------------------------

	const Matrix<3>::Row& Matrix<3>::GetRow(const std::size_t rowIdx) const
	{
		return _rows[rowIdx];
	}

	void Matrix<3>::SetRow(const std::size_t rowIdx, const Row& row)
	{
		_rows[rowIdx] = row;
	}

	const Matrix<3>::Col Matrix<3>::GetCol(const std::size_t colIdx) const
	{
		return Col(_rows[0][colIdx], _rows[1][colIdx], _rows[2][colIdx]);
	}

	void Matrix<3>::SetCol(const std::size_t colIdx, const Matrix<3>::Col& col)
	{
		_rows[0][colIdx] = col[0];
		_rows[1][colIdx] = col[1];
		_rows[2][colIdx] = col[2];
	}

	//-------------------------------------------------------------------------

	Matrix<3>::operator DirectX::XMFLOAT3X3() const
	{
		return DirectX::XMFLOAT3X3(
			_11, _12, _13,
			_21, _22, _23,
			_31, _32, _33
		);
	}

	Matrix<3>::Row& Matrix<3>::operator[](const std::size_t rowIdx)
	{
		return _rows[rowIdx];
	}

	const Matrix<3>::Row& Matrix<3>::operator[](const std::size_t rowIdx) const
	{
		return _rows[rowIdx];
	}

	const float& Matrix<3>::operator() (const std::size_t rowIdx, const std::size_t colIdx) const
	{
		return _rows[rowIdx][colIdx];
	}

	float& Matrix<3>::operator() (const std::size_t rowIdx, const std::size_t colIdx)
	{
		return _rows[rowIdx][colIdx];
	}

	//-------------------------------------------------------------------------

	Vector3 operator * (const Vector3& vector, const Matrix3& matrix)
	{
		return Vector3::Store(DirectX::XMVector3Transform(vector.Load(), matrix.Load()));
	}

	Vector3& operator *= (Vector3& vector, const Matrix3& matrix)
	{
		vector = Vector3::Store(DirectX::XMVector3Transform(vector.Load(), matrix.Load()));
		return vector;
	}

	//-------------------------------------------------------------------------
	// Matrix4
	//-------------------------------------------------------------------------

	XMMatrix Matrix<4>::Load(const Matrix& mat)
	{
		return mat.Load();
	}

	Matrix<4> Matrix<4>::Store(const XMMatrix& matrix)
	{
		DirectX::XMFLOAT4X4 xmFloat4x4;
		DirectX::XMStoreFloat4x4(&xmFloat4x4, matrix);
		return Matrix(xmFloat4x4);
	}

	//-------------------------------------------------------------------------

	constexpr Matrix<4> Matrix<4>::Zero = Matrix<4>(0.f);
	constexpr Matrix<4> Matrix<4>::Identity = Matrix<4>(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f, 
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);

	//-------------------------------------------------------------------------

	Matrix<4> Matrix<4>::LookAtLH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir)
	{
		return Store(DirectX::XMMatrixLookAtLH(eyePos.Load(), focusPos.Load(), upDir.Load()));
	}

	Matrix<4> Matrix<4>::LookAtRH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir)
	{
		return Store(DirectX::XMMatrixLookAtRH(eyePos.Load(), focusPos.Load(), upDir.Load()));
	}

	Matrix<4> Matrix<4>::LookToLH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir)
	{
		return Store(DirectX::XMMatrixLookToLH(eyePos.Load(), eyeDir.Load(), upDir.Load()));
	}

	Matrix<4> Matrix<4>::LookToRH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir)
	{
		return Store(DirectX::XMMatrixLookToRH(eyePos.Load(), eyeDir.Load(), upDir.Load()));
	}

	//-------------------------------------------------------------------------

	Matrix<4> Matrix<4>::Translation(const Vector3& vector)
	{
		return Store(DirectX::XMMatrixTranslationFromVector(vector.Load()));
	}

	Matrix<4> Matrix<4>::Translation(const float x, const float y, const float z)
	{
		return Store(DirectX::XMMatrixTranslation(x, y, z));
	}

	//-------------------------------------------------------------------------

	Matrix<4> Matrix<4>::PerspectiveFovLH(
		const float fovAngleY,
		const float aspectRatio,
		const float nearZ, const float farZ
	)
	{
		return Store(DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ));
	}

	Matrix<4> Matrix<4>::PerspectiveFovRH(
		const float fovAngleY,
		const float aspectRatio,
		const float nearZ, const float farZ
	)
	{
		return Store(DirectX::XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, nearZ, farZ));
	}

	//-------------------------------------------------------------------------

	Matrix<4> Matrix<4>::RotationAxis(const Vector3& axis, const float angle)
	{
		return Store(DirectX::XMMatrixRotationAxis(axis.Load(), angle));
	}

	Matrix<4> Matrix<4>::RotationPitchYawRoll(const float roll, const float pitch, const float yaw)
	{
		return Store(DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	}

	Matrix<4> Matrix<4>::RotationPitchYawRoll(const Vector3& vector)
	{
		return Store(DirectX::XMMatrixRotationRollPitchYawFromVector(vector.Load()));
	}

	Matrix<4> Matrix<4>::RotationX(const float angle)
	{
		return Store(DirectX::XMMatrixRotationX(angle));
	}

	Matrix<4> Matrix<4>::RotationY(const float angle)
	{
		return Store(DirectX::XMMatrixRotationY(angle));
	}

	Matrix<4> Matrix<4>::RotationZ(const float angle)
	{
		return Store(DirectX::XMMatrixRotationZ(angle));
	}

	//Matrix<4> Matrix<4>::Scaling(const float scale)
	//{
	//	return Scaling(scale, scale, scale);
	//}

	//-------------------------------------------------------------------------

	Matrix<4> Matrix<4>::Scaling(const float x, const float y, const float z)
	{
		return Store(DirectX::XMMatrixScaling(x, y, z));
	}

	Matrix<4> Matrix<4>::Scaling(const Vector3& vector)
	{
		return Store(DirectX::XMMatrixScalingFromVector(vector.Load()));
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

	XMMatrix Matrix<4>::Load() const
	{
		DirectX::XMFLOAT4X4 xmFloat4x4(*this);
		return DirectX::XMLoadFloat4x4(&xmFloat4x4);
	}

	Matrix<4> Matrix<4>::Transpose() const
	{
		return Store(DirectX::XMMatrixTranspose(Load()));
	}

	Vector3 Matrix<4>::GetForward() const
	{
		return Vector3(-_31, -_32, -_33);
	}

	Matrix<4>& Matrix<4>::SetForward(const Vector3& vector)
	{
		_31 = -vector.x;
		_32 = -vector.y;
		_33 = -vector.z;
		return *this;
	}

	Vector3 Matrix<4>::GetUp() const
	{
		return Vector3(_21, _22, _23);
	}

	Matrix<4>& Matrix<4>::SetUp(const Vector3& vector)
	{
		_21 = vector.x;
		_22 = vector.y;
		_23 = vector.z;
		return *this;
	}

	Vector3 Matrix<4>::GetRight() const
	{
		return Vector3(_11, _12, _13);
	}

	Matrix<4>& Matrix<4>::SetRight(const Vector3& vector)
	{
		_11 = vector.x;
		_12 = vector.y;
		_13 = vector.z;
		return *this;
	}

	Vector3 Matrix<4>::GetTranslation() const
	{
		return Vector3(_41, _42, _43);
	}

	Matrix<4>& Matrix<4>::SetTranslation(const Vector3& vector)
	{
		_41 = vector.x;
		_42 = vector.y;
		_43 = vector.z;
		return *this;
	}

	//-------------------------------------------------------------------------

	const Matrix<4>::Row& Matrix<4>::GetRow(const std::size_t rowIdx) const
	{
		return _rows[rowIdx];
	}

	void Matrix<4>::SetRow(const std::size_t rowIdx, const Row& row)
	{
		_rows[rowIdx] = row;
	}

	const Matrix<4>::Col Matrix<4>::GetCol(const std::size_t colIdx) const
	{
		return Col(_rows[0][colIdx], _rows[1][colIdx], _rows[2][colIdx], _rows[3][colIdx]);
	}

	void Matrix<4>::SetCol(const std::size_t colIdx, const Matrix<4>::Col& col)
	{
		_rows[0][colIdx] = col[0];
		_rows[1][colIdx] = col[1];
		_rows[2][colIdx] = col[2];
		_rows[3][colIdx] = col[3];
	}

	//-------------------------------------------------------------------------

	Matrix<4>::operator DirectX::XMFLOAT4X4() const
	{
		return DirectX::XMFLOAT4X4(
			_11, _12, _13, _14,
			_21, _22, _23, _24,
			_31, _32, _33, _34,
			_41, _42, _43, _44
		);
	}

	Matrix4::Row& Matrix<4>::operator[](const std::size_t rowIdx)
	{
		return _rows[rowIdx];
	}

	const Matrix4::Row& Matrix<4>::operator[](const std::size_t rowIdx) const
	{
		return _rows[rowIdx];
	}

	const float& Matrix<4>::operator() (const std::size_t rowIdx, const std::size_t colIdx) const
	{
		return _rows[rowIdx][colIdx];
	}

	float& Matrix<4>::operator() (const std::size_t rowIdx, const std::size_t colIdx)
	{
		return _rows[rowIdx][colIdx];
	}

	//-------------------------------------------------------------------------

	Vector4 operator * (const Vector4& vector, const Matrix4& matrix)
	{
		return Vector4::Store(DirectX::XMVector4Transform(vector.Load(), matrix.Load()));
	}

	Vector4& operator *= (Vector4& vector, const Matrix4& matrix)
	{
		vector = Vector4::Store(DirectX::XMVector4Transform(vector.Load(), matrix.Load()));
		return vector;
	}
} // namespace library::math

#include "Matrix.h"

namespace library
{
	namespace math
	{

		//-------------------------------------------------------------------------
		// operators
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// arithmetic operators
		//-------------------------------------------------------------------------

		// add
		template<std::size_t Size>
		inline Matrix<Size>& operator + (const Matrix<Size>& lhs, const Matrix<Size>& rhs)
		{
			return Store(Matrix<Size>::Load(lhs) + Matrix<Size>::Load(rhs));
		}

		template<std::size_t Size>
		inline Matrix<Size>& operator += (Matrix<Size>& lhs, const Matrix<Size>& rhs)
		{
			lhs = Store(Matrix<Size>::Load(lhs) + Matrix<Size>::Load(rhs));
			return lhs;
		}

		// subtract
		template<std::size_t Size>
		inline Matrix<Size>& operator - (const Matrix<Size>& lhs, const Matrix<Size>& rhs)
		{
			return Store(Matrix<Size>::Load(lhs) - Matrix<Size>::Load(rhs));
		}

		template<std::size_t Size>
		inline Matrix<Size>& operator -= (Matrix<Size>& lhs, const Matrix<Size>& rhs)
		{
			lhs = Store(Matrix<Size>::Load(lhs) - Matrix<Size>::Load(rhs));
			return lhs;
		}

		// multiply

		inline Vector3 operator * (const Vector3& vector, const Matrix3& matrix)
		{
			return Vector3::Store(DirectX::XMVector3Transform(Vector3::Load(vector), Matrix3::Load(matrix)));
		}

		inline Vector3& operator *= (Vector3& vector, const Matrix3& matrix)
		{
			vector = Vector3::Store(DirectX::XMVector3Transform(Vector3::Load(vector), Matrix3::Load(matrix)));
			return vector;
		}

		inline Vector4 operator * (const Vector4& vector, const Matrix4& matrix)
		{
			return Vector4::Store(DirectX::XMVector4Transform(Vector4::Load(vector), Matrix4::Load(matrix)));
		}

		inline Vector4& operator *= (Vector4& vector, const Matrix4& matrix)
		{
			vector = Vector4::Store(DirectX::XMVector4Transform(Vector4::Load(vector), Matrix4::Load(matrix)));
			return vector;
		}

		template<std::size_t Size>
		inline Matrix<Size> operator * (const Matrix<Size>& lhs, const Matrix<Size>& rhs)
		{
			return Matrix<Size>::Store(DirectX::XMMatrixMultiply(Matrix<Size>::Load(lhs), Matrix<Size>::Load(rhs)));
		}

		template<std::size_t Size>
		inline Matrix<Size>& operator *= (Matrix<Size>& lhs, const Matrix<Size>& rhs)
		{
			lhs = Matrix<Size>::Store(DirectX::XMMatrixMultiply(Matrix<Size>::Load(lhs), Matrix<Size>::Load(rhs)));
			return lhs;
		}

		// divide

		//-------------------------------------------------------------------------
		// body
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// Matrix3
		//-------------------------------------------------------------------------

		const Matrix<3> Matrix<3>::Zero(0.f);
		const Matrix<3> Matrix<3>::Identity(
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			0.f, 0.f, 1.f
		);

		inline Matrix<3>::Matrix(const float value /*= 0.f*/)
		{
			data.fill(value);
		}

		inline Matrix<3>::Matrix(
			const float _11, const float _12, const float _13,
			const float _21, const float _22, const float _23,
			const float _31, const float _32, const float _33
		)
			: data{
				_11, _12, _13,
				_21, _22, _23,
				_31, _32, _33
			}
		{
		}

		inline Matrix<3>::Matrix(const Vector3& r1, const Vector3& r2, const Vector3& r3)
			: rows{ r1, r2, r3 }
		{
		}

		inline XMMatrix Matrix<3>::Load(const Matrix& mat)
		{
			DirectX::XMFLOAT3X3 xmFloat3x3(
				mat._11, mat._12, mat._13,
				mat._21, mat._22, mat._23,
				mat._31, mat._32, mat._33
			);
			return DirectX::XMLoadFloat3x3(&xmFloat3x3);
		}

		inline Matrix<3> Matrix<3>::Store(const XMMatrix& matrix)
		{
			DirectX::XMFLOAT3X3 xmFloat4x4;
			DirectX::XMStoreFloat3x3(&xmFloat4x4, matrix);
			return Matrix(
				xmFloat4x4._11, xmFloat4x4._12, xmFloat4x4._13,
				xmFloat4x4._21, xmFloat4x4._22, xmFloat4x4._23,
				xmFloat4x4._31, xmFloat4x4._32, xmFloat4x4._33
			);
		}

		//-------------------------------------------------------------------------

		inline std::string Matrix<3>::ToString() const
		{
			std::ostringstream oss;
			oss << std::setprecision(4) <<
				'/' << _11 << ", " << _12 << ", " << _13 <<  '\\' << std::endl <<
				'|' << _21 << ", " << _22 << ", " << _23 <<  '|' << std::endl <<
				'\\' << _11 << ", " << _12 << ", " << _13 << '/' << std::endl;
			return oss.str();
		}

		/*inline Matrix<3> Matrix<3>::Transpose() const
		{
			return Store(DirectX::XMMatrixTranspose(Load(*this)));
		}*/

		inline Matrix<3>& Matrix<3>::Transpose()
		{
			*this = Store(DirectX::XMMatrixTranspose(Load(*this)));
			return *this;
		}

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

		inline Matrix<4>::Matrix(const float value /*= 0.f*/)
		{
			data.fill(value);
		}

		inline Matrix<4>::Matrix(
			const float _11, const float _12, const float _13, const float _14,
			const float _21, const float _22, const float _23, const float _24,
			const float _31, const float _32, const float _33, const float _34,
			const float _41, const float _42, const float _43, const float _44
		)
			: data{
				_11, _12, _13, _14,
				_21, _22, _23, _24,
				_31, _32, _33, _34,
				_41, _42, _43, _44
			}
		{
		}

		inline Matrix<4>::Matrix(const Vector4& r1, const Vector4& r2, const Vector4& r3, const Vector4& r4)
			: rows{ r1, r2, r3, r4 }
		{
		}

		inline XMMatrix Matrix<4>::Load(const Matrix& mat)
		{
			DirectX::XMFLOAT4X4 xmFloat4x4(
				mat._11, mat._12, mat._13, mat._14,
				mat._21, mat._22, mat._23, mat._24,
				mat._31, mat._32, mat._33, mat._34,
				mat._41, mat._42, mat._43, mat._44
			);
			return DirectX::XMLoadFloat4x4(&xmFloat4x4);
		}

		inline Matrix<4> Matrix<4>::Store(const XMMatrix& matrix)
		{
			DirectX::XMFLOAT4X4 xmFloat4x4;
			DirectX::XMStoreFloat4x4(&xmFloat4x4, matrix);
			return Matrix(
				xmFloat4x4._11, xmFloat4x4._12, xmFloat4x4._13, xmFloat4x4._14,
				xmFloat4x4._21, xmFloat4x4._22, xmFloat4x4._23, xmFloat4x4._24,
				xmFloat4x4._31, xmFloat4x4._32, xmFloat4x4._33, xmFloat4x4._34,
				xmFloat4x4._41, xmFloat4x4._42, xmFloat4x4._43, xmFloat4x4._44
			);
		}

		inline Matrix<4> Matrix<4>::LookAtLH(
			const Vector3& eyePos,
			const Vector3& focusPos,
			const Vector3& upDir
		)
		{
			return Store(
				DirectX::XMMatrixLookAtLH(
					Vector3::Load(eyePos),
					Vector3::Load(focusPos),
					Vector3::Load(upDir)
				)
			);
		}

		inline Matrix<4> Matrix<4>::LookAtRH(
			const Vector3& eyePos,
			const Vector3& focusPos,
			const Vector3& upDir
		)
		{
			return Store(
				DirectX::XMMatrixLookAtRH(
					Vector3::Load(eyePos),
					Vector3::Load(focusPos),
					Vector3::Load(upDir)
				)
			);
		}

		inline Matrix<4> Matrix<4>::LookToLH(
			const Vector3& eyePos,
			const Vector3& eyeDir,
			const Vector3& upDir
		)
		{
			return Store(
				DirectX::XMMatrixLookToLH(
					Vector3::Load(eyePos),
					Vector3::Load(eyeDir),
					Vector3::Load(upDir)
				)
			);
		}

		inline Matrix<4> Matrix<4>::LookToRH(
			const Vector3& eyePos,
			const Vector3& eyeDir,
			const Vector3& upDir
		)
		{
			return Store(
				DirectX::XMMatrixLookToRH(
					Vector3::Load(eyePos),
					Vector3::Load(eyeDir),
					Vector3::Load(upDir)
				)
			);
		}

		//-------------------------------------------------------------------------

		inline std::string Matrix<4>::ToString() const
		{
			std::ostringstream oss;
			oss << std::setprecision(4) <<
				'/' << _11 << ", " << _12 << ", " << _13 << ", " << _14 << '\\' << std::endl <<
				'|' << _21 << ", " << _22 << ", " << _23 << ", " << _24 << '|' << std::endl <<
				'|' << _31 << ", " << _32 << ", " << _33 << ", " << _34 << '|' << std::endl <<
				'\\' << _11 << ", " << _12 << ", " << _13 << ", " << _14 << '/' << std::endl;
			return oss.str();
		}

		/*inline Matrix<4> Matrix<4>::Transpose() const
		{
			return Store(DirectX::XMMatrixTranspose(Load(*this)));
		}*/

		inline Matrix<4>& Matrix<4>::Transpose()
		{
			*this = Store(DirectX::XMMatrixTranspose(Load(*this)));
			return *this;
		}

		inline Vector3 Matrix<4>::GetForward() const
		{
			return Vector3(-_31, -_32, -_33);
		}

		inline Matrix<4>& Matrix<4>::SetForward(const Vector3& vector)
		{
			_31 = -vector.x;
			_32 = -vector.y;
			_33 = -vector.z;
			return *this;
		}

		inline Vector3 Matrix<4>::GetUp() const
		{
			return Vector3(_21, _22, _23);
		}

		inline Matrix<4>& Matrix<4>::SetUp(const Vector3& vector)
		{
			_21 = vector.x;
			_22 = vector.y;
			_23 = vector.z;
			return *this;
		}

		inline Vector3 Matrix<4>::GetRight() const
		{
			return Vector3(_11, _12, _13);
		}

		inline Matrix<4>& Matrix<4>::SetRight(const Vector3& vector)
		{
			_11 = vector.x;
			_12 = vector.y;
			_13 = vector.z;
			return *this;
		}

		inline Vector3 Matrix<4>::GetTranslation() const
		{
			return Vector3(_41, _42, _43);
		}

		inline Matrix<4>& Matrix<4>::SetTranslation(const Vector3& vector)
		{
			_41 = vector.x;
			_42 = vector.y;
			_43 = vector.z;
			return *this;
		}

	} // namespace math
} // namespace library

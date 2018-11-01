#include <sstream>
#include <iomanip>

namespace library
{
	namespace math
	{

		//-------------------------------------------------------------------------
		// Vector
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// comparison operators
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// Vector2
		//-------------------------------------------------------------------------

		inline bool operator == (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2Equal(lhs.Load(), rhs.Load());
		}

		inline bool operator != (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2NotEqual(lhs.Load(), rhs.Load());
		}

		inline bool operator > (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2Greater(lhs.Load(), rhs.Load());
		}

		inline bool operator >= (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2GreaterOrEqual(lhs.Load(), rhs.Load());
		}

		inline bool operator < (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2Less(lhs.Load(), rhs.Load());
		}

		inline bool operator <= (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2LessOrEqual(lhs.Load(), rhs.Load());
		}

		//-------------------------------------------------------------------------
		// Vector3
		//-------------------------------------------------------------------------

		inline bool operator == (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3Equal(lhs.Load(), rhs.Load());
		}

		inline bool operator != (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3NotEqual(lhs.Load(), rhs.Load());
		}

		inline bool operator > (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3Greater(lhs.Load(), rhs.Load());
		}

		inline bool operator >= (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3GreaterOrEqual(lhs.Load(), rhs.Load());
		}

		inline bool operator < (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3Less(lhs.Load(), rhs.Load());
		}

		inline bool operator <= (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3LessOrEqual(lhs.Load(), rhs.Load());
		}

		//-------------------------------------------------------------------------
		// Vector4
		//-------------------------------------------------------------------------

		inline bool operator == (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4Equal(lhs.Load(), rhs.Load());
		}

		inline bool operator != (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4NotEqual(lhs.Load(), rhs.Load());
		}

		inline bool operator > (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4Greater(lhs.Load(), rhs.Load());
		}

		inline bool operator >= (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4GreaterOrEqual(lhs.Load(), rhs.Load());
		}

		inline bool operator < (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4Less(lhs.Load(), rhs.Load());
		}

		inline bool operator <= (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4LessOrEqual(lhs.Load(), rhs.Load());
		}

		//-------------------------------------------------------------------------
		// arithmetic operators
		//-------------------------------------------------------------------------

		// add
		template<std::size_t Size>
		inline Vector<Size> operator + (const Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			return Vector<Size>::Store(DirectX::XMVectorAdd(lhs.Load(), rhs.Load()));
		}

		template<std::size_t Size>
		inline Vector<Size>& operator += (Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			lhs = Vector<Size>::Store(DirectX::XMVectorAdd(lhs.Load(), rhs.Load()));
			return lhs;
		}

		template<std::size_t Size>
		inline Vector<Size> operator + (const Vector<Size>& lhs, const float value)
		{
			return lhs + Vector<Size>(value);
		}

		template<std::size_t Size>
		inline Vector<Size> operator + (const float value, const Vector<Size>& lhs)
		{
			return Vector(value) + lhs;
		}

		template<std::size_t Size>
		inline Vector<Size>& operator += (Vector<Size>& lhs, const float value)
		{
			lhs += Vector<Size>(value);
			return lhs;
		}

		// negate
		template<std::size_t Size>
		inline Vector<Size> operator - (const Vector<Size>& vector)
		{
			return Vector<Size>::Store(DirectX::XMVectorNegate(Vector<Size>::Load(vector)));
		}

		// subtract
		template<std::size_t Size>
		inline Vector<Size> operator - (const Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			return Vector<Size>::Store(DirectX::XMVectorSubtract(lhs.Load(), rhs.Load()));
		}

		template<std::size_t Size>
		inline Vector<Size>& operator -= (Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			lhs = Vector<Size>::Store(DirectX::XMVectorSubtract(lhs.Load(), rhs.Load()));
			return lhs;
		}

		template<std::size_t Size>
		inline Vector<Size> operator - (const Vector<Size>& lhs, const float value)
		{
			return lhs - Vector<Size>(value);
		}

		template<std::size_t Size>
		inline Vector<Size>& operator -= (Vector<Size>& lhs, const float value)
		{
			lhs -= Vector<Size>(value);
			return lhs;
		}

		// multiply
		template<std::size_t Size>
		inline Vector<Size> operator * (const Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			return Vector<Size>::Store(DirectX::XMVectorMultiply(lhs.Load(), rhs.Load()));
		}

		template<std::size_t Size>
		inline Vector<Size>& operator *= (Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			lhs = Vector<Size>::Store(DirectX::XMVectorMultiply(lhs.Load(), rhs.Load()));
			return lhs;
		}

		template<std::size_t Size>
		inline Vector<Size> operator * (const Vector<Size>& lhs, const float value)
		{
			return lhs * Vector<Size>(value);
		}

		template<std::size_t Size>
		inline Vector<Size> operator * (const float value, const Vector<Size>& lhs)
		{
			return Vector<Size>(value) * lhs;
		}

		template<std::size_t Size>
		inline Vector<Size>& operator *= (Vector<Size>& lhs, const float value)
		{
			lhs *= Vector<Size>(value);
			return lhs;
		}

		// divide
		template<std::size_t Size>
		inline Vector<Size> operator / (const Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			return Vector<Size>::Store(DirectX::XMVectorDivide(lhs.Load(), rhs.Load()));
		}

		template<std::size_t Size>
		inline Vector<Size> operator /= (const Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			lhs = Vector<Size>::Store(DirectX::XMVectorDivide(lhs.Load(), rhs.Load()));
			return lhs;
		}

		template<std::size_t Size>
		inline Vector<Size> operator / (const Vector<Size>& lhs, const float value)
		{
			return lhs / Vector<Size>(value);
		}

		template<std::size_t Size>
		inline Vector<Size>& operator /= (Vector<Size>& lhs, const float value)
		{
			lhs /= Vector<Size>(value);
			return lhs;
		}

		//-------------------------------------------------------------------------
		// body
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// Vector2
		//-------------------------------------------------------------------------

		inline XMVector Vector<2>::Load(const Vector & vector)
		{
			return vector.Load();
		}

		inline Vector<2> Vector<2>::Store(const XMVector & xmVector)
		{
			DirectX::XMFLOAT2 xmFloat2;
			DirectX::XMStoreFloat2(&xmFloat2, xmVector);
			return Vector(xmFloat2);
		}
		
		//-------------------------------------------------------------------------

		inline std::string Vector<2>::ToString() const
		{
			std::ostringstream oss;
			oss << std::setprecision(4) << '(' << x << ", " << y << ')';
			return oss.str();
		}

		inline XMVector Vector<2>::Load() const
		{
			DirectX::XMFLOAT2 xmFloat2(*this);
			return DirectX::XMLoadFloat2(&xmFloat2);
		}

		inline bool Vector<2>::IsZero() const
		{
			return *this == Zero;
		}

		//-------------------------------------------------------------------------

		inline Vector<2> Vector<2>::Cross(const Vector<2>& vector) const
		{
			return Store(DirectX::XMVector2Cross(Load(), vector.Load()));
		}

		inline Vector<2> Vector<2>::Normalize() const
		{
			return Store(DirectX::XMVector2Normalize(Load()));
		}

		inline float Vector<2>::Length() const
		{
			return DirectX::XMVectorGetX(DirectX::XMVector2Length(Load()));
		}

		//-------------------------------------------------------------------------
		// Vector3
		//-------------------------------------------------------------------------

		inline XMVector Vector<3>::Load(const Vector & vector)
		{
			return vector.Load();
		}

		inline Vector<3> Vector<3>::Store(const XMVector & xmVector)
		{
			DirectX::XMFLOAT3 xmFloat3;
			DirectX::XMStoreFloat3(&xmFloat3, xmVector);
			return Vector(xmFloat3);
		}

		//-------------------------------------------------------------------------

		inline std::string Vector<3>::ToString() const
		{
			std::ostringstream oss;
			oss << std::setprecision(4) << '(' << x << ", " << y << ", " << z << ')';
			return oss.str();
		}

		inline XMVector Vector<3>::Load() const
		{
			DirectX::XMFLOAT3 xmFloat3(*this);
			return DirectX::XMLoadFloat3(&xmFloat3);
		}

		inline bool Vector<3>::IsZero() const
		{
			return *this == Zero;
		}

		//-------------------------------------------------------------------------

		inline Vector<3> Vector<3>::Normalize() const
		{
			return Store(DirectX::XMVector3Normalize(Load()));
		}

		inline float Vector<3>::Length() const
		{
			return DirectX::XMVectorGetX(DirectX::XMVector3Length(Load()));
		}

		inline Vector<3> Vector<3>::Cross(const Vector<3>& vector) const
		{
			return Store(DirectX::XMVector3Cross(Load(), vector.Load()));
		}

		template<std::size_t Size>
		inline Vector<3> Vector<3>::Transform(const Matrix<Size>& matrix) const
		{
			return Store(DirectX::XMVector3Transform(Load(), matrix.Load()));
		}

		template<std::size_t Size>
		inline Vector<3> Vector<3>::TransformNormal(const Matrix<Size>& matrix) const
		{
			return Store(DirectX::XMVector3TransformNormal(Load(), matrix.Load()));
		}

		//-------------------------------------------------------------------------
		// Vector4
		//-------------------------------------------------------------------------

		inline XMVector Vector<4>::Load(const Vector & vector)
		{
			return vector.Load();
		}

		inline Vector<4> Vector<4>::Store(const XMVector & xmVector)
		{
			DirectX::XMFLOAT4 xmFloat4;
			DirectX::XMStoreFloat4(&xmFloat4, xmVector);
			return Vector(xmFloat4);
		}

		//-------------------------------------------------------------------------

		inline std::string Vector<4>::ToString() const
		{
			std::ostringstream oss;
			oss << std::setprecision(4) << '(' << x << ", " << y << ", " << z << ", " << w << ')';
			return oss.str();
		}

		inline XMVector Vector<4>::Load() const
		{
			DirectX::XMFLOAT4 xmFloat4(*this);
			return DirectX::XMLoadFloat4(&xmFloat4);
		}

		inline bool Vector<4>::IsZero() const
		{
			return *this == Zero;
		}

		//-------------------------------------------------------------------------

		inline Vector<4> Vector<4>::Cross(const Vector<4>& vector1, const Vector<4>& vector2) const
		{
			return Store(DirectX::XMVector4Cross(Load(), vector1.Load(), vector2.Load()));
		}

		inline Vector<4> Vector<4>::Normalize() const
		{
			return Store(DirectX::XMVector4Normalize(Load()));
		}

		inline float Vector<4>::Length() const
		{
			return DirectX::XMVectorGetX(DirectX::XMVector4Length(Load()));
		}

		inline Vector<4> Vector<4>::Transform(const Matrix4& matrix) const
		{
			return Store(DirectX::XMVector4Transform(Load(), matrix.Load()));
		}

		//-------------------------------------------------------------------------
		// Matrix
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// arithmetic operators
		//-------------------------------------------------------------------------

		// add
		template<std::size_t Size>
		inline Matrix<Size>& operator + (const Matrix<Size>& lhs, const Matrix<Size>& rhs)
		{
			return Store(lhs.Load() + rhs.Load());
		}

		template<std::size_t Size>
		inline Matrix<Size>& operator += (Matrix<Size>& lhs, const Matrix<Size>& rhs)
		{
			lhs = Store(lhs.Load() + rhs.Load());
			return lhs;
		}

		// subtract
		template<std::size_t Size>
		inline Matrix<Size>& operator - (const Matrix<Size>& lhs, const Matrix<Size>& rhs)
		{
			return Store(lhs.Load() - rhs.Load());
		}

		template<std::size_t Size>
		inline Matrix<Size>& operator -= (Matrix<Size>& lhs, const Matrix<Size>& rhs)
		{
			lhs = Store(lhs.Load() - rhs.Load());
			return lhs;
		}

		// multiply

		inline Vector3 operator * (const Vector3& vector, const Matrix3& matrix)
		{
			return Vector3::Store(DirectX::XMVector3Transform(vector.Load(), matrix.Load()));
		}

		inline Vector3& operator *= (Vector3& vector, const Matrix3& matrix)
		{
			vector = Vector3::Store(DirectX::XMVector3Transform(vector.Load(), matrix.Load()));
			return vector;
		}

		inline Vector4 operator * (const Vector4& vector, const Matrix4& matrix)
		{
			return Vector4::Store(DirectX::XMVector4Transform(vector.Load(), matrix.Load()));
		}

		inline Vector4& operator *= (Vector4& vector, const Matrix4& matrix)
		{
			vector = Vector4::Store(DirectX::XMVector4Transform(vector.Load(), matrix.Load()));
			return vector;
		}

		template<std::size_t Size>
		inline Matrix<Size> operator * (const Matrix<Size>& lhs, const Matrix<Size>& rhs)
		{
			return Matrix<Size>::Store(DirectX::XMMatrixMultiply(lhs.Load(), rhs.Load()));
		}

		template<std::size_t Size>
		inline Matrix<Size>& operator *= (Matrix<Size>& lhs, const Matrix<Size>& rhs)
		{
			lhs = Matrix<Size>::Store(DirectX::XMMatrixMultiply(lhs.Load(), rhs.Load()));
			return lhs;
		}

		// divide

		//-------------------------------------------------------------------------
		// body
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// Matrix3
		//-------------------------------------------------------------------------

		inline Matrix<3>::Matrix(const float value /*= 0.f*/)
			: _11(value), _12(value), _13(value)
			, _21(value), _22(value), _23(value)
			, _31(value), _32(value), _33(value)
		{
		}

		inline Matrix<3>::Matrix(
			const float _11, const float _12, const float _13,
			const float _21, const float _22, const float _23,
			const float _31, const float _32, const float _33
		)
			: _11(_11), _12(_12), _13(_13)
			, _21(_21), _22(_22), _23(_23)
			, _31(_31), _32(_32), _33(_33)
		{
		}

		inline Matrix<3>::Matrix(const Vector3& r1, const Vector3& r2, const Vector3& r3)
			: rows{ r1, r2, r3 }
		{
		}

		inline Matrix<3>::Matrix(const DirectX::XMFLOAT3X3& xmFloat3x3)
			: _11(xmFloat3x3._11), _12(xmFloat3x3._12), _13(xmFloat3x3._13)
			, _21(xmFloat3x3._21), _22(xmFloat3x3._22), _23(xmFloat3x3._23)
			, _31(xmFloat3x3._31), _32(xmFloat3x3._32), _33(xmFloat3x3._33)
		{
		}

		inline XMMatrix Matrix<3>::Load(const Matrix& mat)
		{
			return mat.Load();
		}

		inline Matrix<3> Matrix<3>::Store(const XMMatrix& matrix)
		{
			DirectX::XMFLOAT3X3 xmFloat3x3;
			DirectX::XMStoreFloat3x3(&xmFloat3x3, matrix);
			return Matrix(xmFloat3x3);
		}

		//-------------------------------------------------------------------------

		inline std::string Matrix<3>::ToString() const
		{
			std::ostringstream oss;
			oss << std::setprecision(4) <<
				'/' << _11 << ", " << _12 << ", " << _13 << '\\' << std::endl <<
				'|' << _21 << ", " << _22 << ", " << _23 << '|' << std::endl <<
				'\\' << _11 << ", " << _12 << ", " << _13 << '/' << std::endl;
			return oss.str();
		}

		inline XMMatrix Matrix<3>::Load() const
		{
			DirectX::XMFLOAT3X3 xmFloat3x3(*this);
			return DirectX::XMLoadFloat3x3(&xmFloat3x3);
		}

		inline Matrix<3> Matrix<3>::Transpose() const
		{
			return Store(DirectX::XMMatrixTranspose(Load()));
		}

		inline Matrix<3>::operator DirectX::XMFLOAT3X3() const
		{
			return DirectX::XMFLOAT3X3(
				_11, _12, _13,
				_21, _22, _23,
				_31, _32, _33
			);
		}

		inline const float& Matrix<3>::operator() (const std::size_t rowIdx, const std::size_t colIdx) const
		{
			return rows[rowIdx][colIdx];
		}

		inline float& Matrix<3>::operator() (const std::size_t rowIdx, const std::size_t colIdx)
		{
			return rows[rowIdx][colIdx];
		}

		//-------------------------------------------------------------------------
		// Matrix4
		//-------------------------------------------------------------------------

		inline Matrix<4>::Matrix(const float value /*= 0.f*/)
			: _11(value), _12(value), _13(value), _14(value)
			, _21(value), _22(value), _23(value), _24(value)
			, _31(value), _32(value), _33(value), _34(value)
			, _41(value), _42(value), _43(value), _44(value)
		{
		}

		inline Matrix<4>::Matrix(
			const float _11, const float _12, const float _13, const float _14,
			const float _21, const float _22, const float _23, const float _24,
			const float _31, const float _32, const float _33, const float _34,
			const float _41, const float _42, const float _43, const float _44
		)
			: _11(_11), _12(_12), _13(_13), _14(_14)
			, _21(_21), _22(_22), _23(_23), _24(_24)
			, _31(_31), _32(_32), _33(_33), _34(_34)
			, _41(_41), _42(_42), _43(_43), _44(_44)
		{
		}

		inline Matrix<4>::Matrix(const Vector4& r1, const Vector4& r2, const Vector4& r3, const Vector4& r4)
			: rows{ r1, r2, r3, r4 }
		{
		}

		inline Matrix<4>::Matrix(const DirectX::XMFLOAT4X4& xmFloat4x4)
			: _11(xmFloat4x4._11), _12(xmFloat4x4._12), _13(xmFloat4x4._13), _14(xmFloat4x4._14)
			, _21(xmFloat4x4._21), _22(xmFloat4x4._22), _23(xmFloat4x4._23), _24(xmFloat4x4._24)
			, _31(xmFloat4x4._31), _32(xmFloat4x4._32), _33(xmFloat4x4._33), _34(xmFloat4x4._34)
			, _41(xmFloat4x4._41), _42(xmFloat4x4._42), _43(xmFloat4x4._43), _44(xmFloat4x4._44)
		{
		}

		inline XMMatrix Matrix<4>::Load(const Matrix& mat)
		{
			return mat.Load();
		}

		inline Matrix<4> Matrix<4>::Store(const XMMatrix& matrix)
		{
			DirectX::XMFLOAT4X4 xmFloat4x4;
			DirectX::XMStoreFloat4x4(&xmFloat4x4, matrix);
			return Matrix(xmFloat4x4);
		}

		//-------------------------------------------------------------------------

		inline Matrix<4> Matrix<4>::LookAtLH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir)
		{
			return Store(DirectX::XMMatrixLookAtLH(eyePos.Load(), focusPos.Load(), upDir.Load()));
		}

		inline Matrix<4> Matrix<4>::LookAtRH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir)
		{
			return Store(DirectX::XMMatrixLookAtRH(eyePos.Load(), focusPos.Load(), upDir.Load()));
		}

		inline Matrix<4> Matrix<4>::LookToLH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir)
		{
			return Store(DirectX::XMMatrixLookToLH(eyePos.Load(), eyeDir.Load(), upDir.Load()));
		}

		inline Matrix<4> Matrix<4>::LookToRH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir)
		{
			return Store(DirectX::XMMatrixLookToRH(eyePos.Load(), eyeDir.Load(), upDir.Load()));
		}

		//-------------------------------------------------------------------------

		inline Matrix<4> Matrix<4>::Translation(const Vector3& vector)
		{
			return Store(DirectX::XMMatrixTranslationFromVector(vector.Load()));
		}

		inline Matrix<4> Matrix<4>::Translation(const float x, const float y, const float z)
		{
			return Store(DirectX::XMMatrixTranslation(x, y, z));
		}

		//-------------------------------------------------------------------------

		inline Matrix<4> Matrix<4>::PerspectiveFovLH(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ)
		{
			return Store(DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ));
		}

		inline Matrix<4> Matrix<4>::PerspectiveFovRH(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ)
		{
			return Store(DirectX::XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, nearZ, farZ));
		}

		//-------------------------------------------------------------------------

		inline Matrix<4> Matrix<4>::RotationAxis(const Vector3& axis, const float angle)
		{
			return Store(DirectX::XMMatrixRotationAxis(axis.Load(), angle));
		}

		inline Matrix<4> Matrix<4>::RotationRollPitchYaw(const float roll, const float pitch, const float yaw)
		{
			return Store(DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw));
		}

		inline Matrix<4> Matrix<4>::RotationRollPitchYaw(const Vector3 & vector)
		{
			return Store(DirectX::XMMatrixRotationRollPitchYawFromVector(vector.Load()));
		}

		inline Matrix<4> Matrix<4>::RotationX(const float angle)
		{
			return Store(DirectX::XMMatrixRotationX(angle));
		}

		inline Matrix<4> Matrix<4>::RotationY(const float angle)
		{
			return Store(DirectX::XMMatrixRotationY(angle));
		}

		inline Matrix<4> Matrix<4>::RotationZ(const float angle)
		{
			return Store(DirectX::XMMatrixRotationZ(angle));
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

		inline XMMatrix Matrix<4>::Load() const
		{
			DirectX::XMFLOAT4X4 xmFloat4x4(*this);
			return DirectX::XMLoadFloat4x4(&xmFloat4x4);
		}

		inline Matrix<4> Matrix<4>::Transpose() const
		{
			return Store(DirectX::XMMatrixTranspose(Load()));
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

		inline Matrix<4>::operator DirectX::XMFLOAT4X4() const
		{
			return DirectX::XMFLOAT4X4(
				_11, _12, _13, _14,
				_21, _22, _23, _24,
				_31, _32, _33, _34,
				_41, _42, _43, _44
			);
		}

		inline const float& Matrix<4>::operator() (const std::size_t rowIdx, const std::size_t colIdx) const
		{
			return rows[rowIdx][colIdx];
		}

		inline float& Matrix<4>::operator() (const std::size_t rowIdx, const std::size_t colIdx)
		{
			return rows[rowIdx][colIdx];
		}

	} // namespace math
} // namespace library

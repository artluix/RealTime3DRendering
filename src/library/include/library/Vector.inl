#include "Vector.h"

namespace library
{
	namespace math
	{

		//-------------------------------------------------------------------------
		// comparison operators
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// Vector2
		//-------------------------------------------------------------------------

		bool operator == (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2Equal(Vector2::Load(lhs), Vector2::Load(rhs));
		}

		bool operator != (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2NotEqual(Vector2::Load(lhs), Vector2::Load(rhs));
		}

		bool operator > (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2Greater(Vector2::Load(lhs), Vector2::Load(rhs));
		}


		bool operator >= (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2GreaterOrEqual(Vector2::Load(lhs), Vector2::Load(rhs));
		}


		bool operator < (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2Less(Vector2::Load(lhs), Vector2::Load(rhs));
		}

		bool operator <= (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2LessOrEqual(Vector2::Load(lhs), Vector2::Load(rhs));
		}

		//-------------------------------------------------------------------------
		// Vector3
		//-------------------------------------------------------------------------

		bool operator == (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3Equal(Vector3::Load(lhs), Vector3::Load(rhs));
		}

		bool operator != (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3NotEqual(Vector3::Load(lhs), Vector3::Load(rhs));
		}

		bool operator > (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3Greater(Vector3::Load(lhs), Vector3::Load(rhs));
		}

		bool operator >= (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3GreaterOrEqual(Vector3::Load(lhs), Vector3::Load(rhs));
		}

		bool operator < (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3Less(Vector3::Load(lhs), Vector3::Load(rhs));
		}

		bool operator <= (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3LessOrEqual(Vector3::Load(lhs), Vector3::Load(rhs));
		}

		//-------------------------------------------------------------------------
		// Vector4
		//-------------------------------------------------------------------------

		bool operator == (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4Equal(Vector4::Load(lhs), Vector4::Load(rhs));
		}

		bool operator != (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4NotEqual(Vector4::Load(lhs), Vector4::Load(rhs));
		}

		bool operator > (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4Greater(Vector4::Load(lhs), Vector4::Load(rhs));
		}

		bool operator >= (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4GreaterOrEqual(Vector4::Load(lhs), Vector4::Load(rhs));
		}

		bool operator < (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4Less(Vector4::Load(lhs), Vector4::Load(rhs));
		}

		bool operator <= (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4LessOrEqual(Vector4::Load(lhs), Vector4::Load(rhs));
		}

		//-------------------------------------------------------------------------
		// arithmetic operators
		//-------------------------------------------------------------------------

		// add
		template<std::size_t Size>
		Vector<Size> operator + (const Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			return Vector::Store(DirectX::XMVectorAdd(Vector::Load(lhs), Vector::Load(rhs)));
		}

		template<std::size_t Size>
		Vector<Size>& operator += (Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			lhs = Vector::Store(DirectX::XMVectorAdd(Vector::Load(lhs), Vector::Load(rhs)));
			return lhs;
		}

		template<std::size_t Size>
		Vector<Size> operator + (const Vector<Size>& lhs, const float value)
		{
			return lhs + Vector(value);
		}

		template<std::size_t Size>
		Vector<Size> operator + (const float value, const Vector<Size>& lhs)
		{
			return Vector(value) + lhs;
		}

		template<std::size_t Size>
		Vector<Size>& operator += (Vector<Size>& lhs, const float value)
		{
			lhs += Vector(value);
			return lhs;
		}

		// negate
		template<std::size_t Size>
		Vector<Size> operator - (const Vector<Size>& vector)
		{
			return Vector<Size>::Store(DirectX::XMVectorNegate(Vector<Size>::Load(vector)));
		}

		// subtract
		template<std::size_t Size>
		Vector<Size> operator - (const Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			return Vector<Size>::Store(DirectX::XMVectorSubtract(Vector<Size>::Load(lhs), Vector<Size>::Load(rhs)));
		}

		template<std::size_t Size>
		Vector<Size>& operator -= (Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			lhs = Vector<Size>::Store(DirectX::XMVectorSubtract(Vector<Size>::Load(lhs), Vector<Size>::Load(rhs)));
			return lhs;
		}

		template<std::size_t Size>
		Vector<Size> operator - (const Vector<Size>& lhs, const float value)
		{
			return lhs - Vector<Size>(value);
		}

		template<std::size_t Size>
		Vector<Size>& operator -= (Vector<Size>& lhs, const float value)
		{
			lhs -= Vector<Size>(value);
			return lhs;
		}

		// multiply
		template<std::size_t Size>
		Vector<Size> operator * (const Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			return Vector<Size>::Store(DirectX::XMVectorMultiply(Vector<Size>::Load(lhs), Vector<Size>::Load(rhs)));
		}

		template<std::size_t Size>
		Vector<Size>& operator *= (Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			lhs = Vector<Size>::Store(DirectX::XMVectorMultiply(Vector<Size>::Load(lhs), Vector<Size>::Load(rhs)));
			return lhs;
		}

		template<std::size_t Size>
		Vector<Size> operator * (const Vector<Size>& lhs, const float value)
		{
			return lhs * Vector<Size>(value);
		}

		template<std::size_t Size>
		Vector<Size> operator * (const float value, const Vector<Size>& lhs)
		{
			return Vector<Size>(value) * lhs;
		}

		template<std::size_t Size>
		Vector<Size>& operator *= (Vector<Size>& lhs, const float value)
		{
			lhs *= Vector<Size>(value);
			return lhs;
		}

		// divide
		template<std::size_t Size>
		Vector<Size> operator / (const Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			return Vector<Size>::Store(DirectX::XMVectorDivide(Vector<Size>::Load(lhs), Vector<Size>::Load(rhs)));
		}

		template<std::size_t Size>
		Vector<Size> operator /= (const Vector<Size>& lhs, const Vector<Size>& rhs)
		{
			lhs = Vector<Size>::Store(DirectX::XMVectorDivide(Vector<Size>::Load(lhs), Vector<Size>::Load(rhs)));
			return lhs;
		}

		template<std::size_t Size>
		Vector<Size> operator / (const Vector<Size>& lhs, const float value)
		{
			return lhs / Vector<Size>(value);
		}

		template<std::size_t Size>
		Vector<Size>& operator /= (Vector<Size>& lhs, const float value)
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

		const Vector<2> Vector<2>::Zero(0.f);
		const Vector<2> Vector<2>::One(1.f);

		Vector<2>::Vector(const float value /*= 0.f*/)
		{
			data.fill(value);
		}

		inline XMVector Vector<2>::Load(const Vector & vector)
		{
			DirectX::XMFLOAT2 xmFloat2(vector.x, vector.y);
			return DirectX::XMLoadFloat2(&xmFloat2);
		}

		inline Vector<2> Vector<2>::Store(const XMVector & xmVector)
		{
			DirectX::XMFLOAT2 xmFloat2;
			DirectX::XMStoreFloat2(&xmFloat2, xmVector);
			return Vector(xmFloat2);
		}

		inline std::string Vector<2>::ToString() const
		{
			std::ostringstream oss;
			oss << std::setprecision(4) << '(' << x << ", " << y << ')';
			return oss.str();
		}

		inline bool Vector<2>::IsZero() const
		{
			return *this == Zero;
		}

		inline Vector<2>& Vector<2>::Normalize()
		{
			*this = Store(DirectX::XMVector2Normalize(Load(*this)));
			return *this;
		}

		inline float Vector<2>::Length() const
		{
			return DirectX::XMVectorGetX(DirectX::XMVector2Length(Load(*this)));
		}

		//-------------------------------------------------------------------------
		// Vector3
		//-------------------------------------------------------------------------

		const Vector<3> Vector<3>::Zero(0.f);
		const Vector<3> Vector<3>::One(1.f);

		Vector3::Vector(const float value /*= 0.f*/)
		{
			data.fill(value);
		}

		inline XMVector Vector<3>::Load(const Vector & vector)
		{
			DirectX::XMFLOAT3 xmFloat3(vector.x, vector.y, vector.z);
			return DirectX::XMLoadFloat3(&xmFloat3);
		}

		inline Vector<3> Vector<3>::Store(const XMVector & xmVector)
		{
			DirectX::XMFLOAT3 xmFloat3;
			DirectX::XMStoreFloat3(&xmFloat3, xmVector);
			return Vector(xmFloat3);
		}

		inline std::string Vector<3>::ToString() const
		{
			std::ostringstream oss;
			oss << std::setprecision(4) << '(' << x << ", " << y << ", " << z << ')';
			return oss.str();
		}

		inline bool Vector<3>::IsZero() const
		{
			return *this == Zero;
		}

		inline Vector<3>& Vector<3>::Normalize()
		{
			*this = Store(DirectX::XMVector3Normalize(Load(*this)));
			return *this;
		}

		inline float Vector<3>::Length() const
		{
			return DirectX::XMVectorGetX(DirectX::XMVector3Length(Load(*this)));
		}

		//-------------------------------------------------------------------------
		// Vector4
		//-------------------------------------------------------------------------

		const Vector<4> Vector<4>::Zero(0.f);
		const Vector<4> Vector<4>::One(1.f);

		Vector<4>::Vector(const float value /*= 0.f*/)
		{
			data.fill(value);
		}

		inline XMVector Vector<4>::Load(const Vector & vector)
		{
			DirectX::XMFLOAT4 xmFloat4(vector.x, vector.y, vector.z, vector.w);
			return DirectX::XMLoadFloat4(&xmFloat4);
		}

		inline Vector<4> Vector<4>::Store(const XMVector & xmVector)
		{
			DirectX::XMFLOAT4 xmFloat4;
			DirectX::XMStoreFloat4(&xmFloat4, xmVector);
			return Vector(xmFloat4);
		}

		inline std::string Vector<4>::ToString() const
		{
			std::ostringstream oss;
			oss << std::setprecision(4) << '(' << x << ", " << y << ", " << z << ", " << w << ')';
			return oss.str();
		}

		inline bool Vector<4>::IsZero() const
		{
			return *this == Zero;
		}

		inline Vector<4>& Vector<4>::Normalize()
		{
			*this = Store(DirectX::XMVector4Normalize(Load(*this)));
			return *this;
		}

		inline float Vector<4>::Length() const
		{
			return DirectX::XMVectorGetX(DirectX::XMVector4Length(Load(*this)));
		}

	} // namespace math
} // namespace library

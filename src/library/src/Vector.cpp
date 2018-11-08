#include "StdAfx.h"
#include "library/Vector.h"

#include "library/Matrix.h"

namespace library
{
	namespace math
	{

		//-------------------------------------------------------------------------
		// Vector2
		//-------------------------------------------------------------------------

		XMVector Vector<2>::Load(const Vector & vector)
		{
			return vector.Load();
		}

		Vector<2> Vector<2>::Store(const XMVector & xmVector)
		{
			DirectX::XMFLOAT2 xmFloat2;
			DirectX::XMStoreFloat2(&xmFloat2, xmVector);
			return Vector(xmFloat2);
		}

		//-------------------------------------------------------------------------

		std::string Vector<2>::ToString() const
		{
			std::ostringstream oss;
			oss << std::setprecision(4) << '(' << x << ", " << y << ')';
			return oss.str();
		}

		XMVector Vector<2>::Load() const
		{
			DirectX::XMFLOAT2 xmFloat2(*this);
			return DirectX::XMLoadFloat2(&xmFloat2);
		}

		bool Vector<2>::IsZero() const
		{
			return *this == constants::vector<2>::Zero;
		}

		//-------------------------------------------------------------------------

		Vector<2> Vector<2>::Cross(const Vector<2>& vector) const
		{
			return Store(DirectX::XMVector2Cross(Load(), vector.Load()));
		}

		Vector<2> Vector<2>::Normalize() const
		{
			return Store(DirectX::XMVector2Normalize(Load()));
		}

		float Vector<2>::Length() const
		{
			return DirectX::XMVectorGetX(DirectX::XMVector2Length(Load()));
		}

		//-------------------------------------------------------------------------
		// operators
		//-------------------------------------------------------------------------

		bool operator == (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2Equal(lhs.Load(), rhs.Load());
		}

		bool operator != (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2NotEqual(lhs.Load(), rhs.Load());
		}

		bool operator > (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2Greater(lhs.Load(), rhs.Load());
		}

		bool operator >= (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2GreaterOrEqual(lhs.Load(), rhs.Load());
		}

		bool operator < (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2Less(lhs.Load(), rhs.Load());
		}

		bool operator <= (const Vector2& lhs, const Vector2& rhs)
		{
			return DirectX::XMVector2LessOrEqual(lhs.Load(), rhs.Load());
		}

		//-------------------------------------------------------------------------
		// Vector3
		//-------------------------------------------------------------------------

		XMVector Vector<3>::Load(const Vector & vector)
		{
			return vector.Load();
		}

		Vector<3> Vector<3>::Store(const XMVector & xmVector)
		{
			DirectX::XMFLOAT3 xmFloat3;
			DirectX::XMStoreFloat3(&xmFloat3, xmVector);
			return Vector(xmFloat3);
		}

		//-------------------------------------------------------------------------

		std::string Vector<3>::ToString() const
		{
			std::ostringstream oss;
			oss << std::setprecision(4) << '(' << x << ", " << y << ", " << z << ')';
			return oss.str();
		}

		XMVector Vector<3>::Load() const
		{
			DirectX::XMFLOAT3 xmFloat3(*this);
			return DirectX::XMLoadFloat3(&xmFloat3);
		}

		bool Vector<3>::IsZero() const
		{
			return *this == constants::vector<3>::Zero;
		}

		//-------------------------------------------------------------------------

		Vector<3> Vector<3>::Normalize() const
		{
			return Store(DirectX::XMVector3Normalize(Load()));
		}

		float Vector<3>::Length() const
		{
			return DirectX::XMVectorGetX(DirectX::XMVector3Length(Load()));
		}

		Vector<3> Vector<3>::Cross(const Vector<3>& vector) const
		{
			return Store(DirectX::XMVector3Cross(Load(), vector.Load()));
		}

		//-------------------------------------------------------------------------

		bool operator == (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3Equal(lhs.Load(), rhs.Load());
		}

		bool operator != (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3NotEqual(lhs.Load(), rhs.Load());
		}

		bool operator > (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3Greater(lhs.Load(), rhs.Load());
		}

		bool operator >= (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3GreaterOrEqual(lhs.Load(), rhs.Load());
		}

		bool operator < (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3Less(lhs.Load(), rhs.Load());
		}

		bool operator <= (const Vector3& lhs, const Vector3& rhs)
		{
			return DirectX::XMVector3LessOrEqual(lhs.Load(), rhs.Load());
		}

		//-------------------------------------------------------------------------
		// Vector4
		//-------------------------------------------------------------------------

		XMVector Vector<4>::Load(const Vector & vector)
		{
			return vector.Load();
		}

		Vector<4> Vector<4>::Store(const XMVector & xmVector)
		{
			DirectX::XMFLOAT4 xmFloat4;
			DirectX::XMStoreFloat4(&xmFloat4, xmVector);
			return Vector(xmFloat4);
		}

		//-------------------------------------------------------------------------

		std::string Vector<4>::ToString() const
		{
			std::ostringstream oss;
			oss << std::setprecision(4) << '(' << x << ", " << y << ", " << z << ", " << w << ')';
			return oss.str();
		}

		XMVector Vector<4>::Load() const
		{
			DirectX::XMFLOAT4 xmFloat4(*this);
			return DirectX::XMLoadFloat4(&xmFloat4);
		}

		bool Vector<4>::IsZero() const
		{
			return *this == constants::vector<4>::Zero;
		}

		//-------------------------------------------------------------------------

		Vector<4> Vector<4>::Cross(const Vector<4>& vector1, const Vector<4>& vector2) const
		{
			return Store(DirectX::XMVector4Cross(Load(), vector1.Load(), vector2.Load()));
		}

		Vector<4> Vector<4>::Normalize() const
		{
			return Store(DirectX::XMVector4Normalize(Load()));
		}

		float Vector<4>::Length() const
		{
			return DirectX::XMVectorGetX(DirectX::XMVector4Length(Load()));
		}

		Vector<4> Vector<4>::Transform(const Matrix4& matrix) const
		{
			return Store(DirectX::XMVector4Transform(Load(), matrix.Load()));
		}

		//-------------------------------------------------------------------------

		bool operator == (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4Equal(lhs.Load(), rhs.Load());
		}

		bool operator != (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4NotEqual(lhs.Load(), rhs.Load());
		}

		bool operator > (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4Greater(lhs.Load(), rhs.Load());
		}

		bool operator >= (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4GreaterOrEqual(lhs.Load(), rhs.Load());
		}

		bool operator < (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4Less(lhs.Load(), rhs.Load());
		}

		bool operator <= (const Vector4& lhs, const Vector4& rhs)
		{
			return DirectX::XMVector4LessOrEqual(lhs.Load(), rhs.Load());
		}

	} // namespace math
} // namespace library

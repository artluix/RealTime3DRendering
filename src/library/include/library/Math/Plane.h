#pragma once
#include "library/Math/Vector.h"
#include "library/Math/Ray.h"

namespace library::math
{
struct Plane
{
	union
	{
		struct
		{
			Vector3 normal;
			float d;
		};
		

		Vector4 _vec4;
	};

public:
	constexpr Plane(const float a, const float b, const float c, const float d) : normal(a, b, c),  d(d) {}
	explicit constexpr Plane(const float v = 0.f) : Plane(v, v, v, v) {}

	constexpr Plane(const Vector3& normal, const float d) : Plane(normal.x, normal.y, normal.z, d) {}

	Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3);
	Plane(const Vector3& point, const Vector3& normal);

	explicit constexpr Plane(const Vector4& vec4) : Plane(vec4.x, vec4.y, vec4.z, vec4.w) {}
	Plane& operator=(const Vector4& vec4) { _vec4 = vec4; return *this; }
	explicit operator const Vector4&() const { return ToVector4(); }
	const Vector4& ToVector4() const { return _vec4; }

	explicit Plane(const dx::VECTOR& dxVec) : _vec4(dxVec) {}
	Plane& operator=(const dx::VECTOR& dxVec) { _vec4 = dxVec; return *this; }
	explicit operator dx::VECTOR() const { return static_cast<dx::VECTOR>(_vec4); }

	explicit operator const float*() const { return static_cast<const float*>(_vec4); }

	//-------------------------------------------------------------------------

	Plane Transform(const Matrix4& matrix) const;

	float Dot(const Vector4& vector) const;
	float DotCoordinate(const Vector3& point) const;
	float DotNormal(const Vector3& normal) const;

	Plane Normalize() const;

	//-------------------------------------------------------------------------

	Ray Intersect(const Plane& other) const;
	Vector3 Intersect(const Ray& ray) const;

	//-------------------------------------------------------------------------

	bool operator==(const Plane& other) const { return _vec4 == other._vec4; }
	bool operator!=(const Plane& other) const { return _vec4 != other._vec4; }

	bool operator<(const Plane&) const = delete;
	bool operator<=(const Plane&) const = delete;
	bool operator>(const Plane&) const = delete;
	bool operator>=(const Plane&) const = delete;
};
} // namespace library::math

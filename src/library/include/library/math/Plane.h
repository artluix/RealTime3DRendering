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
			float a, b, c, d;
		};

		struct
		{
			Vector3 normal;
			float _d;
		};

		std::array<float, 4> _data;
	};

	//-------------------------------------------------------------------------

	constexpr Plane(const float a, const float b, const float c, const float d) : a(a), b(b), c(c), d(d) {}
	constexpr Plane(const Vector3& normal, const float d) : normal(normal), _d(d) {}
	explicit constexpr Plane(const float v = 0.f) : Plane(v, v, v, v) {}
	explicit constexpr Plane(const XMVector4& xmFloat4)
		: Plane(xmFloat4.x, xmFloat4.y, xmFloat4.z, xmFloat4.w)
	{}
	explicit constexpr Plane(const XMVector& xmVector)
		: Plane(xmVector.m128_f32[0], xmVector.m128_f32[1], xmVector.m128_f32[2], xmVector.m128_f32[3])
	{}

	Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3);
	Plane(const Vector3& point, const Vector3& normal);

	Plane& operator=(const XMVector& xmVector);

	explicit constexpr Plane(const Vector4& vec4) : Plane(vec4.x, vec4.y, vec4.z, vec4.w) {}

	//-------------------------------------------------------------------------

	const float& operator[](const unsigned idx) const { return _data[idx]; }
	float& operator[](const unsigned idx) { return _data[idx]; }
	operator const float*() const { return _data.data(); }

	//-------------------------------------------------------------------------

	explicit operator XMVector() const; // should be implicit, otherwise SFINAE won't detect it
	explicit operator const XMVector4&() const;
	explicit operator bool() const;

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

	bool operator==(const Plane& other) const;
	bool operator!=(const Plane& other) const;

	bool operator<(const Plane&) const = delete;
	bool operator<=(const Plane&) const = delete;
	bool operator>(const Plane&) const = delete;
	bool operator>=(const Plane&) const = delete;
};
} // namespace library::math

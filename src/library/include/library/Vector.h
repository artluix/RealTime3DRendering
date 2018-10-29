#pragma once
#include <cstdio>
#include <array>
#include <string>
#include <sstream>
#include <iomanip>
#include <DirectXMath.h>

namespace library
{
	namespace math
	{

		using XMVector = DirectX::XMVECTOR;

		// float only
		template<std::size_t Size>
		struct Vector;

		//-------------------------------------------------------------------------
		// Vector2
		//-------------------------------------------------------------------------

		template<>
		struct Vector<2>
		{
			union
			{
				struct
				{
					float x, y;
				};

				std::array<float, 2> data;
			};

			//-------------------------------------------------------------------------

			Vector(const float value = 0.f);
			Vector(const float x, const float y) : x(x), y(y) {}
			Vector(const DirectX::XMFLOAT2& xmFloat2) : x(xmFloat2.x), y(xmFloat2.y) {}

			//-------------------------------------------------------------------------

			static const Vector Zero;
			static const Vector One;

			static XMVector Load(const Vector& vector);
			static Vector Store(const XMVector& xmVector);

			//-------------------------------------------------------------------------

			std::string ToString() const;

			bool IsZero() const;

			Vector& Normalize();
			float Length() const;

			explicit operator DirectX::XMFLOAT2() const { return DirectX::XMFLOAT2(x, y); }
			explicit operator bool() const { return !IsZero(); }
			explicit operator const float* () const { return data.data(); }

			const float& operator[] (const std::size_t idx) const { return data[idx]; }
			float& operator[] (const std::size_t idx) { return data[idx]; }
		};

		//-------------------------------------------------------------------------
		// Vector3
		//-------------------------------------------------------------------------

		template<>
		struct Vector<3>
		{
			union
			{
				struct
				{
					float x, y, z, w;
				};

				std::array<float, 3> data;
			};

			Vector(const float value = 0.f);
			Vector(const float x, const float y, const float z) : x(x), y(y), z(z) {}
			Vector(const DirectX::XMFLOAT3& xmFloat4) : x(xmFloat4.x), y(xmFloat4.y), z(xmFloat4.z) {}

			//-------------------------------------------------------------------------

			static const Vector Zero;
			static const Vector One;

			static XMVector Load(const Vector& vector);
			static Vector Store(const XMVector& vector);

			//-------------------------------------------------------------------------

			std::string ToString() const;
			
			bool IsZero() const;

			Vector& Normalize();
			float Length() const;

			Vector& Transform();

			explicit operator DirectX::XMFLOAT3() const { return DirectX::XMFLOAT3(x, y, w); }
			explicit operator bool() const { return !IsZero(); }
			explicit operator const float* () const { return data.data(); }

			const float& operator[] (const std::size_t idx) const { return data[idx]; }
			float& operator[] (const std::size_t idx) { return data[idx]; }
		};

		//-------------------------------------------------------------------------
		// Vector4
		//-------------------------------------------------------------------------

		template<>
		struct Vector<4>
		{
			union
			{
				struct
				{
					float x, y, z, w;
				};

				std::array<float, 4> data;
			};

			//-------------------------------------------------------------------------

			Vector(const float value = 0.f);
			Vector(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
			Vector(const DirectX::XMFLOAT4& xmFloat4) : x(xmFloat4.x), y(xmFloat4.y), z(xmFloat4.z), w(xmFloat4.w) {}
			Vector(const Vector<3>& vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(0.f) {}

			//-------------------------------------------------------------------------

			static const Vector Zero;
			static const Vector One;

			static XMVector Load(const Vector& vector);
			static Vector Store(const XMVector& xmVector);

			//-------------------------------------------------------------------------

			std::string ToString() const;

			bool IsZero() const;

			Vector& Normalize();
			float Length() const;

			explicit operator DirectX::XMFLOAT4() const { return DirectX::XMFLOAT4(x, y, w, z); }
			explicit operator bool() const { return !IsZero(); }
			explicit operator const float* () const { return data.data(); }

			const float& operator[] (const std::size_t idx) const { return data[idx]; }
			float& operator[] (const std::size_t idx) { return data[idx]; }
		};

		//-------------------------------------------------------------------------
		// aliases
		//-------------------------------------------------------------------------

		using Vector2 = Vector<2>;
		using Vector3 = Vector<3>;
		using Vector4 = Vector<4>;

	} // namespace math
} // namespace library

#include "Vector.inl"

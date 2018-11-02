#pragma once
#include "library/NonConstructible.hpp"

#include <DirectXMath.h>
#include <cstdio>
#include <string>
#include <array>

namespace library
{
	namespace math
	{

		using XMVector = DirectX::XMVECTOR;

		//-------------------------------------------------------------------------
		// forward declarations
		//-------------------------------------------------------------------------

		template<std::size_t Size>
		struct Vector;

		using Vector2 = Vector<2>;
		using Vector3 = Vector<3>;
		using Vector4 = Vector<4>;

		//-------------------------------------------------------------------------

		template<std::size_t Size>
		struct Matrix;

		using Matrix3 = Matrix<3>;
		using Matrix4 = Matrix<4>;

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

			constexpr Vector(const float value = 0.f) : x(value), y(value) {}
			constexpr Vector(const float x, const float y) : x(x), y(y) {}
			constexpr Vector(const DirectX::XMFLOAT2& xmFloat2) : x(xmFloat2.x), y(xmFloat2.y) {}

			//-------------------------------------------------------------------------

			static XMVector Load(const Vector& vector);
			static Vector Store(const XMVector& xmVector);

			//-------------------------------------------------------------------------

			std::string ToString() const;

			XMVector Load() const;

			bool IsZero() const;

			//-------------------------------------------------------------------------

			Vector Cross(const Vector& vector) const;

			Vector Normalize() const;
			float Length() const;

			//-------------------------------------------------------------------------

			explicit operator bool() const { return !IsZero(); }
			explicit operator DirectX::XMFLOAT2() const { return DirectX::XMFLOAT2(x, y); }
			explicit operator const float* () const { return data.data(); }

			const float& operator[] (const std::size_t idx) const { return data[idx]; }
			float& operator[] (const std::size_t idx) { return data[idx]; }
		};

		//-------------------------------------------------------------------------

		bool operator == (const Vector2& lhs, const Vector2& rhs);
		bool operator != (const Vector2& lhs, const Vector2& rhs);
		bool operator > (const Vector2& lhs, const Vector2& rhs);
		bool operator >= (const Vector2& lhs, const Vector2& rhs);
		bool operator < (const Vector2& lhs, const Vector2& rhs);
		bool operator <= (const Vector2& lhs, const Vector2& rhs);

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
					float x, y, z;
				};

				std::array<float, 3> data;
			};

			constexpr Vector(const float value = 0.f) : x(value), y(value), z(value) {}
			constexpr Vector(const float x, const float y, const float z) : x(x), y(y), z(z) {}
			constexpr Vector(const DirectX::XMFLOAT3& xmFloat3) : x(xmFloat3.x), y(xmFloat3.y), z(xmFloat3.z) {}
			constexpr Vector(const Vector<2>& vec2) : x(vec2.x), y(vec2.y), z(0.f) {}

			//-------------------------------------------------------------------------

			static XMVector Load(const Vector& vector);
			static Vector Store(const XMVector& vector);

			//-------------------------------------------------------------------------

			std::string ToString() const;

			XMVector Load() const;

			bool IsZero() const;

			//-------------------------------------------------------------------------

			Vector Cross(const Vector& vector) const;

			Vector Normalize() const;
			float Length() const;

			template <std::size_t Size>
			Vector Transform(const Matrix<Size>& matrix) const;

			template <std::size_t Size>
			Vector TransformNormal(const Matrix<Size>& matrix) const;

			//-------------------------------------------------------------------------

			explicit operator bool() const { return !IsZero(); }
			explicit operator DirectX::XMFLOAT3() const { return DirectX::XMFLOAT3(x, y, z); }
			explicit operator const float* () const { return data.data(); }

			const float& operator[] (const std::size_t idx) const { return data[idx]; }
			float& operator[] (const std::size_t idx) { return data[idx]; }
		};

		//-------------------------------------------------------------------------

		template <std::size_t Size>
		inline Vector3 Vector<3>::Transform(const Matrix<Size>& matrix) const
		{
			return Store(DirectX::XMVector3Transform(Load(), matrix.Load()));
		}

		template <std::size_t Size>
		inline Vector3 Vector<3>::TransformNormal(const Matrix<Size>& matrix) const
		{
			return Store(DirectX::XMVector3TransformNormal(Load(), matrix.Load()));
		}

		bool operator == (const Vector3& lhs, const Vector3& rhs);
		bool operator != (const Vector3& lhs, const Vector3& rhs);
		bool operator > (const Vector3& lhs, const Vector3& rhs);
		bool operator >= (const Vector3& lhs, const Vector3& rhs);
		bool operator < (const Vector3& lhs, const Vector3& rhs);
		bool operator <= (const Vector3& lhs, const Vector3& rhs);

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

				struct
				{
					float r, g, b, a;
				};

				std::array<float, 4> data;
			};

			//-------------------------------------------------------------------------

			constexpr Vector(const float value = 0.f) : x(value), y(value), z(value), w(value) {}
			constexpr Vector(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
			constexpr Vector(const DirectX::XMFLOAT4& xmFloat4) : x(xmFloat4.x), y(xmFloat4.y), z(xmFloat4.z), w(xmFloat4.w) {}
			constexpr Vector(const Vector<3>& vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(0.f) {}

			//-------------------------------------------------------------------------

			static XMVector Load(const Vector& vector);
			static Vector Store(const XMVector& xmVector);

			//-------------------------------------------------------------------------

			std::string ToString() const;

			XMVector Load() const;

			bool IsZero() const;

			//-------------------------------------------------------------------------

			Vector Cross(const Vector& vector1, const Vector& vector2) const;

			Vector Normalize() const;
			float Length() const;

			Vector Transform(const Matrix4& matrix) const;

			//-------------------------------------------------------------------------

			explicit operator bool() const { return !IsZero(); }
			explicit operator DirectX::XMFLOAT4() const { return DirectX::XMFLOAT4(x, y, z, w); }
			explicit operator const float* () const { return data.data(); }

			const float& operator[] (const std::size_t idx) const { return data[idx]; }
			float& operator[] (const std::size_t idx) { return data[idx]; }
		};

		//-------------------------------------------------------------------------

		bool operator == (const Vector4& lhs, const Vector4& rhs);
		bool operator != (const Vector4& lhs, const Vector4& rhs);
		bool operator > (const Vector4& lhs, const Vector4& rhs);
		bool operator >= (const Vector4& lhs, const Vector4& rhs);
		bool operator < (const Vector4& lhs, const Vector4& rhs);
		bool operator <= (const Vector4& lhs, const Vector4& rhs);

		//-------------------------------------------------------------------------
		// explicit instantiation
		//-------------------------------------------------------------------------

		extern template struct Vector<2>;
		extern template struct Vector<3>;
		extern template struct Vector<4>;

		//-------------------------------------------------------------------------
		// constants
		//-------------------------------------------------------------------------

		namespace constants
		{
			template<std::size_t Size>
			struct vector : public NonConstructible<vector<Size>>
			{
				static constexpr Vector<Size> Zero = Vector<Size>(0.f);
				static constexpr Vector<Size> One = Vector<Size>(1.f);
			};

			template<>
			struct vector<3> : public NonConstructible<vector<3>>
			{
				static constexpr Vector3 Zero = Vector3(0.f);
				static constexpr Vector3 One = Vector3(1.f);

				static constexpr Vector3 Forward = Vector3(0.f, 0.f, -1.f);
				static constexpr Vector3 Backward = Vector3(0.f, 0.f, 1.f);
				static constexpr Vector3 Up = Vector3(0.f, 1.f, 0.f);
				static constexpr Vector3 Down = Vector3(0.f, -1.f, 0.f);
				static constexpr Vector3 Right = Vector3(1.f, 0.f, 0.f);
				static constexpr Vector3 Left = Vector3(-1.f, 0.f, 0.f);
			};
		} // namespace constants

		//-------------------------------------------------------------------------
		// common operators
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// add
		//-------------------------------------------------------------------------

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
		inline Vector<Size>& operator += (Vector<Size>& lhs, const float value)
		{
			lhs += Vector<Size>(value);
			return lhs;
		}

		template<std::size_t Size>
		inline Vector<Size> operator + (const float value, const Vector<Size>& lhs)
		{
			return Vector(value) + lhs;
		}

		//-------------------------------------------------------------------------
		// negate
		//-------------------------------------------------------------------------

		template<std::size_t Size>
		inline Vector<Size> operator - (const Vector<Size>& vector)
		{
			return Vector<Size>::Store(DirectX::XMVectorNegate(Vector<Size>::Load(vector)));
		}

		//-------------------------------------------------------------------------
		// subtract
		//-------------------------------------------------------------------------

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

		//-------------------------------------------------------------------------
		// multiply
		//-------------------------------------------------------------------------

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
		inline Vector<Size>& operator *= (Vector<Size>& lhs, const float value)
		{
			lhs *= Vector<Size>(value);
			return lhs;
		}

		template<std::size_t Size>
		inline Vector<Size> operator * (const float value, const Vector<Size>& lhs)
		{
			return Vector<Size>(value) * lhs;
		}

		//-------------------------------------------------------------------------
		// divide
		//-------------------------------------------------------------------------

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

	} // namespace math
} // namespace library

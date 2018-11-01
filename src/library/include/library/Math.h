#pragma once
#include <array>
#include <cstdio>
#include <string>
#include <DirectXMath.h>

namespace library
{
	namespace math
	{

		//-------------------------------------------------------------------------
		// Common
		//-------------------------------------------------------------------------

		template <typename T>
		inline T Min(const T& lhs, const T& rhs)
		{
			return (lhs < rhs) ? lhs : rhs;
		}

		template <typename T>
		inline T Max(const T& lhs, const T& rhs)
		{
			return (lhs > rhs) ? lhs : rhs;
		}

		template <typename T>
		inline T Clamp(const T& x, const T& min, const T& max)
		{
			return Min(Max(x, min), max);
		}

		//-------------------------------------------------------------------------
		// forward declarations
		//-------------------------------------------------------------------------

		using XMVector = DirectX::XMVECTOR;
		using XMMatrix = DirectX::XMMATRIX;

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

			Vector(const float value = 0.f) : x(value), y(value) {}
			Vector(const float x, const float y) : x(x), y(y) {}
			Vector(const DirectX::XMFLOAT2& xmFloat2) : x(xmFloat2.x), y(xmFloat2.y) {}

			//-------------------------------------------------------------------------

			static const Vector Zero;
			static const Vector One;

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

			Vector(const float value = 0.f) : x(value), y(value), z(value) {}
			Vector(const float x, const float y, const float z) : x(x), y(y), z(z) {}
			Vector(const DirectX::XMFLOAT3& xmFloat3) : x(xmFloat3.x), y(xmFloat3.y), z(xmFloat3.z) {}
			Vector(const Vector<2>& vec2) : x(vec2.x), y(vec2.y), z(0.f) {}

			//-------------------------------------------------------------------------

			static const Vector Zero;
			static const Vector One;

			static const Vector Forward;
			static const Vector Backward;
			static const Vector Up;
			static const Vector Down;
			static const Vector Right;
			static const Vector Left;

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

			Vector(const float value = 0.f) : x(value), y(value), z(value), w(value) {}
			constexpr Vector(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
			Vector(const DirectX::XMFLOAT4& xmFloat4) : x(xmFloat4.x), y(xmFloat4.y), z(xmFloat4.z), w(xmFloat4.w) {}
			Vector(const Vector<3>& vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(0.f) {}

			//-------------------------------------------------------------------------

			static const Vector Zero;
			static const Vector One;

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
		// Matrix3
		//-------------------------------------------------------------------------

		template<>
		struct Matrix<3>
		{
			union
			{
				struct
				{
					float _11, _12, _13;
					float _21, _22, _23;
					float _31, _32, _33;
				};

				std::array<Vector3, 3> rows;
				std::array<float, 3 * 3> data;
			};

			//-------------------------------------------------------------------------

			Matrix(const float value = 0.f);
			Matrix(
				const float _11, const float _12, const float _13,
				const float _21, const float _22, const float _23,
				const float _31, const float _32, const float _33
			);
			Matrix(const Vector3& r1, const Vector3& r2, const Vector3& r3);
			Matrix(const DirectX::XMFLOAT3X3& xmFloat3x3);

			//-------------------------------------------------------------------------

			static const Matrix Zero;
			static const Matrix Identity;

			//-------------------------------------------------------------------------

			static XMMatrix Load(const Matrix& matrix);
			static Matrix Store(const XMMatrix& matrix);

			//-------------------------------------------------------------------------

			std::string ToString() const;

			XMMatrix Load() const;

			//-------------------------------------------------------------------------

			Matrix Transpose() const;

			//-------------------------------------------------------------------------

			explicit operator DirectX::XMFLOAT3X3() const;
			explicit operator const float* () const { return data.data(); }

			const float& operator() (const std::size_t rowIdx, const std::size_t colIdx) const;
			float& operator() (const std::size_t rowIdx, const std::size_t colIdx);
		};

		//-------------------------------------------------------------------------
		// Matrix4
		//-------------------------------------------------------------------------

		template<>
		struct Matrix<4>
		{
			union
			{
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
					float _41, _42, _43, _44;
				};

				std::array<Vector4, 4> rows;
				std::array<float, 4 * 4> data;
			};

			//-------------------------------------------------------------------------

			Matrix(const float value = 0.f);
			Matrix(
				const float _11, const float _12, const float _13, const float _14,
				const float _21, const float _22, const float _23, const float _24,
				const float _31, const float _32, const float _33, const float _34,
				const float _41, const float _42, const float _43, const float _44
			);
			Matrix(const Vector4& r1, const Vector4& r2, const Vector4& r3, const Vector4& r4);
			Matrix(const DirectX::XMFLOAT4X4& xmFloat4x4);

			//-------------------------------------------------------------------------

			static const Matrix Zero;
			static const Matrix Identity;

			//-------------------------------------------------------------------------

			static XMMatrix Load(const Matrix& mat);
			static Matrix Store(const XMMatrix& xmMat);

			//-------------------------------------------------------------------------

			static Matrix LookAtLH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir);
			static Matrix LookAtRH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir);

			static Matrix LookToLH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir);
			static Matrix LookToRH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir);

			//-------------------------------------------------------------------------

			static Matrix Translation(const Vector3& vector);
			static Matrix Translation(const float x, const float y, const float z);

			//-------------------------------------------------------------------------

			static Matrix PerspectiveFovLH(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ);
			static Matrix PerspectiveFovRH(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ);

			//-------------------------------------------------------------------------

			static Matrix RotationAxis(const Vector3& axis, const float angle);

			static Matrix RotationRollPitchYaw(const float roll, const float pitch, const float yaw);
			static Matrix RotationRollPitchYaw(const Vector3& vector);

			static Matrix RotationX(const float angle);
			static Matrix RotationY(const float angle);
			static Matrix RotationZ(const float angle);

			//-------------------------------------------------------------------------

			std::string ToString() const;

			XMMatrix Load() const;

			//-------------------------------------------------------------------------

			Matrix Transpose() const;

			Vector3 GetForward() const;
			Matrix& SetForward(const Vector3& vector);

			Vector3 GetUp() const;
			Matrix& SetUp(const Vector3& vector);

			Vector3 GetRight() const;
			Matrix& SetRight(const Vector3& vector);

			Vector3 GetTranslation() const;
			Matrix& SetTranslation(const Vector3& vector);

			//-------------------------------------------------------------------------

			explicit operator DirectX::XMFLOAT4X4() const;
			explicit operator const float* () const { return data.data(); }

			const float& operator() (const std::size_t rowIdx, const std::size_t colIdx) const;
			float& operator() (const std::size_t rowIdx, const std::size_t colIdx);
		};

	} // namespace math
} // namespace library

#include "library/Math.inl"

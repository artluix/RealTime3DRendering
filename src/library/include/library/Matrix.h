#pragma once
#include "library/Vector.h"

namespace library
{
	namespace math
	{
		using XMMatrix = DirectX::XMMATRIX;

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
			// constructors
			//-------------------------------------------------------------------------

			constexpr Matrix(const float value = 0.f)
				: rows{
					Vector3(value),
					Vector3(value),
					Vector3(value),
				}
			{
			}

			constexpr Matrix(
				const float _11, const float _12, const float _13,
				const float _21, const float _22, const float _23,
				const float _31, const float _32, const float _33
			)
				: _11(_11), _12(_12), _13(_13)
				, _21(_21), _22(_22), _23(_23)
				, _31(_31), _32(_32), _33(_33)
			{
			}

			constexpr Matrix(const Vector3& r1, const Vector3& r2, const Vector3& r3)
				: rows{ r1, r2, r3 }
			{
			}

			constexpr Matrix(const DirectX::XMFLOAT3X3& xmFloat3x3)
				: _11(xmFloat3x3._11), _12(xmFloat3x3._12), _13(xmFloat3x3._13)
				, _21(xmFloat3x3._21), _22(xmFloat3x3._22), _23(xmFloat3x3._23)
				, _31(xmFloat3x3._31), _32(xmFloat3x3._32), _33(xmFloat3x3._33)
			{
			}

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

		Vector3 operator * (const Vector3& vector, const Matrix3& matrix);
		Vector3& operator *= (Vector3& vector, const Matrix3& matrix);

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
			// constructors
			//-------------------------------------------------------------------------

			constexpr Matrix(const float value = 0.f)
				: rows{
					Vector4(value),
					Vector4(value),
					Vector4(value),
					Vector4(value),
				}
			{
			}

			constexpr Matrix(
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

			constexpr Matrix(const Vector4& r1, const Vector4& r2, const Vector4& r3, const Vector4& r4)
				: rows{ r1, r2, r3, r4 }
			{
			}

			constexpr Matrix(const DirectX::XMFLOAT4X4& xmFloat4x4)
				: _11(xmFloat4x4._11), _12(xmFloat4x4._12), _13(xmFloat4x4._13), _14(xmFloat4x4._14)
				, _21(xmFloat4x4._21), _22(xmFloat4x4._22), _23(xmFloat4x4._23), _24(xmFloat4x4._24)
				, _31(xmFloat4x4._31), _32(xmFloat4x4._32), _33(xmFloat4x4._33), _34(xmFloat4x4._34)
				, _41(xmFloat4x4._41), _42(xmFloat4x4._42), _43(xmFloat4x4._43), _44(xmFloat4x4._44)
			{
			}

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

		//-------------------------------------------------------------------------

		Vector4 operator * (const Vector4& vector, const Matrix4& matrix);
		Vector4& operator *= (Vector4& vector, const Matrix4& matrix);

		//-------------------------------------------------------------------------
		// explicit instantiation
		//-------------------------------------------------------------------------

		extern template struct Matrix<3>;
		extern template struct Matrix<4>;

		//-------------------------------------------------------------------------
		// constants
		//-------------------------------------------------------------------------

		namespace constants
		{
			template<std::size_t Size>
			struct matrix;

			template<>
			struct matrix<3> : NonConstructible<matrix<3>>
			{
				static constexpr Matrix3 Zero = Matrix3(0.f);
				static constexpr Matrix3 Identity = Matrix3(
					1.f, 0.f, 0.f,
					0.f, 1.f, 0.f,
					0.f, 0.f, 1.f
				);
			};

			template<>
			struct matrix<4> : NonConstructible<matrix<4>>
			{
				static constexpr Matrix4 Zero = Matrix4(0.f);
				static constexpr Matrix4 Identity = Matrix4(
					1.f, 0.f, 0.f, 0.f,
					0.f, 1.f, 0.f, 0.f,
					0.f, 0.f, 1.f, 0.f,
					0.f, 0.f, 0.f, 1.f
				);
			};
		} // namespace constants

		//-------------------------------------------------------------------------
		// common operators
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// add
		//-------------------------------------------------------------------------

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

		//-------------------------------------------------------------------------
		// subtract
		//-------------------------------------------------------------------------

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

		//-------------------------------------------------------------------------
		// multiply
		//-------------------------------------------------------------------------

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

	} // namespace math
} // namespace library

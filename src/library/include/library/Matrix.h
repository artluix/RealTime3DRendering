#pragma once
#include "Vector.h"

namespace library
{
	namespace math
	{

		using XMMatrix = DirectX::XMMATRIX;

		template<std::size_t Size>
		struct Matrix;

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

			static const Matrix Identity;
			static const Matrix Zero;

			static XMMatrix Load(const Matrix& matrix);
			static Matrix Store(const XMMatrix& matrix);

			//-------------------------------------------------------------------------

			std::string ToString() const;

			//Matrix Transpose() const;
			Matrix& Transpose();

			explicit operator const float* () const { return data.data(); }

			const float& operator() (const std::size_t rowIdx, const std::size_t colIdx) const { return rows[rowIdx][colIdx]; }
			float& operator() (const std::size_t rowIdx, const std::size_t colIdx) { return rows[rowIdx][colIdx]; }
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

			static XMMatrix Load(const Matrix& mat);
			static Matrix Store(const XMMatrix& xmMat);

			static Matrix LookAtLH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir);
			static Matrix LookAtRH(const Vector3& eyePos, const Vector3& focusPos, const Vector3& upDir);

			static Matrix LookToLH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir);
			static Matrix LookToRH(const Vector3& eyePos, const Vector3& eyeDir, const Vector3& upDir);

			//-------------------------------------------------------------------------

			std::string ToString() const;

			//Matrix Transpose() const;
			Matrix& Transpose();

			Vector3 GetForward() const;
			Matrix& SetForward(const Vector3& vector);

			Vector3 GetUp() const;
			Matrix& SetUp(const Vector3& vector);

			Vector3 GetRight() const;
			Matrix& SetRight(const Vector3& vector);

			Vector3 GetTranslation() const;
			Matrix& SetTranslation(const Vector3& vector);

			explicit operator const float* () const { return data.data(); }

			const float& operator() (const std::size_t rowIdx, const std::size_t colIdx) const { return rows[rowIdx][colIdx]; }
			float& operator() (const std::size_t rowIdx, const std::size_t colIdx) { return rows[rowIdx][colIdx]; }
		};

		// aliases

		using Matrix3 = Matrix<3>;
		using Matrix4 = Matrix<4>;

	} // namespace math
} // namespace library

#include "Matrix.inl"

#pragma once
#include "library/NonConstructible.hpp"

#include <DirectXMath.h>
#include <string>

namespace library
{
    namespace math
    {
        /////////////////////////////////////////////////////////////////
        // common
        /////////////////////////////////////////////////////////////////

        template <typename T>
        inline T Min(const T lhs, const T rhs)
        {
            return (lhs < rhs) ? lhs : rhs;
        }

        template <typename T>
        inline T Max(const T lhs, const T rhs)
        {
            return (lhs > rhs) ? lhs : rhs;
        }

        template <typename T>
        inline T Clamp(const T x, const T min, const T max)
        {
            return Min(Max(x, min), max);
        }

        /////////////////////////////////////////////////////////////////
        // helpers
        /////////////////////////////////////////////////////////////////

        namespace helpers
        {
            /////////////////////////////////////////////////////////////////
            // vectors
            /////////////////////////////////////////////////////////////////

            struct Vector : public NonConstructible<Vector>
            {
                using Type = DirectX::XMVECTOR;
                using ConstType = DirectX::CXMVECTOR;
            };

            /////////////////////////////////////////////////////////////////

            struct Vector2 : public NonConstructible<Vector2>
            {
                using Type = DirectX::XMFLOAT2;

                static const Type Zero;
                static const Type One;

                static std::string ToString(const Type& vector);
            };

            /////////////////////////////////////////////////////////////////

            struct Vector3 : public NonConstructible<Vector3>
            {
                using Type = DirectX::XMFLOAT3;

                static const Type Zero;
                static const Type One;

                static const Type Forward;
                static const Type Backward;
                static const Type Up;
                static const Type Down;
                static const Type Left;
                static const Type Right;

                static std::string ToString(const Type& vector);
            };

            /////////////////////////////////////////////////////////////////

            struct Vector4 : public NonConstructible<Vector4>
            {
                using Type = DirectX::XMFLOAT4;

                static const Type Zero;
                static const Type One;

                static std::string ToString(const Type& vector);
            };

            /////////////////////////////////////////////////////////////////
            // matrices
            /////////////////////////////////////////////////////////////////

            struct Matrix : public NonConstructible<Matrix>
            {
                using Type = DirectX::XMMATRIX;
                using ConstType = DirectX::CXMMATRIX;
            };

            /////////////////////////////////////////////////////////////////

            struct Matrix3 : public NonConstructible<Matrix3>
            {
                using Type = DirectX::XMFLOAT3X3;

                static const Type Idendity;
            };

            /////////////////////////////////////////////////////////////////

            struct Matrix4 : public NonConstructible<Matrix4>
            {
                using Type = DirectX::XMFLOAT4X4;

                static const Type Identity;

                static Vector3::Type GetForward(Matrix::ConstType matrix);
                static Vector3::Type GetUp(Matrix::ConstType matrix);
                static Vector3::Type GetRight(Matrix::ConstType matrix);
                static Vector3::Type GetTranslation(Matrix::ConstType matrix);

                static Matrix::Type& SetForward(Matrix::Type& matrix, const Vector3::Type& vector);
                static Matrix::Type& SetUp(Matrix::Type& matrix, const Vector3::Type& vector);
                static Matrix::Type& SetRight(Matrix::Type& matrix, const Vector3::Type& vector);
                static Matrix::Type& SetTranslation(Matrix::Type& matrix, const Vector3::Type& vector);
            };
        }
    }
}

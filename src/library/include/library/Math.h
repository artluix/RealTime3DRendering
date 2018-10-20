#pragma once
#include "library/NonConstructible.hpp"
#include "library/Math.inl"

#include <string>
#include <sstream>
#include <iomanip>

namespace library
{
    /////////////////////////////////////////////////////////////////
    // forward DirectX operators to library namespace
    /////////////////////////////////////////////////////////////////

    using DirectX::operator*;
    using DirectX::operator/;
    using DirectX::operator+;
    using DirectX::operator-;
    using DirectX::operator*=;
    using DirectX::operator/=;
    using DirectX::operator+=;
    using DirectX::operator-=;

    namespace math
    {
        /////////////////////////////////////////////////////////////////
        // common
        /////////////////////////////////////////////////////////////////

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

        /////////////////////////////////////////////////////////////////
        // vectors
        /////////////////////////////////////////////////////////////////

        using Vector = XMVector;

        /////////////////////////////////////////////////////////////////
        // vector2
        /////////////////////////////////////////////////////////////////

        template<typename ValueType, bool Aligned=false>
        struct Vector2 : public XMV<2, ValueType, Aligned>
        {
            using XMV<2, ValueType, Aligned>::XMV;

            static const Vector2 Zero;
            static const Vector2 One;

            std::string ToString() const
            {
                std::stringstream ss;
                ss << std::setprecision(4) << "(" << x << ", " << y << ")";
                return ss.str();
            }
        };

        /////////////////////////////////////////////////////////////////

        template<typename ValueType, bool Aligned>
        const Vector2<ValueType, Aligned> Vector2<ValueType, Aligned>::Zero = Vector2(ValueType(0), ValueType(0));
        
        template<typename ValueType, bool Aligned>
        const Vector2<ValueType, Aligned> Vector2<ValueType, Aligned>::One = Vector2(ValueType(1), ValueType(1));

        /////////////////////////////////////////////////////////////////

        using Vector2f = Vector2<float>;
        using Vector2fa = Vector2<float, true>;
        using Vector2i = Vector2<int>;
        using Vector2u = Vector2<unsigned>;

        /////////////////////////////////////////////////////////////////
        // vector 3
        /////////////////////////////////////////////////////////////////

        template<typename ValueType, bool Aligned = false>
        struct Vector3 : public XMV<3, ValueType, Aligned>
        {
            using XMV<3, ValueType, Aligned>::XMV;

            static const Vector3 Zero;
            static const Vector3 One;

            static const Vector3 Forward;
            static const Vector3 Backward;
            static const Vector3 Up;
            static const Vector3 Down;
            static const Vector3 Left;
            static const Vector3 Right;

            std::string ToString() const
            {
                std::stringstream ss;
                ss << std::setprecision(4) << "(" << x << ", " << y << ", " << z << ")";
                return ss.str();
            }
        };

        /////////////////////////////////////////////////////////////////

        template<typename ValueType, bool Aligned>
        const Vector3<ValueType, Aligned> Vector3<ValueType, Aligned>::Zero = Vector3(ValueType(0), ValueType(0), ValueType(0));

        template<typename ValueType, bool Aligned>
        const Vector3<ValueType, Aligned> Vector3<ValueType, Aligned>::One = Vector3(ValueType(1), ValueType(1), ValueType(1));

        template<typename ValueType, bool Aligned>
        const Vector3<ValueType, Aligned> Vector3<ValueType, Aligned>::Forward = Vector3(ValueType(0), ValueType(0), ValueType(-1));

        template<typename ValueType, bool Aligned>
        const Vector3<ValueType, Aligned> Vector3<ValueType, Aligned>::Backward = Vector3(ValueType(0), ValueType(0), ValueType(1));

        template<typename ValueType, bool Aligned>
        const Vector3<ValueType, Aligned> Vector3<ValueType, Aligned>::Up = Vector3(ValueType(0), ValueType(1), ValueType(0));

        template<typename ValueType, bool Aligned>
        const Vector3<ValueType, Aligned> Vector3<ValueType, Aligned>::Down = Vector3(ValueType(0), ValueType(-1), ValueType(0));

        template<typename ValueType, bool Aligned>
        const Vector3<ValueType, Aligned> Vector3<ValueType, Aligned>::Left = Vector3(ValueType(-1), ValueType(0), ValueType(0));

        template<typename ValueType, bool Aligned>
        const Vector3<ValueType, Aligned> Vector3<ValueType, Aligned>::Right = Vector3(ValueType(1), ValueType(0), ValueType(0));

        /////////////////////////////////////////////////////////////////

        using Vector3f = Vector3<float>;
        using Vector3fa = Vector3<float, true>;
        using Vector3i = Vector3<int>;
        using Vector3u = Vector3<unsigned>;

        /////////////////////////////////////////////////////////////////
        // vector 4
        /////////////////////////////////////////////////////////////////

        template<typename ValueType, bool Aligned = false>
        struct Vector4 : public XMV<4, ValueType, Aligned>
        {
            using XMV<4, ValueType, Aligned>::XMV;

            static const Vector4 Zero;
            static const Vector4 One;

            std::string ToString() const
            {
                std::stringstream ss;
                ss << std::setprecision(4) << "(" << x << ", " << y << ", " << z << ", " << w << ")";
                return ss.str();
            }
        };

        /////////////////////////////////////////////////////////////////

        template<typename ValueType, bool Aligned>
        typename const Vector4<ValueType, Aligned> Vector4<ValueType, Aligned>::Zero = Vector4(ValueType(0), ValueType(0), ValueType(0), ValueType(0));

        template<typename ValueType, bool Aligned>
        typename const Vector4<ValueType, Aligned> Vector4<ValueType, Aligned>::One = Vector4(ValueType(1), ValueType(1), ValueType(1), ValueType(1));

        /////////////////////////////////////////////////////////////////

        using Vector4f = Vector4<float>;
        using Vector4fa = Vector4<float, true>;
        using Vector4i = Vector4<int>;
        using Vector4u = Vector4<unsigned>;

        /////////////////////////////////////////////////////////////////
        // matrices (always float and not aligned)
        /////////////////////////////////////////////////////////////////

        using Matrix = XMMatrix;

        /////////////////////////////////////////////////////////////////

        struct Matrix3 : public XMM<3, 3>
        {
            using XMM::XMM;

            static const Matrix3 Zero;
            static const Matrix3 Idendity;
        };

        /////////////////////////////////////////////////////////////////

        struct Matrix4 : public XMM<4, 4>
        {
            using XMM::XMM;

            static const Matrix4 Zero;
            static const Matrix4 Identity;

            static Vector3f GetForward(const Matrix& matrix);
            static Vector3f GetUp(const Matrix& matrix);
            static Vector3f GetRight(const Matrix& matrix);
            static Vector3f GetTranslation(const Matrix& matrix);

            static Matrix& SetForward(Matrix& matrix, const Vector3f& vector);
            static Matrix& SetUp(Matrix& matrix, const Vector3f& vector);
            static Matrix& SetRight(Matrix& matrix, const Vector3f& vector);
            static Matrix& SetTranslation(Matrix& matrix, const Vector3f& vector);
        };
    } // namespace math
} // namespace library

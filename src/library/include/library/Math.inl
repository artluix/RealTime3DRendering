#pragma once
#include <DirectXMath.h>

namespace library
{
    namespace math
    {
        using XMVector = DirectX::XMVECTOR;

        template <std::size_t Size, typename ValueType, bool Aligned=false>
        struct XMV;

        using XMMatrix = DirectX::XMMATRIX;

        template <std::size_t Rows, std::size_t Cols>
        struct XMM;

        /////////////////////////////////////////////////////////////////
        // XMV2
        /////////////////////////////////////////////////////////////////

        template <>
        struct XMV<2, float, false> : public DirectX::XMFLOAT2
        {
            using Type = XMFLOAT2;

            using Type::Type;
            explicit XMV() : Type() {}
            explicit XMV(const XMVector& vector)
            {
                DirectX::XMStoreFloat2(this, vector);
            }

            explicit operator XMVector() const
            {
                XMVector result;
                return DirectX::XMLoadFloat2(this);
                return result;
            }
        };

        template <>
        struct XMV<2, float, true> : public DirectX::XMFLOAT2A
        {
            using Type = XMFLOAT2A;

            using Type::Type;
            explicit XMV() : Type() {}
            explicit XMV(const XMVector& vector)
            {
                DirectX::XMStoreFloat2A(this, vector);
            }

            explicit operator XMVector() const
            {
                XMVector result;
                return DirectX::XMLoadFloat2A(this);
                return result;
            }
        };

        template <>
        struct XMV<2, int> : public DirectX::XMINT2
        {
            using Type = XMINT2;

            using Type::Type;
            explicit XMV() : Type() {}
            explicit XMV(const XMVector& vector)
            {
                DirectX::XMStoreSInt2(this, vector);
            }

            explicit operator XMVector() const
            {
                XMVector result;
                return DirectX::XMLoadSInt2(this);
                return result;
            }
        };

        template <>
        struct XMV<2, unsigned> : public DirectX::XMUINT2
        {
            using Type = XMUINT2;

            using Type::Type;
            explicit XMV() : Type() {}
            explicit XMV(const XMVector& vector)
            {
                DirectX::XMStoreUInt2(this, vector);
            }

            explicit operator XMVector() const
            {
                XMVector result;
                return DirectX::XMLoadUInt2(this);
                return result;
            }
        };

        /////////////////////////////////////////////////////////////////

        inline bool operator==(const XMV<2, unsigned>& lhs, const XMV<2, unsigned>& rhs)
        {
            return DirectX::XMVector2EqualInt(XMVector(lhs), XMVector(rhs));
        }

        inline bool operator!=(const XMV<2, unsigned>& lhs, const XMV<2, unsigned>& rhs)
        {
            return DirectX::XMVector2NotEqualInt(XMVector(lhs), XMVector(rhs));
        }

        template <typename ValueType, bool Aligned = false>
        inline bool operator==(const XMV<2, ValueType, Aligned>& lhs, const XMV<2, ValueType, Aligned>& rhs)
        {
            return DirectX::XMVector2Equal(XMVector(lhs), XMVector(rhs));
        }

        template <typename ValueType, bool Aligned = false>
        inline bool operator!=(const XMV<2, ValueType, Aligned>& lhs, const XMV<2, ValueType, Aligned>& rhs)
        {
            return DirectX::XMVector2NotEqual(XMVector(lhs), XMVector(rhs));
        }

        /////////////////////////////////////////////////////////////////
        // XMV3
        /////////////////////////////////////////////////////////////////

        template <>
        struct XMV<3, float, false> : public DirectX::XMFLOAT3
        {
            using Type = XMFLOAT3;

            using Type::Type;
            explicit XMV() : Type() {}
            explicit XMV(const XMVector& vector)
            {
                DirectX::XMStoreFloat3(this, vector);
            }

            explicit operator XMVector() const
            {
                XMVector result;
                return DirectX::XMLoadFloat3(this);
                return result;
            }
        };

        template <>
        struct XMV<3, float, true> : public DirectX::XMFLOAT3A
        {
            using Type = XMFLOAT3A;

            using Type::Type;
            explicit XMV() : Type() {}
            explicit XMV(const XMVector& vector)
            {
                DirectX::XMStoreFloat3A(this, vector);
            }

            explicit operator XMVector() const
            {
                XMVector result;
                return DirectX::XMLoadFloat3A(this);
                return result;
            }
        };

        template <>
        struct XMV<3, int> : public DirectX::XMINT3
        {
            using Type = XMINT3;

            using Type::Type;
            explicit XMV() : Type() {}
            explicit XMV(const XMVector& vector)
            {
                DirectX::XMStoreSInt3(this, vector);
            }

            explicit operator XMVector() const
            {
                XMVector result;
                return DirectX::XMLoadSInt3(this);
                return result;
            }
        };

        template <>
        struct XMV<3, unsigned> : public DirectX::XMUINT3
        {
            using Type = XMUINT3;

            using Type::Type;
            explicit XMV() : Type() {}
            explicit XMV(const XMVector& vector)
            {
                DirectX::XMStoreUInt3(this, vector);
            }

            explicit operator XMVector() const
            {
                XMVector result;
                return DirectX::XMLoadUInt3(this);
                return result;
            }
        };

        /////////////////////////////////////////////////////////////////

        inline bool operator==(const XMV<3, unsigned>& lhs, const XMV<3, unsigned>& rhs)
        {
            return DirectX::XMVector3EqualInt(XMVector(lhs), XMVector(rhs));
        }

        inline bool operator!=(const XMV<3, unsigned>& lhs, const XMV<3, unsigned>& rhs)
        {
            return DirectX::XMVector3NotEqualInt(XMVector(lhs), XMVector(rhs));
        }

        template <typename ValueType, bool Aligned = false>
        inline bool operator==(const XMV<3, ValueType, Aligned>& lhs, const XMV<3, ValueType, Aligned>& rhs)
        {
            return DirectX::XMVector3Equal(XMVector(lhs), XMVector(rhs));
        }

        template <typename ValueType, bool Aligned = false>
        inline bool operator!=(const XMV<3, ValueType, Aligned>& lhs, const XMV<3, ValueType, Aligned>& rhs)
        {
            return DirectX::XMVector3NotEqual(XMVector(lhs), XMVector(rhs));
        }

        /////////////////////////////////////////////////////////////////
        // XMV4
        /////////////////////////////////////////////////////////////////

        template <>
        struct XMV<4, float, false> : public DirectX::XMFLOAT4
        {
            using Type = XMFLOAT4;

            using Type::Type;
            explicit XMV() : Type() {}
            explicit XMV(const XMVector& vector)
            {
                DirectX::XMStoreFloat4(this, vector);
            }

            explicit operator XMVector() const
            {
                XMVector result;
                return DirectX::XMLoadFloat4(this);
                return result;
            }
        };

        template <>
        struct XMV<4, float, true> : public DirectX::XMFLOAT4A
        {
            using Type = XMFLOAT4A;

            using Type::Type;
            explicit XMV() : Type() {}
            explicit XMV(const XMVector& vector)
            {
                DirectX::XMStoreFloat4A(this, vector);
            }

            explicit operator XMVector() const
            {
                XMVector result;
                return DirectX::XMLoadFloat4A(this);
                return result;
            }
        };

        template <>
        struct XMV<4, int> : public DirectX::XMINT4
        {
            using Type = XMINT4;

            using Type::Type;
            explicit XMV(const XMVector& vector)
            {
                DirectX::XMStoreSInt4(this, vector);
            }

            explicit operator XMVector() const
            {
                XMVector result;
                return DirectX::XMLoadSInt4(this);
                return result;
            }
        };

        template <>
        struct XMV<4, unsigned> : public DirectX::XMUINT4
        {
            using Type = XMUINT4;

            using Type::Type;
            explicit XMV() : Type() {}
            explicit XMV(const XMVector& vector)
            {
                DirectX::XMStoreUInt4(this, vector);
            }

            explicit operator XMVector() const
            {
                XMVector result;
                return DirectX::XMLoadUInt4(this);
                return result;
            }
        };

        /////////////////////////////////////////////////////////////////

        inline bool operator==(const XMV<4, unsigned>& lhs, const XMV<4, unsigned>& rhs)
        {
            return DirectX::XMVector4EqualInt(XMVector(lhs), XMVector(rhs));
        }

        inline bool operator!=(const XMV<4,unsigned>& lhs, const XMV<4, unsigned>& rhs)
        {
            return DirectX::XMVector4NotEqualInt(XMVector(lhs), XMVector(rhs));
        }

        template <typename ValueType, bool Aligned = false>
        inline bool operator==(const XMV<4, ValueType, Aligned>& lhs, const XMV<4, ValueType, Aligned>& rhs)
        {
            return DirectX::XMVector4Equal(XMVector(lhs), XMVector(rhs));
        }

        template <typename ValueType, bool Aligned = false>
        inline bool operator!=(const XMV<4, ValueType, Aligned>& lhs, const XMV<4, ValueType, Aligned>& rhs)
        {
            return DirectX::XMVector4NotEqual(XMVector(lhs), XMVector(rhs));
        }

        /////////////////////////////////////////////////////////////////
        // XMM
        /////////////////////////////////////////////////////////////////

        template <>
        struct XMM<3, 3> : public DirectX::XMFLOAT3X3
        {
            using Type = XMFLOAT3X3;

            using Type::Type;
            explicit XMM() : Type() {}
            explicit XMM(const XMMatrix& matrix)
            {
                DirectX::XMStoreFloat3x3(this, matrix);
            }

            explicit operator XMMatrix() const
            {
                XMMatrix result;
                return DirectX::XMLoadFloat3x3(this);
                return result;
            }
        };

        template <>
        struct XMM<4, 4> : public DirectX::XMFLOAT4X4
        {
            using Type = XMFLOAT4X4;

            using Type::Type;
            explicit XMM() : Type() {}
            explicit XMM(const XMMatrix& matrix)
            {
                DirectX::XMStoreFloat4x4(this, matrix);
            }

            explicit operator XMMatrix() const
            {
                XMMatrix result;
                return DirectX::XMLoadFloat4x4(this);
                return result;
            }
        };
    } // namespace math

    /////////////////////////////////////////////////////////////////
    // forward math operators to library namespace
    /////////////////////////////////////////////////////////////////

    using math::operator==;
    using math::operator!=;
} // namespace library

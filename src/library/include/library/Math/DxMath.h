#pragma once
#include "library/Math/DxMathFwd.h"

#include <dxgiformat.h>

namespace dx
{
inline namespace math
{
//-------------------------------------------------------------------------
// Type wrappers
//-------------------------------------------------------------------------

template<>
struct StaticVector<float, 2>
{
	using Type = XMFLOAT2;
	static constexpr auto DXGI_FORMAT = DXGI_FORMAT_R32G32_FLOAT;

	static VECTOR Load(const Type& type) { return XMLoadFloat2(&type); }
	static void Store(const VECTOR& VEC, Type& type) { XMStoreFloat2(&type, VEC); }

	static inline bool Equal(const VECTOR& lhs, const VECTOR& rhs) { return XMVector2Equal(lhs, rhs); }
	static inline bool NotEqual(const VECTOR& lhs, const VECTOR& rhs) { return XMVector2NotEqual(lhs, rhs); }
};

template<>
struct StaticVector<std::int32_t, 2>
{
	using Type = XMINT2;
	static constexpr auto DXGI_FORMAT = DXGI_FORMAT_R32G32_SINT;

	static VECTOR Load(const Type& type) { return XMLoadSInt2(&type); }
	static void Store(const VECTOR& VEC, Type& type) { XMStoreSInt2(&type, VEC); }

	static inline bool Equal(const VECTOR& lhs, const VECTOR& rhs) { return XMVector2EqualInt(lhs, rhs); }
	static inline bool NotEqual(const VECTOR& lhs, const VECTOR& rhs) { return XMVector2NotEqualInt(lhs, rhs); }
};

template<>
struct StaticVector<std::uint32_t, 2>
{
	using Type = XMUINT2;
	static constexpr auto DXGI_FORMAT = DXGI_FORMAT_R32G32_UINT;

	static VECTOR Load(const Type& type) { return XMLoadUInt2(&type); }
	static void Store(const VECTOR& VEC, Type& type) { XMStoreUInt2(&type, VEC); }

	static inline bool Equal(const VECTOR& lhs, const VECTOR& rhs) { return XMVector2EqualInt(lhs, rhs); }
	static inline bool NotEqual(const VECTOR& lhs, const VECTOR& rhs) { return XMVector2NotEqualInt(lhs, rhs); }
};

// -----------------------------------------------------------------------------

template<>
struct StaticVector<float, 3>
{
	using Type = XMFLOAT3;
	static constexpr auto DXGI_FORMAT = DXGI_FORMAT_R32G32B32_FLOAT;

	static VECTOR Load(const Type& type) { return XMLoadFloat3(&type); }
	static void Store(const VECTOR& VEC, Type& type) { XMStoreFloat3(&type, VEC); }

	static inline bool Equal(const VECTOR& lhs, const VECTOR& rhs) { return XMVector3Equal(lhs, rhs); }
	static inline bool NotEqual(const VECTOR& lhs, const VECTOR& rhs) { return XMVector3NotEqual(lhs, rhs); }
};

template<>
struct StaticVector<std::int32_t, 3>
{
	using Type = XMINT3;
	static constexpr auto DXGI_FORMAT = DXGI_FORMAT_R32G32B32_SINT;

	static VECTOR Load(const Type& type) { return XMLoadSInt3(&type); }
	static void Store(const VECTOR& VEC, Type& type) { XMStoreSInt3(&type, VEC); }

	static inline bool Equal(const VECTOR& lhs, const VECTOR& rhs) { return XMVector3EqualInt(lhs, rhs); }
	static inline bool NotEqual(const VECTOR& lhs, const VECTOR& rhs) { return XMVector3NotEqualInt(lhs, rhs); }
};

template<>
struct StaticVector<std::uint32_t, 3>
{
	using Type = XMUINT3;
	static constexpr auto DXGI_FORMAT = DXGI_FORMAT_R32G32B32_UINT;

	static VECTOR Load(const Type& type) { return XMLoadUInt3(&type); }
	static void Store(const VECTOR& VEC, Type& type) { XMStoreUInt3(&type, VEC); }

	static inline bool Equal(const VECTOR& lhs, const VECTOR& rhs) { return XMVector3EqualInt(lhs, rhs); }
	static inline bool NotEqual(const VECTOR& lhs, const VECTOR& rhs) { return XMVector3NotEqualInt(lhs, rhs); }
};

// -----------------------------------------------------------------------------

template<>
struct StaticVector<float, 4>
{
	using Type = XMFLOAT4;
	static constexpr auto DXGI_FORMAT = DXGI_FORMAT_R32G32B32A32_FLOAT;

	static VECTOR Load(const Type& type) { return XMLoadFloat4(&type); }
	static void Store(const VECTOR& VEC, Type& type) { XMStoreFloat4(&type, VEC); }

	static inline bool Equal(const VECTOR& lhs, const VECTOR& rhs) { return XMVector4Equal(lhs, rhs); }
	static inline bool NotEqual(const VECTOR& lhs, const VECTOR& rhs) { return XMVector4NotEqual(lhs, rhs); }
};

template<>
struct StaticVector<std::int32_t, 4>
{
	using Type = XMINT4;
	static constexpr auto DXGI_FORMAT = DXGI_FORMAT_R32G32B32A32_SINT;

	static VECTOR Load(const Type& type) { return XMLoadSInt4(&type); }
	static void Store(const VECTOR& VEC, Type& type) { XMStoreSInt4(&type, VEC); }

	static inline bool Equal(const VECTOR& lhs, const VECTOR& rhs) { return XMVector4EqualInt(lhs, rhs); }
	static inline bool NotEqual(const VECTOR& lhs, const VECTOR& rhs) { return XMVector4NotEqualInt(lhs, rhs); }
};

template<>
struct StaticVector<std::uint32_t, 4>
{
	using Type = XMUINT4;
	static constexpr auto DXGI_FORMAT = DXGI_FORMAT_R32G32B32A32_UINT;

	static VECTOR Load(const Type& type) { return XMLoadUInt4(&type); }
	static void Store(const VECTOR& VEC, Type& type) { XMStoreUInt4(&type, VEC); }

	static inline bool Equal(const VECTOR& lhs, const VECTOR& rhs) { return XMVector4EqualInt(lhs, rhs); }
	static inline bool NotEqual(const VECTOR& lhs, const VECTOR& rhs) { return XMVector4NotEqualInt(lhs, rhs); }
};

// -----------------------------------------------------------------------------

template<>
struct StaticMatrix<float, 3>
{
	using Type = XMFLOAT3X3;

	static MATRIX Load(const Type& type) { return XMLoadFloat3x3(&type); }
	static void Store(const MATRIX& MAT, Type& type) { XMStoreFloat3x3(&type, MAT); }
};

//-------------------------------------------------------------------------

template<>
struct StaticMatrix<float, 4>
{
	using Type = XMFLOAT4X4;

	static MATRIX Load(const Type& type) { return XMLoadFloat4x4(&type); }
	static void Store(const MATRIX& MAT, Type& type) { XMStoreFloat4x4(&type, MAT); }
};

//-------------------------------------------------------------------------
// Function wrappers
//-------------------------------------------------------------------------

template<unsigned Size>
inline float VectorDot(const VECTOR& lhs, const VECTOR& rhs);

template<unsigned Size>
inline float VectorLength(const VECTOR& VEC);

template<unsigned Size>
inline VECTOR VectorCross(const VECTOR& lhs, const VECTOR& rhs);

template<unsigned Size>
inline VECTOR VectorNormalize(const VECTOR& VEC);

template<unsigned Size>
inline VECTOR VectorTransform(const VECTOR& VEC, const MATRIX& MAT);

//-------------------------------------------------------------------------

template<unsigned Size>
inline bool VectorLess(const VECTOR& lhs, const VECTOR& rhs);

template<unsigned Size>
inline bool VectorLessOrEqual(const VECTOR& lhs, const VECTOR& rhs);

template<unsigned Size>
inline bool VectorGreater(const VECTOR& lhs, const VECTOR& rhs);

template<unsigned Size>
inline bool VectorGreaterOrEqual(const VECTOR& lhs, const VECTOR& rhs);

//-------------------------------------------------------------------------
// Vector2
//-------------------------------------------------------------------------

template<>
inline float VectorDot<2>(const VECTOR& lhs, const VECTOR& rhs) { return XMVectorGetX(XMVector2Dot(lhs, rhs)); }

template<>
inline float VectorLength<2>(const VECTOR& VEC) { return XMVectorGetX(XMVector2Length(VEC)); }

template<>
inline VECTOR VectorCross<2>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector2Cross(lhs, rhs); }

template<>
inline VECTOR VectorNormalize<2>(const VECTOR& VEC) { return XMVector2Normalize(VEC); }

template<>
inline VECTOR VectorTransform<2>(const VECTOR& VEC, const MATRIX& MAT) { return XMVector2Transform(VEC, MAT); }

//-------------------------------------------------------------------------

template<>
inline bool VectorLess<2>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector2Less(lhs, rhs); }

template<>
inline bool VectorLessOrEqual<2>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector2LessOrEqual(lhs, rhs); }

template<>
inline bool VectorGreater<2>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector2Greater(lhs, rhs); }

template<>
inline bool VectorGreaterOrEqual<2>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector2GreaterOrEqual(lhs, rhs); }

//-------------------------------------------------------------------------
// Vector3
//-------------------------------------------------------------------------

template<>
inline float VectorDot<3>(const VECTOR& lhs, const VECTOR& rhs) { return XMVectorGetX(XMVector3Dot(lhs, rhs)); }

template<>
inline float VectorLength<3>(const VECTOR& VEC) { return XMVectorGetX(XMVector3Length(VEC)); }

template<>
inline VECTOR VectorCross<3>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector3Cross(lhs, rhs); }

template<>
inline VECTOR VectorNormalize<3>(const VECTOR& VEC) { return XMVector3Normalize(VEC); }

template<>
inline VECTOR VectorTransform<3>(const VECTOR& VEC, const MATRIX& MAT) { return XMVector3Transform(VEC, MAT); }

//-------------------------------------------------------------------------

template<>
inline bool VectorLess<3>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector3Less(lhs, rhs); }

template<>
inline bool VectorLessOrEqual<3>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector3LessOrEqual(lhs, rhs); }

template<>
inline bool VectorGreater<3>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector3Greater(lhs, rhs); }

template<>
inline bool VectorGreaterOrEqual<3>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector3GreaterOrEqual(lhs, rhs); }

//-------------------------------------------------------------------------
// Vector4
//-------------------------------------------------------------------------

template<>
inline float VectorDot<4>(const VECTOR& lhs, const VECTOR& rhs) { return XMVectorGetX(XMVector4Dot(lhs, rhs)); }

template<>
inline float VectorLength<4>(const VECTOR& VEC) { return XMVectorGetX(XMVector4Length(VEC)); }

template<>
inline VECTOR VectorNormalize<4>(const VECTOR& VEC) { return XMVector4Normalize(VEC); }

template<>
inline VECTOR VectorTransform<4>(const VECTOR& VEC, const MATRIX& MAT) { return XMVector4Transform(VEC, MAT); }

//-------------------------------------------------------------------------

template<>
inline bool VectorLess<4>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector4Less(lhs, rhs); }

template<>
inline bool VectorLessOrEqual<4>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector4LessOrEqual(lhs, rhs); }

template<>
inline bool VectorGreater<4>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector4Greater(lhs, rhs); }

template<>
inline bool VectorGreaterOrEqual<4>(const VECTOR& lhs, const VECTOR& rhs) { return XMVector4GreaterOrEqual(lhs, rhs); }

// -----------------------------------------------------------------------------
// Utils
// -----------------------------------------------------------------------------

//-------------------------------------------------------------------------
// DXGI_FORMAT for basic types
//-------------------------------------------------------------------------

template<typename T>
constexpr inline auto DXGI_FORMAT = DXGI_FORMAT_UNKNOWN;

template<>
constexpr inline auto DXGI_FORMAT<float> = DXGI_FORMAT_R32_FLOAT;

template<>
constexpr inline auto DXGI_FORMAT<std::int32_t> = DXGI_FORMAT_R32_SINT;

template<>
constexpr inline auto DXGI_FORMAT<std::uint32_t> = DXGI_FORMAT_R32_UINT;


// template<typename T = float>
// inline T GetX(const VECTOR& VEC) { return static_cast<T>(XMVectorGetX(VEC)); }
// template<typename T = float>
// inline T GetY(const VECTOR& VEC) { return static_cast<T>(XMVectorGetY(VEC)); }
// template<typename T = float>
// inline T GetZ(const VECTOR& VEC) { return static_cast<T>(XMVectorGetZ(VEC)); }
// template<typename T = float>
// inline T GetW(const VECTOR& VEC) { return static_cast<T>(XMVectorGetW(VEC)); }

} // inline namespace math
} // namespace dx

#pragma once
#include <DirectXMath.h>

#include <type_traits>
#include <cstdint>
#include <array>
#include <vector>

// -----------------------------------------------------------------------------
// DirectXMath
// -----------------------------------------------------------------------------

namespace dx
{
using namespace DirectX;

inline namespace math
{
// -----------------------------------------------------------------------------
// StaticVector<T, Size>
// -----------------------------------------------------------------------------

using VECTOR = XMVECTOR;

template<typename T, unsigned Size>
struct StaticVector;

template<typename T>
using StaticVector2 = StaticVector<T, 2>;

template<typename T>
using StaticVector3 = StaticVector<T, 3>;

template<typename T>
using StaticVector4 = StaticVector<T, 4>;

// -----------------------------------------------------------------------------
// Vector<T, Size>
// -----------------------------------------------------------------------------

template<typename T, unsigned Size>
using Vector = typename StaticVector<T, Size>::Type;

template<typename T>
using Vector2 = Vector<T, 2>;

template<typename T>
using Vector3 = Vector<T, 3>;

template<typename T>
using Vector4 = Vector<T, 4>;

// -----------------------------------------------------------------------------
// StaticMatrix<Size>
// -----------------------------------------------------------------------------

using MATRIX = XMMATRIX;

template<typename T, unsigned Size>
struct StaticMatrix;

template<typename T>
using StaticMatrix3 = StaticMatrix<T, 3>;

template<typename T>
using StaticMatrix4 = StaticMatrix<T, 4>;

// -----------------------------------------------------------------------------
// Matrix<T>
// -----------------------------------------------------------------------------

template<typename T, unsigned Size>
using Matrix = typename StaticMatrix<T, Size>::Type;

template<typename T>
using Matrix3 = Matrix<T, 3>;

template<typename T>
using Matrix4 = Matrix<T, 4>;

} // inline namespace math
} // namespace dx

// -----------------------------------------------------------------------------
// math
// -----------------------------------------------------------------------------

namespace library
{
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

namespace math
{
// -----------------------------------------------------------------------------
// VectorType<T>
// -----------------------------------------------------------------------------

template<typename T, unsigned Size>
struct VectorType;

template<typename T>
using VectorType2 = VectorType<T, 2>;

template<typename T>
using VectorType3 = VectorType<T, 3>;

template<typename T>
using VectorType4 = VectorType<T, 4>;

// -----------------------------------------------------------------------------
// Vector<T>
// -----------------------------------------------------------------------------

using Vector2 = VectorType<float, 2>;
using Vector2i = VectorType<s32, 2>;
using Vector2u = VectorType<u32, 2>;

using Vector3 = VectorType<float, 3>;
using Vector3i = VectorType<s32, 3>;
using Vector3u = VectorType<u32, 3>;

using Vector4 = VectorType<float, 4>;
using Vector4i = VectorType<s32, 4>;
using Vector4u = VectorType<u32, 4>;

//-------------------------------------------------------------------------

template<typename T, unsigned Size, std::size_t Count>
using VectorFixedArray = std::array<VectorType<T, Size>, Count>;

template<typename T, unsigned Size>
using VectorArray = std::vector<VectorType<T, Size>>;

// -----------------------------------------------------------------------------
// Matrix<T>
// -----------------------------------------------------------------------------

template<unsigned Size>
struct Matrix;

using Matrix3 = Matrix<3>;
using Matrix4 = Matrix<4>;

//-------------------------------------------------------------------------

template<unsigned Size, std::size_t Count>
using MatrixFixedArray = std::array<Matrix<Size>, Count>;

template<unsigned Size>
using MatrixArray = std::vector<Matrix<Size>>;

// -----------------------------------------------------------------------------
// utils
// -----------------------------------------------------------------------------

template<typename U, typename T>
using enable_if_float_t = std::enable_if_t<std::is_same_v<U, float> && std::is_same_v<U, T>>;

} // namespace math
} // namespace library

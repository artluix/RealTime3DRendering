#pragma once
#include <DirectXMath.h>

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

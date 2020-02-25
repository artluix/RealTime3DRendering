#include "StdAfx.h"
#include "library/Math/Vector.h"

#include "library/Math/Matrix.h"

#include <sstream>
#include <iomanip>

namespace library::math
{
// -----------------------------------------------------------------------------
// VectorType2<T>
// -----------------------------------------------------------------------------

template<typename T>
const VectorType<T, 2> VectorType<T, 2>::Zero{ 0 };
template<typename T>
const VectorType<T, 2> VectorType<T, 2>::One{ 1 };

// -----------------------------------------------------------------------------

template<typename T>
VectorType<T, 2>::VectorType(const dx::VECTOR& dxVEC)
{
	dx::StaticVector2<T>::Store(dxVEC, _dxVec);
}

template<typename T>
VectorType<T, 2>& VectorType<T, 2>::operator=(const dx::VECTOR& dxVEC)
{
	dx::StaticVector2<T>::Store(dxVEC, _dxVec);
	return *this;
}

template<typename T>
VectorType<T, 2>::operator dx::VECTOR() const
{
	return dx::StaticVector2<T>::Load(_dxVec);
}

//-------------------------------------------------------------------------

template<typename T>
VectorType<T, 2>::operator bool() const
{
	static constexpr VectorType zero;
	return *this != zero;
}

template<typename T>
VectorType2<float> VectorType<T, 2>::Normalize() const
{
	return VectorType2<float>(dx::VectorNormalize<2>(dx::VECTOR(*this)));
}

template<typename T>
VectorType2<float> VectorType<T, 2>::Transform(const Matrix4& mat) const
{
	return VectorType2<float>(dx::VectorTransform<2>(dx::VECTOR(*this), dx::MATRIX(mat)));
}

//-------------------------------------------------------------------------

template<typename T>
std::string VectorType<T, 2>::ToString() const
{
	std::ostringstream oss;
	oss << std::setprecision(4) << '(' << x << ", " << y << ')';
	return oss.str();
}

//-------------------------------------------------------------------------
// VectorType3<T>
// -----------------------------------------------------------------------------

template<typename T>
const VectorType<T, 3> VectorType<T, 3>::Zero{ 0 };
template<typename T>
const VectorType<T, 3> VectorType<T, 3>::One{ 1 };

// -----------------------------------------------------------------------------

template<typename T>
constexpr VectorType<T, 3>::VectorType(const VectorType<T, 2>& xy, const T z /* = T(0) */)
	: VectorType(xy.x, xy.y, z)
{}

template<typename T>
VectorType<T, 3>::VectorType(const dx::VECTOR& dxVEC)
{
	dx::StaticVector3<T>::Store(dxVEC, _dxVec);
}

template<typename T>
VectorType<T, 3>& VectorType<T, 3>::operator=(const dx::VECTOR& dxVEC)
{
	dx::StaticVector3<T>::Store(dxVEC, _dxVec);
	return *this;
}

template<typename T>
VectorType<T, 3>::operator dx::VECTOR() const
{
	return dx::StaticVector3<T>::Load(_dxVec);
}

//-------------------------------------------------------------------------

template<typename T>
VectorType<T, 3>::operator bool() const
{
	static constexpr VectorType zero;
	return *this != zero;
}

template<typename T>
VectorType3<float> VectorType<T, 3>::Normalize() const
{
	return VectorType3<float>(dx::VectorNormalize<3>(dx::VECTOR(*this)));
}

template<typename T>
VectorType3<float> VectorType<T, 3>::Transform(const Matrix4& mat) const
{
	return VectorType3<float>(dx::VectorTransform<3>(dx::VECTOR(*this), dx::MATRIX(mat)));
}

//-------------------------------------------------------------------------

template<typename T>
std::string VectorType<T, 3>::ToString() const
{
	std::ostringstream oss;
	oss << std::setprecision(4) << '(' << x << ", " << y << ", " << z << ')';
	return oss.str();
}

// -----------------------------------------------------------------------------
// VectorType4<T>
// -----------------------------------------------------------------------------

template<typename T>
const VectorType<T, 4> VectorType<T, 4>::Zero{ 0 };
template<typename T>
const VectorType<T, 4> VectorType<T, 4>::One{ 1 };

// -----------------------------------------------------------------------------

template<typename T>
constexpr VectorType<T, 4>::VectorType(const VectorType<T, 3>& xyz, const T w /* = T(0) */)
	: VectorType(xyz.x, xyz.y, xyz.z, w)
{}

template<typename T>
VectorType<T, 4>::VectorType(const dx::VECTOR& dxVEC)
{
	dx::StaticVector4<T>::Store(dxVEC, _dxVec);
}

template<typename T>
VectorType<T, 4>& VectorType<T, 4>::operator=(const dx::VECTOR& dxVEC)
{
	dx::StaticVector4<T>::Store(dxVEC, _dxVec);
	return *this;
}

template<typename T>
VectorType<T, 4>::operator dx::VECTOR() const
{
	return dx::StaticVector4<T>::Load(_dxVec);
}

//-------------------------------------------------------------------------

template<typename T>
VectorType<T, 4>::operator bool() const
{
	static constexpr VectorType zero;
	return *this != zero;
}

template<typename T>
VectorType4<float> VectorType<T, 4>::Normalize() const
{
	return VectorType4<float>(dx::VectorNormalize<4>(dx::VECTOR(*this)));
}

template<typename T>
VectorType4<float> VectorType<T, 4>::Transform(const Matrix4& mat) const
{
	return VectorType4<float>(dx::VectorTransform<4>(dx::VECTOR(*this), dx::MATRIX(mat)));
}

//-------------------------------------------------------------------------

template<typename T>
std::string VectorType<T, 4>::ToString() const
{
	std::ostringstream oss;
	oss << std::setprecision(4) << '(' << x << ", " << y << ", " << z << ", " << w << ')';
	return oss.str();
}

//--------------------------------------------------------------------------
// template instantiation
//--------------------------------------------------------------------------

template struct VectorType<float, 2>;
template struct VectorType<s32, 2>;
template struct VectorType<u32, 2>;

template struct VectorType<float, 3>;
template struct VectorType<s32, 3>;
template struct VectorType<u32, 3>;

template struct VectorType<float, 4>;
template struct VectorType<s32, 4>;
template struct VectorType<u32, 4>;

} // namespace library::math

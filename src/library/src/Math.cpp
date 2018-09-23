#include "library/Math.h"

#include <sstream>

namespace library
{
    namespace math
    {
        namespace helpers
        {
            const Vector2::Type Vector2::Zero = Type(0.f, 0.f);
            const Vector2::Type Vector2::One = Type(1.f, 1.f);

            std::string Vector2::ToString(const Type& vector)
            {
                std::stringstream ss;
                ss << "(" << vector.x << ", " << vector.y << ")";
                return ss.str();
            }

            /////////////////////////////////////////////////////////////////

            const Vector3::Type Vector3::Zero = Type(0.f, 0.f, 0.f);
            const Vector3::Type Vector3::One = Type(1.f, 1.f, 1.f);

            const Vector3::Type Vector3::Forward = Type(0.f, 0.f, -1.f);
            const Vector3::Type Vector3::Backward = Type(0.f, 0.f, 1.f);
            const Vector3::Type Vector3::Up = Type(0.f, 1.f, 0.f);
            const Vector3::Type Vector3::Down = Type(0.f, -1.f, 0.f);
            const Vector3::Type Vector3::Left  = Type(1.f, 0.f, 0.f);
            const Vector3::Type Vector3::Right = Type(-1.f, 0.f, 0.f);

            std::string Vector3::ToString(const Type& vector)
            {
                std::stringstream ss;
                ss << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
                return ss.str();
            }

            /////////////////////////////////////////////////////////////////

            const Vector4::Type Vector4::Zero = Type(0.f, 0.f, 0.f, 0.f);
            const Vector4::Type Vector4::One = Type(1.f, 1.f, 1.f, 1.f);

            std::string Vector4::ToString(const Type& vector)
            {
                std::stringstream ss;
                ss << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
                return ss.str();
            }

            /////////////////////////////////////////////////////////////////

            const Matrix3::Type Matrix3::Idendity = Type(
                1.f, 0.f, 0.f,
                0.f, 1.f, 0.f,
                0.f, 0.f, 1.f
            );

            /////////////////////////////////////////////////////////////////

            const Matrix4::Type Matrix4::Identity = Type(
                1.f, 0.f, 0.f, 0.f,
                0.f, 1.f, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f,
                0.f, 0.f, 0.f, 1.f
            );

            /////////////////////////////////////////////////////////////////

            Vector3::Type Matrix4::GetForward(Matrix::ConstType matrix)
            {
                Vector4::Type r;
                DirectX::XMStoreFloat4(&r, matrix.r[2]);

                return Vector3::Type(-r.x, -r.y, -r.z);
            }

            Vector3::Type Matrix4::GetUp(Matrix::ConstType matrix)
            {
                Vector4::Type r;
                DirectX::XMStoreFloat4(&r, matrix.r[1]);

                return Vector3::Type(r.x, r.y, r.z);
            }

            Vector3::Type Matrix4::GetRight(Matrix::ConstType matrix)
            {
                Vector4::Type r;
                DirectX::XMStoreFloat4(&r, matrix.r[0]);

                return Vector3::Type(r.x, r.y, r.z);
            }

            Vector3::Type Matrix4::GetTranslation(Matrix::ConstType matrix)
            {
                Vector4::Type r;
                DirectX::XMStoreFloat4(&r, matrix.r[3]);

                return Vector3::Type(r.x, r.y, r.z);
            }

            /////////////////////////////////////////////////////////////////

            Matrix::Type& Matrix4::SetForward(Matrix::Type& matrix, const Vector3::Type& vector)
            {
                auto v = DirectX::XMLoadFloat3(&vector);
                auto forwardVector = DirectX::XMVectorNegate(v);
                matrix.r[2] = forwardVector;

                return matrix;
            }

            Matrix::Type& Matrix4::SetUp(Matrix::Type& matrix, const Vector3::Type& vector)
            {
                matrix.r[1] = DirectX::XMLoadFloat3(&vector);

                return matrix;
            }

            Matrix::Type& Matrix4::SetRight(Matrix::Type& matrix, const Vector3::Type& vector)
            {
                matrix.r[0] = DirectX::XMLoadFloat3(&vector);

                return matrix;
            }

            Matrix::Type& Matrix4::SetTranslation(Matrix::Type& matrix, const Vector3::Type& vector)
            {
                matrix.r[3] = DirectX::XMLoadFloat3(&vector);

                return matrix;
            }
        }
    }
}


#include "library/Math.h"

namespace library
{
    namespace math
    {
        const Matrix3 Matrix3::Zero = Matrix3(
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f
        );

        const Matrix3 Matrix3::Idendity = Matrix3(
            1.f, 0.f, 0.f,
            0.f, 1.f, 0.f,
            0.f, 0.f, 1.f
        );

        /////////////////////////////////////////////////////////////////

        const Matrix4 Matrix4::Zero = Matrix4(
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f
        );

        const Matrix4 Matrix4::Identity = Matrix4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
        );

        Vector3f Matrix4::GetForward(const Matrix& matrix)
        {
            const auto r = Vector4f(matrix.r[2]);
            return Vector3f(-r.x, -r.y, -r.z);
        }

        Vector3f Matrix4::GetUp(const Matrix& matrix)
        {
            const auto r = Vector4f(matrix.r[1]);
            return Vector3f(r.x, r.y, r.z);
        }

        Vector3f Matrix4::GetRight(const Matrix& matrix)
        {
            const auto r = Vector4f(matrix.r[0]);
            return Vector3f(r.x, r.y, r.z);
        }

        Vector3f Matrix4::GetTranslation(const Matrix& matrix)
        {
            const auto r = Vector4f(matrix.r[3]);
            return Vector3f(r.x, r.y, r.z);
        }

        /////////////////////////////////////////////////////////////////

        Matrix& Matrix4::SetForward(Matrix& matrix, const Vector3f& vector)
        {
            auto v = Vector(vector);
            matrix.r[2] = DirectX::XMVectorNegate(v);
            return matrix;
        }

        Matrix& Matrix4::SetUp(Matrix& matrix, const Vector3f& vector)
        {
            matrix.r[1] = Vector(vector);
            return matrix;
        }

        Matrix& Matrix4::SetRight(Matrix& matrix, const Vector3f& vector)
        {
            matrix.r[0] = Vector(vector);
            return matrix;
        }

        Matrix& Matrix4::SetTranslation(Matrix& matrix, const Vector3f& vector)
        {
            matrix.r[3] = Vector(vector);
            return matrix;
        }
    } // namespace math
} // namespace library

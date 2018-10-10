#pragma once
#include "library/components/ViewedComponent.h"
#include "library/Color.h"
#include "library/Math.h"

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectPass;
struct ID3DX11EffectMatrixVariable;

namespace library
{
    namespace components
    {
        class GridComponent : public rtti::Class<GridComponent, ViewedComponent>
        {
        public:
            explicit GridComponent(const Application& app, const CameraComponent& camera);
            explicit GridComponent(
                const Application& app,
                const CameraComponent& camera,
                const unsigned size,
                const unsigned scale,
                const Color& color
            );

            ~GridComponent() = default;

            unsigned GetSize() const { return m_size; }
            unsigned GetScale() const { return m_scale; }
            const Color& GetColor() const { return m_color; }
            const math::Vector3f& GetPosition() const { return m_position; }

            void SetSize(const unsigned size);
            void SetScale(const unsigned scale);
            void SetColor(const Color& color);

            void SetPosition(const math::Vector3f& position);
            void SetPosition(const float x, const float y, const float z);

            void Initialize() override;
            void Draw(const Time& time) override;

        private:
            void Build();

            unsigned m_size;
            unsigned m_scale;
            Color m_color;

            math::Vector3f m_position;
            math::Matrix4 m_worldMatrix;

            ComPtr<ID3DX11Effect> m_effect;
            ComPtr<ID3DX11EffectTechnique> m_technique;
            ComPtr<ID3DX11EffectPass> m_pass;
            ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;

            ComPtr<ID3D11InputLayout> m_inputLayout;
            ComPtr<ID3D11Buffer> m_vertexBuffer;
        };
    } // namespace components
} // namespace library

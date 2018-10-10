#include "library/components/GridComponent.h"

#include "library/Utils.h"
#include "library/Path.h"
#include "library/Exception.h"
#include "library/VertexDeclarations.h"

#include <d3dx11effect.h>
#include <vector>

namespace library
{
    namespace
    {
        namespace defaults
        {
            constexpr unsigned k_size = 16;
            constexpr unsigned k_scale = 16;
            const auto k_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        }

        const auto k_effectPath = utils::GetExecutableDirectory().Join(
            filesystem::Path(L"data/effects/BasicEffect.fx")
        );
    }

    namespace components
    {
        GridComponent::GridComponent(const Application& app, const CameraComponent& camera)
            : Class(app, camera)
            , m_size(defaults::k_size)
            , m_scale(defaults::k_scale)
            , m_color(defaults::k_color)
            , m_position(math::Vector3f::Zero)
            , m_worldMatrix(math::Matrix4::Identity)
            , m_effect()
            , m_technique()
            , m_pass()
            , m_wvpVariable()
            , m_inputLayout()
            , m_vertexBuffer()
        {
        }

        GridComponent::GridComponent(const Application& app, const CameraComponent& camera, const unsigned size, const unsigned scale, const Color& color)
            : Class(app, camera)
            , m_size(size)
            , m_scale(scale)
            , m_color(color)
            , m_position(math::Vector3f::Zero)
            , m_worldMatrix(math::Matrix4::Identity)
            , m_effect()
            , m_technique()
            , m_pass()
            , m_wvpVariable()
            , m_inputLayout()
            , m_vertexBuffer()
        {
        }

        /////////////////////////////////////////////////////////////////

        void GridComponent::SetSize(const unsigned size)
        {
            if (m_size != size)
            {
                m_size = size;
                Build();
            }
        }

        void GridComponent::SetScale(const unsigned scale)
        {
            if (m_scale != scale)
            {
                m_scale = scale;
                Build();
            }
        }

        void GridComponent::SetColor(const Color& color)
        {
            if (m_color != color)
            {
                m_color = color;
                Build();
            }
        }

        /////////////////////////////////////////////////////////////////

        void GridComponent::SetPosition(const math::Vector3f& position)
        {
            if (m_position != position)
            {
                m_position = position;

                const auto translation = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
                m_worldMatrix = math::Matrix4(translation);
            }
        }

        void GridComponent::SetPosition(const float x, const float y, const float z)
        {
            const auto position = math::Vector3f(x, y, z);
            SetPosition(position);
        }

        /////////////////////////////////////////////////////////////////

        void GridComponent::Initialize()
        {
            const Application& app = m_app;

            ComPtr<ID3DBlob> errorBlob;

            // compile shader (it should be simple load)
            auto hr = D3DX11CompileEffectFromFile(
                k_effectPath.GetAsWideString().c_str(),
                nullptr,
                nullptr,
                0,
                0,
                app.GetD3DDevice(),
                m_effect.GetAddressOf(),
                errorBlob.GetAddressOf()
            );

            if (FAILED(hr))
            {
                std::string error = std::string("D3DX11CompileEffectFromFile() failed: ")
                    + std::string(static_cast<const char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize());
                throw Exception(error.c_str(), hr);
            }

            // Look up the technique, pass, and WVP variable from the effect
            m_technique = m_effect->GetTechniqueByName("main11");
            if (!m_technique)
            {
                throw Exception("ID3DX11Effect::GetTechniqueByName() could not find the specified technique.", hr);
            }

            m_pass = m_technique->GetPassByName("p0");
            if (!m_pass)
            {
                throw Exception("ID3DX11Effect::GetPassByName() could not find the specified pass.", hr);
            }

            auto variable = m_effect->GetVariableByName("WorldViewProjection");
            if (!variable)
            {
                throw Exception("ID3DX11Effect::GetVariableByName() could not find the specified variable.", hr);
            }

            m_wvpVariable = variable->AsMatrix();
            if (!m_wvpVariable->IsValid())
            {
                throw Exception("Invalid effect variable cast.", hr);
            }

            // Create the input layout
            D3DX11_PASS_DESC passDesc;
            m_pass->GetDesc(&passDesc);

            std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptions = 
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            hr = app.GetD3DDevice()->CreateInputLayout(
                inputElementDescriptions.data(), inputElementDescriptions.size(),
                passDesc.pIAInputSignature, passDesc.IAInputSignatureSize,
                m_inputLayout.GetAddressOf()
            );
            if (FAILED(hr))
            {
                throw Exception("ID3D11Device::CreateInputLayout() failed.", hr);
            }

            Build();
        }

        void GridComponent::Draw(const Time& time)
        {
            auto d3dDeviceContext = m_app.GetD3DDeviceContext();
            d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
            d3dDeviceContext->IASetInputLayout(m_inputLayout.Get());

            unsigned stride = sizeof(vertex::PositionColor);
            unsigned offset = 0;
            d3dDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

            const auto worldMatrix = math::Matrix(m_worldMatrix);
            const auto wvp = worldMatrix * GetCamera().GetViewProjectionMatrix();
            m_wvpVariable->SetMatrix(reinterpret_cast<const float*>(&wvp));

            m_pass->Apply(0, d3dDeviceContext);

            d3dDeviceContext->Draw((m_size + 1) * 4, 0);
        }

        /////////////////////////////////////////////////////////////////

        void GridComponent::Build()
        {
            m_vertexBuffer.Reset();

            auto d3dDevice = m_app.GetD3DDevice();

            const float adjustedScale = m_scale * 0.1f;
            const float maxPosition = m_size * adjustedScale / 2;

            const unsigned length = 4 * (m_size + 1);
            const unsigned size = sizeof(vertex::PositionColor) * length;

            std::vector<vertex::PositionColor> vertices;
            vertices.reserve(length);

            for (unsigned i = 0; i < m_size + 1; i++)
            {
                const float position = maxPosition - (i * adjustedScale);

                // vertical line
                vertices.emplace_back(math::Vector4f(position, 0.0f, maxPosition, 1.0f), m_color);
                vertices.emplace_back(math::Vector4f(position, 0.0f, -maxPosition, 1.0f), m_color);

                // horizontal line
                vertices.emplace_back(math::Vector4f(maxPosition, 0.0f, position, 1.0f), m_color);
                vertices.emplace_back(math::Vector4f(-maxPosition, 0.0f, position, 1.0f), m_color);
            }

            D3D11_BUFFER_DESC vertexBufferDesc{};
            vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
            vertexBufferDesc.ByteWidth = size;
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

            D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
            vertexSubResourceData.pSysMem = vertices.data();

            auto hr = d3dDevice->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, m_vertexBuffer.GetAddressOf());
            if (FAILED(hr))
            {
                throw Exception("ID3D11Device::CreateBuffer() failed.", hr);
            }
        }
    } // namespace components
} // namespace library

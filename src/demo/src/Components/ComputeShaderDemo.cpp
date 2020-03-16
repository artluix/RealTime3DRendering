#include "ComputeShaderDemo.h"

#include <library/Components/TextComponent.h>
#include <library/Components/FullScreenQuadComponent.h>

#include <library/Application.h>
#include <library/Render/Renderer.h>

#include <library/Effect/EffectVariable.h>

#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
constexpr unsigned k_threadsPerGroup = 32;
} // namespace

ComputeShaderDemo::ComputeShaderDemo()
{
}

void ComputeShaderDemo::InitializeInternal()
{
	CreateMaterialWithEffect("ComputeShader");

	m_textures.resize(Texture::Count);

	auto& colorTexture = m_textures[Texture::Default];

	// initialize SRV and UAV
	{
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = GetApp().GetScreenWidth();
		textureDesc.Height = GetApp().GetScreenHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

		ComPtr<ID3D11Texture2D> texture;
		auto hr = GetApp().GetDevice()->CreateTexture2D(&textureDesc, nullptr, &texture);
		assert("ID3D11Device::CreateTexture2D() failed." && SUCCEEDED(hr));


		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
		uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		hr = GetApp().GetDevice()->CreateUnorderedAccessView(
			texture.Get(),
			&uavDesc,
			&m_outputTexture
		);
		assert("ID3D11Device::CreateUnorderedAccessView() failed." && SUCCEEDED(hr));

		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc{};
		resourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resourceViewDesc.Texture2D.MipLevels = 1;

		hr = GetApp().GetDevice()->CreateShaderResourceView(
			texture.Get(),
			&resourceViewDesc,
			&colorTexture
		);
		assert("ID3D11Device::CreateShaderResourceView() failed." && SUCCEEDED(hr));

		m_textureSize.x = static_cast<float>(textureDesc.Width);
		m_textureSize.y = static_cast<float>(textureDesc.Height);

		m_threadGroupCount.x = textureDesc.Width / k_threadsPerGroup;
		m_threadGroupCount.y = textureDesc.Height / k_threadsPerGroup;
	}

	m_fullScreenQuad = std::make_unique<FullScreenQuadComponent>();
	m_fullScreenQuad->SetMaterial(*m_material);
	m_fullScreenQuad->SetMaterialUpdateFunction(
		[this]()
		{
			m_material->GetColorTexture() << m_textures[Texture::Default].Get();
		}
	);
	m_fullScreenQuad->SetActiveTechnique("render");
	m_fullScreenQuad->Initialize(GetApp());

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss << std::setprecision(2) << "Color offset: " << m_blueColor;

			return woss.str();
		}
	);
	m_text->Initialize(GetApp());
}

void ComputeShaderDemo::Update(const Time& time)
{
	m_blueColor = .5f * (std::sin(time.total.GetSeconds()) + 1); // clamp to normalized color range

	m_text->Update(time);

	PrimitiveComponent::Update(time);
}

void ComputeShaderDemo::Draw(const Time& time)
{
	// update compute part of material
	{
		m_material->GetTextureSize() << m_textureSize;
		m_material->GetBlueColor() << m_blueColor;
		m_material->GetOutputTexture() << m_outputTexture.Get();

		auto& pass = m_material->GetCurrentPass();
		pass.Apply(0, GetApp().GetDeviceContext());
	}

	auto deviceContext = GetApp().GetDeviceContext();

	// Dispatch the computer shader
	deviceContext->Dispatch(m_threadGroupCount.x, m_threadGroupCount.y, 1);

	// Unbind the UAV from the compute shader
	// so we can bind the same underlying resource as an SRV
	static ID3D11UnorderedAccessView* emptyUAV = nullptr;
	deviceContext->CSSetUnorderedAccessViews(0, 1, &emptyUAV, nullptr);

	// Draw the texture written by the compute shader
	m_fullScreenQuad->Draw(time);
	GetApp().GetRenderer()->UnbindPSResources(0, 1);
}


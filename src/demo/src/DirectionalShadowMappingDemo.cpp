#include "DirectionalShadowMappingDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Components/UIComponent.h>
#include <library/Components/TextComponent.h>
#include <library/Components/OrthographicProjectorComponent.h>
#include <library/Components/ProxyModelComponent.h>
#include <library/Components/RenderableFrustumComponent.h>
#include <library/Components/DirectionalLightComponent.h>

#include <library/RenderTargets/DepthMapRenderTarget.h>

#include <library/Effect/Effect.h>
#include <library/Effect/EffectTechnique.h>
#include <library/Effect/EffectPass.h>
#include <library/Effect/EffectVariable.h>

#include <library/Application.h>
#include <library/Renderer.h>
#include <library/VertexTypes.h>

#include <library/Model/Model.h>
#include <library/Model/Mesh.h>

#include <SpriteBatch.h>

#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
	constexpr float k_byteMax = static_cast<float>(0xFF);

	constexpr unsigned k_depthMapWidth = 1024;
	constexpr unsigned k_depthMapHeight = 1024U;
	const RECT k_depthMapDestinationRect = { 0, 512, 256, 768 };
	constexpr float k_depthBiasModulationRate = 10000.f;

	constexpr float k_lightModulationRate = 10.f;
	constexpr auto k_lightRotationRate = math::Vector2(math::Pi_Div_2);

	constexpr auto k_proxyModelRotationOffset = math::Vector3(0.f, math::Pi_Div_2, 0.f);
	constexpr float k_proxyModelDistanceOffset = -10.f;
}

//-------------------------------------------------------------------------

DirectionalShadowMappingDemo::DirectionalShadowMappingDemo()
	: m_projectorFrustum(math::Matrix4::Identity)
	, m_projectedTextureScalingMatrix(math::Matrix4::Identity)

	, m_drawDepthMap(true)
	, m_depthBias(0.0005f)
	, m_slopeScaledDepthBias(2.f)

	, m_techniqueType(DirectionalShadowMappingTechnique::Simple)

	, m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
{
	m_model.worldMatrix = math::Matrix4::Identity;

	SetTextureName("Checkerboard");
}

DirectionalShadowMappingDemo::~DirectionalShadowMappingDemo() = default;

//-------------------------------------------------------------------------

void DirectionalShadowMappingDemo::Initialize(const Application& app)
{
	const auto camera = GetCamera();

	assert(camera);

	m_meshesData = { MeshData() };
	auto& md = m_meshesData.front();

	// build plane vertices manually
	{
		using Vertex = Material::Vertex;

		const auto backward = DirectX::XMFLOAT3(math::Vector3::Backward);

		const std::array<Vertex, 6> vertices =
		{
			Vertex(DirectX::XMFLOAT4(-0.5f, -0.5f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), backward),
			Vertex(DirectX::XMFLOAT4(-0.5f, 0.5f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), backward),
			Vertex(DirectX::XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), backward),

			Vertex(DirectX::XMFLOAT4(-0.5f, -0.5f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), backward),
			Vertex(DirectX::XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), backward),
			Vertex(DirectX::XMFLOAT4(0.5f, -0.5f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), backward),
		};

		md.vertexBuffer.elementsCount = vertices.size();
		md.vertexBuffer.buffer = library::Material::CreateVertexBuffer(
			app.GetDevice(),
			vertices
		);

		//-------------------------------------------------------------------------

		const std::array<VertexPosition, 6> positionVertices =
		{
			VertexPosition(vertices[0].position),
			VertexPosition(vertices[1].position),
			VertexPosition(vertices[2].position),
			VertexPosition(vertices[3].position),
			VertexPosition(vertices[4].position),
			VertexPosition(vertices[5].position)
		};

		m_positionVertexBuffer.elementsCount = vertices.size();
		m_positionVertexBuffer.buffer = library::Material::CreateVertexBuffer(
			app.GetDevice(),
			positionVertices
		);
	}

	InitializeMaterial(app, "DirectionalShadowMapping");

	// manually create depth map effect & material
	m_depthMapEffect = Effect::Create(app, "DepthMap");
	m_depthMapMaterial = std::make_unique<DepthMapMaterial>(*m_depthMapEffect);
	m_depthMapMaterial->Initialize();

	SceneComponent::Initialize(app);

	m_directionalLight = std::make_unique<DirectionalLightComponent>();
	//m_directionalLight->Rotate(math::Vector3(0.f, math::Pi, 0.f));

	const auto position = GetPosition() + m_directionalLight->GetDirection() * k_proxyModelDistanceOffset;

	m_proxyModel = std::make_unique<ProxyModelComponent>("DirectionalLightProxy", 0.5f);
	m_proxyModel->SetCamera(*camera);
	m_proxyModel->SetPosition(position);
	m_proxyModel->SetInitialTransform(math::Matrix4::RotationY(math::Pi_Div_2));
	m_proxyModel->Initialize(app);

	// specific
	SetScaling(math::Vector3(10.f));

	m_projector = std::make_unique<OrthographicProjectorComponent>();
	m_projector->SetPosition(position);
	m_projector->Initialize(app);

	m_projectorFrustum.SetProjectionMatrix(m_projector->GetProjectionMatrix());

	m_renderableProjectorFrustum = std::make_unique<RenderableFrustumComponent>();
	m_renderableProjectorFrustum->SetCamera(*camera);
	m_renderableProjectorFrustum->SetPosition(position);
	//m_renderableProjectorFrustum->SetRotation(k_proxyModelRotationOffset);
	m_renderableProjectorFrustum->Initialize(app);
	m_renderableProjectorFrustum->InitializeGeometry(m_projectorFrustum);

	InitializeProjectedTextureScalingMatrix();

	// second model
	{
		Model model(app, "Teapot", true);
		const auto& mesh = model.GetMesh(0);

		m_model.vertexBuffer.buffer = m_material->CreateVertexBuffer(app.GetDevice(), mesh);
		m_model.positionVertexBuffer.buffer = m_depthMapMaterial->CreateVertexBuffer(app.GetDevice(), mesh);

		m_model.vertexBuffer.elementsCount = 
			m_model.positionVertexBuffer.elementsCount = 
				mesh.GetVerticesCount();

		if (mesh.HasIndices())
		{
			m_model.indexBuffer = std::make_optional(
				BufferData{ mesh.CreateIndexBuffer(), mesh.GetIndicesCount() }
			);
		}

		m_model.worldMatrix =
			math::Matrix4::Scaling(math::Vector3(0.1f)) * 
			math::Matrix4::Translation(math::Vector3(0.f, 0.f, 5.f));

	}
	
	m_depthMapRenderTarget = std::make_unique<DepthMapRenderTarget>(app, k_depthMapWidth, k_depthMapHeight);

	m_uiDepthMap = std::make_unique<UIComponent>();
	m_uiDepthMap->SetDestinationRect(k_depthMapDestinationRect);
	m_uiDepthMap->SetTexture(m_depthMapRenderTarget->GetOutputTexture());
	m_uiDepthMap->Initialize(app);

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss <<
				L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n" <<
				L"Directional Light Intensity (+Home/-End): " << m_directionalLight->GetColor().a << "\n" <<
				L"Specular Power (+Insert/-Delete): " << m_specularPower << "\n" <<
				L"Rotate Projector (Arrow Keys)\n" <<
				L"Show Depth Map (Enter): " << (m_drawDepthMap ? "Yes" : "No") << '\n' <<
				L"Active Technique (Space): " << DirectionalShadowMappingTechnique::ToDisplayString(m_techniqueType) << '\n';

				if (m_techniqueType == DirectionalShadowMappingTechnique::PCF)
				{
					woss << std::setprecision(5)
						<< L"Depth Bias Amount (+J/-K): " << (int)m_depthBias << '\n'
						<< L"Slope-Scaled Depth Bias (+O/-P): " << m_slopeScaledDepthBias;
				}

			return woss.str();
		}
	);
	m_text->Initialize(app);
}

//-------------------------------------------------------------------------

void DirectionalShadowMappingDemo::Draw(const library::Time& time)
{
	auto deviceContext = GetApp()->GetDeviceContext();

	//-------------------------------------------------------------------------
	// depth map pass (render the teapot model only)
	//-------------------------------------------------------------------------
	{
		GetApp()->GetRenderer()->SaveRenderState(RenderState::Rasterizer);
		m_depthMapRenderTarget->Begin();

		deviceContext->IASetPrimitiveTopology(m_primitiveTopology);
		deviceContext->ClearDepthStencilView(
			m_depthMapRenderTarget->GetDepthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,
			0
		);

		auto& pass = m_depthMapMaterial->GetCurrentTechnique().GetPass(0);
		auto inputLayout = m_depthMapMaterial->GetInputLayout(pass);
		deviceContext->IASetInputLayout(inputLayout);

		deviceContext->RSSetState(m_depthBiasState.Get());

		const auto stride = m_depthMapMaterial->GetVertexSize();
		const unsigned offset = 0;
		deviceContext->IASetVertexBuffers(
			0, 1, m_model.positionVertexBuffer.buffer.GetAddressOf(), &stride, &offset
		);
		if (m_model.indexBuffer)
			deviceContext->IASetIndexBuffer(m_model.indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		m_depthMapMaterial->GetWorldLightViewProjection() << 
			m_model.worldMatrix * m_projector->GetViewProjectionMatrix();

		pass.Apply(0, deviceContext);

		if (m_model.indexBuffer)
			deviceContext->DrawIndexed(m_model.indexBuffer->elementsCount, 0, 0);
		else
			deviceContext->Draw(m_model.positionVertexBuffer.elementsCount, 0);

		m_depthMapRenderTarget->End();

		GetApp()->GetRenderer()->RestoreRenderState(RenderState::Rasterizer);
	}

	//-------------------------------------------------------------------------
	// projective texture mapping pass
	//-------------------------------------------------------------------------
	{
		// Draw plane
		SceneComponent::Draw(time);
		GetApp()->UnbindPixelShaderResources(0, 3);

		// Draw teapot model
		const auto stride = m_material->GetVertexSize();
		const unsigned offset{ 0 };
		deviceContext->IASetVertexBuffers(0, 1, m_model.vertexBuffer.buffer.GetAddressOf(), &stride, &offset);
		if (m_model.indexBuffer)
			deviceContext->IASetIndexBuffer(m_model.indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		auto modelWvp = m_model.worldMatrix;
		if (!!m_camera)
		{
			modelWvp *= m_camera->GetViewProjectionMatrix();
		}

		const auto modelProjectiveTextureMatrix =
			m_model.worldMatrix * m_projector->GetViewProjectionMatrix() * m_projectedTextureScalingMatrix;

		m_material->GetWVP() << modelWvp;
		m_material->GetWorld() << m_model.worldMatrix;

		m_material->GetSpecularPower() << m_specularPower;
		m_material->GetSpecularColor() << m_specularColor;
		m_material->GetAmbientColor() << m_ambientColor;
		m_material->GetLightColor() << m_directionalLight->GetColor();
		m_material->GetLightDirection() << m_directionalLight->GetDirection();

		m_material->GetProjectiveTextureMatrix() << modelProjectiveTextureMatrix;

		m_material->GetColorTexture() << m_meshesData.front().texture.Get(); // use same texture for model
		m_material->GetShadowMapTexture() << m_depthMapRenderTarget->GetOutputTexture();

		const math::Vector2 shadowMapSize(k_depthMapWidth, k_depthMapHeight);
		m_material->GetShadowMapSize() << shadowMapSize;

		auto& pass = m_material->GetCurrentTechnique().GetPass(0);
		pass.Apply(0, deviceContext);

		if (m_model.indexBuffer)
			deviceContext->DrawIndexed(m_model.indexBuffer->elementsCount, 0, 0);
		else
			deviceContext->Draw(m_model.vertexBuffer.elementsCount, 0);

		GetApp()->UnbindPixelShaderResources(0, 3);
	}
}

void DirectionalShadowMappingDemo::Draw_SetData(const MeshData& meshData)
{
	const auto world = GetWorldMatrix();

	auto wvp = world;
	if (!!m_camera)
	{
		wvp *= m_camera->GetViewProjectionMatrix();

		m_material->GetCameraPosition() << m_camera->GetPosition();
	}

	const auto projectiveTextureMatrix =
		world * m_projector->GetViewProjectionMatrix() * m_projectedTextureScalingMatrix;

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << world;

	m_material->GetSpecularPower() << m_specularPower;
	m_material->GetSpecularColor() << m_specularColor;
	m_material->GetAmbientColor() << m_ambientColor;
	m_material->GetLightColor() << m_directionalLight->GetColor();
	m_material->GetLightDirection() << m_directionalLight->GetDirection();

	//m_material->GetDepthBias() << m_depthBias;
	m_material->GetColorTexture() << meshData.texture.Get();
	m_material->GetShadowMapTexture() << m_depthMapRenderTarget->GetOutputTexture();
	m_material->GetProjectiveTextureMatrix() << projectiveTextureMatrix;

	const math::Vector2 shadowMapSize(k_depthMapWidth, k_depthMapHeight);
	m_material->GetShadowMapSize() << shadowMapSize;

	SceneComponent::Draw_SetData(meshData);
}

//-------------------------------------------------------------------------

void DirectionalShadowMappingDemo::Update(const Time& time)
{
	UpdateTechnique();

	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyPressed(Key::Enter))
			m_drawDepthMap = !m_drawDepthMap;
	}

	UpdateDepthBias(time);
	UpdateDepthBiasState();

	UpdateAmbientLight(time);
	UpdatePointLightAndProjector(time);
	UpdateSpecularLight(time);

	if (m_drawDepthMap)
		m_uiDepthMap->Update(time);

	m_text->Update(time);
	m_proxyModel->Update(time);
	m_projector->Update(time);
	m_renderableProjectorFrustum->Update(time);

	SceneComponent::Update(time);
}

void DirectionalShadowMappingDemo::UpdateTechnique()
{
	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyPressed(Key::Space))
		{
			m_techniqueType = DirectionalShadowMappingTechnique::Next(m_techniqueType);
			m_material->SetCurrentTechnique(
				m_effect->GetTechnique(DirectionalShadowMappingTechnique::ToString(m_techniqueType))
			);

			const auto depthMapTechnique = DirectionalShadowMappingTechnique::GetDepthMapTechniqueType(m_techniqueType);
			m_depthMapMaterial->SetCurrentTechnique(
				m_depthMapEffect->GetTechnique(DepthMappingTechnique::ToString(depthMapTechnique))
			);
		}
	}
}

void DirectionalShadowMappingDemo::UpdateDepthBias(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::O))
		{
			m_slopeScaledDepthBias += elapsedTime;
			UpdateDepthBiasState();
		}

		if (m_keyboard->IsKeyDown(Key::P) && m_slopeScaledDepthBias > 0.f)
		{
			m_slopeScaledDepthBias -= elapsedTime;
			UpdateDepthBiasState();
		}

		//-------------------------------------------------------------------------

		if (m_keyboard->IsKeyDown(Key::J))
		{
			m_depthBias += k_depthBiasModulationRate * elapsedTime;
		}

		if (m_keyboard->IsKeyDown(Key::K) && m_depthBias > 0.f)
		{
			m_depthBias -= k_depthBiasModulationRate * elapsedTime;
			m_depthBias = math::Max(m_depthBias, 0.f);
		}
	}
}

void DirectionalShadowMappingDemo::UpdateDepthBiasState()
{
	m_depthBiasState.Reset();

	D3D11_RASTERIZER_DESC rasterizerStateDesc{};
	rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
	rasterizerStateDesc.DepthClipEnable = true;
	rasterizerStateDesc.DepthBias = static_cast<int>(m_depthBias);
	rasterizerStateDesc.SlopeScaledDepthBias = m_slopeScaledDepthBias;

	const auto hr = GetApp()->GetDevice()->CreateRasterizerState(
		&rasterizerStateDesc, m_depthBiasState.GetAddressOf()
	);
	if (FAILED(hr))
	{
		throw Exception("ID3D11Device::CreateReasterizerState() failed.", hr);
	}
}

void DirectionalShadowMappingDemo::UpdateAmbientLight(const Time& time)
{
	static float ambientLightIntensity = m_ambientColor.a;

	if (!!m_keyboard)
	{
		if (m_keyboard->IsKeyDown(Key::PageUp) && ambientLightIntensity < k_byteMax)
		{
			ambientLightIntensity += k_lightModulationRate * time.elapsed.GetSeconds();
			m_ambientColor.a = math::Min(ambientLightIntensity, k_byteMax);
		}

		if (m_keyboard->IsKeyDown(Key::PageDown) && ambientLightIntensity > 0)
		{
			ambientLightIntensity -= k_lightModulationRate * time.elapsed.GetSeconds();
			m_ambientColor.a = math::Max(ambientLightIntensity, 0.f);
		}
	}
}

void DirectionalShadowMappingDemo::UpdatePointLightAndProjector(const Time& time)
{
	static float pointLightIntensity = m_directionalLight->GetColor().a;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		// update directional light intensity
		if (m_keyboard->IsKeyDown(Key::Home) && pointLightIntensity < k_byteMax)
		{
			pointLightIntensity += k_lightModulationRate * elapsedTime;

			auto pointLightColor = m_directionalLight->GetColor();
			pointLightColor.a = math::Min(pointLightIntensity, k_byteMax);
			m_directionalLight->SetColor(pointLightColor);
		}

		if (m_keyboard->IsKeyDown(Key::End) && pointLightIntensity > 0)
		{
			pointLightIntensity -= k_lightModulationRate * elapsedTime;

			auto pointLightColor = m_directionalLight->GetColor();
			pointLightColor.a = math::Max(pointLightIntensity, 0.f);
			m_directionalLight->SetColor(pointLightColor);
		}

		// rotate projector
		{
			math::Vector2 rotationAmount;

			if (m_keyboard->IsKeyDown(Key::Left))
				rotationAmount.x += k_lightRotationRate.x * elapsedTime;

			if (m_keyboard->IsKeyDown(Key::Right))
				rotationAmount.x -= k_lightRotationRate.x * elapsedTime;

			if (m_keyboard->IsKeyDown(Key::Up))
				rotationAmount.y += k_lightRotationRate.y * elapsedTime;

			if (m_keyboard->IsKeyDown(Key::Down))
				rotationAmount.y -= k_lightRotationRate.y * elapsedTime;

			if (rotationAmount)
			{
				const auto rotation = math::Vector3(rotationAmount.y, rotationAmount.x, 0.f);

				m_directionalLight->Rotate(rotation);
				m_proxyModel->Rotate(rotation);
				m_renderableProjectorFrustum->Rotate(rotation);

				const auto pitchMatrix = math::Matrix4::RotationAxis(
					m_projector->GetRight(), rotationAmount.y
				);
				const auto yawMatrix = math::Matrix4::RotationY(rotationAmount.x);
				m_projector->ApplyRotation(pitchMatrix * yawMatrix);
			}
		}
	}
}

void DirectionalShadowMappingDemo::UpdateSpecularLight(const Time& time)
{
	static float specularLightIntensity = m_specularPower;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::Insert) && specularLightIntensity < k_byteMax)
		{
			specularLightIntensity += k_lightModulationRate * elapsedTime;
			specularLightIntensity = math::Min(specularLightIntensity, k_byteMax);

			m_specularPower = specularLightIntensity;;
		}

		if (m_keyboard->IsKeyDown(Key::Delete) && specularLightIntensity > 0)
		{
			specularLightIntensity -= k_lightModulationRate * elapsedTime;
			specularLightIntensity = math::Max(specularLightIntensity, 0.f);

			m_specularPower = specularLightIntensity;
		}
	}
}

//-------------------------------------------------------------------------

void DirectionalShadowMappingDemo::InitializeProjectedTextureScalingMatrix()
{
	m_projectedTextureScalingMatrix._11 = 0.5f;
	m_projectedTextureScalingMatrix._22 = -0.5f;
	m_projectedTextureScalingMatrix._33 = 1.0f;
	m_projectedTextureScalingMatrix._41 = 0.5f;
	m_projectedTextureScalingMatrix._42 = 0.5f;
	m_projectedTextureScalingMatrix._44 = 1.f;
}

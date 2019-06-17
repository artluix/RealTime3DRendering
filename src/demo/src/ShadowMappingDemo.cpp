#include "ShadowMappingDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Components/UIComponent.h>
#include <library/Components/TextComponent.h>
#include <library/Components/ProjectorComponent.h>
#include <library/Components/ProxyModelComponent.h>
#include <library/Components/RenderableFrustumComponent.h>
#include <library/Components/PointLightComponent.h>

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
constexpr float k_lightMovementRate = 10.f;
constexpr auto k_lightRotationRate = math::Vector2(math::Pi_2);
}

//-------------------------------------------------------------------------

ShadowMappingDemo::ShadowMappingDemo()
	: m_projectorFrustum(math::Matrix4::Identity)
	, m_projectedTextureScalingMatrix(math::Matrix4::Identity)

	, m_drawDepthMap(true)
	, m_depthBias(0.0005f)
	, m_slopeScaledDepthBias(2.f)

	, m_techniqueType(ShadowMappingTechnique::Simple)

	, m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
{
}

ShadowMappingDemo::~ShadowMappingDemo() = default;

//-------------------------------------------------------------------------

void ShadowMappingDemo::InitializeInternal()
{
	const auto camera = GetCamera();
	assert(!!camera);

	// build plane manually
	{
		using Vertex = Material::Vertex;
		using namespace library::math;

		constexpr std::array<Vertex, 6> vertices =
		{
			Vertex(Vector4(-0.5f, -0.5f, 0.0f, 1.0f), Vector2(0.0f, 1.0f), Direction::Backward),
			Vertex(Vector4(-0.5f, 0.5f, 0.0f, 1.0f), Vector2(0.0f, 0.0f), Direction::Backward),
			Vertex(Vector4(0.5f, 0.5f, 0.0f, 1.0f), Vector2(1.0f, 0.0f), Direction::Backward),

			Vertex(Vector4(-0.5f, -0.5f, 0.0f, 1.0f), Vector2(0.0f, 1.0f), Direction::Backward),
			Vertex(Vector4(0.5f, 0.5f, 0.0f, 1.0f), Vector2(1.0f, 0.0f), Direction::Backward),
			Vertex(Vector4(0.5f, -0.5f, 0.0f, 1.0f), Vector2(1.0f, 1.0f), Direction::Backward),
		};

		m_primitivesData.clear();
		auto& pd = m_primitivesData.emplace_back(PrimitiveData{});

		pd.stride = sizeof(Vertex);

		pd.vertexBuffer = VertexBufferData(GetApp().GetDevice(), vertices);
	}

	// textures
	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().LoadTexture("Checkerboard");

	CreateMaterialWithEffect("ShadowMapping");

	// manually create depth map effect & material
	m_depthMapEffect = Effect::Create(GetApp(), "DepthMap");
	m_depthMapMaterial = std::make_unique<DepthMapMaterial>(*m_depthMapEffect);
	m_depthMapMaterial->Initialize();

	m_pointLight = std::make_unique<PointLightComponent>();
	m_pointLight->SetRadius(500.f);
	m_pointLight->SetPosition(math::Vector3(0.f, 0.f, 10.f));

	m_proxyModel = std::make_unique<ProxyModelComponent>("PointLightProxy", 0.5f);
	m_proxyModel->SetCamera(*camera);
	m_proxyModel->SetPosition(m_pointLight->GetPosition());
	m_proxyModel->Initialize(GetApp());

	// specific
	SetScaling(math::Vector3(10.f));

	m_projector = std::make_unique<ProjectorComponent>();
	m_projector->SetPosition(m_pointLight->GetPosition());
	m_projector->Initialize(GetApp());

	m_projectorFrustum.SetProjectionMatrix(m_projector->GetProjectionMatrix());

	m_renderableProjectorFrustum = std::make_unique<RenderableFrustumComponent>();
	m_renderableProjectorFrustum->SetCamera(*camera);
	m_renderableProjectorFrustum->SetPosition(m_pointLight->GetPosition());
	m_renderableProjectorFrustum->SetInitialTransform(math::Matrix4::RotationY(math::Pi));
	m_renderableProjectorFrustum->Initialize(GetApp());
	m_renderableProjectorFrustum->InitializeGeometry(m_projectorFrustum);

	InitializeProjectedTextureScalingMatrix();

	// second model
	{
		using namespace library::math;

		Model model(GetApp(), "Teapot", true);
		const auto& mesh = model.GetMesh(0);

		// depth map vertex buffer
		m_modelPositionVertexBuffer = m_depthMapMaterial->CreateVertexBufferData(GetApp().GetDevice(), mesh);

		auto& pd = m_primitivesData.emplace_back(PrimitiveData{});
		pd = m_material->CreatePrimitiveData(GetApp().GetDevice(), mesh);

		m_modelWorldMatrix = Matrix4::Scaling(Vector3(0.1f)) * Matrix4::Translation(Vector3(0.f, 0.f, 5.f));
	}

	m_depthMapRenderTarget = std::make_unique<DepthMapRenderTarget>(
		GetApp(), k_depthMapWidth, k_depthMapHeight
	);

	m_uiDepthMap = std::make_unique<UIComponent>();
	m_uiDepthMap->SetDestinationRect(k_depthMapDestinationRect);
	m_uiDepthMap->SetTexture(m_depthMapRenderTarget->GetOutputTexture());
	m_uiDepthMap->Initialize(GetApp());

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss <<
				L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n" <<
				L"Point Light Intensity (+Home/-End): " << m_pointLight->GetColor().a << "\n" <<
				L"Specular Power (+Insert/-Delete): " << m_specularPower << "\n" <<
				L"Move Projector/Light (Numpad: 8/2, 4/6, 3/9)\n" <<
				L"Rotate Projector (Arrow Keys)\n" <<
				L"Show Depth Map (Enter): " << (m_drawDepthMap ? "Yes" : "No") << '\n' <<
				L"Active Technique (Space): " << ShadowMappingTechnique::ToDisplayString(m_techniqueType) << '\n';

				if (m_techniqueType == ShadowMappingTechnique::PCF)
				{
					woss << std::setprecision(5)
						<< L"Depth Bias Amount (+J/-K): " << (int)m_depthBias << '\n'
						<< L"Slope-Scaled Depth Bias (+O/-P): " << m_slopeScaledDepthBias;
				}

			return woss.str();
		}
	);
	m_text->Initialize(GetApp());
}

//-------------------------------------------------------------------------

void ShadowMappingDemo::Draw(const library::Time& time)
{
	auto deviceContext = GetApp().GetDeviceContext();

	const auto& planeData = m_primitivesData[Primitive::Plane];
	const auto& modelData = m_primitivesData[Primitive::Model];

	//-------------------------------------------------------------------------
	// depth map pass (render the teapot model only)
	//-------------------------------------------------------------------------
	{
		GetApp().GetRenderer()->SaveRenderState(RenderState::Rasterizer);
		m_depthMapRenderTarget->Begin();

		deviceContext->IASetPrimitiveTopology(modelData.primitiveTopology);
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
			0, 1, &m_modelPositionVertexBuffer.buffer, &stride, &offset
		);
		if (modelData.indexBuffer)
			deviceContext->IASetIndexBuffer(modelData.indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		m_depthMapMaterial->GetWorldLightViewProjection() << 
			m_modelWorldMatrix * m_projector->GetViewProjectionMatrix();

		pass.Apply(0, deviceContext);

		if (modelData.indexBuffer)
			deviceContext->DrawIndexed(modelData.indexBuffer->elementsCount, 0, 0);
		else
			deviceContext->Draw(m_modelPositionVertexBuffer.elementsCount, 0);

		m_depthMapRenderTarget->End();

		GetApp().GetRenderer()->RestoreRenderState(RenderState::Rasterizer);
	}

	//-------------------------------------------------------------------------
	// shadow mapping pass
	//-------------------------------------------------------------------------
	{
		// Draw plane
		Draw_SetIA(planeData);
		Draw_SetData(GetWorldMatrix(), planeData);
		Draw_Render(planeData);
		GetApp().UnbindPixelShaderResources(0, 3);

		// Draw teapot model
		Draw_SetIA(modelData);
		Draw_SetData(m_modelWorldMatrix, modelData);
		Draw_Render(modelData);
		GetApp().UnbindPixelShaderResources(0, 3);
	}
}

void ShadowMappingDemo::Draw_SetData(
	const math::Matrix4& worldMatrix,
	const PrimitiveData& primitiveData
)
{
	auto wvp = worldMatrix;
	if (auto camera = GetCamera())
	{
		wvp *= camera->GetViewProjectionMatrix();

		m_material->GetCameraPosition() << camera->GetPosition();
	}

	const auto projectiveTextureMatrix =
		worldMatrix * m_projector->GetViewProjectionMatrix() * m_projectedTextureScalingMatrix;

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << worldMatrix;

	m_material->GetSpecularPower() << m_specularPower;
	m_material->GetSpecularColor() << m_specularColor.ToVector4();
	m_material->GetAmbientColor() << m_ambientColor.ToVector4();
	m_material->GetLightColor() << m_pointLight->GetColor().ToVector4();
	m_material->GetLightPosition() << m_pointLight->GetPosition();
	m_material->GetLightRadius() << m_pointLight->GetRadius();

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();
	m_material->GetShadowMapTexture() << m_depthMapRenderTarget->GetOutputTexture();

	m_material->GetProjectiveTextureMatrix() << projectiveTextureMatrix;

	constexpr math::Vector2 shadowMapSize(k_depthMapWidth, k_depthMapHeight);
	m_material->GetShadowMapSize() << shadowMapSize;

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}

//-------------------------------------------------------------------------

void ShadowMappingDemo::Update(const Time& time)
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

	PrimitiveComponent::Update(time);
}

void ShadowMappingDemo::UpdateTechnique()
{
	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyPressed(Key::Space))
		{
			m_techniqueType = ShadowMappingTechnique::Next(m_techniqueType);
			m_material->SetCurrentTechnique(ShadowMappingTechnique::ToString(m_techniqueType));

			const auto depthMapTechnique = ShadowMappingTechnique::GetDepthMapTechniqueType(m_techniqueType);
			m_depthMapMaterial->SetCurrentTechnique(DepthMappingTechnique::ToString(depthMapTechnique));
		}
	}
}

void ShadowMappingDemo::UpdateDepthBias(const Time& time)
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

void ShadowMappingDemo::UpdateDepthBiasState()
{
	m_depthBiasState.Reset();

	D3D11_RASTERIZER_DESC rasterizerStateDesc{};
	rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
	rasterizerStateDesc.DepthClipEnable = true;
	rasterizerStateDesc.DepthBias = static_cast<int>(m_depthBias);
	rasterizerStateDesc.SlopeScaledDepthBias = m_slopeScaledDepthBias;

	const auto hr = GetApp().GetDevice()->CreateRasterizerState(&rasterizerStateDesc, &m_depthBiasState);
	assert("ID3D11Device::CreateRasterizerState() failed." && SUCCEEDED(hr));
}

void ShadowMappingDemo::UpdateAmbientLight(const Time& time)
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

void ShadowMappingDemo::UpdatePointLightAndProjector(const Time& time)
{
	static float pointLightIntensity = m_pointLight->GetColor().a;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		// update directional light intensity
		if (m_keyboard->IsKeyDown(Key::Home) && pointLightIntensity < k_byteMax)
		{
			pointLightIntensity += k_lightModulationRate * elapsedTime;

			auto pointLightColor = m_pointLight->GetColor();
			pointLightColor.a = math::Min(pointLightIntensity, k_byteMax);
			m_pointLight->SetColor(pointLightColor);
		}

		if (m_keyboard->IsKeyDown(Key::End) && pointLightIntensity > 0)
		{
			pointLightIntensity -= k_lightModulationRate * elapsedTime;

			auto pointLightColor = m_pointLight->GetColor();
			pointLightColor.a = math::Max(pointLightIntensity, 0.f);
			m_pointLight->SetColor(pointLightColor);
		}

		// move
		{
			math::Vector3i movementAmount;

			if (m_keyboard->IsKeyDown(Key::Num_4))
				movementAmount.x--;
			if (m_keyboard->IsKeyDown(Key::Num_6))
				movementAmount.x++;

			if (m_keyboard->IsKeyDown(Key::Num_9))
				movementAmount.y++;
			if (m_keyboard->IsKeyDown(Key::Num_3))
				movementAmount.y--;

			if (m_keyboard->IsKeyDown(Key::Num_8))
				movementAmount.z--;
			if (m_keyboard->IsKeyDown(Key::Num_2))
				movementAmount.z++;

			if (movementAmount)
			{
				const auto movement = movementAmount * k_lightMovementRate * elapsedTime;
				const auto position = m_pointLight->GetPosition() + movement;

				m_pointLight->SetPosition(position);
				m_proxyModel->SetPosition(position);
				m_projector->SetPosition(position);
				m_renderableProjectorFrustum->SetPosition(position);
			}
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
				const auto rotationQuat = math::Quaternion::RotationPitchYawRoll(
					rotationAmount.y, rotationAmount.x, 0.f
				);

				m_proxyModel->Rotate(rotationQuat);
				m_renderableProjectorFrustum->Rotate(rotationQuat);
				m_projector->Rotate(rotationQuat);
			}
		}
	}
}

void ShadowMappingDemo::UpdateSpecularLight(const Time& time)
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

void ShadowMappingDemo::InitializeProjectedTextureScalingMatrix()
{
	m_projectedTextureScalingMatrix._11 = 0.5f;
	m_projectedTextureScalingMatrix._22 = -0.5f;
	m_projectedTextureScalingMatrix._33 = 1.0f;
	m_projectedTextureScalingMatrix._41 = 0.5f;
	m_projectedTextureScalingMatrix._42 = 0.5f;
	m_projectedTextureScalingMatrix._44 = 1.f;
}

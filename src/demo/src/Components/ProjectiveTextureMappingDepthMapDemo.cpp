#include "ProjectiveTextureMappingDepthMapDemo.h"

#include "Materials/DepthMapMaterial.h"
#include "DemoUtils.h"

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

constexpr float k_depthBiasModulationRate = 1.f;
constexpr unsigned k_depthMapWidth = 1024U;
constexpr unsigned k_depthMapHeight = 1024U;
const RECT k_depthMapDestinationRect = { 0, 512, 256, 768 };

constexpr float k_lightModulationRate = 10.f;
constexpr float k_lightMovementRate = 10.f;
constexpr auto k_lightRotationRate = math::Vector2(math::Pi_2);
}

//-------------------------------------------------------------------------

ProjectiveTextureMappingDepthMapDemo::ProjectiveTextureMappingDepthMapDemo()
	: m_projectorFrustum(math::Matrix4::Identity)
	, m_projectedTextureScalingMatrix(math::Matrix4::Identity)

	, m_drawDepthMap(true)
	, m_depthBias(0.0005f)

	, m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
{
}

ProjectiveTextureMappingDepthMapDemo::~ProjectiveTextureMappingDepthMapDemo() = default;

//-------------------------------------------------------------------------

void ProjectiveTextureMappingDepthMapDemo::InitializeInternal()
{
	const auto camera = GetCamera();

	assert(camera);

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

		pd.vertexBuffer = VertexBufferData(GetApp().GetDevice(), vertices);
	}

	// textures
	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().LoadTexture("Checkerboard");
	m_textures[Texture::Projected] = GetApp().LoadTexture("ProjectedTexture");

	CreateMaterialWithEffect("ProjectiveTextureMapping");
	m_material->SetCurrentTechnique("project_texture_depth_map");

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
				L"Show Depth Map (Space): " << (m_drawDepthMap ? "Yes" : "No") << '\n' <<
				std::setprecision(5) << L"Depth Bias Amount (+O/-P): " << m_depthBias;
			return woss.str();
		}
	);
	m_text->Initialize(GetApp());
}

//-------------------------------------------------------------------------

void ProjectiveTextureMappingDepthMapDemo::Draw(const Time& time)
{
	auto deviceContext = GetApp().GetDeviceContext();

	const auto& planeData = m_primitivesData[Primitive::Plane];
	const auto& modelData = m_primitivesData[Primitive::Model];

	//-------------------------------------------------------------------------
	// depth map pass (render the model(teapot) only)
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

		auto inputLayout = m_depthMapMaterial->GetCurrentInputLayout();
		deviceContext->IASetInputLayout(inputLayout);

		deviceContext->IASetVertexBuffers(
			0,
			1,
			&m_modelPositionVertexBuffer.buffer,
			&m_modelPositionVertexBuffer.stride,
			&m_modelPositionVertexBuffer.offset
		);

		if (modelData.indexBuffer)
			deviceContext->IASetIndexBuffer(
				modelData.indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		m_depthMapMaterial->GetWorldLightViewProjection() <<  m_modelWorldMatrix * m_projector->GetViewProjectionMatrix();

		auto& pass = m_depthMapMaterial->GetCurrentPass();
		pass.Apply(0, deviceContext);

		if (modelData.indexBuffer)
			deviceContext->DrawIndexed(modelData.indexBuffer->elementsCount, 0, 0);
		else
			deviceContext->Draw(m_modelPositionVertexBuffer.elementsCount, 0);

		m_depthMapRenderTarget->End();
		GetApp().GetRenderer()->RestoreRenderState(RenderState::Rasterizer);
	}

	//-------------------------------------------------------------------------
	// projective texture mapping pass
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

void ProjectiveTextureMappingDepthMapDemo::Draw_SetData(
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

	const auto isLightVisible = m_pointLight->IsVisibleFrom(GetPosition());
	m_material->GetPointLightsCount() << unsigned(isLightVisible);
	if (isLightVisible)
		m_material->GetPointLights() << PointLightData(*m_pointLight);

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();
	m_material->GetProjectedTexture() << m_textures[Texture::Projected].Get();

	m_material->GetProjectiveTextureMatrix() << projectiveTextureMatrix;

	m_material->GetDepthMapTexture() << m_depthMapRenderTarget->GetOutputTexture();
	m_material->GetDepthBias() << m_depthBias;

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}

//-------------------------------------------------------------------------

void ProjectiveTextureMappingDepthMapDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto& keyboard = *m_keyboard;

		if (keyboard.WasKeyPressed(Key::Space))
			m_drawDepthMap = !m_drawDepthMap;

		UpdateDepthBias(time, keyboard);
		UpdateAmbientLight(time, keyboard);
		UpdatePointLightAndProjector(time, keyboard);
		UpdateSpecularLight(time, keyboard);
	}

	if (m_drawDepthMap)
		m_uiDepthMap->Update(time);

	m_text->Update(time);
	m_proxyModel->Update(time);
	m_projector->Update(time);
	m_renderableProjectorFrustum->Update(time);

	PrimitiveComponent::Update(time);
}

void ProjectiveTextureMappingDepthMapDemo::UpdateDepthBias(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_depthBiasModulationRate;
	::utils::UpdateValue(m_depthBias, stepValue, math::Interval(.0f, 1.f), keyboard, KeyPair(Key::O, Key::P));
}

void ProjectiveTextureMappingDepthMapDemo::UpdateAmbientLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_ambientColor.a, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::PageUp, Key::PageDown));
}

void ProjectiveTextureMappingDepthMapDemo::UpdatePointLightAndProjector(const Time& time, const KeyboardComponent& keyboard)
{
	const auto elapsedTime = time.elapsed.GetSeconds();

	// update light color intensity
	{
		const float modulationStepValue = elapsedTime * k_lightModulationRate;

		auto lightColor = m_pointLight->GetColor();
		if (::utils::UpdateValue(lightColor.a, modulationStepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::Home, Key::End)))
		{
			m_pointLight->SetColor(lightColor);
		}
	}

	// move
	{
		math::Vector3i movementAmount;

		if (keyboard.IsKeyDown(Key::Num_4))
			movementAmount.x--;
		if (keyboard.IsKeyDown(Key::Num_6))
			movementAmount.x++;

		if (keyboard.IsKeyDown(Key::Num_9))
			movementAmount.y++;
		if (keyboard.IsKeyDown(Key::Num_3))
			movementAmount.y--;

		if (keyboard.IsKeyDown(Key::Num_8))
			movementAmount.z--;
		if (keyboard.IsKeyDown(Key::Num_2))
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
		if (keyboard.IsKeyDown(Key::Left))
			rotationAmount.x += k_lightRotationRate.x * elapsedTime;

		if (keyboard.IsKeyDown(Key::Right))
			rotationAmount.x -= k_lightRotationRate.x * elapsedTime;

		if (keyboard.IsKeyDown(Key::Up))
			rotationAmount.y += k_lightRotationRate.y * elapsedTime;

		if (keyboard.IsKeyDown(Key::Down))
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

void ProjectiveTextureMappingDepthMapDemo::UpdateSpecularLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_specularPower, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::Insert, Key::Delete));
}

//-------------------------------------------------------------------------

void ProjectiveTextureMappingDepthMapDemo::InitializeProjectedTextureScalingMatrix()
{
	m_projectedTextureScalingMatrix._11 = 0.5f;
	m_projectedTextureScalingMatrix._22 = -0.5f;
	m_projectedTextureScalingMatrix._33 = 1.0f;
	m_projectedTextureScalingMatrix._41 = 0.5f;
	m_projectedTextureScalingMatrix._42 = 0.5f;
	m_projectedTextureScalingMatrix._44 = 1.f;
}

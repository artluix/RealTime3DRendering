#include "ProjectiveTextureMappingDepthMapDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Components/UIComponent.h>
#include <library/Components/TextComponent.h>
#include <library/Components/PerspectiveProjectorComponent.h>
#include <library/Components/ProxyModelComponent.h>
#include <library/Components/RenderableFrustumComponent.h>
#include <library/Components/PointLightComponent.h>

#include <library/Materials/DepthMapMaterial.h>
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

	constexpr float k_depthBiasModulationRate = 0.1f;
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
	m_model.worldMatrix = math::Matrix4::Identity;

	SetTextureName("Checkerboard");
}

ProjectiveTextureMappingDepthMapDemo::~ProjectiveTextureMappingDepthMapDemo() = default;

//-------------------------------------------------------------------------

void ProjectiveTextureMappingDepthMapDemo::Initialize()
{
	const auto camera = GetCamera();

	assert(camera);

	// build plane vertices manually
	{
		using Vertex = Material::Vertex;

		m_primitivesData = { PrimitiveData() };
		auto& md = m_primitivesData.front();

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

		m_positionVertices.elementsCount = vertices.size();
		m_positionVertices.buffer = library::Material::CreateVertexBuffer(
			app.GetDevice(),
			positionVertices
		);
	}

	InitializeMaterial("ProjectiveTextureMapping");
	m_material->SetCurrentTechnique(m_effect->GetTechnique("project_texture_depth_map"));

	// manually create depth map effect & material
	m_depthMapEffect = Effect::Create(app, "DepthMap");
	m_depthMapMaterial = std::make_unique<DepthMapMaterial>(*m_depthMapEffect);
	m_depthMapMaterial->Initialize();

	m_projectedTexture = app.LoadTexture("ProjectedTexture");

	m_pointLight = std::make_unique<PointLightComponent>();
	m_pointLight->SetRadius(500.f);
	m_pointLight->SetPosition(math::Vector3(0.f, 0.f, 10.f));

	m_proxyModel = std::make_unique<ProxyModelComponent>("PointLightProxy", 0.5f);
	m_proxyModel->SetCamera(*camera);
	m_proxyModel->SetPosition(m_pointLight->GetPosition());
	m_proxyModel->Initialize();

	// specific
	SetScaling(math::Vector3(10.f));

	m_projector = std::make_unique<PerspectiveProjectorComponent>();
	m_projector->SetPosition(m_pointLight->GetPosition());
	m_projector->Initialize();

	m_projectorFrustum.SetProjectionMatrix(m_projector->GetProjectionMatrix());

	m_renderableProjectorFrustum = std::make_unique<RenderableFrustumComponent>();
	m_renderableProjectorFrustum->SetCamera(*camera);
	m_renderableProjectorFrustum->SetPosition(m_pointLight->GetPosition());
	m_renderableProjectorFrustum->Initialize();
	m_renderableProjectorFrustum->InitializeGeometry(m_projectorFrustum);

	InitializeProjectedTextureScalingMatrix();

	// second model
	{
		Model model(app, "Teapot", true);
		const auto& mesh = model.GetMesh(0);

		m_model.vertexBuffer.buffer = m_material->CreateVertexBuffer(app.GetDevice(), mesh);
		m_model.positionVertexBuffer.buffer = m_depthMapMaterial->CreateVertexBuffer(app.GetDevice(), mesh);

		m_model.vertexBuffer.elementsCount = m_model.positionVertexBuffer.elementsCount = mesh.GetVerticesCount();

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
	
	m_depthMapRenderTarget = std::make_unique<DepthMapRenderTarget>(
		app, k_depthMapWidth, k_depthMapHeight
	);

	m_uiDepthMap = std::make_unique<UIComponent>();
	m_uiDepthMap->SetDestinationRect(k_depthMapDestinationRect);
	m_uiDepthMap->SetTexture(m_depthMapRenderTarget->GetOutputTexture());
	m_uiDepthMap->Initialize();

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
	m_text->Initialize();
}

//-------------------------------------------------------------------------

void ProjectiveTextureMappingDepthMapDemo::Draw(const library::Time& time)
{
	auto deviceContext = GetApp().GetDeviceContext();

	const auto& md = m_primitivesData.front();

	//-------------------------------------------------------------------------
	// depth map pass (render the teapot model only)
	//-------------------------------------------------------------------------
	{
		GetApp().GetRenderer()->SaveRenderState(RenderState::Rasterizer);
		m_depthMapRenderTarget->Begin();

		deviceContext->IASetPrimitiveTopology(md.primitiveTopology);
		deviceContext->ClearDepthStencilView(
			m_depthMapRenderTarget->GetDepthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,
			0
		);

		auto& pass = m_depthMapMaterial->GetCurrentTechnique().GetPass(0);
		auto inputLayout = m_depthMapMaterial->GetInputLayout(pass);
		deviceContext->IASetInputLayout(inputLayout);

		const auto stride = m_depthMapMaterial->GetVertexSize();
		const unsigned offset = 0;
		deviceContext->IASetVertexBuffers(
			0, 1, &m_model.positionVertexBuffer.buffer, &stride, &offset
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
		GetApp().GetRenderer()->RestoreRenderState(RenderState::Rasterizer);
	}

	//-------------------------------------------------------------------------
	// projective texture mapping pass
	//-------------------------------------------------------------------------
	{
		// Draw plane
		SceneComponent::Draw(time);
		GetApp().UnbindPixelShaderResources(0, 3);

		// Draw teapot model
		const auto stride = m_material->GetVertexSize();
		const unsigned offset{ 0 };
		deviceContext->IASetVertexBuffers(0, 1, &m_model.vertexBuffer.buffer, &stride, &offset);
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
		m_material->GetLightColor() << m_pointLight->GetColor();
		m_material->GetLightPosition() << m_pointLight->GetPosition();
		m_material->GetLightRadius() << m_pointLight->GetRadius();

		m_material->GetColorTexture() << md.texture.Get();
		m_material->GetProjectedTexture() << m_projectedTexture.Get();

		m_material->GetProjectiveTextureMatrix() << modelProjectiveTextureMatrix;

		m_material->GetDepthMapTexture() << m_depthMapRenderTarget->GetOutputTexture();
		m_material->GetDepthBias() << m_depthBias;

		m_currentPass->Apply(0, deviceContext);

		if (m_model.indexBuffer)
			deviceContext->DrawIndexed(m_model.indexBuffer->elementsCount, 0, 0);
		else
			deviceContext->Draw(m_model.vertexBuffer.elementsCount, 0);

		GetApp().UnbindPixelShaderResources(0, 3);
	}
}

void ProjectiveTextureMappingDepthMapDemo::Draw_SetData(const PrimitiveData& primitiveData)
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
	m_material->GetLightColor() << m_pointLight->GetColor();
	m_material->GetLightPosition() << m_pointLight->GetPosition();
	m_material->GetLightRadius() << m_pointLight->GetRadius();

	m_material->GetColorTexture() << primitiveData.texture.Get();
	m_material->GetProjectedTexture() << m_projectedTexture.Get();

	m_material->GetProjectiveTextureMatrix() << projectiveTextureMatrix;

	m_material->GetDepthMapTexture() << m_depthMapRenderTarget->GetOutputTexture();
	m_material->GetDepthBias() << m_depthBias;

	SceneComponent::Draw_SetData(primitiveData);
}

//-------------------------------------------------------------------------

void ProjectiveTextureMappingDepthMapDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyPressed(Key::Space))
			m_drawDepthMap = !m_drawDepthMap;
	}

	UpdateDepthBias(time);
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

void ProjectiveTextureMappingDepthMapDemo::UpdateDepthBias(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::O) && m_depthBias < 1.f)
		{
			m_depthBias += k_depthBiasModulationRate * elapsedTime;
			m_depthBias = math::Min(m_depthBias, 1.f);
		}

		if (m_keyboard->IsKeyDown(Key::P) && m_depthBias > 0.f)
		{
			m_depthBias -= k_depthBiasModulationRate * elapsedTime;
			m_depthBias = math::Max(m_depthBias, 0.f);
		}
	}
}

void ProjectiveTextureMappingDepthMapDemo::UpdateAmbientLight(const Time& time)
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

void ProjectiveTextureMappingDepthMapDemo::UpdatePointLightAndProjector(const Time& time)
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
			math::Vector3 movementAmount;

			if (m_keyboard->IsKeyDown(Key::Num_4))
				movementAmount.x = -1.0f;
			if (m_keyboard->IsKeyDown(Key::Num_6))
				movementAmount.x = 1.0f;

			if (m_keyboard->IsKeyDown(Key::Num_9))
				movementAmount.y = 1.0f;
			if (m_keyboard->IsKeyDown(Key::Num_3))
				movementAmount.y = -1.0f;

			if (m_keyboard->IsKeyDown(Key::Num_8))
				movementAmount.z = -1.0f;
			if (m_keyboard->IsKeyDown(Key::Num_2))
				movementAmount.z = 1.0f;

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
				const auto pitchMatrix = math::Matrix4::RotationAxis(GetRight(), rotationAmount.y);
				const auto yawMatrix = math::Matrix4::RotationY(rotationAmount.x);

				m_proxyModel->Rotate(math::Vector3(rotationAmount.y, rotationAmount.x, 0.f));
				m_renderableProjectorFrustum->Rotate(math::Vector3(rotationAmount.y, rotationAmount.x, 0.f));
				m_projector->ApplyRotation(pitchMatrix * yawMatrix);
			}
		}
	}
}

void ProjectiveTextureMappingDepthMapDemo::UpdateSpecularLight(const Time& time)
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

void ProjectiveTextureMappingDepthMapDemo::InitializeProjectedTextureScalingMatrix()
{
	m_projectedTextureScalingMatrix._11 = 0.5f;
	m_projectedTextureScalingMatrix._22 = -0.5f;
	m_projectedTextureScalingMatrix._33 = 1.0f;
	m_projectedTextureScalingMatrix._41 = 0.5f;
	m_projectedTextureScalingMatrix._42 = 0.5f;
	m_projectedTextureScalingMatrix._44 = 1.f;
}
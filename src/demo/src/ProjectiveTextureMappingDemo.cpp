#include "ProjectiveTextureMappingDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Components/TextComponent.h>
#include <library/Components/PerspectiveProjectorComponent.h>
#include <library/Components/ProxyModelComponent.h>
#include <library/Components/RenderableFrustumComponent.h>
#include <library/Components/PointLightComponent.h>

#include <library/Effect/EffectVariable.h>

#include <library/Application.h>

#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
	constexpr float k_byteMax = static_cast<float>(0xFF);
	constexpr float k_lightModulationRate = 10.f;
	constexpr float k_lightMovementRate = 10.f;
	constexpr auto k_lightRotationRate = math::Vector2(math::Pi_2);
}

//-------------------------------------------------------------------------

ProjectiveTextureMappingDemo::ProjectiveTextureMappingDemo()
	: m_projectorFrustum(math::Matrix4::Identity)
	, m_projectedTextureScalingMatrix(math::Matrix4::Identity)

	, m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
{
	SetTextureName("Checkerboard");
}

ProjectiveTextureMappingDemo::~ProjectiveTextureMappingDemo() = default;

//-------------------------------------------------------------------------

void ProjectiveTextureMappingDemo::Initialize(const Application& app)
{
	const auto camera = GetCamera();

	assert(camera);

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

		m_meshesData = { MeshData() };
		auto& md = m_meshesData.front();

		md.vertexBuffer.elementsCount = vertices.size();
		md.vertexBuffer.buffer = library::Material::CreateVertexBuffer(
			app.GetDevice(),
			vertices
		);
	}

	InitializeMaterial(app, "ProjectiveTextureMapping");
	SceneComponent::Initialize(app);

	m_projectedTexture = app.LoadTexture("ProjectedTexture");

	m_pointLight = std::make_unique<PointLightComponent>();
	m_pointLight->SetRadius(500.f);
	m_pointLight->SetPosition(math::Vector3(0.f, 0.f, 10.f));

	m_proxyModel = std::make_unique<ProxyModelComponent>("PointLightProxy", 0.5f);
	m_proxyModel->SetCamera(*camera);
	m_proxyModel->SetPosition(m_pointLight->GetPosition());
	m_proxyModel->Initialize(app);

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
				L"Rotate Projector (Arrow Keys)\n";// <<
				//L"Reverse Projection? (Space)" << (m_useNoReverseTechnique ? "Off" : "On");
			return woss.str();
		}
	);
	m_text->Initialize(app);

	// specific
	SetScaling(math::Vector3(10.f));

	m_projector = std::make_unique<PerspectiveProjectorComponent>();
	m_projector->SetPosition(m_pointLight->GetPosition());
	m_projector->Initialize(app);

	m_projectorFrustum.SetProjectionMatrix(m_projector->GetProjectionMatrix());

	m_renderableProjectorFrustum = std::make_unique<RenderableFrustumComponent>();
	m_renderableProjectorFrustum->SetCamera(*camera);
	m_renderableProjectorFrustum->SetPosition(m_pointLight->GetPosition());
	m_renderableProjectorFrustum->Initialize(app);
	m_renderableProjectorFrustum->InitializeGeometry(m_projectorFrustum);

	// texture scaling matrix
	{
		ComPtr<ID3D11Resource> projectedTextureResource;
		m_projectedTexture->GetResource(projectedTextureResource.GetAddressOf());

		ComPtr<ID3D11Texture2D> projectedTexture;
		auto hr = projectedTextureResource->QueryInterface(
			IID_ID3D11Texture2D,
			reinterpret_cast<void**>(projectedTexture.GetAddressOf())
		);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Resource::QueryInterface (ID3D11Texture2D) failed.", hr);
		}

		D3D11_TEXTURE2D_DESC projectedTextureDesc;
		projectedTexture->GetDesc(&projectedTextureDesc);

		InitializeProjectedTextureScalingMatrix(projectedTextureDesc.Width, projectedTextureDesc.Height);
	}
}

void ProjectiveTextureMappingDemo::Draw_SetData(const MeshData& meshData)
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
	
	m_material->GetColorTexture() << meshData.texture.Get();
	m_material->GetProjectedTexture() << m_projectedTexture.Get();

	m_material->GetProjectiveTextureMatrix() << projectiveTextureMatrix;

	SceneComponent::Draw_SetData(meshData);
}

//-------------------------------------------------------------------------

void ProjectiveTextureMappingDemo::Update(const Time& time)
{
	UpdateAmbientLight(time);
	UpdatePointLightAndProjector(time);
	UpdateSpecularLight(time);

	m_text->Update(time);
	m_proxyModel->Update(time);
	m_projector->Update(time);
	m_renderableProjectorFrustum->Update(time);

	SceneComponent::Update(time);
}

void ProjectiveTextureMappingDemo::UpdateAmbientLight(const Time& time)
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

void ProjectiveTextureMappingDemo::UpdatePointLightAndProjector(const Time& time)
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

void ProjectiveTextureMappingDemo::UpdateSpecularLight(const Time& time)
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

void ProjectiveTextureMappingDemo::InitializeProjectedTextureScalingMatrix(
	const unsigned textureWidth,
	const unsigned textureHeight
)
{
	const float scalingBiasX = 0.5f + (0.5f / textureWidth);
	const float scalingBiasY = 0.5f + (0.5f / textureHeight);

	m_projectedTextureScalingMatrix._11 = 0.5f;
	m_projectedTextureScalingMatrix._22 = -0.5f;
	m_projectedTextureScalingMatrix._33 = 1.0f;
	m_projectedTextureScalingMatrix._41 = scalingBiasX;
	m_projectedTextureScalingMatrix._42 = scalingBiasY;
	m_projectedTextureScalingMatrix._44 = 1.f;
}

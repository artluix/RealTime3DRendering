#include "InstancingPointLightDemo.h"

#include <library/Components/TextComponent.h>
#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/PointLightComponent.h>
#include <library/Components/ProxyModelComponent.h>

#include <library/Application.h>
#include <library/RasterizerStates.h>

#include <library/Math/Math.h>
#include <library/Model/Model.h>

#include <library/Effect/EffectVariable.h>

#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
constexpr float k_byteMax = static_cast<float>(0xFF);
constexpr float k_lightModulationRate = 10.f;
constexpr float k_lightMovementRate = 10.f;

constexpr unsigned k_axisInstanceCount = 5;
constexpr unsigned k_instancesCount = k_axisInstanceCount * k_axisInstanceCount * 2;
constexpr float k_axisOffset = 20.f;
} // namespace

InstancingPointLightDemo::InstancingPointLightDemo()
	: m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
{
}

void InstancingPointLightDemo::InitializeInternal()
{
	assert(!!GetCamera());

	CreateMaterialWithEffect("InstancingPointLight");

	// load model
	{
		Model model(GetApp(), "Sphere", true);
		m_primitivesData = m_material->CreatePrimitivesData(GetApp().GetDevice(), model);
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().LoadTexture("EarthComposite");

	m_pointLight = std::make_unique<PointLightComponent>();
	m_pointLight->SetRadius(500.f);
	m_pointLight->SetPosition(math::Vector3(0.f, 0.f, 10.f));

	m_proxyModel = std::make_unique<ProxyModelComponent>("PointLightProxy", 0.5f);
	m_proxyModel->SetPosition(m_pointLight->GetPosition());
	m_proxyModel->SetCamera(*GetCamera());
	m_proxyModel->Initialize(GetApp());

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::wostringstream woss;
		woss << L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n"
			<< L"Point Light Intensity (+Home/-End): " << m_pointLight->GetColor().a << "\n"
			<< L"Move Point Light (Numpad: 8/2, 4/6, 3/9)\n";
		return woss.str();
	});
	m_text->Initialize(GetApp());

	// instances data
	{
		std::vector<InstanceData> instancesData;
		instancesData.reserve(k_instancesCount);

		// generate grid of object positions
		for (unsigned i = 0; i < k_axisInstanceCount; i++)
		{
			const float xPosition = i * k_axisOffset;

			for (unsigned j = 0; j < k_axisInstanceCount; j++)
			{
				const float zPosition = j * k_axisOffset;

				instancesData.emplace_back(
					math::Matrix4::Translation(-xPosition, 0.f, -zPosition),
					m_specularColor,
					m_specularPower
				);
				instancesData.emplace_back(
					math::Matrix4::Translation(xPosition, 0.f, -zPosition),
					m_specularColor,
					m_specularPower
				);
			}
		}

		m_instanceVertexBuffer = VertexBufferData(GetApp().GetDevice(), instancesData);
	}
}

//-------------------------------------------------------------------------

void InstancingPointLightDemo::Update(const Time& time)
{
	UpdateAmbientLight(time);
	UpdatePointLight(time);

	m_text->Update(time);
	m_proxyModel->Update(time);

	PrimitiveComponent::Update(time);
}

void InstancingPointLightDemo::UpdateAmbientLight(const Time& time)
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

void InstancingPointLightDemo::UpdatePointLight(const Time& time)
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
		}
	}
}

//-------------------------------------------------------------------------

void InstancingPointLightDemo::Draw_SetIA(const PrimitiveData& primitiveData)
{
	auto deviceContext = GetApp().GetDeviceContext();

	deviceContext->IASetPrimitiveTopology(primitiveData.primitiveTopology);
	deviceContext->IASetInputLayout(GetInputLayout());

	// set vertex buffer
	{
		constexpr unsigned vertexBufferCount = 2;

		const std::array<ID3D11Buffer*, vertexBufferCount> vertexBuffers =
		{
			primitiveData.vertexBuffer.buffer.Get(),
			m_instanceVertexBuffer.buffer.Get()
		};
		const std::array<unsigned, vertexBufferCount> strides =
		{
			primitiveData.vertexBuffer.stride,
			m_instanceVertexBuffer.stride
		};
		const std::array<unsigned, vertexBufferCount> offsets =
		{
			primitiveData.vertexBuffer.offset,
			m_instanceVertexBuffer.offset
		};

		deviceContext->IASetVertexBuffers(
			0,
			vertexBufferCount,
			vertexBuffers.data(),
			strides.data(),
			offsets.data()
		);
	}

	// set index buffer (if needed)
	if (!!primitiveData.indexBuffer)
	{
		deviceContext->IASetIndexBuffer(primitiveData.indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}

void InstancingPointLightDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto vp = math::Matrix4::Identity;
	if (auto camera = GetCamera())
	{
		vp = camera->GetViewProjectionMatrix();

		m_material->GetCameraPosition() << camera->GetPosition();
	}

	m_material->GetViewProjection() << vp;
	m_material->GetAmbientColor() <<  m_ambientColor.ToVector4();
	m_material->GetLightData() << m_pointLight->GetColor().ToVector4();
	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}

void InstancingPointLightDemo::Draw_Render(const PrimitiveData& primitiveData)
{
	auto deviceContext = GetApp().GetDeviceContext();

	if (!!primitiveData.indexBuffer)
		deviceContext->DrawIndexedInstanced(
			primitiveData.indexBuffer->elementsCount,
			k_instancesCount,
			0,
			0,
			0
		);
	else
		deviceContext->DrawInstanced(primitiveData.vertexBuffer.elementsCount, k_instancesCount, 0, 0);
}

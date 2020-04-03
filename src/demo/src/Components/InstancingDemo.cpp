#include "InstancingDemo.h"

#include "DemoUtils.h"
#include "VertexTypes.h"

#include <library/Components/TextComponent.h>
#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/PointLightComponent.h>
#include <library/Components/ProxyModelComponent.h>

#include <library/Application.h>
#include <library/Render/RasterizerStates.h>

#include <library/Math/Math.h>
#include <library/Model/Model.h>

#include <library/Effect/EffectVariable.h>

#include <sstream>
#include <iomanip>

using namespace library;

//-------------------------------------------------------------------------

namespace
{
constexpr float k_byteMax = static_cast<float>(0xFF);
constexpr float k_lightModulationRate = 10.f;
constexpr float k_lightMovementRate = 10.f;

constexpr unsigned k_axisInstanceCount = 5;
constexpr unsigned k_instancesCount = k_axisInstanceCount * k_axisInstanceCount * 2;
constexpr float k_axisOffset = 20.f;
} // namespace

InstancingDemo::InstancingDemo()
	: m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
{
}

void InstancingDemo::InitializeInternal()
{
	assert(!!GetCamera());

	CreateMaterialWithEffect("InstancingPointLight");

	// load model
	{
		Model model(GetApp(), "Sphere", true);
		CreatePrimitivesData(model);
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().CreateTexture2DSRV("EarthComposite.jpg");

	m_pointLight = std::make_unique<PointLightComponent>();
	m_pointLight->SetupProxyModel(*GetCamera());
	m_pointLight->SetRadius(500.f);
	m_pointLight->SetPosition(math::Vector3(0.f, 0.f, 10.f));

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
		DynArray<VertexInstanceSpecular> instancesData;
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

		m_instanceVertexBuffer = VertexBufferData(GetApp().GetDevice(), MakeArrayBuffer(instancesData));
	}
}

//-------------------------------------------------------------------------

void InstancingDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto& keyboard = *m_keyboard;

		UpdateAmbientLight(time, keyboard);
		UpdatePointLight(time, keyboard);
	}

	m_text->Update(time);
	m_pointLight->Update(time);

	PrimitiveComponent::Update(time);
}

void InstancingDemo::UpdateAmbientLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_ambientColor.a, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::PageUp, Key::PageDown));
}

void InstancingDemo::UpdatePointLight(const Time& time, const KeyboardComponent& keyboard)
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
	}
}

//-------------------------------------------------------------------------

void InstancingDemo::Draw_SetIA(const PrimitiveData& primitiveData)
{
	auto deviceContext = GetApp().GetDeviceContext();

	deviceContext->IASetPrimitiveTopology(primitiveData.primitiveTopology);
	deviceContext->IASetInputLayout(GetInputLayout());

	// set vertex buffer
	{
		constexpr unsigned vertexBufferCount = 2;

		const Array<ID3D11Buffer*, vertexBufferCount> vertexBuffers =
		{
			primitiveData.vertexBuffer.buffer.Get(),
			m_instanceVertexBuffer.buffer.Get()
		};
		const Array<unsigned, vertexBufferCount> strides =
		{
			primitiveData.vertexBuffer.stride,
			m_instanceVertexBuffer.stride
		};
		const Array<unsigned, vertexBufferCount> offsets =
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

void InstancingDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto vp = math::Matrix4::Identity;
	if (auto camera = GetCamera())
	{
		vp = camera->GetViewProjectionMatrix();

		m_material->GetCameraPosition() << camera->GetPosition();
	}

	m_material->GetViewProjection() << vp;
	m_material->GetAmbientColor() <<  m_ambientColor.ToVector4();

	m_material->GetPointLights() << PointLightData(*m_pointLight);
	m_material->GetPointLightsCount() << 1;

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}

void InstancingDemo::Draw_Render(const PrimitiveData& primitiveData)
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

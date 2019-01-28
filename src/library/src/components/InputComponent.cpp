#include "StdAfx.h"
#include "library/components/InputComponent.h"

#include "library/Application.h"
#include "library/Exception.h"

#include <cassert>

namespace library
{
	InputComponent::InputComponent(
		const ComPtr<IDirectInput8>& directInput,
		const DIDATAFORMAT& dataFormat,
		const GUID& guid,
		const DWORD cooperativeLevel
	)
		: m_directInput(directInput)
		, m_dataFormat(dataFormat)
		, m_guid(guid)
		, m_cooperativeLevel(cooperativeLevel)
	{
		assert(!!m_directInput);
	}

	InputComponent::~InputComponent()
	{
		if (!!m_directInputDevice)
		{
			m_directInputDevice->Unacquire();
			m_directInputDevice.Reset();
		}
	}

	void InputComponent::Initialize(const Application& app)
	{
		Component::Initialize(app);

		auto hr = m_directInput->CreateDevice(m_guid, m_directInputDevice.GetAddressOf(), nullptr);
		if (FAILED(hr))
		{
			throw Exception("IDirectInput8::CreateDevice() failed.", hr);
		}

		hr = m_directInputDevice->SetDataFormat(&m_dataFormat);
		if (FAILED(hr))
		{
			throw Exception("IDirectInputDevice::SetDataFormat() failed.", hr);
		}

		hr = m_directInputDevice->SetCooperativeLevel(app.GetWindowHandle(), m_cooperativeLevel);
		if (FAILED(hr))
		{
			throw Exception("IDirectInputDevice::SetCooperativeLevel() failed.", hr);
		}

		// try to acquire the device
		hr = m_directInputDevice->Acquire();
	}

	void InputComponent::GetState(const std::size_t bufferSize, void* buffer)
	{
		assert(!!buffer);

		auto hr = m_directInputDevice->GetDeviceState(bufferSize, buffer);
		if (FAILED(hr))
		{
			// try to reacquire the device
			hr = m_directInputDevice->Acquire();
			if (SUCCEEDED(hr))
			{
				m_directInputDevice->GetDeviceState(bufferSize, buffer);
			}
		}
	}
} // namespace library

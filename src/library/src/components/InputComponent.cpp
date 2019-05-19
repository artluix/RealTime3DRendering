#include "StdAfx.h"
#include "library/Components/InputComponent.h"

#include "library/Application.h"
#include "library/Exception.h"

namespace library
{
InputComponent::InputComponent(
	IDirectInput8& directInput,
	const DIDATAFORMAT& dataFormat,
	const GUID& guid,
	const DWORD cooperativeLevel)
	: m_directInput(directInput)
	, m_dataFormat(dataFormat)
	, m_guid(guid)
	, m_cooperativeLevel(cooperativeLevel)
{}

InputComponent::~InputComponent()
{
	if (!!m_directInputDevice)
	{
		m_directInputDevice->Unacquire();
		m_directInputDevice.Reset();
	}
}

void InputComponent::Initialize()
{
	auto hr = m_directInput.CreateDevice(m_guid, &m_directInputDevice, nullptr);
	if (FAILED(hr))
	{
		throw Exception("IDirectInput8::CreateDevice() failed.", hr);
	}

	hr = m_directInputDevice->SetDataFormat(&m_dataFormat);
	if (FAILED(hr))
	{
		throw Exception("IDirectInputDevice::SetDataFormat() failed.", hr);
	}

	hr = m_directInputDevice->SetCooperativeLevel(GetApp().GetWindowHandle(), m_cooperativeLevel);
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

	auto hr = m_directInputDevice->GetDeviceState(DWORD(bufferSize), buffer);
	if (FAILED(hr))
	{
		// try to reacquire the device
		hr = m_directInputDevice->Acquire();
		if (SUCCEEDED(hr))
		{
			m_directInputDevice->GetDeviceState(DWORD(bufferSize), buffer);
		}
	}
}
} // namespace library

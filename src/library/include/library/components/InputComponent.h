#pragma once
#include "library/Components/Component.h"
#include "library/CommonTypes.h"

#include <dinput.h>

namespace library
{
class InputComponent : public virtual Component
{
	RTTI_CLASS(InputComponent, Component)

public:
	virtual ~InputComponent();

	void Initialize() override;

protected:
	InputComponent(
		IDirectInput8& directInput,
		const DIDATAFORMAT& dataFormat,
		const GUID& guid,
		const DWORD cooperativeLevel);

	void GetState(const std::size_t bufferSize, void* buffer);

	GUID m_guid;
	DIDATAFORMAT m_dataFormat;
	DWORD m_cooperativeLevel;

	IDirectInput8& m_directInput;
	ComPtr<IDirectInputDevice8> m_directInputDevice;
};
} // namespace library

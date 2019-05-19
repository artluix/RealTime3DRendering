#pragma once
#include "library/Components/DrawableComponent.h"
#include "library/Components/SceneComponent.h"
#include "library/PrimitiveData.h"
#include "library/DirectXForwardDeclarations.h"

#include <vector>

namespace library
{
class CameraComponent;
class EffectPass;

class PrimitiveComponent
	: public DrawableComponent
	, public SceneComponent
{
	RTTI_CLASS(PrimitiveComponent, DrawableComponent, SceneComponent)

public:
	PrimitiveComponent();
	~PrimitiveComponent();

	const CameraComponent* GetCamera() const { return m_camera; }
	void SetCamera(const CameraComponent& camera) { m_camera = &camera; }

	void Update(const Time& time) override;
	void Draw(const Time& time) override;

	ID3D11InputLayout* GetInputLayout() const;
	ID3D11ShaderResourceView* GetTexture(const unsigned textureIdx) const;

protected:
	virtual void Draw_SetIA(const PrimitiveData& primitiveData);
	virtual void Draw_SetData(const PrimitiveData& primitiveData);
	virtual void Draw_Render(const PrimitiveData& primitiveData);

	std::vector<PrimitiveData> m_primitivesData;
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_textures;

	union
	{
		ComPtr<ID3D11InputLayout> m_dxInputLayout;
		ID3D11InputLayout* m_inputLayout = nullptr;
	};

	union
	{
		ComPtr<ID3DX11EffectPass> m_dxEffectPass;
		EffectPass* m_effectPass = nullptr;
	};

private:
	const CameraComponent* m_camera = nullptr;
};
} // namespace library

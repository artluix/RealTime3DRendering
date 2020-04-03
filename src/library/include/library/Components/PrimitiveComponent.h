#pragma once
#include "library/Components/DrawableComponent.h"
#include "library/Components/SceneComponent.h"
#include "library/Render/PrimitiveData.h"
#include "library/DxForward.h"

namespace library
{
class CameraComponent;

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

	virtual ID3D11InputLayout* GetInputLayout() const = 0;

	ID3D11ShaderResourceView* GetTexture(const unsigned textureIdx) const;

protected:
	virtual void Draw_SetIA(const PrimitiveData& primitiveData);
	virtual void Draw_SetData(const PrimitiveData& primitiveData) = 0;
	virtual void Draw_Render(const PrimitiveData& primitiveData);

	DynArray<PrimitiveData> m_primitivesData;
	DynArray<ComPtr<ID3D11ShaderResourceView>> m_textures;

private:
	const CameraComponent* m_camera = nullptr;
};
} // namespace library

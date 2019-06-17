#pragma once
#include <library/Materials/DistortionMappingMaterial.h>

#include <library/Components/ConcreteMaterialPostProcessingComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/PrimitiveData.h>

namespace library
{
class TextComponent;
class CameraComponent;

class EffectPass;
} // namespace library

class DistortionMappingDemo
	: public library::ConcreteMaterialPostProcessingComponent<library::DistortionMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(DistortionMappingDemo, library::PostProcessingComponent, library::InputReceivableComponent)
public:
	enum class Mode
	{
		Fullscreen,
		Masking,
		MaskOnly
	};

	DistortionMappingDemo();
	~DistortionMappingDemo();

	void Update(const library::Time& time) override;
	void Draw(const library::Time& time) override;

	static std::string ModeToString(const Mode mode);
	static Mode NextMode(const Mode mode);

	const library::CameraComponent* GetCamera() const { return m_camera; }
	void SetCamera(const library::CameraComponent& camera) { m_camera = &camera; }

protected:
	void InitializeInternal() override;

private:
	struct Texture
	{
		enum Type : unsigned
		{
			//Default = 0,
			DistortionMap = 0,

			//# Count
			Count
		};
	};

	void UpdateDisplacementScale(const library::Time& time);
	void UpdateDistortion();
	void UpdateDistortionMask();

	void DrawMeshForDistortionCutout();

	std::unique_ptr<library::TextComponent> m_text;

	std::unique_ptr<library::FullScreenRenderTarget> m_cutoutRenderTarget;

	library::EffectPass* m_cutoutPass;

	library::PrimitiveData m_primitiveData;
	std::vector<library::ComPtr<ID3D11ShaderResourceView>> m_textures;

	float m_displacementScale;
	Mode m_mode;

	const library::CameraComponent* m_camera = nullptr;
};

#include "HdrDemo.h"

#include <library/Components/TextComponent.h>
#include <library/Components/FullScreenQuadComponent.h>

#include <sstream>

using namespace library;

HdrDemo::HdrDemo()
{
}

HdrDemo::~HdrDemo() = default;

//-------------------------------------------------------------------------

void HdrDemo::Update(const library::Time& time)
{
	m_text->Update(time);
}

void HdrDemo::Draw(const library::Time& time)
{
	m_fullScreenQuad->Draw(time);
}

//-------------------------------------------------------------------------

void HdrDemo::InitializeInternal()
{

}

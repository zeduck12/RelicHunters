#include "pch.h"
#include "UICameraManager.h"
#include "CPlayerInfo.h"

DEFINITION_SINGLETON(UICameraManager)

bool UICameraManager::Ready(void)
{
	m_pPlayerInfo = make_unique<CPlayerInfo>(L"Jimmy");
	if (!m_pPlayerInfo->Ready())
		return false;

	return true;
}

void UICameraManager::Update(void)
{
	m_pPlayerInfo->Update();
}

void UICameraManager::LateUpdate(void)
{
}

void UICameraManager::Render(void)
{
	m_pPlayerInfo->Render();
}

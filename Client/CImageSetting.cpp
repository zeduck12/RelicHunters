#include "pch.h"
#include "CObj.h"
#include "CImageSetting.h"
#include "CPlayer.h"
#include "CPlayerManager.h"
#include "CMonster.h"
#include "CAnimation.h"

CImageSetting::CImageSetting(CObj* _pOwner, const string& _rStrTag)
{
    m_pOwner = _pOwner;
    m_strTag = _rStrTag;
    m_eImageID = _pOwner->GetImageID();
}

bool CImageSetting::Ready(void)
{
    CAnimation* pAni = m_pOwner->CreateAnimation(m_strTag);
    if (pAni)
        pAni = nullptr;

    switch (m_eImageID)
    {
    case IMAGE::PLAYER:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 1.2f, 12, 0, 12, 0.f, L"Player", L"Idle", L"../Texture/Player/Idle/idle_%d.png");
        m_pOwner->AddAnimationClip("Move", ANIMATION::ONCE_RETURN, 0.6f, 6, 0, 6, 0.f, L"Player", L"Move", L"../Texture/Player/Move/move_%d.png");
        m_pOwner->AddAnimationClip("Attacked", ANIMATION::ONCE_RETURN, 0.2f, 2, 0, 2, 0.f, L"Player", L"Attacked", L"../Texture/Player/Attacked/attacked_%d.png");
        break;
    case IMAGE::DUCK:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 1.4f, 7, 0, 7, 0.f, L"Duck", L"Idle", L"../Texture/Monster/Duck/Idle/idle_%d.png");
        m_pOwner->AddAnimationClip("Move", ANIMATION::ONCE_RETURN, 1.3f, 13, 0, 13, 0.f, L"Duck", L"Move", L"../Texture/Monster/Duck/Move/move_%d.png");
        m_pOwner->AddAnimationClip("Attacked", ANIMATION::ONCE_RETURN, 0.2f, 2, 0, 2, 0.f, L"Duck", L"Attacked", L"../Texture/Monster/Duck/Attacked/attacked_%d.png");
        break;
    case IMAGE::TURTLE:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 1.2f, 12, 0, 12, 0.f, L"Turtle", L"Idle", L"../Texture/Monster/Turtle/Idle/spr_turtle_idle_%d.png");
        m_pOwner->AddAnimationClip("Move", ANIMATION::ONCE_RETURN, 0.6f, 6, 0, 6, 0.f, L"Turtle", L"Move", L"../Texture/Monster/Turtle/Move/spr_turtle_walk_%d.png");
        m_pOwner->AddAnimationClip("Attacked", ANIMATION::ONCE_RETURN, 0.2f, 2, 0, 2, 0.f, L"Turtle", L"Attacked", L"../Texture/Monster/Turtle/Attacked/spr_turtle_hit_%d.png");
        break;
    case IMAGE::KAMIKAZE:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 1.6f, 16, 0, 16, 0.f, L"Kamikaze", L"Idle", L"../Texture/Monster/Kamikaze/Idle/spr_kamikaze_idle_%d.png");
        m_pOwner->AddAnimationClip("Move", ANIMATION::ONCE_RETURN, 0.8f, 8, 0, 8, 0.f, L"Kamikaze", L"Move", L"../Texture/Monster/Kamikaze/Move/spr_kamikaze_walk_%d.png");
        m_pOwner->AddAnimationClip("Attacked", ANIMATION::ONCE_RETURN, 0.2f, 2, 0, 2, 0.f, L"Kamikaze", L"Attacked", L"../Texture/Monster/Kamikaze/Attacked/spr_kamikaze_hit_%d.png");
        break;
    case IMAGE::KAMIKAZE_FLY:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 1.6f, 16, 0, 16, 0.f, L"KamikazeFly", L"Idle", L"../Texture/Monster/KamikazeFly/Idle/spr_kamikazelite_idle_%d.png");
        m_pOwner->AddAnimationClip("Move", ANIMATION::ONCE_RETURN, 0.8f, 8, 0, 8, 0.f, L"KamikazeFly", L"Move", L"../Texture/Monster/KamikazeFly/Move/spr_kamikazelite_walk_%d.png");
        m_pOwner->AddAnimationClip("Attacked", ANIMATION::ONCE_RETURN, 0.2f, 2, 0, 2, 0.f, L"KamikazeFly", L"Attacked", L"../Texture/Monster/KamikazeFly/Attacked/spr_kamikazelite_hit_%d.png");
        m_pOwner->AddAnimationClip("Fly", ANIMATION::ONCE_RETURN, 0.9f, 9, 0, 9, 0.f, L"KamikazeFly", L"Fly", L"../Texture/Monster/KamikazeFly/Fly/spr_kamikazelite_flying_0%d.png");
        break;
    }
          
    return true;
}

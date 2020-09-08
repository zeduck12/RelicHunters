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
        // Jimmy
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 1.2f, 12, 0, 12, 0.f, L"Player", L"Idle", L"../Texture/Player/Idle/idle_%d.png");
        m_pOwner->AddAnimationClip("Move", ANIMATION::ONCE_RETURN, 0.6f, 6, 0, 6, 0.f, L"Player", L"Move", L"../Texture/Player/Move/move_%d.png");
        m_pOwner->AddAnimationClip("Attacked", ANIMATION::ONCE_RETURN, 0.2f, 2, 0, 2, 0.f, L"Player", L"Attacked", L"../Texture/Player/Attacked/attacked_%d.png");
        m_pOwner->AddAnimationClip("Death", ANIMATION::LOOP, 1.1f, 11, 0, 11, 0.f, L"Player", L"Death", L"../Texture/Player/Death/spr_jimmy_death_%d.png");
       // Pinky
        m_pOwner->AddAnimationClip("Pinky_Idle", ANIMATION::LOOP, 0.9f, 9, 0, 9, 0.f, L"Player", L"Pinky_Idle", L"../Texture/Player/Pinky_Idle/spr_pinky_idle_%d.png");
        m_pOwner->AddAnimationClip("Pinky_Move", ANIMATION::ONCE_RETURN, 0.6f, 6, 0, 6, 0.f, L"Player", L"Pinky_Move", L"../Texture/Player/Pinky_Move/spr_pinky_sprint_%d.png");
        m_pOwner->AddAnimationClip("Pinky_Attacked", ANIMATION::ONCE_RETURN, 0.2f, 2, 0, 2, 0.f, L"Player", L"Pinky_Attacked", L"../Texture/Player/Pinky_Attacked/spr_pinky_hit_%d.png");
        m_pOwner->AddAnimationClip("Pinky_Death", ANIMATION::LOOP, 1.1f, 11, 0, 11, 0.f, L"Player", L"Pinky_Death", L"../Texture/Player/Pinky_Death/spr_pinky_death_%d.png");
       // Raff
        m_pOwner->AddAnimationClip("Raff_Idle", ANIMATION::LOOP, 1.f, 10, 0, 10, 0.f, L"Player", L"Raff_Idle", L"../Texture/Player/Raff_Idle/spr_raff_idle_%d.png");
        m_pOwner->AddAnimationClip("Raff_Move", ANIMATION::ONCE_RETURN, 0.6f, 6, 0, 6, 0.f, L"Player", L"Raff_Move", L"../Texture/Player/Raff_Move/spr_raff_sprint_%d.png");
        m_pOwner->AddAnimationClip("Raff_Attacked", ANIMATION::ONCE_RETURN, 0.2f, 2, 0, 2, 0.f, L"Player", L"Raff_Attacked", L"../Texture/Player/Raff_Attacked/spr_raff_hit_%d.png");
        m_pOwner->AddAnimationClip("Raff_Death", ANIMATION::LOOP, 1.1f, 11, 0, 11, 0.f, L"Player", L"Raff_Death", L"../Texture/Player/Raff_Death/spr_raff_death_%d.png");
        break;
    case IMAGE::DUCK:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 1.4f, 7, 0, 7, 0.f, L"Duck", L"Idle", L"../Texture/Monster/Duck/Idle/idle_%d.png");
        m_pOwner->AddAnimationClip("Move", ANIMATION::ONCE_RETURN, 1.3f, 13, 0, 13, 0.f, L"Duck", L"Move", L"../Texture/Monster/Duck/Move/move_%d.png");
        m_pOwner->AddAnimationClip("Attacked", ANIMATION::ONCE_RETURN, 0.2f, 2, 0, 2, 0.f, L"Duck", L"Attacked", L"../Texture/Monster/Duck/Attacked/attacked_%d.png");
        m_pOwner->AddAnimationClip("Death", ANIMATION::ONCE_RETURN, 1.2f, 12, 0, 12, 0.f, L"Duck", L"Death", L"../Texture/Monster/Duck/Death/spr_duck_death_%d.png");
        break;
    case IMAGE::TURTLE:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 1.2f, 12, 0, 12, 0.f, L"Turtle", L"Idle", L"../Texture/Monster/Turtle/Idle/spr_turtle_idle_%d.png");
        m_pOwner->AddAnimationClip("Move", ANIMATION::ONCE_RETURN, 0.6f, 6, 0, 6, 0.f, L"Turtle", L"Move", L"../Texture/Monster/Turtle/Move/spr_turtle_walk_%d.png");
        m_pOwner->AddAnimationClip("Attacked", ANIMATION::ONCE_RETURN, 0.2f, 2, 0, 2, 0.f, L"Turtle", L"Attacked", L"../Texture/Monster/Turtle/Attacked/spr_turtle_hit_%d.png");
        m_pOwner->AddAnimationClip("Death", ANIMATION::ONCE_RETURN, 3.f, 13, 0, 13, 0.f, L"Turtle", L"Death", L"../Texture/Monster/Turtle/Death/spr_turtle_death_%d.png");
        break;
    case IMAGE::KAMIKAZE:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 1.6f, 16, 0, 16, 0.f, L"Kamikaze", L"Idle", L"../Texture/Monster/Kamikaze/Idle/spr_kamikaze_idle_%d.png");
        m_pOwner->AddAnimationClip("Move", ANIMATION::ONCE_RETURN, 0.8f, 8, 0, 8, 0.f, L"Kamikaze", L"Move", L"../Texture/Monster/Kamikaze/Move/spr_kamikaze_walk_%d.png");
        m_pOwner->AddAnimationClip("Attacked", ANIMATION::ONCE_RETURN, 0.2f, 2, 0, 2, 0.f, L"Kamikaze", L"Attacked", L"../Texture/Monster/Kamikaze/Attacked/spr_kamikaze_hit_%d.png");
        m_pOwner->AddAnimationClip("Death", ANIMATION::ONCE_RETURN, 3.f, 13, 0, 13, 0.f, L"Kamikaze", L"Death", L"../Texture/Monster/Kamikaze/Death/spr_kamikaze_death_%d.png");
        break;
    case IMAGE::KAMIKAZE_FLY:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 1.6f, 16, 0, 16, 0.f, L"KamikazeFly", L"Idle", L"../Texture/Monster/KamikazeFly/Idle/spr_kamikazelite_idle_%d.png");
        m_pOwner->AddAnimationClip("Move", ANIMATION::ONCE_RETURN, 0.8f, 8, 0, 8, 0.f, L"KamikazeFly", L"Move", L"../Texture/Monster/KamikazeFly/Move/spr_kamikazelite_walk_%d.png");
        m_pOwner->AddAnimationClip("Attacked", ANIMATION::ONCE_RETURN, 0.2f, 2, 0, 2, 0.f, L"KamikazeFly", L"Attacked", L"../Texture/Monster/KamikazeFly/Attacked/spr_kamikazelite_hit_%d.png");
        m_pOwner->AddAnimationClip("Fly", ANIMATION::ONCE_RETURN, 0.9f, 9, 0, 9, 0.f, L"KamikazeFly", L"Fly", L"../Texture/Monster/KamikazeFly/Fly/spr_kamikazelite_flying_%d.png");
        m_pOwner->AddAnimationClip("Death", ANIMATION::ONCE_RETURN, 3.f, 16, 0, 16, 0.f, L"KamikazeFly", L"Death", L"../Texture/Monster/KamikazeFly/Death/spr_kamikazelite_death1_%d.png");
        break;
    case IMAGE::BOSS:
        m_pOwner->AddAnimationClip("EggIdle", ANIMATION::LOOP, 1.f, 1, 0, 1, 0.f, L"Boss", L"EggIdle", L"../Texture/Monster/Boss/Egg/Idle/spr_boss_egg_idle_%d.png");
        m_pOwner->AddAnimationClip("EggStart", ANIMATION::LOOP, 2.f, 10, 0, 10, 0.f, L"Boss", L"EggStart", L"../Texture/Monster/Boss/Egg/Start/spr_boss_egg_start_%d.png");
        m_pOwner->AddAnimationClip("EggCrack", ANIMATION::LOOP, 4.1f, 21, 0, 21, 0.f, L"Boss", L"EggCrack", L"../Texture/Monster/Boss/Egg/Crack/spr_boss_egg_crack_%d.png");
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 1.4f, 14, 0, 14, 0.f, L"Boss", L"Idle", L"../Texture/Monster/Boss/Idle/spr_boss_idle_%d.png");
        m_pOwner->AddAnimationClip("Move", ANIMATION::ONCE_RETURN, 0.6f, 6, 0, 6, 0.f, L"Boss", L"Move", L"../Texture/Monster/Boss/Move/spr_boss_walk_%d.png");
        m_pOwner->AddAnimationClip("Attacked", ANIMATION::ONCE_RETURN, 0.2f, 2, 0, 2, 0.f, L"Boss", L"Attacked", L"../Texture/Monster/Boss/Attacked/spr_boss_hit_%d.png");
        m_pOwner->AddAnimationClip("Death", ANIMATION::ONCE_RETURN, 1.3f, 13, 0, 13, 0.f, L"Boss", L"Death", L"../Texture/Monster/Boss/Death/spr_boss_death_%d.png");
        break;
    case IMAGE::PICKUP_LIGHT:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 0.1f, 1, 0, 1, 0.f, L"PickUpLight", L"Start", L"../Texture/Item/PickUp/Light/spr_pickup_light_%d.png");
        m_pOwner->AddAnimationClip("Start", ANIMATION::LOOP, 1.6f, 16, 0, 16, 0.f, L"PickUpLight", L"Start", L"../Texture/Item/PickUp/Light/spr_pickup_light_%d.png");     
        break;
    case IMAGE::PICKUP_MEDIUM:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 0.1f, 1, 0, 1, 0.f, L"PickUpMedium", L"Start", L"../Texture/Item/PickUp/Medium/spr_pickup_medium_%d.png");
        m_pOwner->AddAnimationClip("Start", ANIMATION::LOOP, 1.6f, 16, 0, 16, 0.f, L"PickUpMedium", L"Start", L"../Texture/Item/PickUp/Medium/spr_pickup_medium_%d.png");
        break;
    case IMAGE::PICKUP_HEAVY:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 0.1f, 1, 0, 1, 0.f, L"PickUpHeavy", L"Start", L"../Texture/Item/PickUp/Heavy/spr_pickup_heavy_%d.png");
        m_pOwner->AddAnimationClip("Start", ANIMATION::LOOP, 1.6f, 16, 0, 16, 0.f, L"PickUpHeavy", L"Start", L"../Texture/Item/PickUp/Heavy/spr_pickup_heavy_%d.png");
        break;
    case IMAGE::PICKUP_GRENADE:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 0.1f, 1, 0, 1, 0.f, L"PickUpGrenade", L"Start", L"../Texture/Item/PickUp/Grenade/spr_pickup_grenade_%d.png");
        m_pOwner->AddAnimationClip("Start", ANIMATION::LOOP, 1.7f, 17, 0, 17, 0.f, L"PickUpGrenade", L"Start", L"../Texture/Item/PickUp/Grenade/spr_pickup_grenade_%d.png");
        break;
    case IMAGE::COIN:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 1.4f, 14, 0, 14, 0.f, L"Coin", L"Idle", L"../Texture/Coin/Idle/spr_coin_ground_%d.png");
        m_pOwner->AddAnimationClip("Drop", ANIMATION::LOOP, 1.f, 5, 0, 5, 0.f, L"Coin", L"Drop", L"../Texture/Coin/Drop/spr_coin_drop_%d.png");
        break;
    case IMAGE::TELEPORTER:
        m_pOwner->AddAnimationClip("Idle", ANIMATION::LOOP, 1.f, 1, 0, 1, 0.f, L"Teleporter", L"Idle", L"../Texture/Teleporter/Idle/Idle_%d.png");
        m_pOwner->AddAnimationClip("Spawn", ANIMATION::LOOP, 1.1f, 11, 0, 11, 0.f, L"Teleporter", L"Spawn", L"../Texture/Teleporter/Spawn/spr_teleporter_spawn_%d.png");
        m_pOwner->AddAnimationClip("Effect", ANIMATION::LOOP, 1.7f, 17, 0, 17, 0.f, L"Teleporter", L"Effect", L"../Texture/Teleporter/Effect/spr_teleporter_fx_%d.png");
        break;
    }
          
    return true;
}

#pragma once
class CEnemyManager
{
public:
	explicit CEnemyManager() = default;
	~CEnemyManager() = default;

	// 나중에 스테이지마다 필요한 몬스터 정보 셋팅 가능하게 하기.
	// 필요한 몬스터들을 셋팅하는 클래스
	static bool LoadMonsterData();
};


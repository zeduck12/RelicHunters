#pragma once
class CEnemyManager
{
public:
	explicit CEnemyManager() = default;
	~CEnemyManager() = default;

	// ���߿� ������������ �ʿ��� ���� ���� ���� �����ϰ� �ϱ�.
	// �ʿ��� ���͵��� �����ϴ� Ŭ����
	static bool LoadMonsterData();
};


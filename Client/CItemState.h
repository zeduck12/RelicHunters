#pragma once

class CItem;
class CItemState
{
public:
	CItemState() = default;
	virtual ~CItemState() {}

public:
	virtual CItemState* Update(CItem* _pMonster) = 0;
	virtual void Render(CItem* _pMonster)	     = 0;

protected:
	float m_fCoolTime = 0.f;
	float m_fStackTime = 0.f;
};


class CItemIdleState : public CItemState
{
public:
	explicit CItemIdleState() = default;
	virtual ~CItemIdleState() = default;

public:
	// CItemState을(를) 통해 상속됨
	virtual CItemState* Update(CItem* _pItem) override;
	virtual void Render(CItem* _pItem)		 override;
};

class CItemStartState : public CItemState
{
public:
	explicit CItemStartState() = default;
	virtual ~CItemStartState() = default;

public:
	// CItemState을(를) 통해 상속됨
	virtual CItemState* Update(CItem * _pItem) override;
	virtual void Render(CItem * _pItem)		 override;
};


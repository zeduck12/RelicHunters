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

private:
	float m_fAddSpeed = 0.f;
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

class CItemDropState : public CItemState
{
public:
	explicit CItemDropState() = default;
	virtual ~CItemDropState() = default;

public:
	// CItemState을(를) 통해 상속됨
	virtual CItemState* Update(CItem * _pItem) override;
	virtual void Render(CItem * _pItem)		 override;

public:
	void ActiveGravity(void);
private:
	float m_fGravity = 0.f;

};

class CItemSpawnState : public CItemState
{
public:
	explicit CItemSpawnState() = default;
	virtual ~CItemSpawnState() = default;

public:
	// CItemState을(를) 통해 상속됨
	virtual CItemState* Update(CItem * _pItem) override;
	virtual void Render(CItem * _pItem)		 override;

};
#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Client)

class CItem abstract : public CLandObject
{
public:
	typedef struct tagItemDesc : public LANDOBJECT_DESC
	{
		ITEM			eItemType = { ITEM::_END };
		_float			fDurability = { 0.f };
		_float			fAttackDamage = { 0.f };
	}ITEM_DESC;

protected:
	CItem(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CItem(const CItem& _rhs);
	virtual ~CItem() = default;

public:
	virtual HRESULT			Initialize_Prototype()			override;
	virtual HRESULT			Initialize(void* pArg)			override;

public:
	const ITEM&				ItemType() const;

public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free();

protected:
	void					BillBoard_Camera();

private:
	CGameObject*			Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection = nullptr) = 0;

protected:
	ITEM					m_eItemType = { ITEM::_END };			/* 아이템 타입 */

	_float					m_fDurability = { 0.f };				/* 아이템 내구도 */
	_float					m_fAttackDamage = { 0.f };				/* 아이템 공격력 */
};

END
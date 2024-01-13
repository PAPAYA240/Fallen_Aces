#pragma once

#include "CItem.h"

class CHealItem abstract : public CItem
{
public:
	typedef struct tagHealItem : public ITEM_DESC
	{
		_float		fRecorvery_Buff = { 0.f };
		_float		fRBuff_LimitTime = { 0.f };

		_float		fSpeed_Buff = { 0.f };
		_float		fSBuff_LimitTime = { 0.f };
	}HEAL_ITEM_DESC;

protected:
	CHealItem(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CHealItem(const CHealItem& _rhs);
	virtual ~CHealItem() = default;

public:
	virtual HRESULT			Initialize_Prototype()			override;
	virtual HRESULT			Initialize(void* pArg)			override;

public:
	const ITEM& ItemType() const;

public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free();

private:
	CGameObject*			Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection = nullptr) = 0;

protected:
	_float					m_fRecorvery_Buff = { 0.f };
	_float					m_fSpeed_Buff = { 0.f };

	_float					m_fRBuff_LimitTime = { 0.f };
	_float					m_fSBuff_LimitTime = { 0.f };
};


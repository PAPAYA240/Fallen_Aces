#include "stdafx.h"
#include "CHealItem.h"

CHealItem::CHealItem(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CItem(_pGraphic_Device)
{
}

CHealItem::CHealItem(const CHealItem& _rhs)
	: CItem(_rhs)
{
}

HRESULT CHealItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHealItem::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		HEAL_ITEM_DESC* pDesc = (HEAL_ITEM_DESC*)pArg;

		m_fRBuff_LimitTime = pDesc->fRBuff_LimitTime;
		m_fRecorvery_Buff = pDesc->fRecorvery_Buff;

		m_fSBuff_LimitTime = pDesc->fSBuff_LimitTime;
		m_fSpeed_Buff = pDesc->fSpeed_Buff;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

const ITEM& CHealItem::ItemType() const
{
	return m_eItemType;
}

void CHealItem::Free()
{
	__super::Free();
}

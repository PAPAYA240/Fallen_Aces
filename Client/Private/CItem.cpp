#include "stdafx.h"
#include "CItem.h"

CItem::CItem(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CLandObject(_pGraphic_Device)
{
}

CItem::CItem(const CItem& _rhs)
	: CLandObject(_rhs)
{
}

HRESULT CItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		ITEM_DESC* pDesc = (ITEM_DESC*)pArg;

		m_fAttackDamage = pDesc->fAttackDamage;
		m_fDurability = pDesc->fDurability;
		m_eItemType = pDesc->eItemType;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

const ITEM& CItem::ItemType() const
{
	// TODO: 여기에 return 문을 삽입합니다.

	return ITEM::_END;
}

void CItem::Free()
{
	__super::Free();
}

void CItem::BillBoard_Camera()
{
	_float3 vScale = m_pTransformCom->Get_Scaled();

	_float4x4 matView = {};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);

	_float4x4 matViewInv = {};
	D3DXMatrixInverse(&matViewInv, nullptr, &matView);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)& matViewInv.m[0][0] * vScale.x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&matViewInv.m[1][0] * vScale.y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&matViewInv.m[2][0] * vScale.z);
}

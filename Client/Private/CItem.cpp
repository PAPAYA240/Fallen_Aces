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

void CItem::Free()
{
	__super::Free();

	Safe_Release<CTexture*>(m_pTextureCom);
	Safe_Release<CVIBuffer_Rect*>(m_pVIbufferCom);
}

void CItem::BillBoard_Camera()
{
	_float3 vScale = m_pTransformCom->Get_Scaled();

	_float4x4 matView = {};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);

	_float4x4 matViewInv = {};
	D3DXMatrixInverse(&matViewInv, nullptr, &matView);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)& matViewInv.m[0][0] * vScale.x);

	if(XYZ == m_eBillbordType)
		m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&matViewInv.m[1][0] * vScale.y);

	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&matViewInv.m[2][0] * vScale.z);
}

void CItem::Check_Durability()
{
	if (nullptr == m_pTextureCom)
		return;

	if (100.f >= m_fDurability && 50.f < m_fDurability)
		m_eDuraType = ITEM_DURA::PERFECT;
	else if (50.f >= m_fDurability && 0.f < m_fDurability)
		m_eDuraType = ITEM_DURA::GOOD;
	else if(0.f >= m_fDurability)
		m_eDuraType = ITEM_DURA::BROKEN;

	switch (_uint(m_eItemType))
	{
	case _uint(ITEM::APPLE):
	{
		if (ITEM_DURA::GOOD == m_eDuraType)
			m_pTextureCom->Change_Container(TEXT("Item"), TEXT("Apple"));
		else if (ITEM_DURA::BROKEN == m_eDuraType)
			m_pTextureCom->Change_Container(TEXT("Item"), TEXT("Apple_Eat"));
	}
		break;
	case _uint(ITEM::BANANA):
	{
		if (ITEM_DURA::GOOD == m_eDuraType)
			m_pTextureCom->Change_Container(TEXT("Item"), TEXT("Banana"));
		else if (ITEM_DURA::BROKEN == m_eDuraType)
			m_pTextureCom->Change_Container(TEXT("Item"), TEXT("Banana_Eat"));
	}
	break;
	}
}

void CItem::Check_PlayerRadius()
{

}

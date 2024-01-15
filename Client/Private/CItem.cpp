#include "stdafx.h"
#include "CItem.h"

#include "Player.h"

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
		m_eItemID = pDesc->eItemID;
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
	if (nullptr == m_pTextureCom || ITEM_DURA::BROKEN == m_eDuraType)
		return;

	if (100.f >= m_fDurability && 50.f < m_fDurability)
		m_eDuraType = ITEM_DURA::PERFECT;
	else if (50.f >= m_fDurability && 0.f < m_fDurability)
		m_eDuraType = ITEM_DURA::GOOD;
	else if(0.f >= m_fDurability)
		m_eDuraType = ITEM_DURA::BROKEN;

	switch (_uint(m_eItemID))
	{
	case _uint(ITEM_ID::APPLE):
	{
		if (ITEM_DURA::GOOD == m_eDuraType)
			m_pTextureCom->Change_Container(TEXT("Item"), TEXT("Apple"));
		else if (ITEM_DURA::BROKEN == m_eDuraType)
			m_pTextureCom->Change_Container(TEXT("Item"), TEXT("Apple_Eat"));
	}
		break;
	case _uint(ITEM_ID::BANANA):
	{
		if (ITEM_DURA::GOOD == m_eDuraType)
			m_pTextureCom->Change_Container(TEXT("Item"), TEXT("Banana"));
		else if (ITEM_DURA::BROKEN == m_eDuraType)
			m_pTextureCom->Change_Container(TEXT("Item"), TEXT("Banana_Eat"));
	}
	break;

	case _uint(ITEM_ID::MEDKIT):
	{
		// °´Ã¼ ÆÄ±«
		if (ITEM_DURA::GOOD == m_eDuraType)
			m_pTextureCom->Change_Container(TEXT("Item"), TEXT("Medkit"));
	}
	break;

	case _uint(ITEM_ID::COLA):
	{
		// °´Ã¼ ÆÄ±«
		if (ITEM_DURA::GOOD == m_eDuraType)
			m_pTextureCom->Change_Container(TEXT("Item"), TEXT("Cola"));
	}
	break;
	}
}

void CItem::Check_PlayerRadius()
{
	const CTransform* pPlayerTransform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), g_strTransformTag));

	list<class CGameObject*>* pPlayerLayerList = m_pGameInstance->Get_LayerList(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
	CGameObject* pGameObject = *(pPlayerLayerList->begin());

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);

	if (nullptr == pPlayerTransform)
		return;

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDiff = vPlayerPos - vMyPos;

	_float fDistance = D3DXVec3Length(&vDiff);

	if (abs(fDistance) <= 1.0f)
	{
		if (DOWN == m_pGameInstance->Get_KeyState('E'))
		{
			if (ITEM_DURA::BROKEN != m_eDuraType)
			{
				if (ITEM_ID::APPLE == m_eItemID || ITEM_ID::BANANA == m_eItemID || ITEM_ID::COLA == m_eItemID || ITEM_ID::MEDKIT == m_eItemID)
					m_fDurability -= 1;
			}
		}

		if (DOWN == m_pGameInstance->Get_KeyState('R') && !m_isTestDead)
		{
			// ÇÃ·¹ÀÌ¾î ÇÔ¼ö¸¦ »ç¿ëÇÏ¿© °´Ã¼ÀÇ Á¤º¸¸¦ ³Ñ±è
			// ÀÌ °´Ã¼´Â »èÁ¦
			pPlayer->Input_Item(this);
			m_isTestDead = true;
		}
	}
}

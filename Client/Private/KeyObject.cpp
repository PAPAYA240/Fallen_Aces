#include "stdafx.h"
#include "KeyObject.h"
#include "Player.h"

CKeyObject::CKeyObject(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CLandObject(_pGraphic_Device)
{
}

CKeyObject::CKeyObject(const CKeyObject& _rhs)
	: CLandObject(_rhs)
{
}

HRESULT CKeyObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKeyObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
 
	return S_OK;
}

void CKeyObject::Key_Control()
{
	const CTransform* pPlayerTransform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), g_strTransformTag));
	
	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDiff = vMyPos - vPlayerPos;

	_float fDistance = D3DXVec3Length(&vDiff);

	if (abs(fDistance) <= 2.0f)
	{
		if (DOWN == m_pGameInstance->Get_KeyState('E'))
		{
			list<class CGameObject*>* pPlayerLayerList = m_pGameInstance->Get_LayerList(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
			CGameObject* pGameObject = *(pPlayerLayerList->begin());
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);

			if (pPlayer->Get_Key(this))
				m_bGetKey = true;
		}
	}
}

void CKeyObject::BillBoard_Camera()
{
	_float3 vScale = m_pTransformCom->Get_Scaled();

	_float4x4 matView = {};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);

	_float4x4 matViewInv = {};
	D3DXMatrixInverse(&matViewInv, nullptr, &matView);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&matViewInv.m[0][0] * vScale.x);

	if (CItem::XYZ == m_eBillbordType)
		m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&matViewInv.m[1][0] * vScale.y);

	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&matViewInv.m[2][0] * vScale.z);
}


void CKeyObject::Free()
{
	Safe_Release<CTexture*>(m_pTextureCom);
	Safe_Release<CVIBuffer_Rect*>(m_pVIbufferCom);

	__super::Free();
}

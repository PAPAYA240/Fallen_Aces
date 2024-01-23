#include "stdafx.h"
#include "DoorObj.h"
#include "Player.h"

CDoorObj::CDoorObj(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CGameObject(_pGraphic_Device)
{
}

CDoorObj::CDoorObj(const CDoorObj& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CDoorObj::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDoorObj::Initialize(void* pArg)	
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CDoorObj::UnLockAnimation(_float fTimeDelta)
{
	// 열고 닫기
	if(!m_bPos)
	{
		m_bPos = true;

		const CTransform* pPlayerTransform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), g_strTransformTag));

		_float3 pPlayerLook = pPlayerTransform->Get_State(CTransform::STATE_LOOK);
		_float3 pDoorLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		D3DXVec3Normalize(&pPlayerLook, &pPlayerLook);
		D3DXVec3Normalize(&pDoorLook, &pDoorLook);

		m_vPlayerPos = D3DXVec3Dot(&pPlayerLook, &pDoorLook);
	}

	m_bDoorTime += fTimeDelta;

	if(m_bDoorTime < 1.f)
	{
		// 내적이 음수일 때에 +
		if (m_vPlayerPos <= 0.f)
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(fTimeDelta * 60.f));

		// 양수일 때 -
		if (m_vPlayerPos > 0.f)
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), D3DXToRadian(fTimeDelta * -60.f));
	}
	else
	{
		m_bDoorTime = 0.0f;
		m_bDoorRot = false;
		m_bPos = false;
	}
}


void CDoorObj::Free()
{
	Safe_Release<CTexture*>(m_pTextureCom);
	Safe_Release<CVIBuffer_Rect*>(m_pVIbufferCom);

	__super::Free();
}

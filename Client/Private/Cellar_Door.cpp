#include "stdafx.h"
#include "Cellar_Door.h"
#include "Number.h"

CCellar_Door::CCellar_Door(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CDoorObj(_pGraphic_Device)
{
}

CCellar_Door::CCellar_Door(const CCellar_Door& rhs)
	: CDoorObj(rhs)
{
}

HRESULT CCellar_Door::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCellar_Door::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC  MyDesc = {};
	MyDesc.fRotationPerSec = D3DXToRadian(90.0f);
	MyDesc.fSpeedPerSec = 2.f;

	if (nullptr != pArg)
		CGameObject::GAMEOBJECT_DESC* pDesc = (CGameObject::GAMEOBJECT_DESC*)pArg;
	

	if (FAILED(__super::Initialize(&MyDesc)))
		return E_FAIL;

	if (FAILED(Add_MyComponents()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(4.f, 4.f, 4.f));
	m_pTransformCom->Set_Scaled(1.f, 1.f, 1.f);

	m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Cellar_Door"));

	return S_OK;
}

void CCellar_Door::Tick(_float fTimeDelta)
{
	// 만약 좌물쇠가 열리면 움직임 가능
	UnLockingControl(fTimeDelta);
}

void CCellar_Door::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CCellar_Door::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0, 0)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	if (FAILED(m_pVIbufferCom->Render()))
		return E_FAIL;

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCellar_Door::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCellar_Door::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CCellar_Door::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CCellar_Door::UnLockingControl(_float fTimeDelta)
{
	const CTransform* pPlayerTransform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), g_strTransformTag));

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDiff = vMyPos - vPlayerPos;

	_float fDistance = D3DXVec3Length(&vDiff);

	if (abs(fDistance) <= 2.0f )
	{
		CGameObject* p = m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Number"), 0);

		if (DOWN == m_pGameInstance->Get_KeyState('E') && dynamic_cast<CNumber*>(p)->Get_Correct())
		{
			m_bOpenDoor = true;
			m_bPos = false;
		}

		// 만약 문이 열린 상태라면
		if (m_bOpenDoor)
		{
			if (DOWN == m_pGameInstance->Get_KeyState('E') || m_bDoorRot)
			{
				m_bDoorRot = true;
				UnLockAnimation(fTimeDelta);
			}
		}
	}

}

CCellar_Door* CCellar_Door::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCellar_Door* pInstance = new CCellar_Door(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCellar_Door"));

		Safe_Release<CCellar_Door*>(pInstance);
	}

	return pInstance;
}

CGameObject* CCellar_Door::Clone(void* pArg)
{
	CCellar_Door* pInstance = new CCellar_Door(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCellar_Door Clone "));

		Safe_Release<CCellar_Door*>(pInstance);
	}
	return pInstance;
}

CGameObject* CCellar_Door::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	return nullptr;
}

void CCellar_Door::Free()
{
	__super::Free();

}

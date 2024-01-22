#include "stdafx.h"
#include "Red_Door.h"
#include "Player.h"
#include "KeyObject.h"

CRed_Door::CRed_Door(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CDoorObj(_pGraphic_Device)
{
}

CRed_Door::CRed_Door(const CRed_Door& rhs)
	: CDoorObj(rhs)
{
}

HRESULT CRed_Door::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRed_Door::Initialize(void* pArg)
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

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(7.f, 3.f, 3.f));
	m_pTransformCom->Set_Scaled(1.f, 1.f, 1.f);

	m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("Red_Door"));

	m_ePuzzleID = PUZZLE_ID::COLOR_KEY;
	m_eColorKey = CKeyObject::COLOR::RED;

	return S_OK;
}

void CRed_Door::Tick(_float fTimeDelta)
{
	// 만약 좌물쇠가 열리면 움직임 가능
	UnLockingControl(fTimeDelta);

	if (m_bOpenDoor)
	{
		if (DOWN == m_pGameInstance->Get_KeyState('E') || m_bDoorRot)
		{
			m_bDoorRot = true;
			UnLockAnimation(fTimeDelta);
		}
	}
}

void CRed_Door::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CRed_Door::Render()
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

HRESULT CRed_Door::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRed_Door::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CRed_Door::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CRed_Door::UnLockingControl(_float fTimeDelta)
{
	const CTransform* pPlayerTransform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), g_strTransformTag));

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDiff = vMyPos - vPlayerPos;

	_float fDistance = D3DXVec3Length(&vDiff);

	if (abs(fDistance) <= 3.0f)
	{
		list<class CGameObject*>* pPlayerLayerList = m_pGameInstance->Get_LayerList(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
		CGameObject* pGameObject = *(pPlayerLayerList->begin());
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);

		for (auto iter = pPlayer->Get_Key_Address()->begin(); iter != pPlayer->Get_Key_Address()->end();)
		{
			if (*iter != nullptr && (*iter)->Get_Key() == m_ePuzzleID
				&& (*iter)->Get_Color() == m_eColorKey)
			{
				if (DOWN == m_pGameInstance->Get_KeyState('E'))
				{
					m_bOpenDoor = true;
					iter = pPlayer->Get_Key_Address()->erase(iter);
				}
				else
					++iter;
			}
			else
				++iter;
		}
	}
}

CRed_Door* CRed_Door::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRed_Door* pInstance = new CRed_Door(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CRed_Door"));

		Safe_Release<CRed_Door*>(pInstance);
	}

	return pInstance;
}

CGameObject* CRed_Door::Clone(void* pArg)
{
	CRed_Door* pInstance = new CRed_Door(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CRed_Door Clone "));

		Safe_Release<CRed_Door*>(pInstance);
	}
	return pInstance;
}

CGameObject* CRed_Door::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	return nullptr;
}

void CRed_Door::Free()
{
	__super::Free();

}

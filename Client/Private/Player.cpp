#include "stdafx.h"
#include "..\Public\Player.h"

#include "GameInstance.h"
#include "Player_LH.h"
#include "Player_RH.h"

#include "Collider_Box.h"
#include "Collider_Sphere.h"

#include "Player_UI_Manager.h"

#include "CItem.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject(pGraphic_Device)
{

}

CPlayer::CPlayer(const CPlayer & rhs)
	: CLandObject(rhs)
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{	
	CGameObject::GAMEOBJECT_DESC*	pGameObjectDesc = (CGameObject::GAMEOBJECT_DESC*)pArg;
	pGameObjectDesc->fSpeedPerSec = 10.f;
	pGameObjectDesc->fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.0f, 4.5f, 0.f));

	Ready_Layer_Player_Hands(TEXT("Layer_Player_Hand"));

	// Player UI들을 총괄 생성해줄 객체 생성
	CPlayer_UI_Manager::Player_UI_Manager_DESC Desc = {};
	Desc.pPlayer = this;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Player_UI"), TEXT("Prototype_GameObject_Player_UI_Manager"), &Desc)))
		return E_FAIL;

	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	if (m_iHp < 500)
	{
		m_iHp++;
	}

	// 장비 아이템 UI 테스트용 Item 생성문
	if (m_pGameInstance->Get_KeyState(VK_LBUTTON) == EKeyState::DOWN)
	{
		if (nullptr == m_pEquipment)
		{
			/* 랜드오브젝트용 객체들에게 필요한 데이터를 구한다.*/
			CLandObject::LANDOBJECT_DESC		LandObjectDesc = {};

			LandObjectDesc.pTerrainTransform = (CTransform*)(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), g_strTransformTag));
			LandObjectDesc.pTerrainVIBuffer = (CVIBuffer_Terrain*)(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));

			m_pGameInstance->Add_Clone((CGameObject**)&m_pEquipment, LEVEL_GAMEPLAY, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Banana"), &LandObjectDesc);
				
		}
		else if(nullptr != m_pEquipment)
		{
			if(1 == Safe_Release(m_pEquipment))
				m_pEquipment = nullptr;
		}
	}


	//if (m_pGameInstance->Get_KeyState('W') == EKeyState::PRESSING)
	//{
	//	//if (m_pGameInstance->Get_KeyState(VK_SHIFT) == EKeyState::DOWN)
	//	//	//m_pTransformCom->Set_Speed(15.f);
	//	//if (m_pGameInstance->Get_KeyState(VK_SHIFT) == EKeyState::UP)
	//	//	//m_pTransformCom->Set_Speed(10.f);

	//	m_pTransformCom->Go_Straight(fTimeDelta);
	//}
	////if (m_pGameInstance->Get_KeyState('W') == EKeyState::UP)
	////{
	////	//m_pTransformCom->Set_Speed(10.f);
	////}


	//if (m_pGameInstance->Get_KeyState('S') == EKeyState::PRESSING)
	//{
	//	m_pTransformCom->Go_Backward(fTimeDelta);
	//}

	//if (m_pGameInstance->Get_KeyState('A') == EKeyState::PRESSING)
	//{
	//	//if (m_pGameInstance->Get_KeyState(VK_SHIFT) == EKeyState::DOWN)
	//	//	//m_pTransformCom->Set_Speed(30.f);
	//	//if (m_pGameInstance->Get_KeyState(VK_SHIFT) == EKeyState::UP)
	//	//	//m_pTransformCom->Set_Speed(10.f);

	//	m_pTransformCom->Go_Left(fTimeDelta);
	//}

	//if (m_pGameInstance->Get_KeyState('D') == EKeyState::PRESSING)
	//{
	//	//if (m_pGameInstance->Get_KeyState(VK_SHIFT) == EKeyState::DOWN)
	//	//	//m_pTransformCom->Set_Speed(30.f);
	//	//if (m_pGameInstance->Get_KeyState(VK_SHIFT) == EKeyState::UP)
	//	//	//m_pTransformCom->Set_Speed(10.f);

	//	m_pTransformCom->Go_Right(fTimeDelta);
	//}



	if (FAILED(SetUp_OnTerrain(0.5f)))
		return;

	/*if (m_pGameInstance->Get_KeyState(VK_LBUTTON) == EKeyState::DOWN)
	{
		m_bLeftPunch ? m_pPlayer_LH->Set_State(STATE_ATTACK) : m_pPlayer_RH->Set_State(STATE_ATTACK);

		m_bLeftPunch = !m_bLeftPunch;
	}*/

	/*if (m_pGameInstance->Get_KeyState(VK_RBUTTON) == EKeyState::DOWN)
	{
		_uint LCnt = m_pPlayer_LH->m_iAttackCnt;
		_uint RCnt = m_pPlayer_RH->m_iAttackCnt;

		_int a = 10;
	}*/
}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		m_isMove = true;
		m_vTargetPos = Compute_MovePos();

		// m_pVIBufferCom->Compute_Picking(m_pTransformCom);
	}

	if(true == m_isMove)
		m_isMove = !m_pTransformCom->MoveToTarget(m_vTargetPos, fTimeDelta, 0.5f);
	
	/* TODO: 나중에 지울 것 */
	/* 콜리전 테스트 함수 */
	Check_Collision(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CPlayer::Render()
{	
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;
	
	if (FAILED(m_pTextureCom->Bind_Texture(0, 0)))
		return E_FAIL;	

	if (FAILED(Set_RenderState()))
		return E_FAIL;		

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

CGameObject* CPlayer::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	list<CGameObject*>* pObjectList = m_pGameInstance->Get_LayerList(eLevel, strLayerTag);

	if (nullptr == pObjectList)
		return nullptr;

	for (auto pObject : *pObjectList)
	{
		const CTransform* pTargetObjectTransform = dynamic_cast<const CTransform*>(pObject->Get_Component(g_strTransformTag));

		if (nullptr == pTargetObjectTransform)
			return nullptr;

		if (nullptr == pDirection)
		{
			_float3 vDirection;
			if (dynamic_cast<CCollider_Sphere*>(m_pColliderCom)->Check_Sphere(pTargetObjectTransform, &vDirection))
			{
				dynamic_cast<CLandObject*>(pObject)->Add_Collision(this, COLLISION_EVENT::EV_OVERLAP, vDirection);
				return pObject;
			}
		}

		else
			if (dynamic_cast<CCollider_Sphere*>(m_pColliderCom)->Check_Sphere(pTargetObjectTransform, pDirection))
			{
				dynamic_cast<CLandObject*>(pObject)->Add_Collision(this, COLLISION_EVENT::EV_OVERLAP, *pDirection);
				return pObject;
			}
	}

	return nullptr;
}

HRESULT CPlayer::Add_Components()
{	
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	/* For.Com_Collider */
	CCollider::COLLIDER_DESC ColliderDesc = {};
	ColliderDesc.pObjectTransform = m_pTransformCom;

	/*if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Box"),
		TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;*/

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Set_RenderState()
{	
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);	

	/* 알파테스트를 한다. (그려지는 픽셀들의 알파를 검사하여 그릴지 안그릴지를 판단한다.) */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	/* 내가 설정한 알파는 0이다. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);

	/* 그려지는 픽세르이 알파가 내가 설정한 알파보다 클때만 그려라. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CPlayer::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

_float3 CPlayer::Compute_MovePos()
{
	const CVIBuffer_Terrain*	pTerrainBuffer = dynamic_cast<const CVIBuffer_Terrain*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));
	if (nullptr == pTerrainBuffer)
		return _float3(0.f, 0.f, 0.f);

	const CTransform*	pTerrainTransform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_Transform")));
	if (nullptr == pTerrainTransform)
		return _float3(0.f, 0.f, 0.f);	

	return pTerrainBuffer->Compute_Picking(pTerrainTransform);
}

HRESULT CPlayer::Ready_Layer_Player_Hands(const wstring& strLayerTag, void* pArg)
{
	if (FAILED(m_pGameInstance->Add_Clone((CGameObject**)(&m_pPlayer_LH), LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Player_LH"), pArg)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Clone((CGameObject**)(&m_pPlayer_RH), LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Player_RH"), pArg)))
		return E_FAIL;

	return S_OK;
}

/* 원형객첼르 생성하기위한 함수에요. */
CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*		pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPlayer"));

		Safe_Release(pInstance);
	}

	return pInstance;
}
/* 사본객체를 생성하기위한 함수에요. */
CGameObject * CPlayer::Clone(void* pArg)
{	
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPlayer"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

	Safe_Release(m_pPlayer_LH);
	Safe_Release(m_pPlayer_RH);
	Safe_Release(m_pEquipment);
}

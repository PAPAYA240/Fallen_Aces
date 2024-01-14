#include "stdafx.h"
#include "..\Public\Monster.h"

#include "GameInstance.h"
#include "Collider_Box.h"
#include "Collider_Sphere.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject(pGraphic_Device)
{

}

CMonster::CMonster(const CMonster & rhs)
	: CLandObject(rhs)
{

}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC*	pGameObjectDesc = (CGameObject::GAMEOBJECT_DESC*)pArg;
	pGameObjectDesc->fSpeedPerSec = 5;
	pGameObjectDesc->fRotationPerSec = D3DXToRadian(90.0f);
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(2.f, 2.5f, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(_float(rand() % 100), 1.5f, _float(rand() % 100)));

	return S_OK;
}

void CMonster::Tick(_float fTimeDelta)
{
	SetUp_OnTerrain(1.f);
	Monster_Radius(fTimeDelta); // Player 인식 반경

	const CTransform* pPlayer_Transform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), g_strTransformTag));

	if (nullptr == pPlayer_Transform)
		return;

	_float3 fPlayer_Position = pPlayer_Transform->Get_State(CTransform::STATE_POSITION);
	_float3 fMonster_Position = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 fDistance = fMonster_Position - fPlayer_Position;
	m_fDistance = SQRT(fDistance.x, fDistance.y, fDistance.z);

	/* TODO: 나중에 지우거나 함수화*/
	/* TestCode */
	//if (false == m_Collisions.empty())
	//{
	//	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	//	for (auto Collision : m_Collisions)
	//	{
	//		if (COLLISION_EVENT::EV_OVERLAP == Collision.eEvent)
	//		{
	//			vPos.x += Collision.vOverlap.x;
	//			vPos.z += Collision.vOverlap.z;
	//		}			

	//		Safe_Release(Collision.pCollisionObject);
	//	}

	//	m_Collisions.clear();

	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	//}

	if (FAILED(Check_ForChangeContainer()))
		return;
}

void CMonster::Late_Tick(_float fTimeDelta)
{
	Chase_Player(fTimeDelta);

	SetUp_BillBoard();

	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster::Render()
{	
	if (FAILED(__super::Render()))
		return E_FAIL;

	//m_pTransformCom->NormalizeScale(m_pTextureCom->Get_ImageScale(m_fFrame));

	//_float4x4 Test = m_pTransformCom->Get_Test();

	//if (FAILED(m_pGraphic_Device->SetTransform(D3DTS_WORLD, &Test)))
	//	return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0, (_uint)m_fFrame)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;		

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CMonster::Choice_WalkRun()
{
}

/// <summary> 플레이어 인식 반경 계산 </summary>
void CMonster::Monster_Radius(_float fTimeDleta)
{
	if (!m_bPlayer_Follow) // 플레이어를 따라가지 않는다면
	{
		//_float fRadius = { 3.f }; // 플레이어 몬스터 인식 반경

		//_float3 fPlayer_Position = pPlayer_Transform->Get_State(CTransform::STATE_POSITION);
		//_float3 fMonster_Position = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		//_float3 fDistance = fMonster_Position - fPlayer_Position;
		//_float fDistance_Follow = SQRT(fDistance.x, fDistance.y, fDistance.z);
		
		/*CGameObject* pCollidedObject = this->Check_Collision(LEVEL_GAMEPLAY, TEXT("Layer_Player"), &m_fAttack_Radius);
		*/
		if (Check_Collision(LEVEL::LEVEL_GAMEPLAY, TEXT("Layer_Player")))
			m_bPlayer_Follow = true;
	}
}

/// <summary> 플레이어 공격 /// </summary>
void CMonster::Monster_Attack()
{
	//m_iHeavy_Count = // 랜덤;
	if (m_iHeavy_Count == 0)
	{
		// 플레이어 체력 감소
		m_pMonsterState = MONSTER_HEAVY_ATTACK;

		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		_uint iRandom = 3;
		m_iHeavy_Count = iRandom;
	}
	else
	{
		m_pMonsterState = MONSTER_ATTACK;
		m_iHeavy_Count--;
		// Player 체력 감소
	}
}

/// <summary> 플레이어 따라가기 /// </summary>
void CMonster::Player_Follow(_float fDeltaTime)
{
	// 1. 첫 터치때 and 공격 상황 시에
	if(!m_bFollow_Stop)
	{
		m_bFollow_Stop = false;
		const CTransform* pPlayer_Transform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), g_strTransformTag));

		m_pTransformCom->MoveToTarget(pPlayer_Transform, fDeltaTime, 0.5f);

		// 플레이어가 Attack 반경 안에 들어왔을 때
		if (Check_Collision(LEVEL::LEVEL_GAMEPLAY, TEXT("Layer_Player")))
		{
			if(!m_bFirstDash)
			{
				m_bFirstDash = true;
				m_bDash = true; //처음에만 대쉬 시작
			}
			
			//m_bOneMotion = true;
		}
		
		Walk_Range();

	}
	else if (m_bFollow_Stop)
	{
		if (Check_Collision(LEVEL::LEVEL_GAMEPLAY, TEXT("Layer_Player")))
		{
			if (!m_bFirstDash)
			{
				m_bFirstDash = true;
				m_bDash = true; //처음에만 대쉬 시작
			}

			m_bWalk = false;
			//m_bOneMotion = true;
		}
	}

}

/// <summary> 뛸 것인가? 걸을 것인가? </summary>
void CMonster::Walk_Range() 
{
	if (m_fDistance >= m_fRunRange) // 반경 20 이상은 다시 달리기
	{
		m_pMonsterState = MONSTER_RUN;
		m_bFirstDash = false;
		m_bDash = false;
	}
	else if (m_fDistance >= m_fWalkRange)
	{
		m_pMonsterState = MONSTER_WALK;
		m_bFirstDash = false;
		m_bDash = false;
	}
	
}

HRESULT CMonster::Check_ForChangeContainer()
{
	wstring strArrow = m_wstrStateTag;

	m_eBodyDraw = m_pGameInstance->Get_CameraDot(m_pTransformCom, LEVEL_GAMEPLAY, TEXT("Layer_Camera"));

	switch (_uint(m_pMonsterState))
	{
	case _uint(MONSTER_STATE::MONSTER_IDLE):
		strArrow += TEXT("_Idle");
		break;
	case _uint(MONSTER_STATE::MONSTER_WALK):
		strArrow += TEXT("_Walk");
		break;
	case _uint(MONSTER_STATE::MONSTER_RUN):
		strArrow += TEXT("_Run");
		break;
	case _uint(MONSTER_STATE::MONSTER_STRAFING):
		strArrow += TEXT("_Strafing");
		break;
	case _uint(MONSTER_STATE::MONSTER_POOPING):
		strArrow += TEXT("_Pooping");
		break;
	case _uint(MONSTER_STATE::MONSTER_TAUNT):
		strArrow += TEXT("_Taunt");
		break;
	case _uint(MONSTER_STATE::MONSTER_ATTACK_READY):
		strArrow += TEXT("_AttackReady");
		break;
	case _uint(MONSTER_STATE::MONSTER_ATTACK_WALK):
		strArrow += TEXT("_AttackWalk");
		break;
	case _uint(MONSTER_STATE::MONSTER_CROTCH_HIT):
		strArrow += TEXT("_CrotchHit");
		break;
	case _uint(MONSTER_STATE::MONSTER_DASH):
		strArrow += TEXT("_Dash");
		break;
	case _uint(MONSTER_STATE::MONSTER_HEAVY_ATTACK):
		strArrow += TEXT("_HeavyAttack");
		break;
	case _uint(MONSTER_STATE::MONSTER_FLINCH):
		strArrow += TEXT("_Flich");
		break;
	case _uint(MONSTER_STATE::MONSTER_ATTACK):
		strArrow += TEXT("_BasicAttack");
		break;
	case _uint(MONSTER_STATE::MONSTER_COLLAR):
		strArrow += TEXT("_Collar");
		break;
	default:
		return E_FAIL;
	}

	if (MONSTER_STATE::MONSTER_IDLE != m_pMonsterState)
	{
		strArrow += TEXT("_North");
		return m_pTextureCom->Change_Container(m_wstrTypeTag, strArrow);
	}

	switch (_uint(m_eBodyDraw))
	{
	case _uint(CAMERA_DIR::SOUTH):
		strArrow += TEXT("_South");
		break;
	case _uint(CAMERA_DIR::SOUTH_EAST):
		strArrow += TEXT("_SouthEast");
		break;
	case _uint(CAMERA_DIR::SOUTH_WEST):
		strArrow += TEXT("_SouthWest");
		break;
	case _uint(CAMERA_DIR::EAST):
		strArrow += TEXT("_East");
		break;
	case _uint(CAMERA_DIR::NORTH):
		strArrow += TEXT("_North");
		break;
	case _uint(CAMERA_DIR::NORTH_EAST):
		strArrow += TEXT("_NorthEast");
		break;
	case _uint(CAMERA_DIR::NORTH_WEST):
		strArrow += TEXT("_NorthWest");
		break;
	case _uint(CAMERA_DIR::WEST):
		strArrow += TEXT("_WEST");
		break;
	}

	return m_pTextureCom->Change_Container(m_wstrTypeTag, strArrow);
}

HRESULT CMonster::Add_Components()
{	
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider_Box::COLLIDER_DESC ColliderDesc = {};
	ColliderDesc.pObjectTransform = m_pTransformCom;

	/*if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Box"),
		TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;*/

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CMonster::Set_RenderState()
{	
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);	

	/* 알파테스트를 한다. (그려지는 픽셀들의 알파를 검사하여 그릴지 안그릴지를 판단한다.) */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	/* 내가 설정한 알파는 0이다. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);

	/* 그려지는 픽세르이 알파가 내가 설정한 알파보다 클때만 그려라. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CMonster::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

void CMonster::Chase_Player(_float fTimeDelta)
{
	const CTransform*		pTargetTransform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), g_strTransformTag));
	
	if (nullptr == pTargetTransform)
		return;

	m_pTransformCom->LookAt_ForLandObject(pTargetTransform->Get_State(CTransform::STATE_POSITION));
	m_pTransformCom->Refresh_ToLook();

	//m_pTransformCom->MoveToTarget(pTargetTransform, fTimeDelta, 0.5f);
}

void CMonster::SetUp_BillBoard()
{
	_float4x4		ViewMatrix;
	/*m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);*/

	const CTransform*	pCamTransform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), g_strTransformTag));
	ViewMatrix = *pCamTransform->Get_WorldMatrix();


	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[CTransform::STATE_RIGHT][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[CTransform::STATE_LOOK][0]);
}

void CMonster::TestCamera()
{
	/* 카메라의 Transform 을 가지고 옴*/
	m_eBodyDraw = m_pGameInstance->Get_CameraDot(m_pTransformCom, LEVEL_GAMEPLAY, TEXT("Layer_Camera"));
}

CGameObject* CMonster::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	list<CGameObject*>* pObjectList = m_pGameInstance->Get_LayerList(eLevel, strLayerTag);

	if (nullptr == pObjectList)
		return nullptr;

	//	Safe_AddRef(pObjectList);

	for (auto pObject : *pObjectList)
	{
		const CTransform* pTargetObjectTransform = dynamic_cast<const CTransform*>(pObject->Get_Component(g_strTransformTag));

		if (nullptr == pTargetObjectTransform)
			return nullptr;

		if (dynamic_cast<CCollider_Sphere*>(m_pColliderCom)->Check_Sphere(pTargetObjectTransform, pDirection))
		{
			return pObject;
		}
	}

	//	Safe_Release(pObjectList);

	return nullptr;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}

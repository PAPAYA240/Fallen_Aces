#include "stdafx.h"
#include "Goon_Monster.h"

CGoon_Monster::CGoon_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster(pGraphic_Device)
{
}

CGoon_Monster::CGoon_Monster(const CGoon_Monster& rhs)
	: CMonster(rhs)
{
}

HRESULT CGoon_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGoon_Monster::Initialize(void* pArg)
{
	CMonster::MONSTER_DESC* pMonsterDesc = (CMonster::MONSTER_DESC*)pArg;
	m_fSpeedPerSec = pMonsterDesc->fSpeedPerSec = 2.f;
	pMonsterDesc->fRotationPerSec = D3DXToRadian(90.0f);
	
	// 범위
	m_fRunRange = pMonsterDesc->fRun_Range = 40.f; // 뛰는 모션의 범위
	m_fWalkRange = pMonsterDesc->fWalk_Range = 20.f; // 걷는 모션의 범위

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(30.f, 3.5f, 30.f));
	m_pMonsterState = pMonsterDesc->pMonster_Start_State; // 시작 행동

	m_fOrigin_Position = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_wstrStateTag = TEXT("Goon");

	return S_OK;
}

void CGoon_Monster::Tick(_float fTimeDelta)
{
	_uint iMaxSize = m_pTextureCom->Container_MaxSize();

	m_fFrame += iMaxSize * fTimeDelta;

	if(m_pMonsterState == MONSTER_ATTACK || m_pMonsterState == MONSTER_HEAVY_ATTACK) // 한 번의 애니메이션만 사용할 때
	{
		if (m_fFrame >= iMaxSize)
		{
			m_bOneMotion = true;
			m_fFrame = 0.f;
		}
	}
	else 
	{
		if (m_fFrame >= iMaxSize)
			m_fFrame = 0.f;
	}

	__super::Tick(fTimeDelta);

}

void CGoon_Monster::Late_Tick(_float fTimeDelta)
{
	Monster_Patton_Ready(fTimeDelta);
	Change_Texture(m_pMonsterState);

	// Resive Here : Dash Position 
	/*if (m_bDash_Pos)
	{
		if(!m_bRandomTrue)
		{
			m_bRandomTrue = true;
			std::srand(static_cast<unsigned int>(std::time(nullptr)));
			m_iRandom_Pos = 1 + rand() % 4;
		}

		Random_DashPos(m_iRandom_Pos, fTimeDelta);
	}*/

	__super::Late_Tick(fTimeDelta);
}

HRESULT CGoon_Monster::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CGoon_Monster::Add_Components()
{
	return S_OK;
}

void CGoon_Monster::Monster_Patton_Ready(_float fTimeDelta)
{
	const CTransform* pPlayer_Transform = dynamic_cast<const CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), g_strTransformTag));
	
	// 몬스터 발견 -> 준비 -> 공격
	{
		if (m_pMonsterState == MONSTER_IDLE)
			m_bFollow_Stop = true;

		// 카메라가 어디있는지 8방향 하나하나 전부 설정해주기
		if (m_bPlayer_Follow && !m_bTaunt) // 도발
		{
			m_bTaunt = true;
			m_pMonsterState = MONSTER_TAUNT;
		}

		if (m_pMonsterState == MONSTER_TAUNT) // 도발 시 행동
		{
			m_bFollow_Stop = true;
			Player_Teasing();
		}

		//if(//만약 반경이 20 이내라면) RUN

		if (m_bAttack_Start) // 대쉬가 끝나고 공격이 시작됐을 때
		{
			if (m_bPlayer_Follow)
			{
				Walk_Range();
				Player_Follow(fTimeDelta);
			}

			// 대쉬
			if (m_bDash)
				m_pMonsterState = MONSTER_DASH;

			// 대쉬 및 공격
			if (m_pMonsterState == MONSTER_DASH)
			{
				Monster_Dash(fTimeDelta);
			}
			else
				m_bFollow_Stop = false;

			// 공격 : 기본 공격 or Heavy 공격
			if (m_pMonsterState == MONSTER_HEAVY_ATTACK || m_pMonsterState == MONSTER_ATTACK && m_bAttack)
			{
				// 애니메이션이 전부 끝나면 다시 대쉬
				if (m_bOneMotion)
				{
					m_bOneMotion = false;
					m_bDash = true;
					m_bAttack = false;
				}
			}
		}
	}
	
	// 만약 플레이어가 Heavy Attack을 날렸을 때
}

void CGoon_Monster::Change_Texture(MONSTER_STATE _state)
{
	if(m_pMonsterState != m_pPast_MonsterState)
	{
		switch (_state)
		{
		case MONSTER_IDLE:
		{
			
		}
			break;

		case MONSTER_WALK:
			m_pTextureCom->Change_Container(TEXT("Monster"), TEXT("Goon_Walk_South"));
			break;

		case MONSTER_RUN:
			m_pTextureCom->Change_Container(TEXT("Monster"), TEXT("Goon_Run"));
			break;

		case MONSTER_ATTACK_WALK:
			break; 

		case MONSTER_TAUNT:
			m_pTextureCom->Change_Container(TEXT("Monster"), TEXT("Goon_Taunt"));
			break;

		case MONSTER_STRAFING:  // Goon_Crotch_Hit
			break;

		case MONSTER_ATTACK_READY:
			break;

		case MONSTER_DASH:
			m_pTextureCom->Change_Container(TEXT("Monster"), TEXT("Goon_Dash"));
			break;

		case MONSTER_ATTACK:
			m_pTextureCom->Change_Container(TEXT("Monster"), TEXT("Goon_South_BasicAttack"));
			break;

		case MONSTER_HEAVY_ATTACK:
			m_pTextureCom->Change_Container(TEXT("Monster"), TEXT("Goon_South_HeavyAttack"));
			break;

		case MONSTER_COLLAR:
			break;

		case MONSTER_POOPING:
			m_pTextureCom->Change_Container(TEXT("Monster"), TEXT("Goon_South_HeavyAttack"));
			break;

		case MONSTER_CROTCH_HIT:
			m_pTextureCom->Change_Container(TEXT("Monster"), TEXT("Goon_Crotch_Hit"));
			break;

		case MONSTER_FLINCH:
			break;
		}
		m_pPast_MonsterState = (MONSTER_STATE)_state;
		m_fFrame = 0.f;
	}
}

/// <summary> 도발 시간 </summary>
void CGoon_Monster::Player_Teasing()
{
	m_pGameInstance->Add_Timer(TEXT("Goon_Monster_Teasing"));
	m_fTeasing_Time += m_pGameInstance->Compute_TimeDelta(TEXT("Goon_Monster_Teasing"));

	if (m_fTeasing_Time > 3.f) // 4초 간 도발 후
	{
		m_bAttack_Start = true; // 대쉬 및 공격해도 된다.
		m_bFollow_Stop = false;
	}
}

/// <summary> 몬스터 걷기 </summary>
void CGoon_Monster::Monster_Walk(_float fTimeDelta)
{
	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x <= m_fOrigin_Position.x - 3.f && !m_bRightWalk)
	{
		m_pTransformCom->Go_Left(fTimeDelta);

		if(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x == m_fOrigin_Position.x - 1.f)
			m_bRightWalk = true;
	}
	else 
	{
		m_pTransformCom->Go_Right(fTimeDelta);

		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x == m_fOrigin_Position.x)
			m_bRightWalk = false;
	}
}

/// <summary> 대쉬 후 공격 </summary>
void CGoon_Monster::Monster_Dash(_float fTimeDelta)
{
	// 1~4 확률로 대쉬 후 공격
	m_pGameInstance->Add_Timer(TEXT("Goon_Monster_Dash"));
	m_fDash_Time += m_pGameInstance->Compute_TimeDelta(TEXT("Goon_Monster_Dash"));
	
	if(!m_bDash_Start)
	{
		m_bDash_Start = true;
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		m_bDash_Count = rand() % 4;
	}

	if (m_bAttack) // 대쉬가 끝났다면 주먹질
	{
		m_bFollow_Stop = false;

		m_bDash_Start	= false;
		m_bDash_Count	= 0;
		m_bDash_Pos		= false;
		m_bRandomTrue	= false; // 랜덤 숫자 돌리기
		m_bDash			= false;

		Monster_Attack();
	}
	else if (m_fDash_Time > 2.f) // 3초마다 대쉬
	{
		m_bDash_Count--;
		// 스피드 값 Up
		m_fDash_Time = 0.f;

		// m_bDash_Pos = true; // 대쉬할 때 움직일 곳 판단
		
		if(0 == m_bDash_Count)
			m_bAttack = true;
		// m_bAttack_Start = true;
	}
}

void CGoon_Monster::Random_DashPos(_uint iPos, _float fTimeDelta)
{
	m_bFollow_Stop = false;// 대쉬할 때만 따라가기

	switch (iPos)
	{
	case 1:
		m_pTransformCom->Go_Left(fTimeDelta);
		break;

	case 2:
		m_pTransformCom->Go_Right(fTimeDelta);
		break;

	case 3:
		m_pTransformCom->Go_Backward(fTimeDelta);
		break;
	}
}

CGoon_Monster* CGoon_Monster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGoon_Monster* pInstance = new CGoon_Monster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CGoon_Monster"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CGoon_Monster::Clone(void* pArg)
{
	CGoon_Monster* pInstance = new CGoon_Monster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CGoon_Monster"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGoon_Monster::Free()
{
	__super::Free();
}

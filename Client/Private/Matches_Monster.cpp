#include "stdafx.h"
#include "Matches_Monster.h"

CMatches_Monster::CMatches_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster(pGraphic_Device)
{
}

CMatches_Monster::CMatches_Monster(const CMatches_Monster& rhs)
	: CMonster(rhs)
{
}

HRESULT CMatches_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMatches_Monster::Initialize(void* pArg)
{
	CMonster::MONSTER_DESC* pMonsterDesc = (CMonster::MONSTER_DESC*)pArg;
	pMonsterDesc->fSpeedPerSec = 2.f;
	pMonsterDesc->fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(rand() % 20, 0.5f, rand() % 20));
	m_pMonsterState = pMonsterDesc->pMonster_Start_State; // 시작 행동

	m_pTextureCom->Change_Container(TEXT("Monster"), TEXT("Boss_Idle_"));
	m_fOrigin_Position = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	return S_OK;
}

void CMatches_Monster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CMatches_Monster::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CMatches_Monster::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CMatches_Monster::Add_Components()
{
	return S_OK;
}

CMatches_Monster* CMatches_Monster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMatches_Monster* pInstance = new CMatches_Monster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CMatches_Monster"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMatches_Monster::Clone(void* pArg)
{
	CMatches_Monster* pInstance = new CMatches_Monster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CMatches_Monster"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMatches_Monster::Free()
{
	__super::Free();
}

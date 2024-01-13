#include "stdafx.h"
#include "PipeGuy_Monster.h"

CPipeGuy_Monster::CPipeGuy_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster(pGraphic_Device)
{
}

CPipeGuy_Monster::CPipeGuy_Monster(const CPipeGuy_Monster& rhs)
	: CMonster(rhs)
{
}

HRESULT CPipeGuy_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPipeGuy_Monster::Initialize(void* pArg)
{
	CMonster::MONSTER_DESC* pMonsterDesc = (CMonster::MONSTER_DESC*)pArg;
	pMonsterDesc->fSpeedPerSec = 2.f;
	pMonsterDesc->fRotationPerSec = D3DXToRadian(90.0f);
	m_fAttack_Radius = pMonsterDesc->fAttack_Direction = _float3(1.f, 1.f, 1.f);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(rand() % 20, 0.5f, rand() % 20));
	m_pMonsterState = pMonsterDesc->pMonster_Start_State; // 시작 행동

	m_pTextureCom->Change_Container(TEXT("Monster"), TEXT("PipeGuy_Idle"));
	m_fOrigin_Position = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pTextureCom = dynamic_cast<CTexture*>((*m_Components.find(TEXT("Com_Texture"))).second);
	
	return S_OK;
}

void CPipeGuy_Monster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	// Change_Texture(m_pMonsterState);
}

void CPipeGuy_Monster::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CPipeGuy_Monster::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CPipeGuy_Monster::Add_Components()
{
	return S_OK;
}

CPipeGuy_Monster* CPipeGuy_Monster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPipeGuy_Monster* pInstance = new CPipeGuy_Monster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CPipeGuy_Monster"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPipeGuy_Monster::Clone(void* pArg)
{
	CPipeGuy_Monster* pInstance = new CPipeGuy_Monster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPipeGuy_Monster"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPipeGuy_Monster::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

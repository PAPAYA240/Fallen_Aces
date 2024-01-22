#include "stdafx.h"
#include "..\Public\Terrain.h"

#include "GameInstance.h"
#include "Effect_ObjectPool.h"
#include "Effect.h"

CEffect_ObjectPool::CEffect_ObjectPool(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	
}

CEffect_ObjectPool::CEffect_ObjectPool(const CEffect_ObjectPool& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_ObjectPool::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_ObjectPool::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	m_iPoolSize = 20;

	for (_uint i = 0; i < m_iPoolSize; i++)
	{
		if (m_ObjectList.empty())
		{
			if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect"))))
			{
				MSG_BOX(TEXT("NO EFFECT"));
				return E_FAIL;
			}

			for(_uint i = 0; i < m_iPoolSize; i++)
			{
				// 비용
				m_ObjectList.push_back(m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), 0));
			}
		}
	}
	return S_OK;
}

void CEffect_ObjectPool::Tick(_float fTimeDelta)
{
	
}

void CEffect_ObjectPool::Late_Tick(_float fTimeDelta)
{
}

HRESULT CEffect_ObjectPool::Render()
{
	return S_OK;
}

void CEffect_ObjectPool::New_Object() // _float3 매개변수 
{
	if (m_ObjectList.empty()) return;
	 
	for (auto& iter : m_ObjectList)
	{
		if (!dynamic_cast<CEffect*>(iter)->Get_Active())
		{
			dynamic_cast<CEffect*>(iter)->Set_Active(true);
			dynamic_cast<CEffect*>(iter)->Set_Position(_float3(1.f, 1.f, 1.f));

			return;
		}
	}
}

/* 원형객첼르 생성하기위한 함수에요. */
CEffect_ObjectPool* CEffect_ObjectPool::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_ObjectPool* pInstance = new CEffect_ObjectPool(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CObjectPool"));

		Safe_Release(pInstance);
	}

	return pInstance;
}
/* 사본객체를 생성하기위한 함수에요. */
CGameObject* CEffect_ObjectPool::Clone(void* pArg)
{
	CEffect_ObjectPool* pInstance = new CEffect_ObjectPool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CObjectPool"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_ObjectPool::Free()
{
	__super::Free();
	for (auto& iter : m_ObjectList)
		Safe_Release(iter);

	m_ObjectList.clear();
}

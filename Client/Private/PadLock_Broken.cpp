#include "stdafx.h"
#include "PadLock.h"
#include "Player.h"
#include "PadLock_Broken.h"

CPadLock_Broken::CPadLock_Broken(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CGameObject(_pGraphic_Device)
{
}

CPadLock_Broken::CPadLock_Broken(const CPadLock_Broken& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPadLock_Broken::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPadLock_Broken::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC  MyDesc = {};

	if (FAILED(__super::Initialize(&MyDesc)))
		return E_FAIL;

	if (FAILED(Add_MyComponents()))
		return E_FAIL;

	m_pTextureCom->Change_Container(TEXT("Puzzle"), TEXT("PadLookBroken"));
	
	CPadLock::PADLOCK_DESC MyPad = {};

	if (nullptr != pArg)
	{
		CPadLock::PADLOCK_DESC* pDesc = (CPadLock::PADLOCK_DESC*)pArg;
		MyPad.Pos = pDesc->Pos;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(MyPad.Pos.x, MyPad.Pos.y - 0.5f, MyPad.Pos.z));
	m_pTransformCom->Set_Scaled(0.3f, 0.3f, 1.f);

	return S_OK;
}

void CPadLock_Broken::Tick(_float fTimeDelta)
{
	UnLockAnimation(fTimeDelta);
}

void CPadLock_Broken::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CPadLock_Broken::Render()
{

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0,0)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	if (FAILED(m_pVIbufferCom->Render()))
		return E_FAIL;

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPadLock_Broken::Add_MyComponents()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Puzzle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIbufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPadLock_Broken::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CPadLock_Broken::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

//void CPadLock_Broken::UnLockingControl()
//{
//	list<class CGameObject*>* pPlayerLayerList = m_pGameInstance->Get_LayerList(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
//	CGameObject* pGameObject = *(pPlayerLayerList->begin());
//	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);
//	CGameObject* p = m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Lock"), 0);
//	
//	if(dynamic_cast<CPadLock*>(p)->Get_Door_Open())
//		m_bUnLock = true;

//}

void CPadLock_Broken::UnLockAnimation(_float fTimeDelta)
{
	// 아래로 이동
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
	m_pTransformCom->Get_State(CTransform::STATE_POSITION) - _float3(0.f, 4.5f * fTimeDelta, 0.f));

}

CPadLock_Broken* CPadLock_Broken::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPadLock_Broken* pInstance = new CPadLock_Broken(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPadLock_Broken"));

		Safe_Release<CPadLock_Broken*>(pInstance);
	}

	return pInstance;
}

CGameObject* CPadLock_Broken::Clone(void* pArg)
{
	CPadLock_Broken* pInstance = new CPadLock_Broken(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CPadLock_Broken Clone "));

		Safe_Release<CPadLock_Broken*>(pInstance);
	}
	return pInstance;
}

CGameObject* CPadLock_Broken::Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection)
{
	return false;
}

void CPadLock_Broken::Free()
{
	__super::Free();
	Safe_Release<CTexture*>(m_pTextureCom);
	Safe_Release<CVIBuffer_Rect*>(m_pVIbufferCom);

}

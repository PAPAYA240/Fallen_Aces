#include "stdafx.h"
#include "..\Public\BackGround.h"

#include "GameInstance.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CBackGround::CBackGround(const CBackGround & rhs)
	: CGameObject(rhs)
{

}

HRESULT CBackGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;	

	if (FAILED(Add_Components()))
		return E_FAIL;

	D3DXMatrixIdentity(&m_ViewMatrix);

	/* 직교투영행렬을 만들어주는 함수. */
	/* 직교투영을 수행하여 투영스페이로 옮겨주기위한 영역(뷰볼륨whnf)을 설정해달라. */
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.0f, 1.f);

	/* 윈도우좌표!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
	m_fSizeX = g_iWinSizeX;
	m_fSizeY = 70.0f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = g_iWinSizeY - m_fSizeY * 0.5f;

	m_pTransformCom->Set_Scaled(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(
		m_fX - g_iWinSizeX * 0.5f,
		-m_fY + g_iWinSizeY * 0.5f, 
		0.f
		));

	return S_OK;
}

void CBackGround::Tick(_float fTimeDelta)
{	
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT		rcRect;
	SetRect(&rcRect, 
		_uint(m_fX - m_fSizeX * 0.5f), _uint(m_fY - m_fSizeY * 0.5f),
		_uint(m_fX + m_fSizeX * 0.5f), _uint(m_fY + m_fSizeY * 0.5f));

	if (TRUE == PtInRect(&rcRect, ptMouse))
	{
		int a = 10;
	}

	///* 삼각형과 방향벡터의 충돌을 체크 */
	//D3DXIntersectTri();
	
}

void CBackGround::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CBackGround::Render()
{	
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	
	if (FAILED(m_pTextureCom->Bind_Texture(0, 0)))
		return E_FAIL;	

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGround::Add_Components()
{	
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	

	return S_OK;
}

/* 원형객첼르 생성하기위한 함수에요. */
CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround*		pInstance = new CBackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBackGround"));

		Safe_Release(pInstance);
	}

	return pInstance;
}
/* 사본객체를 생성하기위한 함수에요. */
CGameObject * CBackGround::Clone(void* pArg)
{	
	CBackGround*		pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBackGround"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}

#include "stdafx.h"
#include "../Public/Player_RH.h"

CPlayer_RH::CPlayer_RH(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{
}

CPlayer_RH::CPlayer_RH(CPlayer_RH& rhs)
    :CGameObject(rhs)
{
}

HRESULT CPlayer_RH::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayer_RH::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    D3DXMatrixIdentity(&m_ViewMatrix);

    /* 직교투영행렬을 만들어주는 함수. */
    /* 직교투영을 수행하여 투영스페이로 옮겨주기위한 영역(뷰볼륨whnf)을 설정해달라. */
    D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.0f, 1.f);

    m_fSizeX = 482.f * 0.75f;
    m_fSizeY = 361.f * 0.75f;
    m_fX = g_iWinSizeX  - (g_iWinSizeX * 0.25f);
    m_fY = g_iWinSizeY - m_fSizeY * 0.25f;

    m_pTransformCom->Set_Scaled(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(
        m_fX - g_iWinSizeX * 0.5f,
        -m_fY + g_iWinSizeY * 0.5f,
        0.f
    ));

    m_pTextureCom->Change_Container(TEXT("Player"), TEXT("Right_Hand"));

    return S_OK;
}

void CPlayer_RH::Tick(_float fTimeDelta)
{
}

void CPlayer_RH::Late_Tick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayer_RH::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

    if (FAILED(m_pTextureCom->Bind_Texture(0, 1)))
        return E_FAIL;

    if (FAILED(Set_RenderState()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    if (FAILED(Reset_RenderState()))
        return E_FAIL;

    return S_OK;
}

void CPlayer_RH::Set_State(_uint iState)
{
    ++m_iAttackCnt;
}

HRESULT CPlayer_RH::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayer_RH::Set_RenderState()
{
    /* 알파테스트를 한다. (그려지는 픽셀들의 알파를 검사하여 그릴지 안그릴지를 판단한다.) */
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    /* 내가 설정한 알파는 0이다. */
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);

    /* 그려지는 픽세르이 알파가 내가 설정한 알파보다 클때만 그려라. */
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CPlayer_RH::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

CPlayer_RH* CPlayer_RH::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayer_RH* pInstance = new CPlayer_RH(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed To Created : CPlayer_Left_Hand Class"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayer_RH::Clone(void* pArg)
{
    CPlayer_RH* pInstance = new CPlayer_RH(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed To Cloned : CPlayer_Left_Hand Class"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayer_RH::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);
}

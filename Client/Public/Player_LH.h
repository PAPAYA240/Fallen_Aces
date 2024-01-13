#pragma once

#include"Client_Defines.h"
#include"GameObject.h"

BEGIN(Client)

class CPlayer_LH final : public CGameObject
{
private:
	CPlayer_LH(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer_LH(CPlayer_LH& rhs);
	virtual ~CPlayer_LH() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_State(_uint iState);

private:
	// 클래스 내부에서만 사용할 컴포넌트 멤버 변수
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_float						m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4					m_ViewMatrix, m_ProjMatrix;
public:
	_uint						m_iAttackCnt = { 0 };
private:
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_Components();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

public:
	static CPlayer_LH* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
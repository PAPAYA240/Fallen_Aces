#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Player_State final : public CUI
{
public:
	typedef struct tagUIPlayerStateDescription : public CUI::UI_DESC
	{
		_uint*		pPlayerHp;
		_float2		vFontSize;
		_float2		vFontPos;
	}UI_PLAYER_STATE_DESC;

private:
	CUI_Player_State(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_Player_State(CUI_Player_State& rhs);
	virtual ~CUI_Player_State() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	// 클래스 내부에서만 사용할 컴포넌트 멤버 변수
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };
		
	_uint*				m_pPlayerHp = { nullptr };
	_float2				m_vFontSize;
	_float2				m_vFontPos;

private:
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_Components();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

public:
	static CUI_Player_State* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
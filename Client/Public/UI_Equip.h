#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Equip final : public CUI
{
public:
	typedef struct tagUIEquipDescription : public CUI::UI_DESC
	{
		class CItem** pEquipItem;
	}UI_EQUIP_DESC;

private:
	CUI_Equip(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_Equip(CUI_Equip& rhs);
	virtual ~CUI_Equip() = default;

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

	class CItem**		m_pEquipItem = { nullptr };

private:
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_Components();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

public:
	static CUI_Equip* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
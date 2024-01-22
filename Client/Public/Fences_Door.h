#pragma once

#include "Client_Defines.h"
#include "DoorObj.h"

class CFences_Door final : public CDoorObj
{
private:
	CFences_Door(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CFences_Door(const CFences_Door& rhs);
	virtual ~CFences_Door() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private :
	virtual	void		UnLockingControl(_float fTimeDelta) override;

private:
	/* 이 객체에게 필요한 컴포넌트를 원형을 복제하여 이 객체에게 추가한다. */
	HRESULT Add_MyComponents();
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

public:
	static CFences_Door* Create(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형객체를 생성하기위한 함수. */
	virtual CGameObject* Clone(void* pArg) override;

	CGameObject* Check_Collision(LEVEL eLevel, const wstring& strLayerTag, _float3* pDirection);
	virtual void Free() override;
};


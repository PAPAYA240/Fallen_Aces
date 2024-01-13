#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

// test

class CPlayer_UI_Manager final : public CGameObject
{
public:
	typedef struct tagPlayerUIManagerDescription : public CGameObject::GAMEOBJECT_DESC
	{
		class CPlayer* pPlayer;
	}Player_UI_Manager_DESC;

private:
	CPlayer_UI_Manager(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer_UI_Manager(CPlayer_UI_Manager& rhs);
	virtual ~CPlayer_UI_Manager() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	class CPlayer*		m_pPlayer = { nullptr };

public:
	static CPlayer_UI_Manager* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
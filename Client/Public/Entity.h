#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

class CEntity abstract : public CGameObject
{
protected:
	CEntity(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEntity(const CEntity& rhs);
	virtual ~CEntity() = default;

public:
	class CComponent* Get_Component(const wstring& strComTag);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();


protected:
	HRESULT Add_Component(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strComponentTag, class CComponent** ppOut, void* pArg = nullptr);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};


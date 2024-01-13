#pragma once
#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CPipeGuy_Monster final : public CMonster
{
private:
	CPipeGuy_Monster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPipeGuy_Monster(const CPipeGuy_Monster& rhs);
	virtual ~CPipeGuy_Monster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private :
	virtual HRESULT Add_Components();

public:
	static CPipeGuy_Monster* Create(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형객체를 생성하기위한 함수. */
	virtual CGameObject* Clone(void* pArg) override;

	virtual void Free() override;
};
END


#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "LandObject.h"
#include "Monster.h"
#include "KeyObject.h"

/* 1. 레벨에 필요한 객체들을 생성한다. */
/* 2. 레벨을 반복적으로 갱신하여 화면에 보여준다. */

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const wstring& strLayerTag);

	HRESULT Ready_LandObject();
	HRESULT Ready_Layer_Player(const wstring& strLayerTag, CLandObject::LANDOBJECT_DESC& LandObjectDesc);
	HRESULT Ready_Layer_Monster(const wstring& strLayerTag, CMonster::MONSTER_DESC& MonsterDesc);
	HRESULT	Ready_Layer_Items(const wstring& strLayerTag, CLandObject::LANDOBJECT_DESC& LandObjectDesc);
	HRESULT Ready_Layer_Effect(const wstring& strLayerTag);
	HRESULT Ready_Layer_Key(const wstring& strLayerTag);
	HRESULT Ready_Layer_Switch(const wstring& strLayerTag);
	HRESULT Ready_Layer_Object(const wstring& strLayerTag);
	
public:
	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
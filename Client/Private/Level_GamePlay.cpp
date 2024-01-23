#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "Camera_Free.h"
#include "Camera_Object.h"

#include "KeyObject.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_LandObject()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Key(TEXT("Layer_Key"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Switch(TEXT("Layer_Switch"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object(TEXT("Layer_Object"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Door(TEXT("Layer_Door"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const wstring & strLayerTag)
{
	CCamera_Object::CAMERA_OBJECT_DESC		CameraDesc = {};

	CameraDesc.vEye = _float3(0.f, 10.0f, -10.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;	
	CameraDesc.fMouseSensor = 0.1f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.strTargetTag = TEXT("Layer_Player");

	/*if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
		return E_FAIL;*/

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Camera_Object"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_LandObject()
{
	/* 랜드오브젝트용 객체들에게 필요한 데이터를 구한다.*/
	CLandObject::LANDOBJECT_DESC		LandObjectDesc = {};

	LandObjectDesc.pTerrainTransform = (CTransform*)(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), g_strTransformTag));
	LandObjectDesc.pTerrainVIBuffer = (CVIBuffer_Terrain*)(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));	

	/* 구한정보들을 각 랜드오브젝트르 생성할 때 던진다. */
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"), LandObjectDesc)))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"), MonsnterDesc)))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_Items(TEXT("Layer_Items"), LandObjectDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const wstring & strLayerTag, CLandObject::LANDOBJECT_DESC& LandObjectDesc)
{
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Player"), &LandObjectDesc)))
		return E_FAIL;

	return S_OK;
}

<<<<<<< Updated upstream
HRESULT CLevel_GamePlay::Ready_Layer_Monster(const wstring & strLayerTag, CMonster::MONSTER_DESC& MonsterDesc)
{
	for (size_t i = 0; i < 5; i++)
	{
		MonsterDesc.pMonster_Start_State = MONSTER_IDLE;
		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_PipeGuy_Monster"), &MonsterDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Matches_Monster"), &MonsterDesc)))
			return E_FAIL;
	}

	MonsterDesc.pMonster_Start_State = MONSTER_IDLE;
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Goon_Monster"), &MonsterDesc)))
		return E_FAIL;

	MonsterDesc.pMonster_Start_State = MONSTER_IDLE;
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Goon_Monster"), &MonsterDesc)))
		return E_FAIL;

=======
HRESULT CLevel_GamePlay::Ready_Layer_Enemy(const wstring& wstrLayerTag, CLandObject::LANDOBJECT_DESC& LandObjectDesc)
{
	for (_uint i = 0; i < 5; ++i)
	{
		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, wstrLayerTag, TEXT("Prototype_GameObject_Enemy_Goon"), &LandObjectDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, wstrLayerTag, TEXT("Prototype_GameObject_Enemy_Pipe"), &LandObjectDesc)))
			return E_FAIL;
	}

>>>>>>> Stashed changes
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Items(const wstring& strLayerTag, CLandObject::LANDOBJECT_DESC& LandObjectDesc)
{
	for (_uint i = 0; i < 20; ++i)
	{
		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Apple"), &LandObjectDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Banana"), &LandObjectDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Cola"), &LandObjectDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_MedKit"), &LandObjectDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const wstring & strLayerTag)
{
	// 현재 플레이어에서 생성 중
	//if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_CObjectPool"))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Key(const wstring& strLayerTag)
{
	CKeyObject::KEY_DESC	KeyObjectDesc = {};

	KeyObjectDesc.pTerrainTransform = (CTransform*)(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), g_strTransformTag));
	KeyObjectDesc.pTerrainVIBuffer = (CVIBuffer_Terrain*)(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Boss_Key"), &KeyObjectDesc)))
		return E_FAIL;

	KeyObjectDesc.eColor = CKeyObject::COLOR::BULE;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Key"), &KeyObjectDesc)))
		return E_FAIL;

	KeyObjectDesc.eColor = CKeyObject::COLOR::RED;
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Key"), &KeyObjectDesc)))
		return E_FAIL;

	KeyObjectDesc.eColor = CKeyObject::COLOR::GREEN;
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_KeyCard"), &KeyObjectDesc)))
		return E_FAIL; 

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_PadLock_Key"), &KeyObjectDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Switch(const wstring& strLayerTag)
{
	for (_uint i = 0; i < 20; ++i)
	{
		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Butten_Switch"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Lever_Swich"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_RedButten_Switch"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Light_Switch"))))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Object(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Lock"), TEXT("Prototype_GameObject_PadLock"))))
		return E_FAIL;

	/* Combo Lock Door */
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_PadLock"), TEXT("Prototype_GameObject_Combo_Lock"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_LockObj"), TEXT("Prototype_GameObject_Combo_Lock_Obj"))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Door(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Boss_Door"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Cellar_Door"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_RedDoor"))))
		return E_FAIL;

	return S_OK;

}


HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const wstring & strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CLevel_GamePlay"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}

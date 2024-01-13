#include "stdafx.h"
#include "..\Public\Loader.h"
#include <process.h>
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Camera_Object.h"
#include "BackGround.h"
#include "Terrain.h"
#include "Monster.h"

#include "Player.h"
#include "Player_LH.h"
#include "Player_RH.h"

#include "Player_UI_Manager.h"

#include "Effect.h"
#include "Sky.h"

#include "Collider_Box.h"
#include "Collider_Sphere.h"

#include "Goon_Monster.h"
#include "PipeGuy_Monster.h"
#include "Matches_Monster.h"

/* Item Header */
#include "CItemApple.h"
#include "CItemBanana.h"
#include "CItemCola.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	/* 로더에게 지정된 레벨을 준비해라*/
	CLoader*		pLoader = (CLoader*)pArg;

	if (FAILED(pLoader->Start()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_Critical_Section);

	/* 스레드를 생성하낟. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Start()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr = { 0 };

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Logo();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_Critical_Section);

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo()
{
	m_strLoadingText = TEXT("텍스쳐를(을) 로딩 중 입니다.");
	/* For.Prototype_Component_Texture_Logo */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEX2D, TEXT("../Bin/Resources/Textures/Logo")))))
		return E_FAIL;

	
	m_strLoadingText = TEXT("모델를(을) 로딩 중 입니다.");
	
	m_strLoadingText = TEXT("셰이더를(을) 로딩 중 입니다.");
	
	m_strLoadingText = TEXT("객체의 원형를(을) 로딩 중 입니다.");

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	
	m_strLoadingText = TEXT("로딩이 완료되었습니다.");

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	m_strLoadingText = TEXT("텍스쳐를(을) 로딩 중 입니다.");
	/* Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEX2D, TEXT("../Bin/Resources/Textures/Terrain")))))
		return E_FAIL;

	/* Prototype_Component_Texture_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEX2D, TEXT("../Bin/Resources/Textures/UI")))))
		return E_FAIL;

	/* Prototype_Component_Texture_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEX2D, TEXT("../Bin/Resources/Textures/Player")))))
		return E_FAIL;

	/* Prototype_Component_Texture_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEX2D, TEXT("../Bin/Resources/Textures/Monster")))))
		return E_FAIL;

	/* Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXCUBE, TEXT("../Bin/Resources/Textures/SkyBox")))))
		return E_FAIL;

	/* Prototype_Component_Texture_Effect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEX2D, TEXT("../Bin/Resources/Textures/Effect")))))
		return E_FAIL;

	/* Prototype_Component_Texture_Items */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEX2D, TEXT("../Bin/Resources/Textures/Item")))))
		return E_FAIL;
	
	m_strLoadingText = TEXT("모델를(을) 로딩 중 입니다.");
	/* Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Terrain/Height.bmp")))))
		return E_FAIL;

	/* Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/*if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 1000, 1000))))
		return E_FAIL;*/	
	
	m_strLoadingText = TEXT("셰이더를(을) 로딩 중 입니다.");
	
	m_strLoadingText = TEXT("객체를(을) 로딩 중 입니다.");
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Object */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Object"),
		CCamera_Object::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_LH"),
		CPlayer_LH::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_RH"),
		CPlayer_RH::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player_UI_Manager */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_UI_Manager"),
		CPlayer_UI_Manager::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
		CEffect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;


	//	For.Prototype_Component_Collider_Box
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Box"),
		CCollider_Box::Create(m_pGraphic_Device))))
		return E_FAIL;

	//	For.Prototype_Component_Collider_Sphere
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		CCollider_Sphere::Create(m_pGraphic_Device))))
		return E_FAIL;

	/*#### For. Prototype_GameObject_Item ####*/
		/* For.Prototype_GameObject_Apple */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Apple"),
		CItemApple::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Banana */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Banana"),
		CItemBanana::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cola */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cola"),
		CItemCola::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* For.Prototype_GameObject_Monster */
	/* 1. Goon Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Goon_Monster"),
		CGoon_Monster::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* 2. Pipe Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PipeGuy_Monster"),
		CPipeGuy_Monster::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* 3. Matches_Malone */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Matches_Monster"),
		CMatches_Monster::Create(m_pGraphic_Device))))
		return E_FAIL;

	m_strLoadingText = TEXT("로딩이 완료되었습니다.");

	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLoader*		pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed To Created : CLoader"));

		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}

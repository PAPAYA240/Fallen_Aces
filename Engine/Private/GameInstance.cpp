#include "..\Public\GameInstance.h"

#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Font_Manager.h"
#include "Key_Manager.h"
#include "Renderer.h"
#include "Picking.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()	
{

}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const ENGINE_DESC& EngineDesc, _Out_ IDirect3DDevice9** ppGraphic_Device)
{
	/* 그래픽 디바이스를 초기화한다 .*/
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppGraphic_Device);
	if(nullptr == m_pGraphic_Device)
		return E_FAIL;	

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pPicking = CPicking::Create(*ppGraphic_Device, EngineDesc.hWnd, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY);
	if (nullptr == m_pPicking)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppGraphic_Device);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pFont_Manager = CFont_Manager::Create(*ppGraphic_Device);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	/* 인풋 디바이스를 초기화한다 .*/

	m_pKey_Manager = CKey_Manager::Create();
	if (nullptr == m_pKey_Manager)
		return E_FAIL;

	/* 사운드 디바이스를 초기화한다 .*/

	/* 오브젝트 매니져의 공간 예약을 한다. */
	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	/* 컴포넌트 매니져의 공간 예약을 한다. */
	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;
	
	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pObject_Manager)
		return;

	m_pKey_Manager->Update();

	m_pObject_Manager->Tick(fTimeDelta);

	m_pPicking->Update();

	m_pObject_Manager->Late_Tick(fTimeDelta);
	
	/* 반복적인 갱신이 필요한 객체들의 Tick함수를 호출한다. */
	m_pLevel_Manager->Tick(fTimeDelta);

}

HRESULT CGameInstance::Draw()
{
	if (nullptr == m_pGraphic_Device || 
		nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pGraphic_Device->Render_Begin();

	/* 화면에 그려져야할 객체들을 그리낟. == 오브젝트 매니져에 들어가있을꺼야 .*/
	/* 오브젝트 매니져에 렌더함수를 만들어서 호출하면 객체들을 다 그린다. */

	/* But. CRenderer객체의 렌더함수를 호출하여 객체를 그리낟. */
	m_pRenderer->Render();

	m_pLevel_Manager->Render();	

	m_pGraphic_Device->Render_End();

	return S_OK;
}

HRESULT CGameInstance::Clear(_uint iClearLevelIndex)
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	/* 지정된 레벨용 자원(텍스쳐, 사운드, 객체등등) 을 삭제한다. */

	/* 사본 게임오브젝트. */
	m_pObject_Manager->Clear(iClearLevelIndex);

	/* 컴포넌트 원형 */
	m_pComponent_Manager->Clear(iClearLevelIndex);


	return S_OK;
}

HRESULT CGameInstance::Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);	
}

HRESULT CGameInstance::Open_Level(_uint iNewLevelID, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iNewLevelID, pNewLevel);
}

HRESULT CGameInstance::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);	
}

HRESULT CGameInstance::Add_Clone(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Clone(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_Clone(CGameObject** Cloned_Object, _uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Clone(Cloned_Object, iLevelIndex, strLayerTag, strPrototypeTag, pArg);

}

const CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strComTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComTag, iIndex);
}

CAMERA_DIR CGameInstance::Get_CameraDot(const CTransform* _pTransform, _uint _iLevelIdx, const wstring& _wstrLayerTag, _uint _iNum)
{
	if (nullptr == m_pObject_Manager)
		return CAMERA_DIR::_END;

	return m_pObject_Manager->Get_CameraDot(_pTransform, _iLevelIdx, _wstrLayerTag, _iNum);
}

list<class CGameObject*>* CGameInstance::Get_LayerList(int iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_LayerList(iLevelIndex, strLayerTag);
}

CGameObject* CGameInstance::Get_Object(_uint _iLevelIdx, const wstring& _wstrLayerTag, _uint _iNum)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Object(_iLevelIdx, _wstrLayerTag, _iNum);
}


HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring & strPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_Timer(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimerTag);	
}

_float CGameInstance::Compute_TimeDelta(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

void CGameInstance::Transform_PickingToLocalSpace(const CTransform * pTransform, _float3 * pRayDir, _float3 * pRayPos)
{
	return m_pPicking->Transform_PickingToLocalSpace(pTransform, pRayDir, pRayPos);
}

_uint CGameInstance::Get_KeyState(int _iKey)
{
	return m_pKey_Manager->Get_KeyState(_iKey);
}

HRESULT CGameInstance::Add_Font(const wstring& strFontTag, const wstring& strFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	return m_pFont_Manager->Add_Font(strFontTag, strFontType, iWidth, iHeight, iWeight);
}

void CGameInstance::Render_Font(const wstring& strFontTag, const wstring& strText, const _float2* pPos, D3DXCOLOR Color)
{
	return m_pFont_Manager->Render_Font(strFontTag, strText, pPos, Color);
}

void CGameInstance::Release_Engine()
{
	CGameInstance::Get_Instance()->Free();

	Destroy_Instance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pPicking);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pRenderer);	
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pFont_Manager);
}

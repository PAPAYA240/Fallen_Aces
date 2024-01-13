#pragma once

/* 클라이언트개발자가 엔진의 기능을 사용하기위해서 항상 접근해야하는 클래스. */
#include "Renderer.h"
#include "Component_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(_uint iNumLevels, const ENGINE_DESC& EngineDesc, _Out_ IDirect3DDevice9** ppGraphic_Device);
	void Tick_Engine(_float fTimeDelta);
	HRESULT Draw();
	HRESULT Clear(_uint iClearLevelIndex);

public: /* For.Renderer */
	HRESULT Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iNewLevelID, class CLevel* pNewLevel);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_Clone(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	HRESULT Add_Clone(_Out_ class CGameObject** Cloned_Object, _uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	const CComponent* Get_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComTag, _uint iIndex = 0);
	CAMERA_DIR Get_CameraDot(const CTransform* _pTransform, _uint _iLevelIdx, const wstring& _wstrLayerTag, _uint _iNum = 0);
	list<class CGameObject*>* Get_LayerList(int iLevelIndex, const wstring& strLayerTag);


public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg = nullptr);


public: /* For.Timer_Manager */
	HRESULT Add_Timer(const wstring& strTimerTag);
	_float Compute_TimeDelta(const wstring& strTimerTag);

public: /* For.Picking */
	void Transform_PickingToLocalSpace(const class CTransform* pTransform, _Out_ _float3* pRayDir, _Out_ _float3* pRayPos);

public: // For Key Manager
	_uint Get_KeyState(int _iKey);


private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CRenderer*				m_pRenderer = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CPicking*					m_pPicking = { nullptr };
	class CKey_Manager*				m_pKey_Manager = { nullptr };

public:		
	static void Release_Engine();
	virtual void Free() override;
};

END
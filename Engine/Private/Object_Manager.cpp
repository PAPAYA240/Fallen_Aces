#include "..\Public\Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"

CObject_Manager::CObject_Manager()
{
}

const CComponent * CObject_Manager::Get_Component(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strComTag, _uint iIndex)
{
	CLayer*		pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(strComTag, iIndex);	
}

CAMERA_DIR CObject_Manager::Get_CameraDot(const CTransform* _pTransform, _uint _iLevelIdx, const wstring& _wstrLayerTag, _uint _iNum)
{
	if (_iNum >= m_iNumLevels)
		return CAMERA_DIR::_END;

	CLayer* pLayer = Find_Layer(_iLevelIdx, _wstrLayerTag);

	if (nullptr == pLayer)
		return CAMERA_DIR::_END;

	return pLayer->Get_CameraDot(_pTransform, _iNum);
}

HRESULT CObject_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pLayers = new map<const wstring, class CLayer*>[iNumLevels];

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr != Find_Prototype(strPrototypeTag))
		return E_FAIL;

	m_Prototypes.emplace(strPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObject_Manager::Add_Clone(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void* pArg)
{
	/* 복제해야할 원형객체를 검색한다. */
	CGameObject*	pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	/* 복제한 사본객체를 추가해야할 레이어를 찾는다.*/
	CLayer*		pLayer = Find_Layer(iLevelIndex, strLayerTag);

	/* 레이어가 없었다면 만들어서 객체를 추가하고 만든 레이어를 다시 맵에 추가해준다. */
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;
		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	/* 추가하려고하느 ㄴ레이어가 이미 있었다.*/
	else	
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

HRESULT CObject_Manager::Add_Clone(CGameObject** Cloned_Object, _uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg)
{
	// 생성하려는 객체의 원본을 찾아본다
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	// 없을경우 Fail 반환
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject* pClone = pPrototype->Clone(pArg);
	if (nullptr == pClone)
		return E_FAIL;

	*Cloned_Object = pClone;

	Safe_AddRef(pClone);

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	// 찾는 레이어가 없을 경우 만들어 객체를 추가해준 후 맵에도 추가 
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;
		// 위에서 생성한 클론을 레이어의 리스트에 추가
		pLayer->Add_GameObject(pClone);

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	// 레이어가 있었을 경우 리스트에 추가만 해줌
	else
		pLayer->Add_GameObject(pClone);

	return S_OK;
}

void CObject_Manager::Tick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			/* 필요한 위치의 갱신작어블 수행한다. */
			Pair.second->Tick(fTimeDelta);
		}
	}
}

void CObject_Manager::Late_Tick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			/* 갱신된 위치를 활용하여 추가적인 기능을 수행하낟. */
			Pair.second->Late_Tick(fTimeDelta);
		}
	}
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
	m_pLayers[iLevelIndex].clear();
}

list<class CGameObject*>* CObject_Manager::Get_LayerList(int iLevelIndex, const wstring& strLayerTag)
{
	class CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_LayerList();
}

CGameObject * CObject_Manager::Find_Prototype(const wstring & strPrototypeTag)
{
	auto		iter = m_Prototypes.find(strPrototypeTag);

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;	
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const wstring & strLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto		iter = m_pLayers[iLevelIndex].find(strLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}


CObject_Manager * CObject_Manager::Create(_uint iNumLevels)
{
	CObject_Manager*		pInstance = new CObject_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX(TEXT("Failed To Created : CObject_Manager"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Manager::Free()
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);
		m_pLayers[i].clear();		
	}

	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();
}

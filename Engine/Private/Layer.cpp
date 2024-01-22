#include "..\Public\Layer.h"

#include "GameObject.h"
#include "Camera.h"

CLayer::CLayer()
{
}

const CComponent * CLayer::Get_Component(const wstring & strComTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;
	

	return (*iter)->Get_Component(strComTag);
}

HRESULT CLayer::Initialize()
{
	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);	

	return S_OK;
}

void CLayer::Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Tick(fTimeDelta);
	}
}

void CLayer::Late_Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Tick(fTimeDelta);
	}
}

CAMERA_DIR CLayer::Get_CameraDot(const CTransform* _pTransform, _uint _iNum)
{
	if (m_GameObjects.size() <= _iNum)
		return CAMERA_DIR::_END;

	auto iter = m_GameObjects.begin();

	for (size_t i = 0; i < _iNum; ++i)
		++iter;

	CCamera* pCamera = dynamic_cast<CCamera*>(*iter);

	return pCamera->Get_CameraDot(_pTransform);
}

CGameObject* CLayer::Get_Object(_uint _iNum)
{
	if (m_GameObjects.size() <= _iNum)
		return nullptr;

	auto iter = m_GameObjects.begin();

	for (size_t i = 0; i < _iNum; ++i)
		++iter;

	CGameObject* pCamera = (*iter);

	return pCamera;
}

CLayer * CLayer::Create()
{
	CLayer*		pInstance = new CLayer();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CLayer"));

		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLayer::Free()
{
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}

#include "CTexLayer.h"

CTexLayer::CTexLayer()
{
}

HRESULT CTexLayer::Initialize()
{
	return S_OK;
}

HRESULT CTexLayer::Add_Texture(const wstring& _wstrStateTag, LPDIRECT3DBASETEXTURE9 _pTexture)
{
	auto iter = m_TexContainer.find(_wstrStateTag);

	if (iter == m_TexContainer.end() && nullptr == _pTexture)
	{
		vector<LPDIRECT3DBASETEXTURE9> vecTemp;
		m_TexContainer.emplace(_wstrStateTag, vecTemp);
	}
	else if (nullptr == _pTexture)
		return E_FAIL;

	if (iter == m_TexContainer.end())
	{
		vector<LPDIRECT3DBASETEXTURE9> vecTemp;

		vecTemp.push_back(_pTexture);

		m_TexContainer.emplace(_wstrStateTag, vecTemp);
	}
	else
		iter->second.push_back(_pTexture);

	return S_OK;
}

void CTexLayer::Add_Ref()
{
	for (auto pair : m_TexContainer)
	{
		for (auto& pTexture : pair.second)
			Safe_AddRef(pTexture);
	}
}

vector<LPDIRECT3DBASETEXTURE9>* CTexLayer::Find_State(const wstring& _wstrStateTag)
{
	auto iter = m_TexContainer.find(_wstrStateTag);

	if (iter == m_TexContainer.end())
		return nullptr;

	return &iter->second;
}

CTexLayer* CTexLayer::Create()
{
	CTexLayer* pInstance = new CTexLayer();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : TexLayer"));
		Safe_Release<CTexLayer*>(pInstance);
	}

	return pInstance;
}

void CTexLayer::Free()
{
	for (auto iter : m_TexContainer)
	{
		for (auto pTexture : iter.second)
			Safe_Release<LPDIRECT3DBASETEXTURE9>(pTexture);

		iter.second.clear();
	}

	m_TexContainer.clear();
}

#include "CTexLayer.h"

CTexLayer::CTexLayer()
{
}

HRESULT CTexLayer::Initialize()
{
	return S_OK;
}

HRESULT CTexLayer::Add_Texture(const wstring& _wstrStateTag, LPDIRECT3DBASETEXTURE9 _pTexture, POINT _ptSize)
{
	auto iter = m_TexContainer.find(_wstrStateTag);

	if (iter == m_TexContainer.end() && nullptr == _pTexture)
	{
		vector<T_IMAGE> vecTemp;
		m_TexContainer.emplace(_wstrStateTag, vecTemp);
	}
	else if (nullptr == _pTexture)
		return E_FAIL;

	if (iter == m_TexContainer.end())
	{
		vector<T_IMAGE> vecTemp;

		T_IMAGE tImage = {};

		tImage.pTexture = _pTexture;
		tImage.ptImageSize = _ptSize;

		vecTemp.push_back(tImage);

		m_TexContainer.emplace(_wstrStateTag, vecTemp);
	}
	else
	{
		T_IMAGE tImage = {};

		tImage.pTexture = _pTexture;
		tImage.ptImageSize = _ptSize;

		iter->second.push_back(tImage);
	}

	return S_OK;
}

void CTexLayer::Add_Ref()
{
	for (auto pair : m_TexContainer)
	{
		for (auto& tImage : pair.second)
			Safe_AddRef(tImage.pTexture);
	}
}

vector<T_IMAGE>* CTexLayer::Find_State(const wstring& _wstrStateTag)
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
			Safe_Release<LPDIRECT3DBASETEXTURE9>(pTexture.pTexture);

		iter.second.clear();
	}

	m_TexContainer.clear();
}

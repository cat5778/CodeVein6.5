#include "stdafx.h"
#include "Field.h"
#include "Export_Function.h"
#include <fstream>
#include "StaticObject.h"
#include "RussianHat.h"
#include "Shield.h"
#include "RedDevil.h"
#include "Halberd.h"
#include "SkySphere.h"
#include "CocoonDevil.h"
CField::CField(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
	m_uiStageIdx = 1;
}

CField::~CField(void)
{

}

HRESULT CField::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(L"UI"), E_FAIL);

	//���̴��������߰� 
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	//

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CField::LateReady_Scene(void)
{
	FAILED_CHECK_RETURN(Engine::CScene::LateReady_Scene(), E_FAIL);

	//Engine::CCamera* pCamera = dynamic_cast<Engine::CCamera*>(Engine::Get_GameObject(L"GameLogic", L"DynamicCamera"));
	//NULL_CHECK_RETURN(pCamera, E_FAIL);
	//Engine::Get_Renderer()->Set_Perspective(*pCamera->Get_Projection());
	dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"))->Set_InvenVec(Get_Inven());

	return S_OK;
}

_int CField::Update_Scene(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;


	Engine::Get_Light(0)->Range = 1100;
	Engine::Get_Light(0)->Diffuse=D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.35f);// ����




	_int iEvent=Engine::CScene::Update_Scene(fTimeDelta);

	return iEvent;
}

_int CField::LateUpdate_Scene(const _float & fTimeDelta)
{
	return NO_EVENT;
}

void CField::Render_Scene(void)
{
	++m_dwRenderCnt;

	if (m_fTime >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
		m_fTime = 0.f;
		m_dwRenderCnt = 0;
	}

	Engine::Render_Font(L"Font_Default", m_szFPS, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f,1.f));

}

CField* CField::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CField*	pInstance = new CField(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CField::Free(void)
{
	Engine::CScene::Free();
}

HRESULT CField::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	//pLayer->set_LayerName(pLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*		pGameObject = nullptr;


	// Player
	pGameObject = CPlayer::Create(m_pGraphicDev,0, m_uiStageIdx);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);


	//pGameObject = CRussianHat::Create(m_pGraphicDev,L"RussianHat",0,m_uiStageIdx);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);
	//
	////Shield 
	//pGameObject = CShield::Create(m_pGraphicDev, 0);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Shield", pGameObject), E_FAIL);


	pGameObject = CSkySphere::Create(m_pGraphicDev, 1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkySphere", pGameObject), E_FAIL);


	m_ppGameObjectMap = &pLayer->Get_ObjectMap();

	switch ((LOADMODE)m_uiStageIdx)
	{
	case LOAD_NOMAL:
		Load_Text(L"../../Resource/Data/Base.txt");
		break;
	case LOAD_NOMAL2:
		//Load_Text(L"../../Resource/Data/SnowMap.txt");
		Load_Text(L"../../Resource/Data/SnowMap2.txt");
		break;
	case LOAD_NOMAL3:
		Load_Text(L"../../Resource/Data/Base.txt");
		break;
	case LOAD_PLAYER:
		break;
	case LOAD_MONSTER:
		break;
	case LOAD_END:
		break;
	default:
		break;
	}
	/*
	pGameObject = CRedDevil::Create(m_pGraphicDev, L"RedDevil", 0,_vec3(-11.4f,0.334f,7.88f),48);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);

	pGameObject = CHalberd::Create(m_pGraphicDev, 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);

	pGameObject = CRedDevil::Create(m_pGraphicDev, L"RedDevil", 1, _vec3(-43.8f, 2.66f, 42.94f), 39);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);

	pGameObject = CHalberd::Create(m_pGraphicDev, 1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);


	pGameObject = CRedDevil::Create(m_pGraphicDev, L"RedDevil", 2, _vec3(-1.8f,0.0826f,15.9f), 47);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);

	pGameObject = CHalberd::Create(m_pGraphicDev, 2);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);


	pGameObject = CCocoonDevil::Create(m_pGraphicDev, L"CocoonDevil", 0, _vec3(-15.36f, 0.9416f, -7.8253), 65);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);


*/




	m_pLayerMap.emplace(pLayerTag, pLayer);	

	return S_OK;
}

HRESULT CField::Ready_UI_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;
/*
	auto	iter = find_if(m_pLayerMap.begin(), m_pLayerMap.end(), [](map<wstring, Engine::CLayer*>::const_iterator mapIter)
	{
		if (mapIter->first==(L"GameLogic"))
			return mapIter;
	}
	);

	if (iter == m_pLayerMap.end())
		return E_FAIL;

*/

//Engine::CTransform*	pPlayerTransform =	dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));

	//NULL_CHECK_RETURN(pPlayerTransform, E_FAIL);

	// StaticCamera
	pGameObject = CThirdPersonCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 10.f, -5.f),
		&_vec3(0.f, 0.f, 0.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(45.f),
		_float(WINCX) / WINCY,
		1.f,
		1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ThirdPersonCamera", pGameObject), E_FAIL);

	m_pLayerMap.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CField::Ready_LightInfo(void)
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(0.95f, 0.95f, 1.0f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);


	//tLightInfo.Type = D3DLIGHT_POINT;

	//tLightInfo.Diffuse = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	//tLightInfo.Specular = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	//tLightInfo.Position = _vec3(10.f, 2.f, 10.f);
	//tLightInfo.Range = 3.f;

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1), E_FAIL);


	//tLightInfo.Type = D3DLIGHT_POINT;

	//tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	//tLightInfo.Specular = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	//tLightInfo.Position = _vec3(30.f, 5.f, 10.f);
	//tLightInfo.Range = 10.f;

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 2), E_FAIL);

	return S_OK;
}

//HRESULT CField::Ready_Prototype(void)
//{
//	Engine::CComponent*		pComponent = nullptr;
//
//	// Optimization
//	pComponent = Engine::COptimization::Create(m_pGraphicDev, Engine::COptimization::Enable, 129, 129);
//	NULL_CHECK_RETURN(pComponent, E_FAIL);
//	Engine::Ready_Prototype(L"Proto_Optimization", pComponent);
//
//
//	return S_OK;
//}
//

HRESULT CField::Load_Text(const _tchar * pFilePath)
{
	ifstream fin;

	fin.open(pFilePath);

	if (fin.fail())
		return E_FAIL;
	wstring wstrTemp;
	char cTemp[MIN_STR];
	Transform_Info tInfo;
	while (!fin.eof())
	{
		D3DXVECTOR3 vPos;

		fin.getline(cTemp, MIN_STR);
		wchar_t* ppwchar= CharToWChar(cTemp);
		wstrTemp = ppwchar;
		delete ppwchar;
		//wstring wstrConvert(cTemp, &cTemp[MIN_STR]);
		//wstrTemp = wstrConvert;

		fin.getline(cTemp, MIN_STR); // ������ ������ ���� ����(���� ����)�� �о����.
		tInfo.vScale.x = (_float)atof(cTemp);
		fin.getline(cTemp, MIN_STR); // ������ ������ ���� ����(���� ����)�� �о����.
		tInfo.vScale.y = (_float)atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		tInfo.vScale.z = (_float)atof(cTemp);

		fin.getline(cTemp, MIN_STR); // ������ ������ ���� ����(���� ����)�� �о����.
		tInfo.vRotation.x = (_float)atof(cTemp);
		fin.getline(cTemp, MIN_STR); // ������ ������ ���� ����(���� ����)�� �о����.
		tInfo.vRotation.y = (_float)atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		tInfo.vRotation.z = (_float)atof(cTemp);

		fin.getline(cTemp, MIN_STR); // ������ ������ ���� ����(���� ����)�� �о����.
		tInfo.vPosition.x = (_float)atof(cTemp);
		fin.getline(cTemp, MIN_STR); // ������ ������ ���� ����(���� ����)�� �о����.
		tInfo.vPosition.y = (_float)atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		tInfo.vPosition.z = (_float)atof(cTemp);

		_uint uiNameCnt = wstrTemp.find_last_of(L'_');
		wstring wstrObjectName = wstrTemp.substr(0, uiNameCnt);

		_uint uiObjIdx = 0;
		uiObjIdx = _wtoi(wstrTemp.substr(uiNameCnt + 1, wstring::npos).c_str());

		//if (wstrTemp.find(L"Ceilling_a_ba01") != wstring::npos)
		//	continue;
		//if (wstrTemp.find(L"AlterCeiling_ba01") != wstring::npos)
		//	continue;

		if (wstrTemp.compare(L"") == 0)
			break;




		Engine::CGameObject*		pGameObject = nullptr;
		pGameObject = CStaticObject::Create(m_pGraphicDev, wstrObjectName, uiObjIdx, tInfo);

		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		(*m_ppGameObjectMap).insert(make_pair(wstrTemp, pGameObject));

	}
	fin.close();

	return S_OK;
}

wchar_t * CField::CharToWChar(const char * pstrSrc)
{
	int nLen = strlen(pstrSrc) + 1;
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t)* nLen);
	mbstowcs(pwstr, pstrSrc, nLen);
	return pwstr;
}

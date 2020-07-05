#include "stdafx.h"
#include "Stage.h"
#include "Export_Function.h"
#include <fstream>
#include "StaticObject.h"
#include "RussianHat.h"
#include "Shield.h"
#include "Field.h"
#include "RedDevil.h"
#include "Halberd.h"
#include "CocoonDevil.h"
#include "SkySphere.h"
#include "Davis.h"
#include "UI.h"
#include "Gauge.h"
#include "Effect.h"

//#include "SoundMgr.h"
CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
	m_uiStageIdx = 0;
}

CStage::~CStage(void)
{

}

HRESULT CStage::Ready_Scene(void)
{

	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(L"UI"), E_FAIL);


	//CSoundMgr::GetInstance()->PlayBGM(L"Adam Levine - Lost Stars Lyrics.mp3");
	//CSoundMgr::GetInstance()->SetVolume(CSoundMgr::BGM, 0.5f);
	//쉐이더적용후추가 
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	return S_OK;
}

HRESULT CStage::LateReady_Scene(void)
{
	FAILED_CHECK_RETURN(Engine::CScene::LateReady_Scene(), E_FAIL);

	//Engine::CCamera* pCamera = dynamic_cast<Engine::CCamera*>(Engine::Get_GameObject(L"GameLogic", L"DynamicCamera"));
	//NULL_CHECK_RETURN(pCamera, E_FAIL);
	//Engine::Get_Renderer()->Set_Perspective(*pCamera->Get_Projection());

	return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
	_int iEvent=Engine::CScene::Update_Scene(fTimeDelta);

	m_fSin += fTimeDelta * 1000.f;
	Engine::Get_Light(1)->Range = 8.f + sinf(D3DXToRadian(m_fSin))*-0.4f;

	//m_fNeonSin += fTimeDelta * 10000.f;
	//Engine::Get_Light(6)->Range = 0.5f + sinf(D3DXToRadian(m_fNeonSin))*-0.15f;

	//cout << "LightRange =" << Engine::Get_Light(3)->Range << endl;
	//Engine::Get_Light(0)->Position = _vec3(19.3f, 2.f, -1.f+sinf(D3DXToRadian(m_fSin))*-10.f);
	//if (CKeyMgr::GetInstance()->KeyDown(KEY_G))
	//{
	//	m_fTest++;
	//	Engine::Get_Light(6)->Range= m_fTest;
	//	//cout << "Range" << m_fTest << endl;
	//}

	//if (CKeyMgr::GetInstance()->KeyDown(KEY_H))
	//{
	//	m_fTest2 -= 1.f;
	//	Engine::Get_Light(2)->Attenuation0 = m_fTest2;
	//	cout << "att0" << m_fTest2 << endl;
	//}


	//if (CKeyMgr::GetInstance()->KeyDown(KEY_J))
	//{
	//	m_fTest3 -= 1.f;
	//	Engine::Get_Light(2)->Attenuation1 = m_fTest3;
	//	cout << "att1" << m_fTest3 << endl;

	//}





	if (m_pPlayer->ChangeScene())
	{

		Engine::CScene*		pScene = CField::Create(m_pGraphicDev);
		if (pScene == nullptr)
			return -1;
		pScene->Set_Inven(*dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"))->Get_InvenVec());
		
		Engine::SetUp_Scene(pScene);
		return iEvent;
	}

	m_fTime += fTimeDelta;


	return iEvent;
}

_int CStage::LateUpdate_Scene(const _float & fTimeDelta)
{
	return NO_EVENT;
}

void CStage::Render_Scene(void)
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

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStage::Free(void)
{
	Engine::CScene::Free();
}

HRESULT CStage::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	//pLayer->set_LayerName(pLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*		pGameObject = nullptr;

	//// Player
	pGameObject = m_pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	pGameObject = CSkySphere::Create(m_pGraphicDev,0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkySphere", pGameObject), E_FAIL);


	pGameObject = CDavis::Create(m_pGraphicDev, L"Davis", 0,_vec3(3.6598f,0.2853f,3.7742f),41);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);




	//if (LOAD_MODE == LOAD_NOMAL2)
	//{


		//pGameObject = CRussianHat::Create(m_pGraphicDev,L"RussianHat",0);
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);

	//	////Shield 
	//	pGameObject = CShield::Create(m_pGraphicDev, 0);
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Shield", pGameObject), E_FAIL);


	//pGameObject = CRedDevil::Create(m_pGraphicDev, L"RedDevil",0,_vec3(-10.72f,0.338f,7.8f),48);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);


	//pGameObject = CEffect::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Effect", pGameObject), E_FAIL);

	//	pGameObject = CHalberd::Create(m_pGraphicDev, 0);
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);

		//pGameObject = CRedDevil::Create(m_pGraphicDev, L"RedDevil", 1, _vec3(-1.8f,0.0826f,15.9f), 47);
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);

	//	pGameObject = CHalberd::Create(m_pGraphicDev, 1);
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);

	//pGameObject = CCocoonDevil::Create(m_pGraphicDev, L"CocoonDevil", 0, _vec3(-15.36f, 0.9416f, -7.8253), 65);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject->Get_InstName().c_str(), pGameObject), E_FAIL);
	//}


	m_ppGameObjectMap = &pLayer->Get_ObjectMap();

	if (LOAD_MODE != 0)
		m_uiStageIdx = LOAD_MODE;

	switch ((LOADMODE)m_uiStageIdx)
	{
	case LOAD_NOMAL:
		Load_Text(L"../../Resource/Data/Base2.txt");
		break;
	case LOAD_NOMAL2:
		Load_Text(L"../../Resource/Data/SnowMap.txt");
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
	


	m_pLayerMap.emplace(pLayerTag, pLayer);	

	return S_OK;
}

HRESULT CStage::Ready_UI_Layer(const _tchar* pLayerTag)
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

HRESULT CStage::Ready_LightInfo(void)
{
	for (int i = 0; i < 9; i++)
		ZeroMemory(&m_tLightInfo[i], sizeof(D3DLIGHT9));

	//태양
	m_tLightInfo[0].Type = D3DLIGHT_POINT;
	m_tLightInfo[0].Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 0.4f);
	//m_tLightInfo[0].Diffuse = D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.35f);// 눈맵
	//m_tLightInfo[0].Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//m_tLightInfo[0].Ambient = D3DXCOLOR(0.5f, 0.5f, 0.7f, 1.f);
	m_tLightInfo[0].Range = 1100.f;
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &m_tLightInfo[0], 0), E_FAIL);


	//포탈 
	m_tLightInfo[1].Type = D3DLIGHT_POINT;
	m_tLightInfo[1].Diffuse = D3DXCOLOR(0.5f, 0.85f, 1.f, 1.f);
	//m_tLightInfo[1].Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//m_tLightInfo[1].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tLightInfo[1].Range = 10.f;
	m_tLightInfo[1].Position = _vec3(19.3f, 1.f, -2.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &m_tLightInfo[1], 1), E_FAIL);

	//바 조명
	m_tLightInfo[2].Type = D3DLIGHT_POINT;
	m_tLightInfo[2].Diffuse = D3DXCOLOR(1.f, 0.6f, 0.8f, 1.f);
	//m_tLightInfo[2].Specular = D3DXCOLOR(1.f, 0.5f, 0.5f, 1.f);
	//m_tLightInfo[2].Ambient = D3DXCOLOR(1.f, 0.6f, 0.6f, 1.f);
	m_tLightInfo[2].Range = 5.f;
	m_tLightInfo[2].Position = _vec3(10.f, 0.4f, -5.8f);
	m_tLightInfo[2].Attenuation0 = 0.f;
	m_tLightInfo[2].Attenuation1 = 1.f;
	m_tLightInfo[2].Attenuation2 = 0.f;
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &m_tLightInfo[2], 2), E_FAIL);

	//NPC
	m_tLightInfo[3].Type = D3DLIGHT_POINT;
	m_tLightInfo[3].Diffuse = D3DXCOLOR(0.95f, 0.8f, 0.7f, 1.f);
	//m_tLightInfo[3].Specular = D3DXCOLOR(1.f, 0.7f, 0.7f, 1.f);
	//m_tLightInfo[3].Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	m_tLightInfo[3].Range = 7.f;
	m_tLightInfo[3].Position = _vec3(3.6598f, 2.5f, 3.7742f);
	//m_tLightInfo[3].Position = _vec3(7.f, 2.5f, 4.2f);
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &m_tLightInfo[3], 3), E_FAIL);

	//쇼케이스
	m_tLightInfo[4].Type = D3DLIGHT_POINT;
	m_tLightInfo[4].Diffuse = D3DXCOLOR(0.95f, 0.8f, 0.7f, 1.f);
	//m_tLightInfo[4].Specular = D3DXCOLOR(1.f, 0.7f, 0.7f, 1.f);
	//m_tLightInfo[4].Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	m_tLightInfo[4].Range = 7.f;
	m_tLightInfo[4].Position = _vec3(10.414f, 0.5f, 4.79f);
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &m_tLightInfo[4], 4), E_FAIL);

	//당구장
	m_tLightInfo[5].Type		= D3DLIGHT_POINT;
	m_tLightInfo[5].Diffuse		= D3DXCOLOR(0.95f, 0.8f, 0.6f, 1.f);
	//m_tLightInfo[5].Specular	= D3DXCOLOR(1.f, 0.7f, 0.7f, 1.f);
	//m_tLightInfo[5].Ambient		= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	m_tLightInfo[5].Range		= 10.f;
	m_tLightInfo[5].Position	= _vec3(-10.213, 2.f,2.58f);
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &m_tLightInfo[5], 5), E_FAIL);

	////네온
	//m_tLightInfo[6].Type = D3DLIGHT_POINT;
	//m_tLightInfo[6].Diffuse = D3DXCOLOR(0.95f, 0.5f, 0.95f, 1.f);
	//m_tLightInfo[6].Specular = D3DXCOLOR(1.f, 0.6f, 1.f, 1.f);
	//m_tLightInfo[6].Ambient = D3DXCOLOR(1.f, 0.5f, 1.f, 1.f);
	//m_tLightInfo[6].Range = 1.f;
	//m_tLightInfo[6].Position = _vec3(6.6567f,0.35f, 3.2512f);
	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &m_tLightInfo[6], 6), E_FAIL);



	return S_OK;
}

//HRESULT CStage::Ready_Prototype(void)
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

HRESULT CStage::Load_Text(const _tchar * pFilePath)
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

		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		tInfo.vScale.x = atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		tInfo.vScale.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		tInfo.vScale.z = atof(cTemp);

		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		tInfo.vRotation.x = atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		tInfo.vRotation.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		tInfo.vRotation.z = atof(cTemp);

		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		tInfo.vPosition.x = atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		tInfo.vPosition.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		tInfo.vPosition.z = atof(cTemp);

		_uint uiNameCnt = wstrTemp.find_last_of(L'_');
		wstring wstrObjectName = wstrTemp.substr(0, uiNameCnt);

		_uint uiObjIdx = 0;
		uiObjIdx = _wtoi(wstrTemp.substr(uiNameCnt + 1, wstring::npos).c_str());

		//if (wstrTemp.find(L"SM_Ceilling_a_ba01") != wstring::npos)
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

wchar_t * CStage::CharToWChar(const char * pstrSrc)
{
	int nLen = strlen(pstrSrc) + 1;
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t)* nLen);
	mbstowcs(pwstr, pstrSrc, nLen);
	return pwstr;
}


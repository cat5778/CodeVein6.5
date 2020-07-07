#include "stdafx.h"
#include "Player.h"
#include <fstream>
#include "Export_Function.h"
#include "ThirdPersonCamera.h"
#include "ColliderManager.h"
#include "DynamicObject.h"
#include "LockOn.h"
#include "3DUI.h"
#include "3DButton.h"
#include "Shop.h"
#include "ShopSub.h"
#include "InvenSub.h"
#include "Inven.h"
#include "Sword.h"
#include "Portal.h"
#include "PortalSub.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiIdx,_uint uiStageIdx)
	: Engine::CGameObject(pGraphicDev)
{
	m_uiIdx = uiIdx;
	m_ObjName = L"Player2";
	m_uiStageSet = uiStageIdx;
	m_wstrInstName = m_ObjName + L"_" + to_wstring(m_uiIdx);
	m_fCurHP = m_fMaxHP;
	m_fCurSP = m_fMaxSP;
}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (LOAD_MODE )
		m_uiStageSet = LOAD_MODE;
	switch ((LOADMODE)m_uiStageSet)
	{
	case LOAD_NOMAL:
		m_pNaviCom->Set_Index(38);// Base Init Idx 38 
		Load_Text(L"../../Resource/Data/NavMash/BaseCompleteNav.txt");
		break;
	case LOAD_NOMAL2:

		//m_pTransformCom->Set_Pos(50.f, -4.75f, 58.f); //New Map StartPos 
		//m_pNaviCom->Set_Index(1);// Base Init Idx 38 
		//Load_Text(L"../../Resource/Data/NavMash/AttachMapNav.txt");

		//m_pTransformCom->Set_Pos(0.069f,6.208f,-56.f); //New Map Boss Front
		//m_pNaviCom->Set_Index(134);// //New Map Boss Front


		m_pTransformCom->Set_Pos(-5.75f, 3.5f, -20.3f); //New Map CocoonFront
		m_pNaviCom->Set_Index(86);// New Map CocoonFront
		Load_Text(L"../../Resource/Data/NavMash/AttachMapNav.txt");


		//m_pTransformCom->Set_Pos(-85.f, 1.3f, 0.01f);Start
		//m_pNaviCom->Set_Index(0);// Base Init Idx 38 
		break;
	case LOAD_NOMAL3:
		m_pMeshCom->Set_AnimationSet(46);
		break;

	case LOAD_PLAYER:
		break;
	case LOAD_MONSTER:
		break;
	case LOAD_BATTLE:
		m_pTransformCom->Set_Pos(-5.75f, 3.5f, -20.3f); //New Map CocoonFront
		m_pNaviCom->Set_Index(86);// New Map CocoonFront
		Load_Text(L"../../Resource/Data/NavMash/AttachMapNav.txt");

		//m_pTransformCom->Set_Pos(-8.f, 0.8f, -2.6f);
		//m_pNaviCom->Set_Index(18);// Base Init Idx 38 
		//Load_Text(L"../../Resource/Data/NavMash/BaseCompleteNav.txt");
		break;
	case LOAD_SLIDE:
		m_pTransformCom->Set_Pos(-14.7f, 2.16f, -20.2f); //Boss
		m_pNaviCom->Set_Index(70);// Base Init Idx 38 
		Load_Text(L"../../Resource/Data/NavMash/Temp5.txt");

		break;

	case LOAD_END:
		break;
	default:
		break;
	}


	m_eCurState = OBJ_START;

	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);

	
	return S_OK;
}

HRESULT CPlayer::LateReady_GameObject(void)
{

	if (m_ppGameObjectMap == nullptr)
		m_ppGameObjectMap = &Engine::Get_Layer(L"GameLogic")->Get_ObjectMap();

	m_pCam = dynamic_cast<CThirdPersonCamera*>(Engine::Get_GameObject(L"UI", L"ThirdPersonCamera"));

	Engine::CGameObject* pGameObject = nullptr;
	Engine::CLayer* pLayer = Engine::Get_Layer(L"UI");


	pGameObject = m_pHPGaugeBar = CGauge::Create(m_pGraphicDev, L"Texture_HPGaugeBar", _vec3(20.f, 80.f, 0.1f), PIVOT_LT);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_HPBar", pGameObject), E_FAIL);

	pGameObject = m_pHPGauge = CGauge::Create(m_pGraphicDev, L"Texture_HPGauge", _vec3(26.f, 86.f, 0.09f), PIVOT_LT, _vec3(0.05f, 0.f, 1.0f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_HPGauge", pGameObject), E_FAIL);

	pGameObject = m_pSPGaugeBar = CGauge::Create(m_pGraphicDev, L"Texture_StaminaGaugeBar", _vec3(20.f, 130.f, 0.1f), PIVOT_LT);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_SPBar", pGameObject), E_FAIL);

	pGameObject = m_pSPGauge = CGauge::Create(m_pGraphicDev, L"Texture_StaminaGauge", _vec3(26.f, 136.f, 0.09f), PIVOT_LT, _vec3(0.05f, 0.f, 1.0f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_SPGauge", pGameObject), E_FAIL);

	pGameObject = CLockOn::Create(m_pGraphicDev, L"LockOnSite");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LockOnUI", pGameObject), E_FAIL);

	pGameObject = m_pShopSub = CShopSub::Create(m_pGraphicDev, L"ShopUI", 2.f, 40.f, true, UI_SHOP_SUB);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShopSubUI", pGameObject), E_FAIL);

	pGameObject = m_pShoplist = CShop::Create(m_pGraphicDev, L"ShopUI3", 2.f, -40.f, false, UI_SHOP);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShopUI", pGameObject), E_FAIL);
	

	pGameObject = m_pInven = CInven::Create(m_pGraphicDev, L"EmptyFrame", 2.f, -40.f, false, UI_INVEN);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"InvenUI", pGameObject), E_FAIL);

	pGameObject = m_pInvenSub = CInvenSub::Create(m_pGraphicDev, L"InvenFrame", 2.f, 40.f, true, UI_INVEN_SUB);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"InvenSubUI", pGameObject), E_FAIL);

	pGameObject = m_pPortal = CPortal::Create(m_pGraphicDev, L"PortalEmpty", 2.f, -40.f, false, UI_PORTAL);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PortalUI", pGameObject), E_FAIL);

	pGameObject = m_pPortalSub = CPortalSub::Create(m_pGraphicDev, L"PortalUI", 2.f, 40.f, true, UI_PORTAL_SUB);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PortalSubUI", pGameObject), E_FAIL);

	pLayer = Engine::Get_Layer(L"GameLogic");
	//// //Sword2
	pGameObject = m_pSword[0] = CSword::Create(m_pGraphicDev, L"SM_NormalGreatSwordA_ba01", 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_SwordA", pGameObject), E_FAIL);
	if (LOAD_MODE != 5)
	{
		pGameObject = m_pSword[1] = CSword::Create(m_pGraphicDev, L"SM_NormalGreatSwordB_ba01", 0);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_SwordB", pGameObject), E_FAIL);

		pGameObject = m_pSword[2] = CSword::Create(m_pGraphicDev, L"SK_NormalHalberdB", 0);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_Halberd", pGameObject), E_FAIL);
	}

	for( int i=0; i>3 ; i++)
		m_pSword[i]->Set_Enable(false);

	//m_pHalberd->Set_Enable(false);
	//m_pHalberd->Set_EquipObject(L"Player");
	/*
	pGameObject  = C3DButton::Create(m_pGraphicDev, L"Select", 1.9f, 40.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Shop_Sub_Select", pGameObject), E_FAIL);
*/


	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{

	PlayerUI();
	UpdateGague(fTimeDelta);

	m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_ATTACK, false);
	if (m_eCurState >= OBJ_HURT_F && m_eCurState <= OBJ_STRONG_HURT_L)
		KnockBack(fTimeDelta);
	else if (m_eCurState == OBJ_GUARD_H)
		Guard_H(fTimeDelta);
	else 
		Key_Input(fTimeDelta);
	
	StateMachine();
	if (m_eCurState >= OBJ_ATTACK && m_eCurState <= OBJ_CHARGE_ATTACK)
	{

		if (m_eCurState == OBJ_ATTACK)
		{
			SetColliderEnable(0.15f, 0.5f);
		}
		else if (m_eCurState == OBJ_CHARGE_ATTACK)
		{
			SetColliderEnable(0.1f, 0.15f);
			SetColliderEnable(0.175f, 0.2f);
			SetColliderEnable(0.225f, 0.25f);
			SetColliderEnable(0.275f, 0.3f);
		}
	}

	if (m_eCurState == OBJ_DODGE)
	{
		if (Get_AniRatio() <= 0.3f)
			m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_HURT, false);
		else
			m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_HURT, true);
	}


	_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	cout << "X=" << vPos.x << "y=" << vPos.y << "Z=" << vPos.z << endl;
	cout << "Cur Cell " << m_pNaviCom->Get_CurIndex() << endl;
	//Guard_H(fTimeDelta);


	Collision_Check(fTimeDelta);

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pMeshCom->Play_Animation(fTimeDelta*m_fAnimSpeed);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	return 0;
}

void CPlayer::Render_GameObject(void)
{
	//m_pSpherColliderCom->Render_Collider(Engine::COLLTYPE::COL_TRUE);
	m_pNaviCom->Render_NaviMesh();

	_matrix mat = m_pTransformCom->m_matWorld;

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	SetUp_ConstantTable(pEffect);

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);

	m_pColliderGroupCom->Render_Collider();
}

_float CPlayer::Get_AniRatio()
{
	return (_float)(m_pMeshCom->Get_TrackPosition() / m_pMeshCom->Get_Period());
}

HRESULT CPlayer::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Player"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);


	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pColliderGroupCom = Engine::CColliderGroup::Create(m_pGraphicDev, m_wstrInstName, m_pTransformCom, m_pMeshCom);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_ColliderGroup", pComponent);
	for(int i=0; i<Engine::COLOPT_END;i++)
		m_pColliderGroupCom->Set_ColliderEnable((Engine::COLLOPTION)i, true);
	m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_ATTACK, false);

	CColliderManager::GetInstance()->Get_ObjCollider(m_pColliderGroupCom, m_ObjName);



	//m_pSpherColliderCom->Set_Radius(14.0f);
	//m_pSpherColliderCom->Set_Pos(_vec3(0.f,250.f,0.f));

	//pComponent=m_pSpherColliderCom = Engine::CSphereColliderCom::Create(m_pGraphicDev,L"Mesh_Player",m_pTransformCom);
	//const Engine::D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("RightHandAttach");

	//m_pSpherColliderCom->Set_DMParentMatrix(pBone);

	//m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_SphereColl", pComponent);


	m_pKeyMgr = CKeyMgr::GetInstance();

	//// collider
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
	//	m_pMeshCom->Get_VtxPos(),
	//	m_pMeshCom->Get_NumVtx(),
	//	m_pMeshCom->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{

	m_dwDirectionFlag = 0;
	m_bIsShift = false;
	_bool Test = false;

	if (m_pKeyMgr->KeyDown(KEY_NUM4))
		m_pRendererCom->Change_RenderMode();

	IdleOption();

	if (m_pKeyMgr->KeyDown(KEY_Q))
	{
		
		m_bIsLockOn ? m_bIsLockOn = false : m_bIsLockOn = true;
		m_pCam->LockOn(m_bIsLockOn);
	}



	if (m_pKeyMgr->KeyPressing(KEY_W))
	{
		m_dwDirectionFlag |= FRONT;
		if (CheckEnableState())
		{
			m_eCurState = OBJ_WALK;
		}
	}

	if (m_pKeyMgr->KeyPressing(KEY_S))
	{
		m_dwDirectionFlag |= BACK;
		if (CheckEnableState())
		{
			m_eCurState = OBJ_WALK;
		}

	}

	if (m_pKeyMgr->KeyPressing(KEY_A))
	{
		m_dwDirectionFlag |= LEFT;
		if (CheckEnableState())
		{
			m_eCurState = OBJ_WALK;
		}

	}
	if (m_pKeyMgr->KeyPressing(KEY_D))
	{
		m_dwDirectionFlag |= RIGHT;
		if (CheckEnableState())
		{
			m_eCurState = OBJ_WALK;
		}

	}
	if (m_pKeyMgr->KeyPressing(KEY_SHIFT))
	{
		if(m_eCurState ==OBJ_WALK)
			m_eCurState = OBJ_RUN;
	}

	if (m_pKeyMgr->KeyDown(KEY_LBUTTON))
	{
		if (m_eCurState == OBJ_ATTACK)
		{
			_double dRatio = m_pMeshCom->Get_TrackPosition() / m_pMeshCom->Get_Period();
			if (dRatio >= 0.2&&dRatio < 0.7)
			{

				m_uiCombo++;
				switch (m_uiCombo)
				{
				case 2:
					m_pMeshCom->Set_AnimationSet(33);
					break;
				case 3:
					m_pMeshCom->Set_AnimationSet(32);
					break;
				case 4:
					m_pMeshCom->Set_AnimationSet(31);
					break;
				case 5:
					m_pMeshCom->Set_AnimationSet(30);
					break;
				case 6:
				{
					m_pMeshCom->Set_AnimationSet(34);
					m_uiCombo = 1;
				}
					break;
				default:
					break;
				}
				m_fCurSP -= 10.f;

			}
		}
		else
		{
			if (m_eCurState == OBJ_DODGE)
			{
				_double dRatio = m_pMeshCom->Get_TrackPosition() / m_pMeshCom->Get_Period();
				if (dRatio >= 0.2&&dRatio < 0.9)
				{
					m_eCurState = OBJ_DODGE_ATTACK;
					m_pMeshCom->Set_AnimationSet(23);

				}

			}
			else
			{

				m_eCurState = OBJ_ATTACK;
				m_pMeshCom->Set_AnimationSet(34);
				m_fCurSP -= 10.f;
				//m_pCam->Set_ShakeTime(2.f);
				m_uiCombo = 1;
			}
		}

	}


	if (m_pKeyMgr->KeyPressing(KEY_ALT))
	{
		
		Guard(fTimeDelta);
		m_dwDirectionFlag = 0;

	}
	
	if (m_pKeyMgr->KeyUp(KEY_ALT))
	{
		m_bIsGuard = false;

		m_eCurState = OBJ_IDLE;
	}


	if (m_pKeyMgr->KeyPressing(KEY_RBUTTON))
	{
		m_eCurState = OBJ_STRONG_ATTACK;

		m_pMeshCom->Set_AnimationSet(28);
		if(m_fChargeTime<1.0f)
		m_fChargeTime += fTimeDelta;
	}
	if (m_pKeyMgr->KeyUp(KEY_RBUTTON))
	{
		if (m_fChargeTime >= 0.75f)
		{
			m_eCurState = OBJ_CHARGE_ATTACK;

		}
		else
			m_eCurState = OBJ_STRONG_ATTACK;

	}
	if (m_pKeyMgr->KeyDown(KEY_SPACE))
	{
		if (m_eCurState != OBJ_DODGE)
		{
			m_eCurState = OBJ_DODGE;
			if (m_dwDodge_DirectionFlag == 0)
				m_dwDodge_DirectionFlag = m_dwDirectionFlag;
		}
	}

	if (m_eCurState >= OBJ_ATTACK && m_eCurState <= OBJ_CHARGE_ATTACK)
	{
		AttackMoveSet(fTimeDelta);
		StorngAttackMoveSet(fTimeDelta);
		ChargeAttackMoveSet(fTimeDelta);
		DodgeAttackMoveSet(fTimeDelta);
		m_dwDirectionFlag = 0;

	}

	

	if (m_pCameraTransformCom != nullptr)
	{
		Check_Direction(fTimeDelta);
	}

}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9*		pLight = Engine::Get_LightInfo(0);
	NULL_CHECK_RETURN(pLight, S_OK);
	pEffect->SetVector("g_vLightDir", &_vec4(pLight->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLight->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Ambient);

	D3DMATERIAL9		tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 100.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);

	_vec4		vCamPos;
	D3DXMatrixInverse(&matView, NULL, &matView);
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec4));

	pEffect->SetVector("g_vCamPos", &vCamPos);
	pEffect->SetFloat("g_fPower", tMtrlInfo.Power);

	return S_OK;

}

HRESULT CPlayer::Load_Text(const TCHAR * pFilePath)
{
	ifstream fin;
	fin.open(pFilePath);
	if (fin.fail())
		return E_FAIL;
	wstring wstrTemp;
	char cTemp[MIN_STR];
	_uint uidx = 0;
	while (!fin.eof())
	{
		fin.getline(cTemp, MIN_STR);
		wchar_t* ppwchar = CharToWChar(cTemp);
		wstrTemp = ppwchar;
		delete ppwchar;
		
		if (wstrTemp.compare(L"") == 0)
			break;

		Engine::NAVI_DATA* pNaviData = new Engine::NAVI_DATA;
		
		pNaviData->vPosition1.x = _wtof(wstrTemp.c_str()); 
		fin.getline(cTemp, MIN_STR); // ������ ������ ���� ����(���� ����)�� �о����.
		pNaviData->vPosition1.y = (_float)atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		pNaviData->vPosition1.z = (_float)atof(cTemp);

		fin.getline(cTemp, MIN_STR); // ������ ������ ���� ����(���� ����)�� �о����.
		pNaviData->vPosition2.x = (_float)atof(cTemp);
		fin.getline(cTemp, MIN_STR); // ������ ������ ���� ����(���� ����)�� �о����.
		pNaviData->vPosition2.y = (_float)atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		pNaviData->vPosition2.z = (_float)atof(cTemp);

		fin.getline(cTemp, MIN_STR); // ������ ������ ���� ����(���� ����)�� �о����.
		pNaviData->vPosition3.x = (_float)atof(cTemp);
		fin.getline(cTemp, MIN_STR); // ������ ������ ���� ����(���� ����)�� �о����.
		pNaviData->vPosition3.y = (_float)atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		pNaviData->vPosition3.z = (_float)atof(cTemp);

		pNaviData->uiIdx = uidx;

		m_pNaviCom->Add_Cell(pNaviData);

		delete pNaviData;
		uidx++;
	}
	fin.close();
	return S_OK;
}

wchar_t * CPlayer::CharToWChar(const char * pstrSrc)
{
	int nLen = strlen(pstrSrc) + 1;
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t)* nLen);
	mbstowcs(pwstr, pstrSrc, nLen);
	return pwstr;
}

void CPlayer::Check_Direction(_float fTimeDelta)
{
	_vec3 vCamLook = *m_pCameraTransformCom->Get_Info(Engine::INFO_LOOK);
	_vec3 vPlayerLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
	_vec3 vPlayerRight = { 1.0f,0.f,0.f };
	vCamLook.y = 0;
	vPlayerLook.y = 0;

	D3DXVec3TransformNormal(&vPlayerRight, &vPlayerRight, &m_pTransformCom->m_matWorld);
	D3DXVec3Normalize(&vCamLook, &vCamLook);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

	float fDgree = Get_Angle(vCamLook, vPlayerLook) + 180.f;

	DWORD dwDirectionFlag;

	if (m_eCurState == OBJ_DODGE)
		dwDirectionFlag = m_dwDodge_DirectionFlag;
	else		
		dwDirectionFlag = m_dwDirectionFlag;
	if (m_bIsLockOn)
	{
		if (m_pCam != nullptr &&m_pCam->Get_MonTransform() !=nullptr)
		{

			_vec3 vPos, vMonPos;

			vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
			memcpy(&vMonPos, &m_pCam->Get_MonTransform()->m_matWorld, sizeof(_vec3));

			_float fTargetDist = D3DXVec3Length(&(vPos - vMonPos));

			if (fTargetDist > 1.f)
				RotateToLook(fTimeDelta);
			_vec3	vDir;

			switch (dwDirectionFlag)
			{
			case DIR_F:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_F;
				vDir = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
				CheckMoveMesh(fTimeDelta, vDir, false, m_fSpeed);
			}
				break;
			case DIR_FR:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_FR;
				vDir = *m_pTransformCom->Get_Info(Engine::INFO_LOOK)+ *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);
			
				CheckMoveMesh(fTimeDelta, vDir, false, m_fSpeed);

			}
				break;
			case DIR_R:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_R;
					//if (CheckEnableState())
				
				vDir = *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);
				CheckMoveMesh(fTimeDelta, vDir, false, m_fSpeed);
			}
				break;
			case DIR_BR:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_BR;
				vDir = -*m_pTransformCom->Get_Info(Engine::INFO_LOOK) + *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);

				CheckMoveMesh(fTimeDelta, vDir, false, m_fSpeed);
			}
				break;
			case DIR_B:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_B;

				vDir = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
				CheckMoveMesh(fTimeDelta, -vDir, false, m_fSpeed);
			}
			break;
			case DIR_BL:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_BL;
				vDir = -*m_pTransformCom->Get_Info(Engine::INFO_LOOK) - *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);

				CheckMoveMesh(fTimeDelta, vDir, false, m_fSpeed);
			}
				break;
			case DIR_L:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_L;

				vDir = *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);
				CheckMoveMesh(fTimeDelta, -vDir, false, m_fSpeed);
			}
				break;
			case DIR_FL:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_FL;
				vDir = *m_pTransformCom->Get_Info(Engine::INFO_LOOK) - *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);

				CheckMoveMesh(fTimeDelta, vDir, false, m_fSpeed);
			}
			break;
			//default:
				//m_eCurState =OBJ_IDLE;

				break;
			}
		}
	}

	else
	{
		switch (dwDirectionFlag)
		{
		case DIR_F:
		{
			if (fDgree > 5.f&&fDgree < 180.f) //�������� ����
				m_pTransformCom->Rotation(Engine::ROT_Y, m_fRotSpeed* fTimeDelta);
			else if (fDgree < 355.f && fDgree >= 180.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

			CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_LOOK, false, m_fSpeed);
		}
		break;
		case DIR_R:
		{
			if (fDgree >= 90.f&&fDgree <= 265.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);
			else if ((fDgree >= 275.f && fDgree <= 360.f) || (fDgree >= 0.f&& fDgree < 90.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);


				_vec3	vRight = *m_pCameraTransformCom->Get_Info(Engine::INFO_RIGHT);
				//D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &m_pCam->Get_LockOnLook()) ;
				CheckMoveMesh(fTimeDelta, vRight, false, m_fSpeed);

		}
		break;
		case DIR_FR:
		{
			if ((fDgree <= 310.f && fDgree >= 135.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);
			else if ((fDgree >= 320.f&&fDgree <= 360.f) || (fDgree >= 0.f&& fDgree < 135.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);

			CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_LOOK, false, m_fSpeed, Engine::INFO_RIGHT, false);
		}
		break;
		case DIR_B:
			if (fDgree > 185.f&& fDgree < 360.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);
			else if (fDgree <= 175.f&&fDgree >= 0.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

			CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_LOOK, true, m_fSpeed);
			break;
		case DIR_BR:
		{
			if ((fDgree >= 45.f&&fDgree <= 220.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);
			else if ((fDgree <= 360.f && fDgree >= 230.f) || (fDgree >= 0.f&& fDgree < 45.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);

			CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_LOOK, true, m_fSpeed, Engine::INFO_RIGHT, false);
		}
		break;
		case DIR_L:
		{
			if (fDgree >= 95.f&&fDgree <= 270.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);
			else if ((fDgree <= 85.f&& fDgree >= 0.f) || (fDgree >= 270.f&& fDgree < 360.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

			CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_RIGHT, false, m_fSpeed);
		}
		break;
		case DIR_FL:
		{
			if ((fDgree >= 50.f&&fDgree <= 225.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);
			else if ((fDgree <= 360.f && fDgree >= 225.f) || (fDgree >= 0.f&& fDgree < 40.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

			CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_LOOK, false, m_fSpeed, Engine::INFO_RIGHT, true);
		}
		break;
		case DIR_BL:
		{
			if (fDgree >= 140.f&& fDgree <= 315.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);
			else if ((fDgree <= 360.f&&fDgree >= 315.f) || (fDgree >= 0.f&& fDgree < 130.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

			CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_LOOK, true, m_fSpeed, Engine::INFO_RIGHT, true);
		}
		break;
		default:
			break;
		}
	}
}

void CPlayer::StateMachine()
{
	_float  fHurtSpeed = 2.5f;
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case OBJ_IDLE:
		{
			m_fAnimSpeed = 1.0f;
			m_fRotSpeed = 4.f;
			m_pMeshCom->Set_AnimationSet(48);
		}
			break;
		case OBJ_WALK:
		{
			m_fAnimSpeed = 2.0f;
			m_fSpeed = 2.0f;
			m_fRotSpeed = 6.f;
			m_pMeshCom->Set_AnimationSet(45);
		}
			break;
		case OBJ_RUN:
		{	
			m_fAnimSpeed = 2.0f;

			m_fSpeed = 10.f;//3.5
			m_fRotSpeed = 6.f;

			if (m_bIsLockOn)
			{
				switch (m_dwDirectionFlag)
				{
				case DIR_F:
					m_pMeshCom->Set_AnimationSet(40);
					break;
				case DIR_R:
					m_pMeshCom->Set_AnimationSet(36);
					break;
				case DIR_FR:
					m_pMeshCom->Set_AnimationSet(36);
					break;
				case DIR_B:
					m_pMeshCom->Set_AnimationSet(42);
					break;
				case DIR_BR:
					m_pMeshCom->Set_AnimationSet(36);
					break;
				case DIR_L:
					m_pMeshCom->Set_AnimationSet(38);
					break;
				case DIR_FL:
					m_pMeshCom->Set_AnimationSet(38);
					break;
				case DIR_BL:
					m_pMeshCom->Set_AnimationSet(38);
					break;
				default:
					break;
				}
			}
			else
			{
				m_pMeshCom->Set_AnimationSet(40);

			}
		}
			break;
		case OBJ_DODGE:
		{
			m_fAnimSpeed = 2.25f;
			m_fSpeed = 10.f;
			m_fRotSpeed = 10.f;
			m_fCurSP -= 10.f;

			if (m_bIsLockOn)
			{
				switch (m_dwDirectionFlag)
				{
				case DIR_F:
					m_pMeshCom->Set_AnimationSet(27);
					break;
				case DIR_R:
					m_pMeshCom->Set_AnimationSet(24);
					break;
				case DIR_FR:
					m_pMeshCom->Set_AnimationSet(24);
					break;
				case DIR_B:
					m_pMeshCom->Set_AnimationSet(26);
					break;
				case DIR_BR:
					m_pMeshCom->Set_AnimationSet(24);
					break;
				case DIR_L:
					m_pMeshCom->Set_AnimationSet(25);
					break;
				case DIR_FL:
					m_pMeshCom->Set_AnimationSet(25);
					break;
				case DIR_BL:
					m_pMeshCom->Set_AnimationSet(25);
					break;
				default:
					break;
				}
			}
			else
			{
				m_pMeshCom->Set_AnimationSet(27);
			}
		}
			break;
		case OBJ_ATTACK:
		{
			m_fAnimSpeed = 1.75f;
		}
			break;
		case OBJ_STRONG_ATTACK:
		{
			m_fAnimSpeed = 2.f;
			m_fCurSP -= 30.f;

			m_pMeshCom->Set_AnimationSet(28);
			m_fChargeTime = 0.f;

		}
			break;
		case OBJ_CHARGE_ATTACK:
		{
			m_fAnimSpeed = 2.f;
			m_pMeshCom->Set_AnimationSet(29);
			m_pMeshCom->Set_AddTrackTime(m_fChargeTime + 0.3f); //0.3f <-���� Animation  ������ 
			m_fChargeTime = 0.f;
		}
			break;
		case OBJ_GUARD:
			m_fAnimSpeed = 1.f;
			m_pMeshCom->Set_AnimationSet(21);
			break;
		case OBJ_GUARD_H:
			m_fAnimSpeed = 1.f;
			m_pMeshCom->Set_AnimationSet(19);
			m_fCurSP -= 30.f;
			break;

		case OBJ_HURT_F://10
			m_fAnimSpeed = fHurtSpeed;
			m_pMeshCom->Set_AnimationSet(18);

			break;
		case OBJ_HURT_FR:
		case OBJ_HURT_R:
		case OBJ_HURT_BR:
			m_fAnimSpeed = fHurtSpeed;
			m_pMeshCom->Set_AnimationSet(16);
			break;
		case OBJ_HURT_B:
			m_fAnimSpeed = fHurtSpeed;
			m_pMeshCom->Set_AnimationSet(17);
			break;
		case OBJ_HURT_BL:
		case OBJ_HURT_L:
		case OBJ_HURT_FL:
			m_fAnimSpeed = fHurtSpeed;
			m_pMeshCom->Set_AnimationSet(15);
			break;

		case OBJ_STRONG_HURT_F:
			m_fAnimSpeed = fHurtSpeed;
			m_pMeshCom->Set_AnimationSet(14);
			break;
		case OBJ_STRONG_HURT_FR:
		case OBJ_STRONG_HURT_R:
		case OBJ_STRONG_HURT_BR:
			m_fAnimSpeed = fHurtSpeed;
			m_pMeshCom->Set_AnimationSet(12);
			break;
		case OBJ_STRONG_HURT_B:
			m_fAnimSpeed = fHurtSpeed;
			m_pMeshCom->Set_AnimationSet(13);
			break;
			m_fAnimSpeed = fHurtSpeed;
		case OBJ_STRONG_HURT_BL:
		case OBJ_STRONG_HURT_L:
		case OBJ_STRONG_HURT_FL://25
			m_fAnimSpeed = fHurtSpeed;
			m_pMeshCom->Set_AnimationSet(11);
			break;
		case OBJ_DEAD:
			break;
		case OBJ_START:
			break;
		case OBJ_LOCK_WALK_F:
		{
			m_fSpeed = 1.5f;
			m_fAnimSpeed = 1.5f;
			m_pMeshCom->Set_AnimationSet(45);
		}
			break;
		case OBJ_LOCK_WALK_FR:
		case OBJ_LOCK_WALK_R:
		case OBJ_LOCK_WALK_BR:
		{
			m_fSpeed = 1.5f;
			m_fAnimSpeed = 1.5f;

			m_pMeshCom->Set_AnimationSet(43);
		}
			break;

		case OBJ_LOCK_WALK_B:
		{
			m_fSpeed = 1.5f;
			m_fAnimSpeed = 1.5f;
			m_pMeshCom->Set_AnimationSet(46);
		}
			break;
		case OBJ_LOCK_WALK_BL:
		case OBJ_LOCK_WALK_L:
		case OBJ_LOCK_WALK_FL:
		{	
			m_fSpeed = 1.5f;
			m_fAnimSpeed = 1.5f;
			m_pMeshCom->Set_AnimationSet(44);
		}
		break;
		case OBJ_DODGE_ATTACK:
		{
			m_fCurSP -= 10.f;

		}
			break;
		case OBJ_CHECKPOINT_START:
			m_pMeshCom->Set_AnimationSet(8);

			break;
		case OBJ_END:
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;

	}

}

void CPlayer::IdleOption()
{
	if (m_eCurState == OBJ_DODGE)
	{
		if (m_pMeshCom->Get_TrackPosition() >= 1.0)
			m_dwDodge_DirectionFlag = 0;
		if (m_pMeshCom->Get_TrackPosition() >= 2.26)
			m_eCurState = OBJ_IDLE;
	}
	else if (m_eCurState != OBJ_IDLE )
	{
		if (m_pMeshCom->Is_AnimationSetEnd())
		{
			m_eCurState = OBJ_IDLE;
			m_uiCombo = 0;
		}
	}

}

void CPlayer::CheckMoveMesh(_float fTimeDelta, Engine::CTransform * pTransform, Engine::INFO eDir, _bool bIsDirRvc, _float fSpeed, Engine::INFO eDir2, _bool bIsDir2Rvc)
{
	_vec3	vPos, vDir, vDiagonalDir, vOutPos;
	
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	pTransform->Get_Info(eDir, &vDir);
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	if (eDir2 == Engine::INFO::INFO_END)
	{
		if (bIsDirRvc)
			m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * fSpeed* fTimeDelta), &vOutPos);
		else
			m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vDir * fSpeed* fTimeDelta), &vOutPos);
		m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);
			

	}
	else
	{
		pTransform->Get_Info(eDir2, &vDiagonalDir);
		D3DXVec3Normalize(&vDiagonalDir, &vDiagonalDir);
		if (!bIsDirRvc)
			vDir *= -1.f;
		if (bIsDir2Rvc)
			vDiagonalDir *= -1.f;

		vDir += vDiagonalDir;
		D3DXVec3Normalize(&vDir, &vDir);
		m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * fSpeed* fTimeDelta), &vOutPos);
		m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);

	}

}

void CPlayer::CheckMoveMesh(_float fTimeDelta, _vec3 vDir, _bool bIsDirRvc, _float fSpeed)
{

	_vec3	vPos, vTempDir, vDiagonalDir, vOutPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	vTempDir = vDir;
	vTempDir.y = 0.f;
	D3DXVec3Normalize(&vTempDir, &vTempDir);

	m_pNaviCom->Move_OnNaviMesh(&vPos, &(vTempDir* fSpeed* fTimeDelta), &vOutPos);

	m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);
	

}

void CPlayer::AttackMoveSet(_float fTimeDelta)
{
	if (m_eCurState == OBJ_ATTACK)
	{
		switch (m_uiCombo)
		{
		case 1:
			MoveAni(fTimeDelta, 0.1f, 0.15f, 4.f, _vec3{ 1.f,1.f,1.f });
			break;
		case 2:
			MoveAni(fTimeDelta, 0.1f, 0.2f, 4.f, _vec3{ 1.f,1.f,1.f });
			break;
		case 3:
			MoveAni(fTimeDelta, 0.18f, 0.3f, 3.f, _vec3{ 1.f,1.f,1.f });
			break;
		case 4:
			MoveAni(fTimeDelta, 0.11f, 0.2f, 3.f, _vec3{ 1.f,1.f,1.f });
			break;
		case 5:
			MoveAni(fTimeDelta, 0.01f, 0.14f, 5.0f, _vec3{ 1.f,1.f,1.f });
			break;
		default:
			break;
		}
	}

}

void CPlayer::StorngAttackMoveSet(_float fTimeDelta)
{
	if (m_eCurState == OBJ_STRONG_ATTACK)
	{
		MoveAni(fTimeDelta, 0.0f, 0.025f, 5.0f, _vec3{ 1.f,1.f,1.f });
		MoveAni(fTimeDelta, 0.25f, 0.285f, 5.0f, _vec3{ 1.f,1.f,1.f });

	}
}

void CPlayer::ChargeAttackMoveSet(_float fTimeDelta)
{
	if (m_eCurState == OBJ_CHARGE_ATTACK)
	{
		MoveAni(fTimeDelta, 0.15f, 0.2f, 4.0f, _vec3{ 1.f,1.f,1.f });

		MoveAni(fTimeDelta, 0.2f, 0.26f, 8.0f, _vec3{ 1.f,1.f,1.f });
	}

}

void CPlayer::DodgeAttackMoveSet(_float fTimeDelta)
{
	if (m_eCurState == OBJ_DODGE_ATTACK)
	{
		MoveAni(fTimeDelta, 0.0f, 0.1f, 2.0f, _vec3{ 1.f,1.f,1.f });

		MoveAni(fTimeDelta, 0.1f, 0.2f, 7.0f, _vec3{ 1.f,1.f,1.f });
		SetColliderEnable(0.1f, 0.5f);

	}
}

void CPlayer::Guard(_float fTimeDelta)
{
	if (m_eCurState == OBJ_GUARD)
	{
		if (m_fCurSP <= 0.f)
		{
			m_eCurState = OBJ_IDLE;
			return;
		}
		if (Get_AniRatio() >= 0.1f)
		{
			m_bIsGuard = true;
			if(m_bIsLockOn)
				RotateToLook(fTimeDelta);
			m_fCurSP -= fTimeDelta*7.f;
		}


	}
	m_eCurState = OBJ_GUARD;
}


void CPlayer::Guard_H(_float fTimeDelta)
{
	if (m_eCurState == OBJ_GUARD_H)
	{
		if (m_fCurSP < 0.f)
		{
			m_eCurState = OBJ_STRONG_HURT_F;
			KnockBack(fTimeDelta);
			return;
		}

		m_pMeshCom->Set_AnimationSet(19);

		if (Get_AniRatio() >= 0.89f)
			m_eCurState = OBJ_GUARD;
		else
		{
			if(Get_AniRatio()<=0.1f)
			CheckMoveMesh(fTimeDelta, m_vKnockBackDir, false, 10.f);
		}
	}
}


void CPlayer::MoveAni(_float fTimeDelta, _float fMinRatio, _float fMaxRatio, _float fSpeed, _vec3 vDir)
{
	if (Get_AniRatio() >= fMinRatio && Get_AniRatio() <= fMaxRatio)
	{
		CheckMoveMesh(fTimeDelta, m_pTransformCom, Engine::INFO_LOOK, true, fSpeed);
	}
}

_float CPlayer::Get_AngleOnTheLook()
{
	_vec3	vPos, vDir, vDiagonalDir, vOutPos, vLook;
	_float	fDgree;
	vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
	vDir = m_pCam->Get_LockOnLook();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	fDgree = Get_Angle(vDir, vLook);

	return fDgree;
}

void CPlayer::RotateToLook(_float fTimeDelta)
{
	_vec3 vRight, vCamLook;
	_float Get_AniRatio(), fDgree, fRotateDir;
	vRight = *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);
	vRight.y = 0.f;
	D3DXVec3Normalize(&vRight, &vRight);

	vCamLook = m_pCam->Get_LockOnLook();
	vCamLook.y = 0.f;
	D3DXVec3Normalize(&vCamLook, &vCamLook);



	fDgree = Get_AngleOnTheLook(); //ĳ���� ��� ī�޶��� �� ���� 

	fRotateDir = Get_Angle(vCamLook, vRight);  //ȸ������ ����


	if (cosf(D3DXToRadian(fDgree)) >= 0.97f)
		return;
	else
	{
		if (cosf(D3DXToRadian(fRotateDir)) >= 0.f)
			m_pTransformCom->Rotation(Engine::ROT_Y, m_fRotSpeed* fTimeDelta);
		else
			m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);
	}

}

_bool CPlayer::CheckEnableState()
{
	if (!(m_eCurState>= OBJ_ATTACK && m_eCurState<= OBJ_RENKETSU_SEARCH)&& m_eCurState != OBJ_DODGE)
		return true;
	else
		return false;

}

void CPlayer::Collision_Check(_float fTimeDelta)
{
	Engine::CColliderGroup* pTargetCollCom = nullptr;
	for (auto &pObject : *m_ppGameObjectMap)
	{
		if (!pObject.second->IsMonster())
			continue;


		pTargetCollCom = dynamic_cast<Engine::CColliderGroup*>(Engine::Get_Component(L"GameLogic",
																					pObject.second->Get_InstName().c_str(),
																					L"Com_ColliderGroup",
																					Engine::ID_DYNAMIC));
		wstring name = pObject.first;

		if (pTargetCollCom != nullptr)
		{
			_float fPower = 0.f;
			_bool bIsAttackColl, bIsStepColl, bIsHurtColl = { false, };
		
			bIsAttackColl = m_pCalculatorCom->Collsion_Sphere(m_pColliderGroupCom->Get_CollVec(Engine::COLOPT_ATTACK),
				m_pColliderGroupCom->Get_ColliderEnable(Engine::COLOPT_ATTACK),
				pTargetCollCom->Get_CollVec(Engine::COLOPT_HURT),
				pTargetCollCom->Get_ColliderEnable(Engine::COLOPT_HURT));
			bIsStepColl = m_pCalculatorCom->Bounding_Sphere(m_pColliderGroupCom->Get_CollVec(Engine::COLOPT_STEP),
				pTargetCollCom->Get_CollVec(Engine::COLOPT_STEP), &fPower);

			
			bIsHurtColl = m_pCalculatorCom->Collsion_Sphere(m_pColliderGroupCom->Get_CollVec(Engine::COLOPT_HURT),
															m_pColliderGroupCom->Get_ColliderEnable(Engine::COLOPT_HURT),
															pTargetCollCom->Get_CollVec(Engine::COLOPT_ATTACK),
															pTargetCollCom->Get_ColliderEnable(Engine::COLOPT_ATTACK));
			m_pColliderGroupCom->Update_Component(fTimeDelta);

			CDynamicObject* pMonster = dynamic_cast<CDynamicObject*>(pObject.second);

			if (pTargetCollCom->IsColl(Engine::COLOPT_HURT, Engine::STATE_ENTER))
			{
				//cout << "���� �߻� " << endl;

				m_pCam->Shake(0.2f,25.f);
				pMonster->HurtMon(m_fDamage, false);

			}		
			/*	if (m_pColliderGroupCom->IsColl(Engine::COLOPT_ATTACK, Engine::STATE_ENTER))
			{
			pMonster->HurtMon(m_fDamage,false);

					cout << "���� �߻� " << endl;
					}
			else if (m_pColliderGroupCom->IsColl(Engine::COLOPT_ATTACK, Engine::STATE_STAY))
			{
				cout << "������ " << endl;
			}
			*/
		/*	if (bIsAttackColl)
			{
				m_pColliderGroupCom->Update_Component(fTimeDelta);

			}*/
			if (pTargetCollCom->IsColl(Engine::COLOPT_HURT, Engine::STATE_EXIT))
			{
				cout << "���� �� " << endl;
			}
			_vec3 vPos, vTargetPos, vOutPos, vDir;
			if (bIsStepColl)
			{
				Engine::CTransform* pTargetTransCom = dynamic_cast<Engine::CTransform*>(pObject.second->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

				vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);   
				vTargetPos = *pTargetTransCom->Get_Info(Engine::INFO_POS);	
				vOutPos;

				vDir = vPos - vTargetPos;
				D3DXVec3Normalize(&vDir, &vDir);
				vDir = vDir*fPower;

				m_pNaviCom->Move_OnNaviMesh(&vPos, &vDir, &vOutPos);

				m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);	//����
			}
			Engine::CTransform* pTargetTransCom = dynamic_cast<Engine::CTransform*>(pObject.second->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

			vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);

			if (pTargetCollCom->IsColl(Engine::COLOPT_ATTACK, Engine::STATE_ENTER))
			{
				for (auto &pColl : *pTargetCollCom->Get_CollVec(Engine::COLOPT_ATTACK)) //������ �ݶ��̴� ��������� 
				{
					if (pColl->IsColl())
					{
						Hurt(vPos, pColl->Get_WorldPos(), 12.f);
						m_pCam->Shake(0.3f,5.f);


					}

				}
			}
			else if (m_pColliderGroupCom->IsColl(Engine::COLOPT_HURT, Engine::STATE_STAY))
			{
				//cout << "�ǰ��� " << endl;

			}
			//if (bIsHurtColl)
			//{
			//	//cout << "�ǰ� �߻� " << endl;
			//	
			//	Engine::CTransform* pTargetTransCom = dynamic_cast<Engine::CTransform*>(pObject.second->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

			//	vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);

			//	if (pTargetCollCom->IsColl(Engine::COLOPT_ATTACK, Engine::STATE_ENTER))
			//	{
			//		for (auto &pColl : *pTargetCollCom->Get_CollVec(Engine::COLOPT_ATTACK)) //������ �ݶ��̴� ��������� 
			//		{
			//			if (pColl->IsColl())
			//			{
			//				Hurt(fTimeDelta, vPos, pColl->Get_WorldPos(), 12.f);
			//	
			//			
			//			}

			//		}
			//	}
			//	else if (m_pColliderGroupCom->IsColl(Engine::COLOPT_HURT, Engine::STATE_STAY))
			//	{
			//		//cout << "�ǰ��� " << endl;

			//	}
			//}
			//if (m_pColliderGroupCom->IsColl(Engine::COLOPT_HURT, Engine::STATE_EXIT))
			//{
			//	//cout << "�ǰ� �� " << endl;
			//}


		}
	}


}

void CPlayer::Hurt(_vec3 vPos, _vec3 vTargetPos, _float fDamage)
{
	// �������� ����ؼ� �˹� �Ÿ�, �ִϸ��̼� ����
	if (m_eCurState>=OBJ_HURT_F&&m_eCurState <= OBJ_STRONG_HURT_L)
	{
		//cout << "�´��� " << endl;
		return;
	}

	m_dwHurtDirectionFlag = 0;
	_vec3 vDir;
	ExtractY_NormalDir(vPos, vTargetPos, &vDir);

	_vec3 vLook=*m_pTransformCom->Get_Info(Engine::INFO_LOOK);
	_vec3 vRight= *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);
	_float fAngle=0.f;
	
	_float fCos60 = 0.15425144988;

	fAngle = Get_Angle(vDir, vLook);

	if (cosf(D3DXToRadian(fAngle)) >= fCos60)
		m_dwHurtDirectionFlag |= BACK;
	else if (cosf(D3DXToRadian(fAngle)) <= -fCos60)
		m_dwHurtDirectionFlag |= FRONT;

	fAngle = Get_Angle(vDir, vRight);
	if (cosf(D3DXToRadian(fAngle)) >= fCos60)
		m_dwHurtDirectionFlag |= LEFT;
	else if (cosf(D3DXToRadian(fAngle)) <= -fCos60)
		m_dwHurtDirectionFlag |= RIGHT;

	//cout << m_dwHurtDirectionFlag << endl;
	m_fCurHP -= 10.f;


	m_vKnockBackDir = vDir;
	_bool bIsStrongAttack = false;
	if (fDamage >= 10.f)
		bIsStrongAttack = true;

	if (m_bIsGuard)
	{
		m_eCurState = OBJ_GUARD_H;
		return;
	}
	switch (m_dwHurtDirectionFlag)	
	{
		
		case DIR_F:
			bIsStrongAttack ? m_eCurState = OBJ_STRONG_HURT_F	: m_eCurState = OBJ_HURT_F;
			break;
		case DIR_FR:
			bIsStrongAttack ? m_eCurState = OBJ_STRONG_HURT_FR	: m_eCurState = OBJ_HURT_FR;
			break;
		case DIR_R:
			bIsStrongAttack ? m_eCurState = OBJ_STRONG_HURT_R	: m_eCurState = OBJ_HURT_R;
			break;
		case DIR_B:
			bIsStrongAttack ? m_eCurState = OBJ_STRONG_HURT_B	: m_eCurState = OBJ_HURT_B;
			break;
		case DIR_BR:
			bIsStrongAttack ? m_eCurState = OBJ_STRONG_HURT_BR	: m_eCurState = OBJ_HURT_BR;
			break;
		case DIR_BL:
			bIsStrongAttack ? m_eCurState = OBJ_STRONG_HURT_BL	: m_eCurState = OBJ_HURT_BL;
			break;
		case DIR_L:
			bIsStrongAttack ? m_eCurState = OBJ_STRONG_HURT_L	: m_eCurState = OBJ_HURT_L;
			break;
		case DIR_FL:
			bIsStrongAttack ? m_eCurState = OBJ_STRONG_HURT_FL	: m_eCurState = OBJ_HURT_FL;
			break;
		default:
			break;
	}
	//cout << m_eCurState << endl;






}

void CPlayer::KnockBack(_float fTimeDelta)
{

	if (m_dwHurtDirectionFlag)
	{
		if (m_eCurState >= OBJ_HURT_F&&m_eCurState <= OBJ_HURT_FL)
		{
			if (Get_AniRatio() >= 0.f&& Get_AniRatio() <= 0.3f)
				CheckMoveMesh(fTimeDelta, m_vKnockBackDir, false, 1.5f);
		}
		else
		{
			if (Get_AniRatio() >= 0.f&& Get_AniRatio() <= 0.3f)
				CheckMoveMesh(fTimeDelta, m_vKnockBackDir, false, 3.5f);
		}

		if (Get_AniRatio() >= 0.86f)
		{

			m_eCurState = OBJ_IDLE;
			m_dwOldHurtDirectionFlag = 0;
			m_dwDodge_DirectionFlag = 0;
			m_dwDirectionFlag = 0;
			m_dwHurtDirectionFlag = 0;
		}
	}		

}

void CPlayer::SetColliderEnable(_float fMin, _float fMax)
{
	if (Get_AniRatio() >= fMin&& Get_AniRatio() <= fMax)
		m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_ATTACK, true);
	else
		m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_ATTACK, false);

}

void CPlayer::PlayerUI()
{
	if (m_pKeyMgr->KeyDown(KEY_X))
	{
		m_eCurState = OBJ_IDLE;
		Engine::CTransform* pDavisTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Davis_0", L"Com_Transform", Engine::ID_DYNAMIC));
		if (pDavisTransform == nullptr)
			return;
		_vec3 vDist = *pDavisTransform->Get_Info(Engine::INFO_POS) - *m_pTransformCom->Get_Info(Engine::INFO_POS);
		_float fLength = D3DXVec3Length(&vDist);
		if (fLength < 1.5f)
		{
			m_pShopSub->ChangeEnable(true);
			m_pShoplist->ChangeEnable(true);
			m_bIsLockOn = true;
		}
		else
		{
			m_pShopSub->ChangeEnable(false);
			m_pShoplist->ChangeEnable(false);
			m_bIsLockOn = false;

		}
	}
	if (m_pKeyMgr->KeyDown(KEY_I))
	{
		m_eCurState = OBJ_IDLE;
		m_pInvenSub->ChangeEnable();
		m_pInven->ChangeEnable();
		m_bIsLockOn = !m_bIsLockOn;
	}
	
	if (m_pKeyMgr->KeyDown(KEY_O))
	{
		m_eCurState = OBJ_IDLE;
		m_pPortal->ChangeEnable();
		m_pPortalSub->ChangeEnable();
		m_bIsLockOn = !m_bIsLockOn;
	}

	if (m_pKeyMgr->KeyDown(KEY_RETURN))
	{
		wstring wstrItem;
		if (m_pShoplist->IsOn())
		{
			wstrItem = m_pShoplist->Get_ItemName();
			if (wstrItem.find(L"��") != wstring::npos)
			{
				if (m_pShopSub->Get_ItemName().find(L"����") != wstring::npos)
					AddItem_Inventory(wstrItem);
				else if (m_pShopSub->Get_ItemName().find(L"�Ǹ�") != wstring::npos)
					DeleteItem_Inventory(wstrItem);
				else if (m_pShopSub->Get_ItemName().find(L"��ȭ") != wstring::npos)
					EnhanceItem_Inventory(wstrItem);

			}

		}
		else if (m_pInven->IsOn())
		{
			wstrItem = m_pInven->Get_ItemName();
			if (wstrItem.find(L"��") != wstring::npos)
			{
				if (m_pInvenSub->Get_ItemName().find(L"����") != wstring::npos)
				{
					if (wstrItem.find(L"����") != wstring::npos)
					{
						m_pSword[0]->Set_Enable(true);
						m_pSword[1]->Set_Enable(false);
						m_pSword[2]->Set_Enable(false);

					}
					else if (wstrItem.find(L"�����") != wstring::npos)
					{
						m_pSword[0]->Set_Enable(false);
						m_pSword[1]->Set_Enable(true);
						m_pSword[2]->Set_Enable(false);

					}
					else if (wstrItem.find(L"������") != wstring::npos)
					{
						m_pSword[0]->Set_Enable(false);
						m_pSword[1]->Set_Enable(false);
						m_pSword[2]->Set_Enable(true);
					}

				}
			}
		}
		else if (m_pPortal->IsOn())
		{
			
			if (m_pPortal->Get_PortalIdx() == 1)
			{
				if (m_pPortalSub->Get_ItemName().find(L"�̵�") != wstring::npos)
				{
					m_bIsSceneChangeFlag = true;
				}

			}
		}

	}


}

void CPlayer::UpdateGague(_float fTimeDelta)
{
	//if (CKeyMgr::GetInstance()->KeyDown(KEY_G))
	//	m_fCurHP -= 10.f;
	//if (CKeyMgr::GetInstance()->KeyDown(KEY_H))
	//	m_fCurSP -= 10.f;




	if (m_fMaxSP > m_fCurSP)
		m_fCurSP += fTimeDelta*5.f;
	else
		m_fCurSP = m_fMaxSP;
	if (m_pHPGauge!=nullptr&&m_pSPGauge != nullptr)
	{
		m_pHPGauge->Set_GaugeRatio(m_fCurHP / m_fMaxHP);
		m_pSPGauge->Set_GaugeRatio(m_fCurSP / m_fMaxSP);
	}
}

void CPlayer::AddItem_Inventory(wstring wstrName)
{
	_bool	bIsFind = false;
	for (auto &Item : m_InventoryVec)
	{
		if (Item.first.compare(wstrName) ==0)
		{
			Item.second++;
			bIsFind = true;
		}
	}
	if (!bIsFind)
		m_InventoryVec.push_back(make_pair(wstrName, 1));

	//if (m_InventoryVec.find(wstrName)== m_InventoryVec.end())
	//{
	//	m_InventoryVec.insert(pair<wstring, _uint>(wstrName, 1));
	//}
	//else
	//{
	//	m_InventoryVec.find(wstrName)->second++;
	//}
}

void CPlayer::DeleteItem_Inventory(wstring wstrName)
{
	auto InvenItr = m_InventoryVec.begin();
	_bool	bIsFind = false;

	for (auto &Item : m_InventoryVec)
	{
		if (Item.first.compare(wstrName)==0)
		{
			Item.second--;
			if (Item.second <= 0)
			{
				InvenItr = m_InventoryVec.erase(InvenItr);
				return;
			}
		}
		InvenItr++;
	}

	/*if (m_InventoryVec.find(wstrName) == m_InventoryVec.end())
		return;
	else
	{
		_uint uiVal= m_InventoryVec.find(wstrName)->second--;
		uiVal = 0;
		m_InventoryVec.erase(m_InventoryVec.find(wstrName));
	}*/
}

void CPlayer::EnhanceItem_Inventory(wstring wstrName)
{
	wstring wstrEnhaceIdx, wstrEnhance;
	_uint uiEnhanceNum, uiFindNum;

	if (wstrName.find(L'+') == wstring::npos)
	{
		wstrEnhance = wstrName + L"+" + to_wstring(1);

	}
	else
	{
		uiFindNum = wstrName.find(L'+');
		wstrEnhaceIdx = wstrName.substr(uiFindNum+1);
		uiEnhanceNum = _wtoi(wstrEnhaceIdx.c_str());
		wstrEnhance = wstrName.substr(0, uiFindNum);
		wstrEnhance = wstrEnhance + L"+" + to_wstring(uiEnhanceNum + 1);
	}

	_bool	bIsFind = false;
	
	for (auto &Item : m_InventoryVec) //��ȭ�Ⱦ������߰�
	{
		if (Item.first.compare(wstrEnhance) ==0)
		{
			Item.second++;
			bIsFind = true;
		}
	}
	if (!bIsFind)
		m_InventoryVec.push_back(make_pair(wstrEnhance, 1));


	auto InvenItr = m_InventoryVec.begin();
	for (auto &Item : m_InventoryVec)
	{
		if (Item.first.compare(wstrName) ==0)
		{
			Item.second--;
			if (Item.second <= 0)
			{
				wstring wstrTemp = InvenItr->first;
				wcout << wstrTemp << L"erase " << endl;
				InvenItr = m_InventoryVec.erase(InvenItr);
				break;
			}
		}
		else
			InvenItr++;
	}// �ϳ����� 



}


CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiIdx , _uint uiStageIdx)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev, uiIdx, uiStageIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free(void)
{
	Engine::CGameObject::Free();
}

float CPlayer::Get_Angle(const D3DXVECTOR3& a, const D3DXVECTOR3& b)
{
	float fRadian = acosf(D3DXVec3Dot(&a, &b) / (D3DXVec3Length(&a) * D3DXVec3Length(&b)));

	fRadian = D3DXToDegree(fRadian);
	float fDgree = (a.x * b.z - a.z * b.x > 0.0f) ? fRadian : -fRadian;

	return fDgree;

}

#include "stdafx.h"
#include "CocoonDevil.h"
#include "Export_Function.h"
#include "ColliderManager.h"
#include "Shield.h"
#include "Projectile.h"

CCocoonDevil::CCocoonDevil(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx, _uint uiStageIdx )
	: CDynamicObject(pGraphicDev,wstrName,uiIdx, uiStageIdx)
{
	m_bIsMonster = true;

}

CCocoonDevil::CCocoonDevil(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo, _uint uiStageIdx)
	:CDynamicObject(pGraphicDev, wstrName, uiIdx, tInfo, uiStageIdx)

{
	m_bIsMonster = true;
}

CCocoonDevil::CCocoonDevil(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, _vec3 vPos, _uint uiCellIdx)
	:CDynamicObject(pGraphicDev, wstrName, uiIdx, 0)
{
	m_bIsMonster = true;
	m_uiCellIdx = uiCellIdx;
	m_vRespawn = vPos;
}

CCocoonDevil::~CCocoonDevil()
{
}

HRESULT CCocoonDevil::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fCurHp = m_fMaxHp =30.f;
	m_fAttackRange = 10.f;
	Set_TransformData();

	m_eCurState = CO_Fight_Idle;
	m_ePreState = CO_END;

	//if (LOAD_MODE != 0)
	//	m_uiStageIdx = LOAD_MODE;

	//if (LOAD_MODE)
	//{
	//	switch ((LOADMODE)LOAD_MODE)
	//	{

	//	case LOAD_BATTLE:
	//		//눈맵 플레이어바로앞
	//		m_pTransformCom->Set_Pos(-12.32f, 2.73f, -25.3241f);
	//		m_pNaviCom->Set_Index(76);// Base Init Idx 38 
	//		Load_Text(L"../../Resource/Data/NavMash/Temp5.txt");
	//		break;


	//	case LOAD_END:
	//		break;
	//	default:
	//		break;
	//	}
	//}
	//else
	{
		Load_Text(L"../../Resource/Data/NavMash/Temp5.txt");
		m_pTransformCom->Set_Pos(m_vRespawn.x, m_vRespawn.y, m_vRespawn.z);
		m_pNaviCom->Set_Index(m_uiCellIdx);// Base Init Idx 38 
	}


	return S_OK;
}

HRESULT CCocoonDevil::LateReady_GameObject()
{
	m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));

	if (m_ppGameObjectMap == nullptr)
		m_ppGameObjectMap = &Engine::Get_Layer(L"GameLogic")->Get_ObjectMap();
	
	if (m_pParentBoneMatrix == nullptr)
		m_pParentBoneMatrix = &m_pMeshCom->Get_FrameByName("Head")->CombinedTransformationMatrix;
	
	

	return S_OK;
}

_int CCocoonDevil::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsMonDead)
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	
		return S_OK;
	}


	m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_ATTACK, false);

	srand((unsigned int)time(NULL));
	m_fDistance = Get_TargetDist();
	KnockBack(fTimeDelta);
	StateMachine();

	Pattern(fTimeDelta);

	Collision_Check(fTimeDelta);

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pMeshCom->Play_Animation(m_fAnimSpeed*fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return S_OK;
}

void CCocoonDevil::Render_GameObject(void)
{

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

HRESULT CCocoonDevil::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	//pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"RedDevil"));
	pComponent = m_pMeshCom =Engine::CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/DynamicMesh/CocoonDevil/",L"CocoonDevil.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);// 변경

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
	for (int i = 0; i < Engine::COLOPT_END; i++)
		m_pColliderGroupCom->Set_ColliderEnable((Engine::COLLOPTION)i, true);

	pComponent = m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(Engine::CNaviMesh::Create(m_pGraphicDev));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);

	CColliderManager::GetInstance()->Get_ObjCollider(m_pColliderGroupCom, m_ObjName);

	return S_OK;
}

void CCocoonDevil::Set_TransformData()
{
	if (m_bIsRespawn)
	{
		m_pTransformCom->m_vScale = m_tTransformInfo.vScale;
		m_pTransformCom->m_vAngle = m_tTransformInfo.vRotation;
		m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_tTransformInfo.vPosition;
	}
	else
	{
		m_pTransformCom->Set_Scale(DEFAULT_MESH_SCALE);
		m_pTransformCom->Set_Pos(INIT_VEC3);
	}
}

void CCocoonDevil::StateMachine()
{
	if (m_ePreState != m_eCurState)
	{
		m_ePreState = m_eCurState;
		m_fAnimSpeed = 1.0f;

		switch (m_eCurState)
		{
			m_fAnimSpeed = 3.0f;
		case CO_Damage_B:
			m_pMeshCom->Set_AnimationSet(44);
			break;
		case CO_Damage_F:
			m_pMeshCom->Set_AnimationSet(45);
			break;
		case CO_Death:
			m_fAnimSpeed = 2.0f;

			m_pMeshCom->Set_AnimationSet(46);
			break;
		case CO_NoneFight_Idle:
			m_pMeshCom->Set_AnimationSet(47);
			break;
		case CO_Fight_Idle:
			m_pMeshCom->Set_AnimationSet(47);
			break;
		case CO_Attack_Shoot_N:
			m_fAnimSpeed = 2.f;
			m_pMeshCom->Set_AnimationSet(48);
			break;
		case CO_Attack_Mist_N:
			m_fAnimSpeed = 2.0f;

			m_pMeshCom->Set_AnimationSet(49);
			break;
		case CO_END:
			break;
		default:
			break;
		}

	}
	

}

void CCocoonDevil::Pattern(_float fTimeDelta)
{
	switch (m_eCurState)
	{
	case CO_Damage_B:
	case CO_Damage_F:
		Hurt(fTimeDelta);
		break;
	case CO_Death:
		Death(fTimeDelta);
		break;
	case CO_NoneFight_Idle:
	case CO_Fight_Idle:
		Idle(fTimeDelta);
		break;

	case CO_Attack_Shoot_N:
		Shoot(fTimeDelta);
		break;
	case CO_Attack_Mist_N:
		m_fAttackRange = 7.0f;
		Mist(fTimeDelta);
		break;
	default:
		m_fAnimSpeed = 2.0f;
		Idle(fTimeDelta);
		break;
	}
	
}


void CCocoonDevil::KnockBack(_float fTimeDelta)
{
	if (m_bIsHurt)
	{
		m_dwHurtDirectionFlag = 0;
		_vec3 vDir;
		ExtractY_NormalDir(Get_Pos(), Get_TargetPos(), &vDir);


		_vec3 vLook = Get_Pos();
		_vec3 vRight = Get_Right();
		_float fAngle = 0.f;
		_float fCos60 = 0.15425144988;

		fAngle = Get_Angle(vDir, vLook);
		

		if (cosf(D3DXToRadian(fAngle)) >= fCos60)
			m_dwHurtDirectionFlag |= FRONT;
		else if (cosf(D3DXToRadian(fAngle)) <= -fCos60)
			m_dwHurtDirectionFlag |= BACK;

		fAngle = Get_Angle(vDir, vRight);
		if (cosf(D3DXToRadian(fAngle)) >= fCos60)
			m_dwHurtDirectionFlag |= LEFT;
		else if (cosf(D3DXToRadian(fAngle)) <= -fCos60)
			m_dwHurtDirectionFlag |= RIGHT;

		if (m_eCurState >= CO_Damage_B && m_eCurState <= CO_Damage_F)
			m_pMeshCom->Set_AnimationSet(47);

		if(m_fCurHp > 0.f )
		{
			switch (m_dwHurtDirectionFlag)
			{
			case DIR_F:
			case DIR_FR:
			case DIR_R:
			case DIR_L:
			case DIR_FL:
				
				m_eCurState = CO_Damage_B;
				m_pMeshCom->Set_AnimationSet(44);
				break;
			case DIR_B:
			case DIR_BR:
			case DIR_BL:
				m_eCurState = CO_Damage_F;
				m_pMeshCom->Set_AnimationSet(45);
				break;



			}
		}
		else
		{
			m_eCurState = CO_Death;
		}
		m_bIsHurt = false;
	}
}

void CCocoonDevil::Chaing_Target(_float fTimeDelta)
{
	if (m_pNaviCom != nullptr)
	{
			_vec3 vDir, vOutPos;
			_float	fDgree = Get_AngleOnTheTarget();
			vDir = Get_TargetPos() - Get_Pos();
			m_fDistance = D3DXVec3Length(&vDir);
			vDir.y = 0.f;
			D3DXVec3Normalize(&vDir, &vDir);
			RotateToTarget(fTimeDelta, 0.0f);

			m_pNaviCom->Move_OnNaviMesh(&Get_Pos(), &(vDir * m_fSpeed* fTimeDelta), &vOutPos);
			m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);
	}

}

void CCocoonDevil::Shoot(_float fTimeDelta)
{
	if (m_eCurState == CO_Attack_Shoot_N)
	{
		m_fAttackRange = 10.f;
		SetColliderEnable(0.05f, 0.5f);
		if (Get_AniRatio() >= 0.9f)
		{
			m_fRotSpeed = 2.0f;
			m_eCurState = CO_Fight_Idle;
		
		
		}
		else
		{
			if (Get_AniRatio() >= 0.76f && !m_bIsShoot)
			{
				m_bIsShoot = true;
				_matrix matWorld;
				m_pTransformCom->Get_WorldMatrix(&matWorld);
				_matrix matShoot = *m_pParentBoneMatrix * matWorld;
				memcpy(&m_vShootPos, &matShoot._41, sizeof(_vec3));

				CGameObject* pGameObject= CProjectile::Create(m_pGraphicDev, m_vShootPos);
				wstring wstrProjName = m_wstrInstName +L"_"+ to_wstring(m_uiShootCount);
				m_ppGameObjectMap->emplace(wstrProjName, pGameObject);
				m_uiShootCount++;
			}
			
			RotateToTarget(fTimeDelta, 0.f, 0.6f);

		}
	}
}




void CCocoonDevil::Mist(_float fTimeDelta)
{
	if (m_eCurState == CO_Attack_Mist_N)
	{
		m_fAttackRange = 7.f;
		SetColliderEnable(0.3f, 0.5f);

		if (Get_AniRatio() >= 0.8f)
		{
			m_eCurState = CO_Fight_Idle;
		}
		else
		{
			m_fRotSpeed = 3.0f;
			RotateToTarget(fTimeDelta, 0.f, 0.35f);

		}
	}
}

void CCocoonDevil::Idle(_float fTimeDelta)
{
	m_fAttackRange = 12.0f;

	if (m_eCurState == CO_Fight_Idle)
	{
		m_bIsShoot = false;
		if (m_fDistance >= m_fAttackRange)
		{
			return;
		}
		else
		{
			if (m_fDistance >= 6.f)
				m_eCurState = CO_Attack_Shoot_N;
			else
				m_eCurState = CO_Attack_Mist_N;
		}
		
	}

	
}

void CCocoonDevil::Collision_Check(_float fTimeDelta)
{
	Engine::CColliderGroup* pTargetCollCom = nullptr;
	Engine::CTransform* pTargetTransCom = nullptr;
	for (auto &pObject : *m_ppGameObjectMap)
	{
		if (!pObject.second->IsMonster())
			continue;
		if (m_wstrInstName.compare(pObject.first) == 0)
			continue;
		pTargetTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic",
																pObject.second->Get_InstName().c_str(),
																L"Com_Transform",
																Engine::ID_DYNAMIC));
		_vec3 vPos, vTargetPos , vDir;
		vPos = Get_Pos();
		vTargetPos = *pTargetTransCom->Get_Info(Engine::INFO_POS);
		vDir = vPos - vTargetPos;
		_float fLen = D3DXVec3Length(&vDir);
		
		if (fLen >= 5.f)
			continue;

		pTargetCollCom = dynamic_cast<Engine::CColliderGroup*>(Engine::Get_Component(L"GameLogic",
																pObject.second->Get_InstName().c_str(),
																L"Com_ColliderGroup",
																Engine::ID_DYNAMIC));
		if (pTargetCollCom != nullptr)
		{
			_float fPower = 0.f;
			_bool bIsStepColl = false;
			_vec3 vOutPos;

			bIsStepColl = m_pCalculatorCom->Bounding_Sphere(m_pColliderGroupCom->Get_CollVec(Engine::COLOPT_STEP),
				pTargetCollCom->Get_CollVec(Engine::COLOPT_STEP), &fPower);
	
			if (bIsStepColl)
			{
				pTargetTransCom = dynamic_cast<Engine::CTransform*>(pObject.second->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

				vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
				vTargetPos = *pTargetTransCom->Get_Info(Engine::INFO_POS);

				D3DXVec3Normalize(&vDir, &vDir);
				vDir = vDir*fPower;

				m_pNaviCom->Move_OnNaviMesh(&vPos, &vDir, &vOutPos);

				m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);	//적용
			}

		}


	}
}

void CCocoonDevil::Hurt(_float fTimeDelta)
{
	if (m_eCurState == CO_Damage_B || m_eCurState == CO_Damage_F)
	{
		if (Get_AniRatio()>=0.8f)
		{
			m_eCurState = CO_Fight_Idle;


		}

	}
}

void CCocoonDevil::Death(_float fTimeDelta)
{
	if (Get_AniRatio() > 0.8f)
	{
		m_bIsMonDead = true;
	
		for (int i = 0; i < Engine::COLOPT_END; i++)
			m_pColliderGroupCom->Set_ColliderEnable((Engine::COLLOPTION)i, false);
	
	}
}

CCocoonDevil * CCocoonDevil::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx, _uint uiStageIdx)
{
	CCocoonDevil*	pInstance = new CCocoonDevil(pGraphicDev, wstrName, uiIdx, uiStageIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
CCocoonDevil * CCocoonDevil::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo, _uint uiStageIdx)
{
	CCocoonDevil*	pInstance = new CCocoonDevil(pGraphicDev, wstrName, uiIdx, tInfo, uiStageIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;

}
CCocoonDevil * CCocoonDevil::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, _vec3 vPos, _uint uiCellIdx)
{
	CCocoonDevil*	pInstance = new CCocoonDevil(pGraphicDev, wstrName, uiIdx, vPos, uiCellIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
void CCocoonDevil::Free(void)
{
	Engine::CGameObject::Free();
}

// ../Code/MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "ObjectTool.h"
#include "CameraTool.h"
#include "NavMeshTool.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "ColliderTool.h"
// CMyForm
IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{
}

CMyForm::~CMyForm()
{
	//Engine::Safe_Release(m_pDevice);
	//Engine::Safe_Release(m_pDeviceClass);


	Safe_Delete(m_pObjectTool);
	Safe_Delete(m_pCameraTool);
	Safe_Delete(m_pNavMeshTool);

}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB2, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CMyForm::OnTcnSelchangeTab2)
END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_TabCtrl.InsertItem(0, _T("Object"));
	m_TabCtrl.InsertItem(1, _T("Camera"));
	m_TabCtrl.InsertItem(2, _T("NavMesh"));
	m_TabCtrl.InsertItem(3, _T("Collider"));
	m_TabCtrl.GetCurSel();
	CRect rect;
	m_TabCtrl.GetWindowRect(&rect);


	//m_pCameraTab = new CCameraTab;
	if (m_pDeviceClass == nullptr)
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		m_pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(m_pToolView);
		m_pDeviceClass = m_pToolView->GetDeviceClass();
		m_pDevice = m_pToolView->GetDevice();
		m_pScene = m_pToolView->GetScene();

	}
	m_pObjectTool = new CObjectTool;
	m_pObjectTool->Create(IDD_DIALOG1, &m_TabCtrl);
	m_pObjectTool->MoveWindow(0, 21, rect.Width(), rect.Height());
	m_pObjectTool->ShowWindow(SW_HIDE);
	
	m_pCameraTool = new CCameraTool;
	m_pCameraTool->Create(IDD_DIALOG2, &m_TabCtrl);
	m_pCameraTool->MoveWindow(0, 21, rect.Width(), rect.Height());
	m_pCameraTool->ShowWindow(SW_HIDE);

	m_pNavMeshTool = new CNavMeshTool;
	m_pNavMeshTool->Create(IDD_DIALOG3, &m_TabCtrl);
	m_pNavMeshTool->MoveWindow(0, 21, rect.Width(), rect.Height());
	m_pNavMeshTool->ShowWindow(SW_HIDE);

	m_pColliderTool = new CColliderTool;
	m_pColliderTool->Create(IDD_DIALOG4, &m_TabCtrl);
	m_pColliderTool->MoveWindow(0, 21, rect.Width(), rect.Height());
	m_pColliderTool->ShowWindow(SW_HIDE);


	m_TabCtrl.SetCurSel(0);
	m_pObjectTool->ShowWindow(SW_SHOW);


}


void CMyForm::OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult)
{
	//// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iSel = m_TabCtrl.GetCurSel();
	switch (iSel)
	{
	case 0:
		m_pObjectTool->ShowWindow(SW_SHOW);
		m_pCameraTool->ShowWindow(SW_HIDE);
		m_pNavMeshTool->ShowWindow(SW_HIDE);
		m_pColliderTool->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pObjectTool->ShowWindow(SW_HIDE);
		m_pCameraTool->ShowWindow(SW_SHOW);
		m_pNavMeshTool->ShowWindow(SW_HIDE);
		m_pColliderTool->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_pObjectTool->ShowWindow(SW_HIDE);
		m_pCameraTool->ShowWindow(SW_HIDE);
		m_pNavMeshTool->ShowWindow(SW_SHOW);
		m_pColliderTool->ShowWindow(SW_HIDE);
		break;
	case 3:
		m_pObjectTool->ShowWindow(SW_HIDE);
		m_pCameraTool->ShowWindow(SW_HIDE);
		m_pNavMeshTool->ShowWindow(SW_HIDE);
		m_pColliderTool->ShowWindow(SW_SHOW);
		break;
	//default:
	//	break;
	}

	*pResult = 0;

}

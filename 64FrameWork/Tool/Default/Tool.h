
// Tool.h : Tool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CToolApp:
// �� Ŭ������ ������ ���ؼ��� Tool.cpp�� �����Ͻʽÿ�.
//
class CToolView;
class CObjectTool;
class CNavMeshTool;
class CColliderTool;
class CToolApp : public CWinApp
{
public:
	CToolApp();


// �������Դϴ�.
public:
	virtual BOOL		InitInstance();
	virtual int			ExitInstance();

// �����Դϴ�.
	afx_msg void		OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL		OnIdle(LONG lCount);

private:
	CToolView*			m_pToolView;
	CObjectTool*		m_pObjectTool;
	CNavMeshTool*		m_pNavMeshTool;
	CColliderTool*		m_pColliderTool;
};

extern CToolApp theApp;

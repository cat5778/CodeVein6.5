// ../Code/CameraTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "../Code/CameraTool.h"
#include "afxdialogex.h"


// CCameraTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCameraTool, CDialogEx)

CCameraTool::CCameraTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CCameraTool::~CCameraTool()
{
	//Engine::Safe_Release(m_pDevice);
	//Engine::Safe_Release(m_pDeviceClass);

}

void CCameraTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraTool, CDialogEx)
END_MESSAGE_MAP()


// CCameraTool �޽��� ó�����Դϴ�.

#include "stdafx.h"
#include "DirectoryMgr.h"


CDirectoryMgr::CDirectoryMgr()
{
}


CDirectoryMgr::~CDirectoryMgr()
{
}

//CString CDirectoryMgr::ConvertRelativePath(const TCHAR * pFullPath)
//{
//	// From���� To�� ���� ����θ� Output�� �����ϴ� �Լ�.
//	//::PathRelativePathTo(Output, From, FILE_ATTRIBUTE_DIRECTORY, To, FILE_ATTRIBUTE_DIRECTORY);
//	
//	TCHAR szCurrent[MAX_STR] = L"";
//	TCHAR szRelative[MAX_STR] = L"";
//	
//	::GetCurrentDirectory(MAX_STR, szCurrent);
//	::PathRelativePathTo(szRelative, szCurrent, FILE_ATTRIBUTE_DIRECTORY, pFullPath, FILE_ATTRIBUTE_DIRECTORY);
//
//
//
//	return CString(szRelative);
//}

void CDirectoryMgr::ExtractPathInfo(const TCHAR * pFullPath, list<MESH_PATH*>& rPathInfoList)
{
	CString strFindPath = CString(pFullPath) + L"\\*.*";

	// ���� Ž�� ���� MFC Ŭ����.
	CFileFind	find;

	// ����� �˻��ؼ� �����Ѵٸ� TRUE�� ��ȯ�Ѵ�.
	// ���� "Test\\*.*" ���·� ���ڸ� �����ϸ� Test���� �ȿ� ���� �ϳ��� �����ϸ� TRUE ��ȯ.
	BOOL bIsFind = find.FindFile(strFindPath);

	while (bIsFind)
	{
		// FindFile �Լ� ȣ�� �� ��󿡰� �����ϰ��� �Ѵٸ� �ݵ�� FindNextFile�Լ��� ȣ�����־�� �Ѵ�.
		// ���� FindFile �Լ��� "Test\\*.*" ���·� ���ڸ� ������ ��쿡 FindNextFile�Լ��� ȣ���ϸ�
		// ���� ù��° ��󿡰� ������ �� �ִ�.
		bIsFind = find.FindNextFile(); // ����� ���ų� ������ ��󿡰� ������ ��� FALSE�� ��ȯ.

		if (find.IsDots()) // ������ ����� .�̳� ..�� ���� ��Ŀ��� �ǳʶ�.
			continue;
		else if (find.IsDirectory()) // ������
			ExtractPathInfo(find.GetFilePath(), rPathInfoList);
		else // �����̳�
		{
			if (find.IsSystem()) // �ý��� �����̸� �ǳʶ�.
				continue;

			MESH_PATH* pPathInfo = new MESH_PATH;

			// ���� ���� ����.
			TCHAR szFilePath[MAX_STR] = L"";
			lstrcpy(szFilePath, find.GetFilePath());
			//::PathRemoveFileSpec(szFilePath);

			//pPathInfo->iImgCount = GetFileCount(szFilePath);

			// ��� ��� ����
			CString strRelative = ConvertRelativePath(find.GetFilePath());
			TCHAR tcConvert[MAX_STR] = L"";
			lstrcpy(tcConvert, strRelative);
			::PathRemoveFileSpec(tcConvert);
			strRelative = tcConvert;
			pPathInfo->wstrRelative = strRelative;
			pPathInfo->wstrRelative += L"\\";

			strRelative.Replace(L"0.", L"%d."); // (old, new)
			//::PathRemoveFileSpec(szFilePath);

			lstrcpy(szFilePath, strRelative);
			if (pPathInfo->wstrRelative.find(L"DynamicMesh") != wstring::npos) //DynamicMesh
			{
				// MeshName
				pPathInfo->wstrName = ::PathFindFileName(szFilePath);
				pPathInfo->wstrName += L".X";
				pPathInfo->wstrObjectType = ::PathFindFileName(szFilePath);
				//::PathRemoveFileSpec(szFilePath);

				// MeshName
				//::PathRemoveFileSpec(szFilePath);
				//pPathInfo->wstrObjectType = ::PathFindFileName(szFilePath);

				// MeshType
				::PathRemoveFileSpec(szFilePath);
				pPathInfo->wstrMeshType = ::PathFindFileName(szFilePath);

			}
			else //StaticMesh
			{
				// MeshName
				//::PathRemoveFileSpec(szFilePath);
				pPathInfo->wstrName = ::PathFindFileName(szFilePath);
				pPathInfo->wstrName += L".X";
				pPathInfo->wstrObjectType = ::PathFindFileName(szFilePath);

				// MeshType
				pPathInfo->wstrMeshType = ::PathRemoveFileSpec(szFilePath);

				// MeshGroup 
				pPathInfo->wstrGroup = ::PathFindFileName(szFilePath);

				// MeshMap 
				::PathRemoveFileSpec(szFilePath);
				pPathInfo->wstrMap = ::PathFindFileName(szFilePath);

				// MeshType 
				::PathRemoveFileSpec(szFilePath);
				pPathInfo->wstrMeshType = ::PathFindFileName(szFilePath);
			}


			rPathInfoList.push_back(pPathInfo);
			break;
		}
	}

}

int CDirectoryMgr::GetFileCount(const TCHAR * pFullPath)
{
	CString strFindPath = CString(pFullPath) + L"\\*.*";

	CFileFind	find;
	int iCount = 0;

	BOOL bIsFind = find.FindFile(strFindPath);

	while (bIsFind)
	{
		bIsFind = find.FindNextFile();

		if (find.IsDots() || find.IsDirectory() || find.IsSystem())
			continue;

		++iCount;
	}

	return iCount;

}

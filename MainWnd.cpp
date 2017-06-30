#include "StdAfx.h"
#include "MainWnd.h"
#include "helper/SMenu.h"
#include <helper/SMenuEx.h>


#include <ShellAPI.h>
#include <commoncontrols.h>

////////----------�˵���  �궨�� -------///////////////
#define MENU_FLAG_OPEN				0x1			//��
#define MENU_FLAG_CHECKIN			0x2			//ǩ��
#define MENU_FLAG_CHECKOUT		0x4			//ǩ��
#define MENU_FLAG_CUT					0x8			//����
#define MENU_FLAG_COPY				0x10		//����
#define MENU_FLAG_PASTE				0x20		//ճ��
#define MENU_FLAG_DELETE			0x40		//ɾ��
#define MENU_FLAG_RENAME			0x80		//������
#define MENU_FLAG_REFRESH			0x100		//ˢ��
#define MENU_FLAG_NEW				0x200		//�½�
#define MENU_FLAG_PROPERTY		0x400		//����

#define MENU_FLAG_VIEW				0x800		//�鿴
#define MENU_FLAG_RESTORE			0x1000		//��ԭ

#define MENU_FLAG_COPYTO			0x2000		//���Ƶ�
#define MENU_FLAG_MOVETO			0x4000		//�ƶ���

#define MENU_FLAG_EXPAN				0x8000		//չ��
#define MENU_FLAG_FOLD				0x10000		//�۵�

#define MENU_FLAG_PRINT				0x20000		//��ӡ




CMainWnd::CMainWnd(void)
	: SHostWnd(_T("layout:wnd_main"))//���ﶨ����������Ҫʹ�õĲ����ļ� ��uires.idx�ļ��ж����
	, m_DropTarget(this)
{
	m_bChangeNotify = false;
}


CMainWnd::~CMainWnd(void)
{
}


void CMainWnd::OnBtnClose()
{
	m_DropTarget.DragDropRevoke(m_hWnd);
	DestroyWindow();
}


#define		InitWnd(p, class, name)	p = FindChildByName2<class>(name);\
													assert(p);\

static TCHAR* sUnit[] = { _T("�ֽ�"), _T("KB"), _T("MB"), _T("GB") };
SStringT FileSizeToStr(INT64 fileSize)
{
#if 0
	//������� �о�����  û���������� Ҳֻ��3λ����  
	TCHAR       szFileLen[64] = { 0 };
	StrFormatByteSize64(fileSize, szFileLen, 64);
	return CString(szFileLen);
#else			
	if (fileSize < 0) return _T("δ֪");

	INT32 nIndex = 0;

	double dSize = static_cast<double>(fileSize);
	while (dSize >= 1024.0)
	{
		dSize /= 1024.0;
		nIndex++;
	}

	SStringT sTemp;
	sTemp.Format(_T("%.2f %s"), dSize, sUnit[nIndex]);
	return sTemp;
#endif

}

int GetFileIconIndex(LPCTSTR lpFileName)
{
	SHFILEINFO fileInfo;
	::SHGetFileInfo(lpFileName, FILE_ATTRIBUTE_NORMAL, &fileInfo, sizeof(fileInfo), SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES);
	return fileInfo.iIcon;
}

BOOL CMainWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	m_DropTarget.DragDropRegister(m_hWnd);
	
	InitWnd(m_pTextStatus1, SStatic, L"text_status1")
	InitWnd(m_pTextStatus2, SStatic, L"text_status2")
	InitWnd(m_pTextStatus3, SStatic, L"text_status3")
	
	SPathBar* pPathBar = NULL;
	InitWnd(pPathBar, SPathBar, L"bar_dir")
	
	pPathBar->InsertItem(0, L"��������");
	pPathBar->InsertItem(0, L"1213");
	pPathBar->InsertItem(0, L"С��Ӱ");
	
	InitWnd(m_pFileList, SFileList, L"lc_file")
		

	SStringT szFileName;
	SStringT szFileTime = _T("2015-12-01 12:22:11");
	
	for (int i=0; i<100; ++i)
	{
		szFileName.Format(_T("�½��ı�_%03d.txt"), i);

		
		m_pFileList->InsertItem(i, szFileName, GetFileIconIndex(szFileName));
		m_pFileList->SetSubItemText(i, 1, szFileTime);
		m_pFileList->SetSubItemText(i, 2, FileSizeToStr(3891 + i * 121));
	}

	
	//ע�� �߳��¼�  ��֪ͨ����
	//SNotifyCenter::getSingleton().addEvent(EVENTID(EventThread));
	
	// ����ȫ�ּ��� �ļ��½� ����  �����ж� ������ ճ������ �Ӷ��������� �ļ�
	SHChangeNotifyEntry shEntry = { 0 };
	shEntry.fRecursive = TRUE;
	shEntry.pidl = 0;

	//ע��Shell���Ӻ���
	ULONG uNotifyId = SHChangeNotifyRegister(
		m_hWnd,
		SHCNRF_ShellLevel,//SHCNRF_InterruptLevel|SHCNRF_ShellLevel,
		SHCNE_CREATE,		//ֻ ���� �½� �Ĳ���
		WM_CHANGENOTIFY, //�Զ�����Ϣ
		1,
		&shEntry
	);
	if (0 == uNotifyId)
	{
		return TRUE;
	}
	

	return TRUE;
}

LRESULT CMainWnd::OnChangeNotify(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bChangeNotify)
		return 0;

	SStringT sDestPath;
	if (SHCNE_CREATE != lParam)   // ֻ �����½����ļ�  
	{
		return 0;
	}

	// ��ص��� ���� �е�� 

	DWORD* pDw = (DWORD*)wParam;
	//DWORD* pDw2 = (DWORD*)(wParam + sizeof(DWORD));
	if (NULL == pDw) return 0;
	TCHAR lpFilePath[MAX_PATH] = { 0 };
	SHGetPathFromIDList((LPCITEMIDLIST)*pDw, lpFilePath);

	OutputDebugString(lpFilePath);

	//�����Ҫ�����ļ���  �� �ж� �Ƿ� �������Լ���Ҫ���ļ�
	LPCTSTR lpFileName = PathFindFileName(lpFilePath);
	if (0 != _tcscmp(lpFileName, _T("123456789.cfg")))
	{
		return 0; //���� �ͷ��� 
	}

	// ���Ǹ���ʱ�ļ�  ��Ҫɾ�� 
	DeleteFile(lpFilePath);

	PathRemoveFileSpec(lpFilePath);
	PathAddBackslash(lpFilePath);

	sDestPath = lpFilePath;

	sDestPath.Trim();//ȥ����β�ո�
	if (sDestPath.IsEmpty())
		return 0;

	m_bChangeNotify = false;

	MsgBox(sDestPath);

	return 0;
}

void CMainWnd::OnTimer(UINT_PTR idEvent)
{
	if(1 == idEvent)			
	{
		
	}
	else
	{
		SHostWnd::OnTimer(idEvent);
	}
}

void CMainWnd::OnBtnBack()
{
	//m_pListFile->EditLabel(0);
	FindChildByName(L"btn_back")->EnableWindow(FALSE);
}

void CMainWnd::OnBtnForward()
{

}

bool CMainWnd::OnEventPathCmd(EventArgs *e)
{
	EventPathCmd* pEvt = sobj_cast<EventPathCmd>(e);
	if(NULL == pEvt)
		return false;

	//SMessageBox(m_hWnd, m_PathBarHandle->GetItemText(pEvt->iItem), _T("��ʾ"), MB_ICONASTERISK);
	return true;
}


bool CMainWnd::OnList_Click(EventArgs* e)
{
	EventFLClick* pEvt = sobj_cast<EventFLClick>(e);
	if(NULL == pEvt)
		return false;

	SFileList* pFileList = sobj_cast<SFileList>(pEvt->sender);

	SStringT sText;
	if(-1 != pEvt->iItem)
	{
		sText = pFileList->GetSubItemText(pEvt->iItem, 0);
	}
	else
		sText.Format(_T("Click ListItem %d"), pEvt->iItem);
	//SMessageBox(m_hWnd, sText, _T("��ʾ"), 0);
	m_pTextStatus2->SetWindowText(sText);
	
	return true;
}

bool CMainWnd::OnList_DbClick(EventArgs* e)
{
	EventFLDBClick* pEvt = sobj_cast<EventFLDBClick>(e);
	if(NULL == pEvt)
		return false;

	if(-1 == pEvt->iItem)
		return true;



	return true;
}

bool CMainWnd::OnList_Menu(EventArgs* e)
{
	EventFLMenu* pEvt = sobj_cast<EventFLMenu>(e);
	if(NULL == pEvt)
		return false;

	POINT pt = pEvt->pt;
	ClientToScreen(&pt);
	UINT nShowCode = MENU_FLAG_OPEN;
	nShowCode |= MENU_FLAG_PRINT;
	nShowCode |= MENU_FLAG_CHECKIN;
	nShowCode |= MENU_FLAG_CUT;
	nShowCode |= MENU_FLAG_COPY;
	nShowCode |= MENU_FLAG_DELETE;
	nShowCode |= MENU_FLAG_RENAME;
	nShowCode |= MENU_FLAG_PROPERTY;

	UINT uCmd = ShowFileListItem(pt, nShowCode);

	if(111 == uCmd)
	{
		
	}
	else if(151 == uCmd)
	{
	
	}
	return true;
}

bool CMainWnd::OnList_BeginDrag(EventArgs* e)
{
	EventFLBeginDrag* pEvt = sobj_cast<EventFLBeginDrag>(e);
	if(NULL == pEvt) return false;

	OutputDebugString(L"OnList_BeginDrag");

#if 1
	
	DataObjectEx* pDataOj = new DataObjectEx;
	if(NULL == pDataOj)
		return false;

	TCHAR lpPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, lpPath, MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathAddBackslash(lpPath);
	_tcscat_s(lpPath, MAX_PATH, _T("123456789.cfg"));		// ����ļ�������� 

	pDataOj->CacheSingleFileAsHdrop(lpPath);
	//pDataOj->CacheSingleFileAsHdrop(L"C:\\123444.txt");
	//POINT pt = {-1, 30000};
	pDataOj->SetDragImageWindow(NULL);

	m_bChangeNotify = true;
	DWORD dwEffect = pDataOj->DoDragDrop(DROPEFFECT_COPY);
	
	//pDataOj->Release();
	delete pDataOj;
	pDataOj = NULL;

	
#endif
	return true;
}

//  DropTarget  ������ ����  
HRESULT CMainWnd::OnDragEnter(IDataObject* pDataObject, DWORD dwKeyState, const POINT& point)
{
	return S_OK;
}

DROPEFFECT CMainWnd::OnDragOver(IDataObject* pDataObject, 
								   DWORD dwKeyState, 
								   const POINT& point, 
								   std::wstring& szMessage,
								   std::wstring& szInsert)
{
	DROPEFFECT dwEffect = DROPEFFECT_NONE;
	POINT pt = point;
	ScreenToClient(&pt);

	SWND s = SwndFromPoint(pt, FALSE);
	if(s == m_pFileList->GetSwnd())
	{
		szMessage = _T("�ϴ���");
		szInsert = _T("����");
		dwEffect = DROPEFFECT_COPY;
	}
	else
	{
		szMessage = _T("�����ϴ�");
		szInsert = _T("22");
	}

	return dwEffect;
}

BOOL CMainWnd::OnDrop(IDataObject* pDataObject, DWORD dwKeyState, const POINT& point)
{
	HGLOBAL hg = DragDropHelper::GetGlobalData(pDataObject, CF_HDROP );
	if (NULL != hg)
	{
		HDROP hdrop = (HDROP) GlobalLock ( hg );
		if (NULL != hdrop)
		{
			TCHAR lpFilePath[MAX_PATH];
			UINT nLen = DragQueryFile ( hdrop, 0, lpFilePath, MAX_PATH );
			DragFinish(hdrop);
			OutputDebugString(lpFilePath);

			//int nCount = m_pFileList->GetItemCount();

			m_pFileList->InsertItem(0, lpFilePath, GetFileIconIndex(lpFilePath));
		}
		GlobalUnlock(hg);
	}
	else
		OutputDebugString(_T("not file"));

	return TRUE;
}

UINT CMainWnd::ShowFileListNull(CPoint Point, UINT nShowCode)
{
	return 0;
}

UINT CMainWnd::ShowFileListItem(CPoint Point, UINT nShowCode)
{
#if 0
	SMenuEx menu;
	if(FALSE == menu.LoadMenu(_T("layout:Menu_FileListItem")))
	{
		MsgBox(_T("���ز˵� ʧ�ܣ�"));
		return ;
	}
	menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);
#else
	SMenu menuMain;
	if(FALSE == menuMain.LoadMenu(_T("Menu_FileList"), _T("layout")))
	{
		MsgBox(_T("���ز˵� ʧ�ܣ�"));
		return 0;
	}

	::EnableMenuItem(menuMain.m_hMenu, 112, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	::CheckMenuItem(menuMain.m_hMenu, 121, MF_BYCOMMAND | MF_CHECKED);
	::CheckMenuItem(menuMain.m_hMenu, 132, MF_BYCOMMAND | MF_CHECKED);
	::DeleteMenu(menuMain.m_hMenu, 122, MF_BYCOMMAND);
	//menuMain.InsertMenu(0, MF_BYPOSITION, 1, _T("����ϵͳ"), 0);

	//WebUrlInfoVct webList = theFun.GetWebUrlInfo();
	//int nWebCount = webList.size();
	//if(nWebCount <= 0)		//û������   ��ֻ
	//{
	//	menuMain.InsertMenu(0, MF_BYPOSITION, 1, _T("����ϵͳ"), 0);
	//}
	//else
	{
		//SMenu menuSys;
		//menuSys.m_hMenu = CreatePopupMenu();

		//for (int i=0; i<nWebCount; ++i)
		//{
		//	menuSys.InsertMenu(i, 0, 10 + i, webList[i].sWebTitle, 0);
		//}

		//menuMain.InsertMenu(0, MF_POPUP|MF_BYPOSITION, (UINT_PTR)&menuSys, _T("����ϵͳ"), 0);
	}

	//TPM_RETURNCMD  ֱ�ӷ��� �����id  ����SendMess��Ϣ WM_COMMAND�� 
	UINT uCmd = menuMain.TrackPopupMenu(TPM_RETURNCMD, Point.x, Point.y, m_hWnd);
	//MenuHanld(uCmd);
	return uCmd;
#endif
}



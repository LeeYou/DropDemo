#pragma once
#include "resource.h"
#include "WinFileIconSkin.hpp"
#include "DragDropHandle.h"
#include "SFileList.h"
#include "SPathBar.h"


#define WM_TRAYNOTIFY				WM_USER + 201

#define WM_CHANGENOTIFY			WM_APP + 191 

class CMainWnd : public SHostWnd
							, public TAutoEventMapReg<CMainWnd>//֪ͨ�����Զ�ע��
							, public IDropTargetIF
{
public:
	HRESULT OnDragEnter(IDataObject* pDataObject, DWORD dwKeyState, const POINT& point);
	DROPEFFECT OnDragOver(IDataObject* pDataObject, DWORD dwKeyState, const POINT& point, std::wstring& szMessage, std::wstring& szInsert);
	BOOL OnDrop(IDataObject* pDataObject, DWORD dwKeyState, const POINT& point);
public:
	CMainWnd(void);
	~CMainWnd(void);
	
	UINT MsgBox(LPCTSTR lpText)
	{
		return SMessageBox(m_hWnd, lpText, _T("��ʾ"), MB_ICONERROR);
	}

	void OnBtnClose();
	void OnMax()
	{
		SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE);
	}
	void OnRestore()
	{
		SendMessage(WM_SYSCOMMAND,SC_RESTORE);
	}
	void OnBtnMin()
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	}
	void OnSize(UINT nType, CSize size)
	{
		SetMsgHandled(FALSE);
		//if(!m_bLayoutInited) return;
		if(nType==SIZE_MAXIMIZED)
		{
			SWindow* pWin = FindChildByName(L"btn_restore");
			if(NULL != pWin)
			{
				pWin->SetVisible(TRUE);
			}
			pWin = FindChildByName(L"btn_max");
			if(NULL != pWin)
			{
				pWin->SetVisible(FALSE);
			}
			//FindChildByName(L"btn_restore")
			//FindChildByName(L"btn_max")->SetVisible(FALSE);
		}else if(nType==SIZE_RESTORED)
		{
			SWindow* pWin = FindChildByName(L"btn_restore");
			if(NULL != pWin)
			{
				pWin->SetVisible(FALSE);
			}
			pWin = FindChildByName(L"btn_max");
			if(NULL != pWin)
			{
				pWin->SetVisible(TRUE);
			}
		}
	}
public:
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnTimer(UINT_PTR idEvent);
	//�Զ���ؼ��¼�����
	void OnBtnBack();
	void OnBtnForward();
	
	bool OnEventPathCmd(EventArgs* e);
	
	bool OnList_BeginDrag(EventArgs* e);
	bool OnList_Click(EventArgs* e);
	bool OnList_DbClick(EventArgs* e);
	bool OnList_Menu(EventArgs* e);
protected:
	//�¼�����ӳ���
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_max", OnMax)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_NAME_COMMAND(L"btn_close", OnBtnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnBtnMin)
		EVENT_NAME_COMMAND(L"btn_back", OnBtnBack)
		EVENT_NAME_COMMAND(L"btn_forward", OnBtnForward)
		EVENT_NAME_HANDLER(L"bar_dir", EventPathCmd::EventID, OnEventPathCmd)
		EVENT_NAME_HANDLER(L"lc_file", EventFLClick::EventID, OnList_Click)
		EVENT_NAME_HANDLER(L"lc_file", EventFLDBClick::EventID, OnList_DbClick)
		EVENT_NAME_HANDLER(L"lc_file", EventFLBeginDrag::EventID, OnList_BeginDrag)
		EVENT_NAME_HANDLER(L"lc_file", EventFLMenu::EventID, OnList_Menu)

		EVENT_MAP_END()

		LRESULT OnChangeNotify(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//������Ϣ����ӳ���
	BEGIN_MSG_MAP_EX(CMainWnd)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_SIZE(OnSize)
		MESSAGE_HANDLER_EX(WM_CHANGENOTIFY, OnChangeNotify)
		MSG_WM_TIMER(OnTimer)
		CHAIN_MSG_MAP(SHostWnd)
	END_MSG_MAP()
public:		//���ܺ���  ���ǽ�����ص�
	
	UINT ShowFileListNull(CPoint Point, UINT nShowCode);
	// ��ʾ FileList ѡ�� ��Ĳ˵�
	UINT ShowFileListItem(CPoint Point, UINT nShowCode);

private:
	DropTargetEx				m_DropTarget;
private:				//�����Ա 
	//STabCtrl*					m_pTab;
	SFileList*					m_pFileList;
	SStatic*						m_pTextStatus1;			//״̬�� 
	SStatic*						m_pTextStatus2;			//
	SStatic*						m_pTextStatus3;			//

	bool							m_bChangeNotify;
};


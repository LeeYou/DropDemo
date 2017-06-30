// EtimesHelper.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MainWnd.h"
#include "com-cfg.h"

#ifdef _DEBUG
#define RES_TYPE 0
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#else
#define RES_TYPE 1
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif


// �˴���ģ���а����ĺ�����ǰ������:


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	HRESULT hRes = OleInitialize(NULL);
	SASSERT(SUCCEEDED(hRes));

	
	
	SComMgr* pComMgr = new SComMgr(_T("imgdecoder-gdip"));
	{
		CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
		CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
		if (!pComMgr->CreateRender_Skia((IObjRef**)&pRenderFactory))
		{
			delete pComMgr;
			::MessageBox(NULL, _T("Load CreateRender_Skia Error!"), _T("��ʾ"), MB_ICONINFORMATION);
			return 0;
		}
		if (!pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory))
		{
			delete pComMgr;
			::MessageBox(NULL, _T("Load CreateImgDecoder Error!"), _T("��ʾ"), MB_ICONINFORMATION);
			return 0;
		}

		pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);

		//����һ��Ψһ��SApplication����SApplication��������Ӧ�ó������Դ
		SApplication *theApp=new SApplication(pRenderFactory, hInstance);
		//��appע���Զ�����
		theApp->RegisterWindowClass<SPathBar>();
		theApp->RegisterWindowClass<SFileList>();
		theApp->RegisterSkinClass<SSkinSystemIconList>();
		
		//�� dll ���� soui �Դ�ϵͳ��Դ
		HMODULE hModSysResource = LoadLibrary(SYS_NAMED_RESOURCE);
		if(NULL != hModSysResource)
		{
			CAutoRefPtr<IResProvider> sysResProvider;
			CreateResProvider(RES_PE, (IObjRef**)&sysResProvider);
			sysResProvider->Init((WPARAM)hModSysResource, 0);
			theApp->LoadSystemNamedResource(sysResProvider);
			FreeLibrary(hModSysResource);
			hModSysResource = NULL;
		}
		else
		{
			MessageBox(NULL, _T("Load SYS_NAMED_RESOURCE Error!"), _T("��ʾ"), MB_ICONINFORMATION);
			SASSERT(0);
		}

		CAutoRefPtr<IResProvider>   pResProvider;
#if (RES_TYPE == 0)
		CreateResProvider(RES_FILE,(IObjRef**)&pResProvider);
		TCHAR lpResPath[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, lpResPath, MAX_PATH);
		PathRemoveFileSpec(lpResPath);
		PathRemoveFileSpec(lpResPath);
		PathAddBackslash(lpResPath);
		// �������Լ����� ��
		_tcscat_s(lpResPath, MAX_PATH, _T("demos\\DropDemo\\uires"));
		if (!pResProvider->Init((LPARAM)lpResPath, 0))
		{
			SASSERT(0);
			return 1;
		}
#else 
		CreateResProvider(RES_PE,(IObjRef**)&pResProvider);
		pResProvider->Init((WPARAM)hInstance,0);
#endif

		//��������IResProvider����SApplication����
		theApp->AddResProvider(pResProvider);

		SNotifyCenter* pNotifyCenter = new SNotifyCenter;
		{
		
			CMainWnd dlg;
			//dlg.DoModal();
			
			dlg.Create(GetActiveWindow(),0,0,0,0);

			dlg.GetNative()->SendMessage(WM_INITDIALOG);
			
			dlg.ShowWindow(SW_SHOWNORMAL);
					
			theApp->Run(dlg.m_hWnd);
		
		}
		delete pNotifyCenter;
		delete theApp;
	}
	
	delete pComMgr;

	OleUninitialize();
	return 0;
}



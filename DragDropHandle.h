#pragma once
#include "oleidl.h"
#include "ShObjIdl.h"
#include <string>
#include <vector>

/*
#define	DROPEFFECT_NONE	( 0 )    

#define	DROPEFFECT_COPY	( 1 )

#define	DROPEFFECT_MOVE	( 2 )

#define	DROPEFFECT_LINK	( 4 )
*/
typedef DWORD					DROPEFFECT;


//����  ������ �Ľӿ�  ������ �̳иýӿ�  Ȼ��Ϳ������������ˡ�
/*�һ��ǽ����� �����ڼ̳����  �����Ƿַ�������ؼ�
��Ϊ�Ϸ� һ�㶼������ҵ���߼�.
���ڴ��ڵĻ�����  �����ж� �����ĸ��ӿؼ� �� д����д�� �麯��  ����������*/
class IDropTargetIF
{
public:
	//************************************
	// Method:    OnDragEnter  �ж��Ƿ���Խ���һ���ϲ������Լ�����֮���Ч��
	// FullName:  IDropTargetIF::OnDragEnter
	// Access:    virtual public 
	// Returns:   HRESULT ���� S_FALSE ��ʾ�����Ϸ�   S_OK ��ʾ���� 
	// Qualifier:
	// Parameter: IDataObject * pDataObject  /ָ��Դ���ݶ���Ľӿ�ָ�� ���а� ���ݽṹ 
	// Parameter: DWORD dwKeyState				// ��ǰ�������η���״̬ ���� ״̬  �� MK_LBUTTON�� һ����ֵ
	// Parameter: const POINT & point				��Ļ������ ��Ҫ�Լ�ת��
	//************************************
	virtual HRESULT OnDragEnter(IDataObject* pDataObject, DWORD dwKeyState, const POINT& point) PURE;
	
	//************************************
	// Method:    OnDragOver  �ṩͨ��DoDragDrop����ִ�е�Ŀ�귴��
	// FullName:  IDropTargetIF::OnDragOver
	// Access:    virtual public 
	// Returns:   DROPEFFECT ���� DROPEFFECT_COPY ���Ƶ�ֵ  ������ʾ ͼ������  ���ƶ� ���� �� 
	// Qualifier:
	// Parameter: IDataObject * pDataObject		���а� ���ݽṹ
	// Parameter: DWORD dwKeyState					���� ״̬  �� MK_LBUTTON�� һ����ֵ
	// Parameter: const POINT & point					��Ļ������ ��Ҫ�Լ�ת��  ������ ���ĸ��ؼ���
	// Parameter: std::wstring & szMessage			�Ϸ�ͼ���ұߵĲ����������ơ����� ���Ƶ� ���ϴ���
	// Parameter: std::wstring & szInsert				�Ϸ�ͼ���ұߵ�Ŀ������ ������ 123�ļ���   
	//************************************
	virtual DROPEFFECT OnDragOver(IDataObject* pDataObject,
		DWORD dwKeyState,
		const POINT& point, 
		std::wstring& szMessage,
		std::wstring& szInsert) PURE;
	// ���ݷŽ�Ŀ�괰��
	virtual BOOL OnDrop(IDataObject* pDataObject, DWORD dwKeyState, const POINT& point)PURE;
	//����һ��dropĿ��������ķ�����Ϊ
	virtual void OnDragLeave() {}
};


class DropTargetEx : public IDropTarget
{
public:
	DropTargetEx(IDropTargetIF* iDropTargetIF);
	~DropTargetEx(void);  
	bool DragDropRegister(HWND hWnd, DWORD AcceptKeyState = MK_LBUTTON);  
	bool DragDropRevoke(HWND hWnd);  
	HRESULT	STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void **ppvObject);  
	ULONG		STDMETHODCALLTYPE AddRef();  
	ULONG		STDMETHODCALLTYPE Release();  
	//����  
	HRESULT	STDMETHODCALLTYPE DragEnter(__RPC__in_opt IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, __RPC__inout DWORD *pdwEffect);  
	//�ƶ�  
	HRESULT	STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, __RPC__inout DWORD *pdwEffect);  
	//�뿪  
	HRESULT	STDMETHODCALLTYPE DragLeave();  
	//�ͷ�  
	HRESULT	STDMETHODCALLTYPE Drop(__RPC__in_opt IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, __RPC__inout DWORD *pdwEffect);  

private:
	//�����ϷŹ����е����� ��Ϣ
	bool SetDropDescription(DROPIMAGETYPE nImageType, LPCWSTR lpszText, LPCWSTR lpszInsert=NULL);
private:  
	HWND								m_hWnd;  
	IDropTargetHelper*				m_piDropHelper;  
	DWORD								m_dwAcceptKeyState;  
	IDataObject*						m_pDataObj;
	IDropTargetIF*					m_pIDropTargetHandle;
protected:
	
};



/////////////////////////////// �Ϸ� ������ ///////////////////////////////////////////
class DataObjectEx : public IDataObject
{
public:
	// IUnknown members
	HRESULT STDMETHODCALLTYPE QueryInterface (REFIID riid, void ** ppvObject);
	ULONG   STDMETHODCALLTYPE AddRef (void);
	ULONG   STDMETHODCALLTYPE Release (void);

	// IDataObject members
	HRESULT STDMETHODCALLTYPE GetData (FORMATETC* pFormatEtc, STGMEDIUM* pMedium);
	HRESULT STDMETHODCALLTYPE GetDataHere (FORMATETC* pFormatEtc, STGMEDIUM* pMedium);
	HRESULT STDMETHODCALLTYPE QueryGetData (FORMATETC *pFormatEtc);
	HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc (FORMATETC* pFormatEct, FORMATETC* pFormatEtcOut);
	HRESULT STDMETHODCALLTYPE SetData (FORMATETC* pFormatEtc, STGMEDIUM* pMedium,  BOOL fRelease);
	HRESULT STDMETHODCALLTYPE EnumFormatEtc (DWORD dwDirection, IEnumFORMATETC** ppEnumFormatEtc);
	HRESULT STDMETHODCALLTYPE DAdvise (FORMATETC* pFormatEtc, DWORD advf, IAdviseSink*, DWORD *);
	HRESULT STDMETHODCALLTYPE DUnadvise (DWORD dwConnection);
	HRESULT STDMETHODCALLTYPE EnumDAdvise (IEnumSTATDATA** ppEnumAdvise);

	// Constructor / Destructor
	DataObjectEx();
	~DataObjectEx();
public:
	bool CacheSingleFileAsHdrop(LPCTSTR lpszFilePath);			//���õ����ļ� �����а�
	// ��ʼ �Ϸ� 
	DROPEFFECT DoDragDrop(DROPEFFECT dwEffect);
	bool SetDragImage(HBITMAP hBitmap, POINT* pPoint=NULL, COLORREF clr=GetSysColor(COLOR_WINDOW));
	bool SetDragImageWindow(HWND hWnd, POINT* pPoint=NULL);

private:
	int	 FindFmtEtc(const FORMATETC* pFormatEtc);
	void CopyMedium(STGMEDIUM* pMedDest, STGMEDIUM* pMedSrc, FORMATETC* pFmtSrc);
private:
	std::vector<FORMATETC*>			m_vctFormatEtc;
	std::vector<STGMEDIUM*>			m_vctStgMedium;
	IDragSourceHelper*						m_pDragSourceHelper;
	IDragSourceHelper2*					m_pDragSourceHelper2;		// Drag image helper 2 (SetFlags function)
};

//
class EnumFormatEtcEx : public IEnumFORMATETC
{
public:
	EnumFormatEtcEx(const std::vector<FORMATETC>& arrFE);
	EnumFormatEtcEx(const std::vector<FORMATETC*>& arrFE);
	//IUnknown members
	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
	STDMETHOD_(ULONG, AddRef)(void);
	STDMETHOD_(ULONG, Release)(void);

	//IEnumFORMATETC members
	STDMETHOD(Next)(ULONG ulCelt, LPFORMATETC lpFormatEtc, ULONG FAR* pCeltFetched);
	STDMETHOD(Skip)(ULONG ulCelt);
	STDMETHOD(Reset)(void);
	STDMETHOD(Clone)(IEnumFORMATETC FAR* FAR* ppCloneEnumFormatEtc);

private:
	volatile LONG								m_ulRefCount;
	std::vector<FORMATETC>	m_vctFmtEtc;
	UINT									m_nCurrent;
};

//////////////////////////////////////////////////////////////////////////
class DropSourceEx : public IDropSource
{
public:
	DropSourceEx();
	HRESULT STDMETHODCALLTYPE QueryInterface    (REFIID riid, void ** ppvObject);
	ULONG   STDMETHODCALLTYPE AddRef            (void);
	ULONG   STDMETHODCALLTYPE Release           (void);
	//
	// IDropSource members
	//
	HRESULT STDMETHODCALLTYPE QueryContinueDrag (BOOL fEscapePressed, DWORD grfKeyState);
	HRESULT STDMETHODCALLTYPE GiveFeedback      (DWORD dwEffect);
private:
	bool SetDragImageCursor(DROPEFFECT dwEffect);
private:
	friend class DataObjectEx;
	bool					m_bSetCursor;		// internal flag set when Windows cursor must be set
	LPDATAOBJECT	m_pIDataObj;			// set by DataObjectEx to its IDataObject
};

//������
class DragDropHelper
{
public:
	static bool GetGlobalData(LPDATAOBJECT pIDataObj, LPCTSTR lpszFormat, FORMATETC& formatEtc, STGMEDIUM& stgMedium);
	static DWORD	GetGlobalDataDWord(LPDATAOBJECT pIDataObj, LPCTSTR lpszFormat);

	static bool ClearDescription(DROPDESCRIPTION* pDropDescription);
	static DROPIMAGETYPE DropEffectToDropImage(DROPEFFECT dwEffect);
	static HGLOBAL GetGlobalData(LPDATAOBJECT pIDataObj, CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc=NULL);
};
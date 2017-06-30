
#ifndef __PATHBAR_H__
#define __PATHBAR_H__

//#include "core/SWnd.h"
namespace SOUI
{
	struct BarItemInfo
	{
		SStringT sText;
		int nWidth;
		DWORD dwData;
	};

class EventPathCmd : public TplEventArgs<EventPathCmd>
{
	SOUI_CLASS_NAME(EventPathCmd, L"on_path_cmd")
public:
	EventPathCmd(SObject *pSender):TplEventArgs<EventPathCmd>(pSender){}
	enum{EventID=EVT_EXTERNAL_BEGIN+4000};

	int iItem;
};

class SPathBar : public SWindow
{
public:
	SOUI_CLASS_NAME(SPathBar, L"pathbar")
	SPathBar(void);
	~SPathBar(void);

protected:
   
	//Describe  ��ʼ������         
    void Init();

    // �滭��Ϣ
    void OnPaint(IRenderTarget *pRT);
	void OnLButtonDown(UINT nFlags, CPoint pt);
    void OnLButtonUp(UINT nFlags, CPoint pt);
    void OnMouseMove(UINT nFlags,CPoint pt);
    void OnMouseLeave();

protected:
	int HitTest(CPoint  pt);
	virtual BOOL NeedRedrawWhenStateChange()
	{
		return TRUE;
	}
protected:
	ISkinObj*				m_pSkin;   /**< ״̬ͼƬ��Դ */
	SOUI_ATTRS_BEGIN()
		ATTR_SKIN(L"skin", m_pSkin, FALSE)
	SOUI_ATTRS_END()

	SOUI_MSG_MAP_BEGIN()
		MSG_WM_PAINT_EX(OnPaint)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
	SOUI_MSG_MAP_END()

public:
	int InsertItem(int nItem, LPCTSTR pszText);
	BOOL SetItemData(int nItem, DWORD dwData);
	DWORD GetItemData(int nItem);
	//ɾ����  nCount ��ʾ Ҫɾ�� nItem �����������   -1 ��ʾɾ��nItem����������
	void DeleteItem(int nItem, int nCount=1);
	void DeleteAllItems();
	SStringT GetItemText(int nItem) const;
	DWORD GetItemCount() const;
protected:
	SArray<BarItemInfo> m_arrItems; //

	int m_nHoverItem;
};


}



#endif	//__PATHBAR_H__
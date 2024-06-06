
// GraphAppView.h: интерфейс класса CGraphAppView
//

#pragma once


class CGraphAppView : public CView
{
protected: // создать только из сериализации
	CGraphAppView() noexcept;
	DECLARE_DYNCREATE(CGraphAppView)

// Атрибуты
public:
	CGraphAppDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Реализация
public:
	virtual ~CGraphAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в GraphAppView.cpp
inline CGraphAppDoc* CGraphAppView::GetDocument() const
   { return reinterpret_cast<CGraphAppDoc*>(m_pDocument); }
#endif


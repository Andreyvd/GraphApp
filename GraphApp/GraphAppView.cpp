
// GraphAppView.cpp: реализация класса CGraphAppView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "GraphApp.h"
#endif

#include "GraphAppDoc.h"
#include "GraphAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGraphAppView

IMPLEMENT_DYNCREATE(CGraphAppView, CView)

BEGIN_MESSAGE_MAP(CGraphAppView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CGraphAppView

CGraphAppView::CGraphAppView() noexcept
{
	// TODO: добавьте код создания

}

CGraphAppView::~CGraphAppView()
{
}

BOOL CGraphAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CGraphAppView

void CGraphAppView::OnDraw(CDC* pDC)
{
	CGraphAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	RECT r ;
	CPen pen;
	
	pen.CreatePen(0, 20, COLOR_WINDOWTEXT);

	CFont font;
	font.CreateFontA(30, 20, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));
	this->GetClientRect(&r);
	CPen* oldPen = pDC->SelectObject(&pen);
	CFont* oldFont = pDC->SelectObject(&font);
	
	pDC->DrawTextEx(pDoc->GetText(), &r, 0, 0);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldFont);
}

void CGraphAppView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGraphAppView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Диагностика CGraphAppView

#ifdef _DEBUG
void CGraphAppView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphAppDoc* CGraphAppView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphAppDoc)));
	return (CGraphAppDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CGraphAppView

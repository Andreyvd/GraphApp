
// GraphAppDoc.h: интерфейс класса CGraphAppDoc 
//


#pragma once
#include <vector>
#include <string>
#include <map>



class CGraphAppDoc : public CDocument
{
private:
	std::vector<CString> graph();
	CString _logText;
	

protected: 
	CGraphAppDoc() noexcept;
	DECLARE_DYNCREATE(CGraphAppDoc)
	CString _text;

	

// Атрибуты
public:


// Операции
public:
	virtual CString& GetText();
	virtual void SetText(CString t);
	virtual void DFS(std::map<CString, std::vector<CString>>& graph, CString v, std::vector<CString>& component, std::map<CString, bool>& visited);
	virtual void logInfo(CString message, CString type);
// Переопределение
public:
	virtual BOOL OnNewDocument();
	void OnAssignLayersClick();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CGraphAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
/*public:
	afx_msg void IncidentVertices();*/
};

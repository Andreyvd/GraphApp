
// GraphAppDoc.h: интерфейс класса CGraphAppDoc 
//


#pragma once
#include <vector>
#include <string>



class CGraphAppDoc : public CDocument
{
private:
	std::vector<CString> graph();

	CString _logText;
	virtual std::vector<int> splitString(const std::string& str);
	void DFS(std::vector<std::vector<int>>& graph, int v, std::vector<int>& component, std::vector<bool>& visited);

	void logInfo(CString message, CString type);

protected: 
	CGraphAppDoc() noexcept;
	DECLARE_DYNCREATE(CGraphAppDoc)
	CString _text;

	

// Атрибуты
public:


// Операции
public:
	const CString& GetText();
	void SetText(CString t);

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

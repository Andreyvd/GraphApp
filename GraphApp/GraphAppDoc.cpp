

#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "GraphApp.h"
#endif

#include "GraphAppDoc.h"

#include <propkey.h>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <sstream>


// CGraphAppDoc

IMPLEMENT_DYNCREATE(CGraphAppDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraphAppDoc, CDocument)
	ON_COMMAND(ID_TOOLS_GRAPH32772, &CGraphAppDoc::OnAssignLayersClick)
END_MESSAGE_MAP()


CGraphAppDoc::CGraphAppDoc() noexcept
{

}

CGraphAppDoc::~CGraphAppDoc()
{
}

BOOL CGraphAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

void CGraphAppDoc::OnAssignLayersClick()
{
	if (_text.GetLength() > 0)
	{
		try
		{
			_logText.Format("Succsefuly assign layers");
			logInfo(_logText, "INFO");

			std::vector<CString> components = graph();
			for (const auto& component : components)
			{
				
				auto pos = theApp.GetFirstDocTemplatePosition();
				auto pDocTemplate = theApp.GetNextDocTemplate(pos);
				if (pDocTemplate)
				{
					auto doc = dynamic_cast<CGraphAppDoc*>(pDocTemplate->CreateNewDocument());
					doc->SetText(component);

					auto childWnd = pDocTemplate->CreateNewFrame(doc, this->GetRoutingFrame());
					childWnd->ShowWindow(true);
				}
			}
		}
		catch (const std::exception& e)
		{
			_logText.Format("%s. See log file", e.what());
			MessageBox(NULL, _logText, _T("Warning"), MB_ICONWARNING);
		}
	}
	else
	{
		MessageBox(NULL, _T("No text in doc. Open file with text and retry..."), _T("Message"), MB_OK);
	}
}





void CGraphAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{   
		ar.WriteString(_text);
	}
	else
	{
		CString buf;
		while (ar.ReadString(buf))
		{
			_text.Append(buf);
			_text.Append("\n");

		}
		
	}
}

CString& CGraphAppDoc::GetText() 
{
	return _text;
}

#ifdef SHARED_HANDLERS


void CGraphAppDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}


void CGraphAppDoc::InitializeSearchContent()
{
	CString strSearchContent;
	
	SetSearchContent(strSearchContent);
}

void CGraphAppDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif 


#ifdef _DEBUG
void CGraphAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraphAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif 



void CGraphAppDoc::SetText(CString t) 
{
	_text = t;
}

void CGraphAppDoc::DFS(std::map<CString, std::vector<CString>>& graph, CString v, std::vector<CString>& component, std::map<CString,bool>& visited) {
	visited[v] = true;
	component.push_back(v);
	for (const auto& neighbor : graph[v]) {
		if (!visited[neighbor]) {
			DFS(graph, neighbor, component, visited);
		}
	}
}

std::vector<CString> CGraphAppDoc::graph()
{
	CString delimiter = _T("\n");
	int position = 0;

	std::vector<std::vector<CString>> result;

	CString token = _text.Tokenize(delimiter, position);
	while (!token.IsEmpty())
	{
		std::vector<CString> lineWords;
		CString wordDelimiter = _T(" ");
		int wordPosition = 0;
		CString wordToken = token.Tokenize(wordDelimiter, wordPosition);
		while (!wordToken.IsEmpty())
		{
			lineWords.push_back(wordToken);
			wordToken = token.Tokenize(wordDelimiter, wordPosition);
		}
		result.push_back(lineWords);

		token = _text.Tokenize(delimiter, position);
	}

	std::map<CString, std::vector<CString>> graph;
	std::map<CString, bool> visited;
	std::vector<CString> vertexMap;
	int numVertices = _ttoi(result[0][0]);
	_logText.Format("numVertices: %d", numVertices);
	logInfo(_logText, "INFO");
	int count = 0;
	std::map<CString, int> result_map;

	if (numVertices < result.size() - 1) {
		MessageBox(NULL, _T("you entered more vertex nodes than specified"), _T("Message"), MB_OK);
	}
	else {
		for (int i = 1; i < result.size(); i++)
		{
			CString vertexName = result[i][0];
			result_map[vertexName] = i;
			vertexMap.push_back(vertexName);
			for (int j = 1; j < result[i].size(); j++)
			{
				CString neighborName = result[i][j];
				graph[vertexName].push_back(neighborName);
			}
		}
		std::vector<std::vector<CString>> components;
		bool is_neighbor = true;
		for (auto& v : vertexMap) {
			for (const auto& neighbor : graph[v]) {
				if (std::find(graph[neighbor].begin(), graph[neighbor].end() , v) == graph[neighbor].end()) {
					is_neighbor = false;
					break;
				}
			}
			if (!is_neighbor) {
				break;
			}

		}
		
		if (is_neighbor) {
			for (int i = 0; i < numVertices; i++) {
				for (const auto& elem : vertexMap)
				{
					std::vector<CString> component;
					if (!visited[vertexMap[i]]) {
						DFS(graph, vertexMap[i], component, visited);
						components.push_back(component);
					}
				}
			}
		}
		else {
			MessageBox(NULL, _T("Invalid input format"), _T("Message"), MB_OK);
		}

		

		std::vector<CString> gcomponent;
		for (int i = 0; i < components.size(); i++) {
			CString component_str;
			std::vector<CString> component = components[i];
			component_str.Format(_T("%d"), component.size());
			component_str.Append("\n");
			for (CString node : component) {
				component_str.Append(node);
				component_str.Append(" ");
				for (const auto& elem : graph[node]) {
					component_str.Append(elem);
					component_str.Append(" ");

				}
				component_str.Append("\n");
			}
			gcomponent.push_back(component_str);
		}

		return gcomponent;
	}
}


void CGraphAppDoc::logInfo(CString message, CString type)
{
	CString logFilePath = _T("log.txt");
	CString fullMessageWithTypeAndTime;

	CStdioFile logFile;

	CTime currentTime = CTime::GetCurrentTime();

	int year = currentTime.GetYear();
	int month = currentTime.GetMonth();
	int day = currentTime.GetDay();
	int hour = currentTime.GetHour();
	int minute = currentTime.GetMinute();
	int second = currentTime.GetSecond();

	CString timeString;
	timeString.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), year, month, day, hour, minute, second);

	fullMessageWithTypeAndTime.Append(timeString);
	fullMessageWithTypeAndTime.Append("\t");
	fullMessageWithTypeAndTime.Append(type);
	fullMessageWithTypeAndTime.Append("\t");
	fullMessageWithTypeAndTime.Append(message);
	fullMessageWithTypeAndTime.Append("\n");

	if (logFile.Open(logFilePath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)) {
		logFile.SeekToEnd();

		logFile.WriteString(fullMessageWithTypeAndTime);

		logFile.Close();
	}
}



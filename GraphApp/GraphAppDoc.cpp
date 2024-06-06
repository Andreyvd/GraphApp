

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
	int startVertex;

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

const CString& CGraphAppDoc::GetText() 
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

void DFS(std::vector<std::vector<int>>& graph, int v, std::vector<int>& component, std::vector<bool>& visited) {
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

	std::vector<std::vector<int>> graph;
	std::vector<bool> visited;
	std::map<CString, int> vertexMap;
	int numVertices = _ttoi(result[0][0]);
	_logText.Format("numVertices: %d", numVertices);
	logInfo(_logText, "INFO");
	graph.resize(numVertices);
	visited.resize(numVertices, false);
	int count = 0;

	for (int i = 1; i < result.size(); i++)
	{
		
		CString vertexName = result[i][0];
		if (vertexMap.find(vertexName) == vertexMap.end()) {
			vertexMap[vertexName] = count;
		}
		for (int j = 1; j < result[i].size(); j++)
        {
            CString neighborName = result[i][j];
            if (vertexMap.find(neighborName) == vertexMap.end()) {
                vertexMap[neighborName] = vertexMap.size();
				count++;
                
            }
            graph[vertexMap[vertexName]].push_back(vertexMap[neighborName]);
        }
		count = vertexMap.size() ;
	}

	std::vector<std::vector<int>> components;
	for (int i = 0; i < numVertices; i++)
	{
		if (!visited[i]) {
			std::vector<int> component;
			DFS(graph, i, component, visited);
			components.push_back(component);	
		}

	}
	std::vector<CString> gcomponent;
	for (int i = 0; i < components.size(); i++) {
		CString component_str;
		std::vector<int> component = components[i];
		component_str.Format(_T("%d"), component.size());
			component_str += _T("\n");
		for (int j = 0; j < component.size() ; j++) {
			std::vector<CString> component_ = result[component[j] + 1];		
			for (const auto& elem : component_) {
				component_str += elem;
				component_str += " ";	
			}
			component_str += _T("\n");
		}
		gcomponent.push_back(component_str);
	}


	return gcomponent;
}



std::vector<int> CGraphAppDoc::splitString(const std::string& str) {
	try
	{
		std::stringstream iss(str);

		int number;
		std::vector<int> myNumbers;
		while (iss >> number)
			myNumbers.push_back(number);

		return myNumbers;
	}
	catch (const std::exception& e) {
		throw std::invalid_argument("Entered a non integer value in string");
	}

}

void CGraphAppDoc::DFS(std::vector<std::vector<int>>& graph, int v, std::vector<int>& component, std::vector<bool>& visited)
{
visited[v] = true;
component.push_back(v);
for (const auto& neighbor : graph[v]) {
	if (!visited[neighbor]) {
		DFS(graph, neighbor, component, visited);
	}
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



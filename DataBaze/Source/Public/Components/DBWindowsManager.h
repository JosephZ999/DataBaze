#pragma once

#include "framework.h"
#include "Resource.h"

#include "DBInterface.h"
#include "DBDataTypes.h"

class DBWindowViewer;
class DBWindowWriter;

class DBWindowsManager : public DBInterface
{
public:
	void Initialize(HINSTANCE HInstance);
	~DBWindowsManager();

private:
	HWND ViewerHandle = 0;
	HWND WriterHandle = 0;

	DBWindowViewer* WindowViewer = nullptr;
	DBWindowWriter* WindowWriter = nullptr;

public:
	HWND				   GetViewerHandle() const { return ViewerHandle; }
	HWND				   GetWriterHandle() const { return WriterHandle; }
	DBInterface*		   GetViewerClass() const;
	DBInterface*		   GetWriterClass() const;
	inline DBWindowViewer* GetViewer() { return WindowViewer; }
	inline DBWindowWriter* GetWriter() { return WindowWriter; }

public:
	void OpenWindowByType(EWindows WindowType);
	void CloseWindowByType(EWindows WindowType);

	void OnWriterCloseHandle();
	void OnViewerCloseHandle();
	void SetViewerData(int MemberId, const DBFamilyData& InData);
	void SetWriterData(int MemberId, const DBFamilyData& Data, EPeopleType People);

private:
	void CreateViewer();
	void DestroyViewer();
	void CreateWriter();
	void DestroyWriter();
};

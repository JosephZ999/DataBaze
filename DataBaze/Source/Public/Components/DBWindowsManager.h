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
	void OpenWindow(EWindows WindowType);
	void CloseWindow(EWindows WindowType);
	void EndConstruct();

	DBWindowViewer* GetViewer() { return WindowViewer; }
	DBWindowWriter* GetWriter() { return WindowWriter; }

public:
	virtual DBInterface* GetSystem() override;
	HWND				 GetViewerHandle() const { return ViewerHandle; }
	HWND				 GetWriterHandle() const { return WriterHandle; }
	DBInterface*		 GetViewerClass() const;
	DBInterface*		 GetWriterClass() const;

private:
	void CreateViewer();
	void DestroyViewer();
	void CreateWriter();
	void DestroyWriter();
};

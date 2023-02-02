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
	DBWindowsManager(HINSTANCE HInstance);
	HINSTANCE HIns;
	~DBWindowsManager();

private:
	HWND ViewerHandle = 0;
	HWND WriterHandle = 0;

	DBWindowViewer* WindowViewer = nullptr;
	DBWindowWriter* WindowWriter = nullptr;

protected:
	void OpenWindow(EWindows WindowType);
	void CloseWindow(EWindows WindowType);

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

public:
	friend class DBSystem;
};

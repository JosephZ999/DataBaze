#include "Source/Public/Utils/DBFunctionLibrary.h"

void DBLib::SetWindowVisibility(HWND Window, bool bShow)
{
	ShowWindow(Window, (bShow) ? SW_SHOWDEFAULT : SW_HIDE);
}

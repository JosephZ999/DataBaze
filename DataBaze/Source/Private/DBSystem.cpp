
#include "DBSystem.h"

//void DBSystem::SetMinimizedMode(bool Enabled)
//{
//	MinimizedMode = Enabled;
//	const int Offset = MinimizedMode ? -300 : 0;
//	SetWindowPos(MainWindow, HWND_TOP, 0, 0, 740 + Offset, 450, SWP_NOMOVE);
//	if (Enabled)
//	{
//		SetWindowPos(MainWindow, HWND_TOP, 25, 25, 0, 0, SWP_NOSIZE);
//	}
//
//	const int Offset2 = MinimizedMode ? -190 : 0;
//	for (auto nBtn : Buttons.Windows)
//	{
//		switch (nBtn.Id)
//		{
//		case IDC_PrevFolder:
//		{
//			SetWindowPos(nBtn.Window, 0, nBtn.Position.X + Offset2, nBtn.Position.Y, 0, 0, SWP_NOSIZE);
//			break;
//		}
//		case IDC_NextFolder:
//		{
//			SetWindowPos(nBtn.Window, 0, nBtn.Position.X + Offset2, nBtn.Position.Y, 0, 0, SWP_NOSIZE);
//			break;
//		}
//		case IDC_Minimize:
//		{
//			SetWindowPos(nBtn.Window, 0, nBtn.Position.X + (int)((float)Offset * 0.95f), nBtn.Position.Y, 0, 0, SWP_NOSIZE);
//			break;
//		}
//		default:
//		{
//			SetWindowPos(nBtn.Window, 0, nBtn.Position.X + Offset, nBtn.Position.Y, 0, 0, SWP_NOSIZE);
//		}
//		} //  switch id
//	}
//	SetWindowPos(FolderText.Window, 0, FolderText.Position.X + Offset2, FolderText.Position.Y, 0, 0, SWP_NOSIZE);
//	SetWindowPos(ListLen.Window, 0, ListLen.Position.X + Offset2, ListLen.Position.Y, 0, 0, SWP_NOSIZE);
//	SetWindowPos(ListBox, HWND_TOP, 200 + Offset2, 25, 500 + (int)((float)Offset2 * 0.5f), 280, 0);
//}

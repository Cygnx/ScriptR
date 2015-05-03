/////////////////////Memory.h
#include <Windows.h>
#include "pointers.h"
#include <exception>
using namespace std;
#define NewThread(Function) CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Function, NULL, 0, NULL);
void telemousefly();
template <typename T>
bool Read(T* ptValue, void* lpvBase, unsigned int uOffset)
{
	if (IsReadable(lpvBase))
	{
		void* lpvTemp = *reinterpret_cast<void**>(lpvBase);
		lpvTemp = reinterpret_cast<unsigned char*>(lpvTemp)+uOffset;

		if (IsReadable(lpvTemp))
		{
			memcpy(ptValue, lpvTemp, sizeof(T));
			return true;
		}
	}

	return false;
}

template <typename T>
bool VirtualRead(T* ptValue, void* lpvBase, unsigned int uOffsets, ...)
{
	if (IsReadable(lpvBase))
	{
		va_list va;
		va_start(va, uOffsets);

		void* lpvTemp = lpvBase;

		for (unsigned int uIndex = 0; uIndex < uOffsets; uIndex++)
		{
			if (!IsReadable(lpvTemp))
			{
				va_end(va);
				return false;
			}

			lpvTemp = *reinterpret_cast<void**>(lpvTemp);
			lpvTemp = reinterpret_cast<unsigned char*>(lpvTemp)+va_arg(va, unsigned int);
		}

		va_end(va);

		if (IsReadable(lpvTemp))
		{
			memcpy(ptValue, lpvTemp, sizeof(T));
			return true;
		}
	}

	return false;
}

template <typename T>
bool Write(void* lpvBase, unsigned int uOffset, T tValue)
{
	if (IsReadable(lpvBase))
	{
		void* lpvTemp = *reinterpret_cast<void**>(lpvBase);
		lpvTemp = reinterpret_cast<unsigned char*>(lpvTemp)+uOffset;

		if (IsWriteable(lpvTemp))
		{
			memcpy(lpvTemp, &tValue, sizeof(T));
			return true;
		}
	}
	return false;
}

template <typename T>
bool VirtualWrite(void* lpvBase, unsigned int uOffsets, T tValue, ...)
{
	if (IsReadable(lpvBase))
	{
		va_list va;
		va_start(va, tValue);

		void* lpvTemp = lpvBase;

		for (unsigned int uIndex = 0; uIndex < uOffsets; uIndex++)
		{
			if (!IsReadable(lpvTemp))
			{
				va_end(va);
				return false;
			}

			lpvTemp = *reinterpret_cast<void**>(lpvTemp);
			lpvTemp = reinterpret_cast<unsigned char*>(lpvTemp)+va_arg(va, unsigned int);
		}

		va_end(va);

		if (IsWriteable(lpvTemp))
		{
			memcpy(lpvTemp, &tValue, sizeof(T));
			return true;
		}
	}

	return false;
}

bool IsReadable(void* lpvAddress)
{
	if (lpvAddress == NULL)
		return false;

	MEMORY_BASIC_INFORMATION mbi;

	if (VirtualQuery(lpvAddress, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) != 0)
	{
		if (!mbi.Protect || mbi.Protect & PAGE_GUARD)
			return false;

		if ((mbi.Protect & PAGE_EXECUTE_READ) || (mbi.Protect & PAGE_EXECUTE_READWRITE) || (mbi.Protect & PAGE_READONLY) || (mbi.Protect & PAGE_READWRITE))
			return true;
	}

	return false;
}

bool IsWriteable(void* lpvAddress)
{
	if (lpvAddress == NULL)
		return false;

	MEMORY_BASIC_INFORMATION mbi;

	if (VirtualQuery(lpvAddress, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == sizeof(MEMORY_BASIC_INFORMATION))
	{
		if (!mbi.Protect || mbi.Protect & PAGE_GUARD)
			return false;

		if ((mbi.Protect & PAGE_WRITECOPY) || (mbi.Protect & PAGE_READWRITE) || (mbi.Protect & PAGE_READWRITE) || (mbi.Protect & PAGE_EXECUTE_READWRITE) || (mbi.Protect & PAGE_EXECUTE_WRITECOPY))
			return true;
	}

	return false;
}

ULONG_PTR ReadPointer(DWORD Address, INT nOffset)
{
	ULONG_PTR* ulBase = (ULONG_PTR *)Address;
	if (!IsBadReadPtr((VOID*)ulBase, sizeof(ULONG_PTR)))
		if (!IsBadReadPtr((VOID*)((*(ULONG_PTR*)ulBase) + nOffset), sizeof(ULONG_PTR)))
			return *(ULONG_PTR*)((*(ULONG_PTR*)ulBase) + nOffset);
	return 0;
}

ULONG_PTR ReadMultiPointer(ULONG_PTR ulBase, int level, ...)
{
	ULONG_PTR ulResult = 0;
	int i, offset;

	va_list vaarg;
	va_start(vaarg, level);

	if (!IsBadReadPtr((void*)ulBase, sizeof(ULONG_PTR)))
	{
		ulBase = *(ULONG_PTR*)ulBase;
		for (i = 0; i < level; i++)
		{
			offset = va_arg(vaarg, int);
			if (IsBadReadPtr((void*)(ulBase + offset), sizeof(ULONG_PTR)))
			{
				va_end(vaarg);
				return 0;
			}
			ulBase = *(ULONG_PTR*)(ulBase + offset);
		}
		ulResult = ulBase;
	}

	va_end(vaarg);
	return ulResult;
}

HWND WINAPI FindProcessWindow(__in_z LPCSTR lpcszWindowClass, __in DWORD dwProcessId)
{
	DWORD   dwWindowId;
	CHAR    pszClassName[200];
	HWND    hWnd;

	hWnd = GetTopWindow(NULL);

	while (hWnd != NULL)
	{
		if (GetClassNameA(hWnd, pszClassName, 200) > 0)
			if (lstrcmpiA(lpcszWindowClass, pszClassName) == 0)
				if (GetWindowThreadProcessId(hWnd, &dwWindowId))
					if (dwWindowId == dwProcessId)
						return hWnd;

		hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
	}
	return NULL;
}
//------------------------------------------------------------------------------
int GetMapID()
{
	return (int)ReadPointer(MapInfoBase, MapIDOff);
}
int GetMobCount()
{
	return (int)ReadPointer(MobBase, MobCountOff);
}

int GetCharX()
{
	return (int)ReadPointer(dwCharacterBase, dwCharacterX);
}

int GetCharY()
{
	return (int)ReadPointer(dwCharacterBase, dwCharacterY);
}

int GetAnimation()
{
	return (int)ReadMultiPointer(dwCharacterBase, dwPID, dwAnimation);
}

bool IsInRangeX(int iPosition, int iMin, int iMax)
{
	return iPosition >= iMin && iPosition <= iMax;
}

bool IsInRangeY(int iPosition, int iMin, int iMax)
{
	return iMin <= iPosition && iMax >= iPosition;
}

void ToggleKnockback()
{
	VirtualWrite(CUserLocal, 2, TRUE, CUserLocal__Data::iPIDOffset, CUserLocal__Data::PID::iKnockbackOffset);
}

void ChangeDirection(bool fLeft)
{
	VirtualWrite(CUserLocal, 2, fLeft ? 4 : 5, CUserLocal__Data::iPIDOffset, CUserLocal__Data::PID::iAnimationOffset);
}

void InduceKnockback(bool fXAxis, bool fPositive)
{
	static const float fKnockBack = 3.763671875;
	if (fXAxis)// left right
	{
		VirtualWrite(CUserLocal, 2, fKnockBack * (fPositive ? -1 : 1), CUserLocal__Data::iPIDOffset, CUserLocal__Data::PID::iKnockbackXOffset);
	}
	else
	{
		VirtualWrite(CUserLocal, 2, fKnockBack * (fPositive ? -1 : 1), CUserLocal__Data::iPIDOffset, CUserLocal__Data::PID::iKnockbackYOffset);
	}
}

void JumpDown()
{
	typedef void(__fastcall* CUserLocal__FallDown_t)(void* lpvEcx, void* lpvEdx);
	static CUserLocal__FallDown_t CUserLocal__FallDown = reinterpret_cast<CUserLocal__FallDown_t>(0x01C0C104); // 8B 35 ? ? ? ? ? 8B F9 8D 8E (Start)

	CUserLocal__FallDown(*reinterpret_cast<void**>(CUserLocal), 0);
}

bool Knockback(int X, int Y, int Trys)
{
	bool bLeft = false;
	HWND MShwnd = FindProcessWindow("MapleStoryClass", GetCurrentProcessId());

	int iTry = 0;
	while (iTry < Trys)
	{
		if (bLeft && !IsInRangeX(GetCharX(), X , X))
		{
			InduceKnockback(true, X < GetCharX());
		}
		else if (!bLeft && !IsInRangeX(GetCharX(), X , X))
		{
			InduceKnockback(true, X < GetCharX());
		}

		if (!IsInRangeY(GetCharY(), Y , Y))
		{
			InduceKnockback(false, Y < GetCharY());
		}
		else if (IsInRangeY(GetCharY(), Y, Y))
		{
			InduceKnockback(false, Y < GetCharY());
		}

		Sleep(5);
		iTry++;
		ToggleKnockback();
		//  PostMessage(MShwnd, WM_KEYDOWN, 0x60, MapVirtualKey(VK_DOWN, 0) << 16);
		KeyDown(DOWN);
		SendKey(VK_MENU);
		KeyDown(DOWN);
		SendKey(VK_MENU);

	}
	KeyUp(DOWN);

	if (bLeft)
		return (IsInRangeX(GetCharX(), X + 10, X) && IsInRangeY(GetCharY(), Y - 10, Y));
	else
		return (IsInRangeX(GetCharX(), X - 10, X) && IsInRangeY(GetCharY(), Y - 10, Y));
}
void flyTo(int x, int y)
{
	while (GetCharX() !=x && GetCharY() != y)
	{
		Knockback(x, y, 1);

		Sleep(20);
	}
}
bool bKami;
void mouseFly()
{
	bKami = true;

	while (bKami)
	{
		int CurrentMouseX = readMultiPointer((ULONG_PTR)MouseBase,2,MouseLocation,MouseX);
		int CurrentMouseY = readMultiPointer((ULONG_PTR)MouseBase,2,MouseLocation,MouseY);
		string title = "X:" + std::to_string(CurrentMouseX) + " || Y:" + std::to_string(CurrentMouseY);
		SetConsoleTitle (title.c_str());
		// Monster = GetMobXY();

		//// Knockback(CurrentMouseX - 70, CurrentMouseY - 30, 30);
		Knockback(CurrentMouseX, CurrentMouseY, 1);

		//JumpDown();
		Sleep(10);
	}

}

const DWORD MapBase = 0x01C107B0; //A1 ? ? ? ? 8B 50 ? 83 C0
const DWORD LeftWallOff = 0x0000001C; //mov below MapBase
const DWORD TopWallOff = LeftWallOff + 4;
const DWORD RightWallOff = TopWallOff + 4;
const DWORD BottomWallOff = RightWallOff + 4;
const DWORD PlatformOff = 0x00000080; //
const DWORD ADDRESS_SetMapleStoryData = 0x5E1B70; //56 8B ? 8B ? ? ? ? ? 41 [4th Result]005C6F60

const DWORD TeleportX = 0x0000B694; //8D 8E ? ? ? ? C7 44 24 14 0A 00 00 00 E8 ? ? ? ? 68
const DWORD TeleportY = TeleportX - 0x0C;
const DWORD TeleportToggle = TeleportY - 0x18; //or TeleportX - 0x24
bool LeftWall(int X)
{
	return ((int)ReadPointer(MapBase, LeftWallOff) <= X);
}

bool TopWall(int Y)
{
	return ((int)ReadPointer(MapBase, TopWallOff ) <= Y);
}

bool RightWall(int X)
{
	return ((int)ReadPointer(MapBase, RightWallOff ) >= X);
}

bool BottomWall(int Y)
{
	return ((int)ReadPointer(MapBase, BottomWallOff) >= Y);
}

typedef void (__thiscall* TSecType_long__SetData_t)(void* lpvEcx, const int data);
TSecType_long__SetData_t SetMapleStoryData = reinterpret_cast<TSecType_long__SetData_t>(ADDRESS_SetMapleStoryData);

void WallCheckTeleport(int X, int Y)
{
	cout <<"HIASD" <<endl;

	cout << "yes" << endl;
	void* lpvBase = *reinterpret_cast<void**>(dwCharacterBase);

	SetMapleStoryData(reinterpret_cast<unsigned char*>(lpvBase) + TeleportX, X);
	SetMapleStoryData(reinterpret_cast<unsigned char*>(lpvBase) + TeleportY, Y);
	SetMapleStoryData(reinterpret_cast<unsigned char*>(lpvBase) + TeleportToggle, 1);

}
void Teleport(long x, long y)
{
	try{
		auto GetTeleportBase = reinterpret_cast<void*(__thiscall*)(void*)>(0x01312970); // GetCVecCtrlUser: 8B ? ? ? ? ? 85 ? 74 ? 83 ? ? 74 ? 83 ? ? C3 33 ? C3 CC CC CC CC CC CC CC CC CC CC 8B 44 [Third Result]
		auto Teleport = reinterpret_cast<void(__thiscall*)(void* lpvEcx, bool bToggle, long x, long y)>(0x0146A880); // CVecCtrlUser__OnTeleport: 8B ? ? ? 8B ? ? 8B ? ? 56 8D ? ? 8B ? ? ? 51 52 56 FF ? 85 ? 7D ? 68 ? ? ? ? 56 50 E8 ? ? ? ? 5E C2 [First Result]

		return Teleport(GetTeleportBase(PVOID(*reinterpret_cast<DWORD*>(0x01C0A108) + 4)), TRUE, x, y); // TSingleton_CUserLocal___ms_pInstance also known as "CharBase": A1 ? ? ? ? 85 C0 75 ? 5F C3 8D 48 [Pointer]

	}
	catch (std::exception& e)
	{
		dbgOutput("tele FAILED");
	}
}
void telemousefly()
{
	const DWORD MouseBase = 0x01C0C114; // 8B 0D ? ? ? ? 74 ? 83
		const DWORD MouseLocation = 0x978; // 8B ? ? ? ? ? 85 ? 75 ? 68 ? ? ? ? E8 ? ? ? 00 8B ? 24 ? 8B ? 8B ? ? ? ? ? F7
		const DWORD MouseX = 0x8c-4; //8B 88 ? ? ? ? 6A ? 57 83 C5 ? 55 [Any Result]
		const DWORD MouseY = MouseX + 4;
	int CurrentMouseX = readMultiPointer((ULONG_PTR)MouseBase,2,MouseLocation,MouseX);
	int CurrentMouseY = readMultiPointer((ULONG_PTR)MouseBase,2,MouseLocation,MouseY);
	// Monster = GetMobXY();

	//// Knockback(CurrentMouseX - 70, CurrentMouseY - 30, 30);
	if (LeftWall(CurrentMouseX) && RightWall(CurrentMouseX) && TopWall(CurrentMouseY) && BottomWall(CurrentMouseY))
		Teleport(CurrentMouseX, CurrentMouseY);

}
void record()
{
	vector <string> records;
	string mystring;
	for(;;Sleep(100))
	{
			mystring = " [MapID]::" + to_string(GetMapID()) + " [X]::" + std::to_string(GetCharX()) + " [Y]::" + std::to_string(GetCharY());
		if(GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(0x53) ) // s key
		{
			dbgOutput(mystring);
		}
		if(GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(0x52) ) //r key
		{
			MessageBoxA(HWND_DESKTOP,mystring.c_str(),"CHORD", MB_OK);
		}
	}
}
void walkto(int int1, int int2, bool key)
{
	if(GetCharX() != int1)
	{
	while(GetCharX() > int1)
	{
		if(!key)
			SendKey(int2);
		KeyDown(LEFT);
		Sleep(1);
	}
	KeyUp(LEFT);
	}
	if(GetCharX() != int1)
	{
	while(GetCharX() < int1)
	{
		if(!key)
			SendKey(int2);
		KeyDown(RIGHT);
		Sleep(1);
	}
	KeyUp(RIGHT);
	}
}
void waitForMapID(int id)
{
	while(GetMapID() != id)
	{
		Sleep(1);
	}
}
void waitIfNoMonsters()
{
	while(GetMobCount() == 0)
	{
		Sleep(1);
	}
}
void leeBot()
{
	for(;;)
	{
		dbgOutput("Looping...");
		switch(GetMapID())
		{
		case 620100052: 
			{
				dbgOutput("1st map");
				walkto(205,0,false);
				KeyDown(UP);
				for(int i = GetMapID();i==GetMapID();Sleep(1))
				{
					KeyDown(UP);
				}
				KeyUp(UP);
				break;
			}
		case 620100066: 
			{
				dbgOutput("2nd map");
				while(GetMobCount() > 0)
				{
					int pos = GetCharX();
					if(pos > 1434)
						walkto(352,0,false);
					else
						walkto(2517,0,false);
				}
				for(int i = GetMapID();i==GetMapID();Sleep(1))
				{
					KeyDown(UP);
				}
				KeyUp(UP);
				break;
			}
		default: 
			{
				//Sleep(100);
				dbgOutput("Other maps");
		/*		int posi = GetCharX(); 
				if(GetMapID() == 620100056)
					posi = -317;*/
				for(int i = GetMapID();i==GetMapID();Sleep(1))
				{
		//			walkto(posi,0,false);
					KeyDown(UP);
				}
				KeyUp(UP);
				break;
			}
		}
	}
}
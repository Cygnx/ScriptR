#ifndef POINTERS_H
#define POINTERS_H
#include <Windows.h>
#include <strsafe.h>

bool Check(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for(; *szMask; ++szMask, ++pData, ++bMask)
        if(*szMask == 'x' && *pData != *bMask ) 
            return false;
	
    return (*szMask) == NULL;
}

string& replaceAll(string& context, const string& from, const string& to) 
{
	size_t lookHere = 0;
	size_t foundHere;
	while((foundHere = context.find(from, lookHere)) != string::npos) 
	{
		context.replace(foundHere, from.size(), to);
		lookHere = foundHere + to.size();
	}
	return context;
}

DWORD FindPattern (string Bytes, DWORD dwOffset, bool isPointer)
{
	replaceAll(Bytes, " ", "");

	char AddyBytes [100];
	char mask [60];

	sprintf (mask, "");
	sprintf (AddyBytes, "");

	for (unsigned i = 0, pos = 0; i <  Bytes.length(); i+=2, pos++)
	{
		string SendBuffer = Bytes.substr(i, 2).c_str();

		if (!strcmp (SendBuffer.c_str(), "??"))
		{
			AddyBytes [pos] = 0;
			strcat (mask, "?");
		}
		else
		{
			AddyBytes [pos] = (BYTE)strtol (SendBuffer.c_str(), 0, 16);
			strcat (mask, "x");
		}
	}

	DWORD Addy = NULL;

	//Get Addy
    for(DWORD i = 0; i < 0x00F00000; i++)
	{
		if( Check ((BYTE*)( 0x00400000 + i ), (BYTE *)AddyBytes, mask) )
		{
			Addy = (DWORD)(0x00400000 + i + dwOffset);
			break;
		}
	}

	if (isPointer)
		Addy = *(DWORD*)Addy;

    return Addy;
}

__inline ULONG_PTR ReadPointer(ULONG_PTR* ulBase, INT nOffset)
{
	if ( !IsBadReadPtr((VOID*)ulBase, sizeof(ULONG_PTR)) )
		if ( !IsBadReadPtr((VOID*)((*(ULONG_PTR*)ulBase)+nOffset), sizeof(ULONG_PTR)) )
			return *(ULONG_PTR*)((*(ULONG_PTR*)ulBase)+nOffset);
	return 0;
}
unsigned long GetPointedAddress(unsigned long ulBase, int iOffset)
{
        __try { return *(unsigned long*)ulBase + iOffset; }
        __except (EXCEPTION_EXECUTE_HANDLER) { return 0; }
}

ULONG_PTR readMultiPointer(ULONG_PTR ulBase, int level, ...)
{
	ULONG_PTR ulResult = 0;
	int i, offset;

	va_list vaarg;
	va_start(vaarg, level);

	if (!IsBadReadPtr((void*)ulBase, sizeof(ULONG_PTR)))
	{
		ulBase = *(ULONG_PTR*)ulBase; //dereference the address of the pointer
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
//-------------------------------------------------------------------------------------------------------------------------------------------
DWORD charInfoAddy = 0x01C5C268;//8B 0D ? ? ? ? 89 ? 24 ? ? ? 6A
DWORD charHpOffset = 0x00002624;//89 8E ? ? 00 00 80 BE ? ? 00 00 00 75
DWORD charMpOffset = charHpOffset + 4;

DWORD CharAlertBase = 0x01C609F8;//8B 15 ? ? ? ? 8B 4A ? 8D 0C 89
DWORD CharHPAlertOffset = 0x00000058; //Under AlertBase
DWORD CharMPAlertOffset = CharHPAlertOffset + 4;


DWORD dwCharacterBase = 0x01C5C264, // 8B 3D ? ? ? ? 8B 40 
      dwCharacterX = 0x0000CA70, // 89 8E ? ? ? ? 8B 50 ? 8B 06 89 96 ? ? ? ? 8B 50
          dwCharacterY = dwCharacterX + 4,
          dwPID = 0x4BCC,
          dwAnimation = 0x0144,
		  dwCharAtkCountOffset = 0x0000B930; //89 ? ? ? ? 00 C7 ? ? ? ? 00 ? ? ? 00 8D ? ? ? ? 00 C6
const DWORD MapInfoBase = 0x01C61010; //8B 0D ? ? ? ? E8 ? ? ? ? 8B 0D ? ? ? ? 85 C9 ? ? ? ? ? ? E8
const DWORD MapIDOff = 0x000013B4; //8B 8F ? ? ? ? ? ? 8B 0D ? ? ? ? 8D 54
// -- need this for kami
const DWORD MobBase = 0x01C6090C; //8B 0D ? ? ? ? ? E8 ? ? ? ? 8B ? 85 ? 74 ? 8B ? ? 8B ? ? 8D [both work]
const DWORD MobCountOff = 0x10; //8B 52 ? 83 C7 ? 8D 44 24 ? 50 8B CF FF D2 8B 00 89
namespace CUserLocal__Data
{
        const int iPIDOffset = 0x00004D30; // 8B 86 ? ? 00 00 6A D8
        namespace PID
        {       const int iKnockbackXOffset = 0x000001C0; // 83 B9 ? ? 00 00 00 D9 EE
                const int iKnockbackYOffset = iKnockbackXOffset+0x8; // iKnockbackXOffset + 0x08
                const int iAnimationOffset = 0x0144;
                const int iKnockbackOffset = iKnockbackXOffset - 0x0C;  // iKnockbackXOffset - 0x0C

        }
        const int iCharacterX = 0xC264;
        const int iCharacterY = 0xC268;
}
const LPDWORD CUserLocal = reinterpret_cast<const LPDWORD>(0x01C5C264); // 8B 3D ? ? ? ? 8B 40
const DWORD MouseBase = 0x01C5C260; // 8B 0D ? ? ? ? 74 ? 83 [use last result]
		const DWORD MouseLocation = 0x00000978; // 8B ? ? ? ? ? 85 ? 75 ? 68 ? ? ? ? E8 ? ? ? 00 8B ? 24 ? 8B ? 8B ? ? ? ? ? F7
		const DWORD MouseX = 0x00000098; //8B 88 ? ? ? ? 6A ? 57 83 C5 ? 55 [Any Result]
		const DWORD MouseY = MouseX + 4;
// 	const DWORD MobBase = 0x01B69140; //8B 0D ? ? ? ? ? E8 ? ? ? ? 8B ? 85 ? 74 ? 8B ? ? 8B ? ? 8D
//						const DWORD Mob1Off = 0x00000028; //89 7E ? 89 7E ? ? ? ? ? ? 89 46
//						const DWORD Mob2Off = 0x00000004; //89 7B ? 89 ? 24 ? 0F ? D6
//						const DWORD Mob3Off = 0x000001C4; //89 ? ? ? ? 00 89 ? ? ? ? 00 89 ? ? ? ? 00 89 ? ? ? ? 00 89 ? ? ? ? 00 89 ? ? ? ? 00 89 ? ? ? ? 00 C7 ? ? ? ? 00 ? ? ? ? 89 ? ? ? ? 00 89 ? ? ? ? 00 89 ? ? ? ? 00 8B ? 24 ? 8D
//						const DWORD Mob4Off = 0x00000020; //89 ? 24 ? C7 01 ? ? ? ? 89 59 ? C7
//						const DWORD MobXOff = 0x00000058; //89 ? ? 8D ? ? 89 ? 8D ? ? 89 ? 8D
//						const DWORD MobYOff = MobXOff + 4;
//
//						unsigned long Kami1 = GetPointedAddress(MobBase, Mob1Off);
//						unsigned long Kami2 = GetPointedAddress(Kami1, Mob2Off);
//						unsigned long Kami3 = GetPointedAddress(Kami2, Mob3Off);
//						unsigned long Kami4 = GetPointedAddress(Kami3, Mob4Off);
//
//						int xMOB = ReadPointer(LPDWORD(Kami4), MobXOff);
//						int yMOB = ReadPointer(LPDWORD(Kami4), MobYOff);
#endif

//http://pastebin.com/AzEMscRU
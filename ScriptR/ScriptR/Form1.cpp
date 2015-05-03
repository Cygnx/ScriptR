#include "Form1.h"
#include "keys.h"
#include "pointers.h"
#include <iostream>
#include <iomanip>
#include "kami.h"
using namespace ScriptR; 

void dbgOutput(string s)
{
	SYSTEMTIME t;
	GetLocalTime(&t); 
	printf("%02i:%02i:%02i:: ", t.wHour, t.wMinute, t.wSecond);
	cout << s;
	cout << endl;
}
void printScriptStatuses()
{
	system("cls");
	cout << "Status: " << endl;
	for(script & c: myScripts)
		c.printStatus();
}
void runScript(LPVOID i)
{
	script* found = NULL;
	int hotkey = (int)i;

	for(script & c: myScripts)
	{
		if(c.key == hotkey){
			while(c.status)
			{
				for(string s: c.commands)
				{
					int key = -1, int2 = 0, int1 = 0;
					string buffer = "";

					int scanResultForString = sscanf(s.c_str(), "%s %i %i", &buffer, &int1, &int2);
					if(sscanf(s.c_str(), "%x %i %i", &key, &int2, &int1) == 3)	// Key delay repeats
					{
						for(int i = 0;i < int1;i++)
						{
							SendKey(key);
							Sleep(int2);
						}
					}
					else if(scanResultForString == 2 && strcmp(buffer.c_str(), "sleep") == 0)	// sleep time
					{
						Sleep(int1);
					}
					else if(scanResultForString == 3 && strcmp(buffer.c_str(), "autohp") == 0) //autohp key threshold
					{
						sscanf(s.c_str(), "%s %x %i", &buffer, &int1, &int2);

						*(DWORD*)(*(DWORD*)CharAlertBase  + CharHPAlertOffset) = 20;
						if(ReadPointer((ULONG_PTR*)charInfoAddy, charHpOffset) <= int2)
							SendKey(int1);
						Sleep(100);
					}
					else if(scanResultForString == 3 && strcmp(buffer.c_str(), "automp") == 0) //automp key threshold
					{
						sscanf(s.c_str(), "%s %x %i", &buffer, &int1, &int2);

						*(DWORD*)(*(DWORD*)CharAlertBase  + CharMPAlertOffset) = 20;
						if(ReadPointer((ULONG_PTR*)charInfoAddy, charMpOffset) <= int2)
							SendKey(int1);
						Sleep(100);
					}
					else if(strcmp(buffer.c_str(), "left") == 0 || strcmp(buffer.c_str(), "right") == 0 || strcmp(buffer.c_str(), "down") == 0 || strcmp(buffer.c_str(), "up") == 0)	// move
					{
						cout << ReadPointer((ULONG_PTR*)dwCharacterBase, dwCharacterX)<<endl;
						if(strcmp(buffer.c_str(), "right") == 0) // equal strings
						{
							for(int i = ReadPointer((ULONG_PTR*)dwCharacterBase, dwCharacterX);
								ReadPointer((ULONG_PTR*)dwCharacterBase, dwCharacterX) <  i + int1;
								KeyDown(RIGHT))
							{
								Sleep(1);
							}
							KeyUp(RIGHT); 
							Sleep(int2);
						}
						else if(strcmp(buffer.c_str(), "left") == 0) // equal strings
						{
							for(int i = ReadPointer((ULONG_PTR*)dwCharacterBase, dwCharacterX);
								ReadPointer((ULONG_PTR*)dwCharacterBase, dwCharacterX) >  i - int1;
								KeyDown(LEFT))
							{
								Sleep(1);
							}
							KeyUp(LEFT); 
							Sleep(int2);
						}
						else if(strcmp(buffer.c_str(), "down") == 0) // equal strings
						{
							for(int i = 0;
								i < int1;
								KeyDown(DOWN))
							{
								i++;
								Sleep(1);
							}
							dbgOutput("key UP");
							KeyUp(DOWN);
						}
						else if(strcmp(buffer.c_str(), "up") == 0) // equal strings
						{
							for(int i = 0;
								i < int1;
								KeyDown(UP))
							{
								i++;
								Sleep(1);
							}
							dbgOutput("key UP");
							KeyUp(UP);
						}
					}
					else if(scanResultForString == 1 && strcmp(buffer.c_str(), "autochat") == 0) //autochat string
					{
						s.erase (s.begin(),s.begin()+strlen("autochat")); 
						//dbgOutput( s.c_str());
						SendKey(0x0D);
						Sleep(200);
						HWND MSHWND = FindWindow ("MapleStoryClass",0);
						for(int i = 0;i < strlen(s.c_str()); i++)
						{
							PM(MSHWND, WM_CHAR, (int)s[i], 0);
						}
						SendKey(0x0D);
						Sleep(200);
						SendKey(0x0D);
						Sleep(200);
					}
					else if(strcmp(buffer.c_str(), "mousefly") == 0) //dbg
					{
						mouseFly();
					}
					else if(strcmp(buffer.c_str(), "mousetele") == 0) 
					{
						cout<< "here"<<endl;
						telemousefly();
					}
					else if(strcmp(buffer.c_str(), "showcharcords") == 0) //dbg
					{
						string title = "X:" + std::to_string(GetCharX()) + " || Y:" + std::to_string(GetCharY());
						SetConsoleTitle (title.c_str());
					}
					else if(strcmp(buffer.c_str(), "walkto") == 0) //dbg
					{
						bool key = false;
						if(sscanf(s.c_str(), "%s %i %x", &buffer, &int1, &int2) == 3)
							bool key = true;
						walkto(int1, int2, key);
					}
					else if(strcmp(buffer.c_str(), "flyto") == 0) 
					{
						flyTo(int1, int2);
					}
					else if(strcmp(buffer.c_str(), "jumpdown") == 0) 
					{
						KeyDown(DOWN);
						SendKey(VK_MENU);
						KeyDown(DOWN);
						SendKey(VK_MENU);
						KeyUp(DOWN);
					}
					else if(strcmp(buffer.c_str(), "climbup") == 0) 
					{
						SendKey(VK_MENU);
						while(GetCharY() != int1)
						{
							KeyDown(UP);
						}
						KeyUp(UP);
					}
					else if(strcmp(buffer.c_str(), "climbdown") == 0) 
					{
						SendKey(VK_MENU);
						while(GetCharY() != int1)
						{
							KeyDown(DOWN);
						}
						KeyUp(DOWN);
					}
					else if(strcmp(buffer.c_str(), "turnleft") == 0) 
					{
						ChangeDirection(FALSE);
						Sleep(1000);
					}
					else if(strcmp(buffer.c_str(), "turnright") == 0) 
					{
						ChangeDirection(TRUE);
						Sleep(1000);
					}
					else if(strcmp(buffer.c_str(), "scriptoff") == 0) 
					{
						c.status = FALSE;
					}
					else if(strcmp(buffer.c_str(), "record") == 0) 
					{
						record();
					}
					else if(strcmp(buffer.c_str(), "leeBot") == 0) 
					{
						leeBot();
					}
					else if(strcmp(buffer.c_str(), "waitformap") == 0) 
					{
						waitForMapID(int1);
					}
					else if(strcmp(buffer.c_str(), "waitifnomonsters") == 0) 
					{
						waitIfNoMonsters();
					}

					else
					{
						dbgOutput("Invalid command: " + s);
						dbgOutput("Disabling all scripts for this hotkey");
						c.commands.clear();
					}
				}
			}
		}
	}
}
void TrainerLoop()
{    
	for(;;)
	{
		for(script & i: myScripts)
		{
			if (GetAsyncKeyState(i.key) & 0x8000)
			{
				if(!i.status) // script is off 
				{
					i.status = true; // script is on
					i.scriptThread = CreateThread(NULL, 0, (::LPTHREAD_START_ROUTINE)&runScript, reinterpret_cast<LPVOID>(i.key), 0, NULL) ;
				}
				else
				{
					i.status = false;
					TerminateThread(i.scriptThread, 0);
					CloseHandle(i.scriptThread);
				}
				printScriptStatuses();
				Sleep(300);
			}
		}
	}
}
void Main(void)
{
	dbgOutput("ScriptR GMS v154 -Cygnx");
	dbgOutput("Load a script to begin");
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew Form1); // Your Form Name
	Application::Exit();
}

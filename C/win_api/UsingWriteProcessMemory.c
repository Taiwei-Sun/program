//**************************************
// Name: ^!!~Using WriteProcessMemory~!!^
// Description:I have found out many people want to know how to use WriteProcessMemory due to the fact of writing memory trainers to cheat in video games in such. I have seen that PSC did not have a very good example of this so I guess ill just post this small code snipet! This is the non-mfc version.
// By: fritz0x00 (from psc cd)
//
// Assumes:This will write a couple of bytes to a address in the running memory of a process. This will not work in MFC. I will post a MFC version soon. ~ Revised 3-20-04, cleaned up error handling, couple of bugs and etc.
//**************************************

#include <windows.h>
void main()
{
	HWND hWnd = FindWindow(0, "MSN Messenger"); //** Define the window we are looking for
	if(hWnd == NULL) //** If window is not found, Error cannot find window! 
	{ 
		MessageBox(0, "Error cannot find window!", "Error!", MB_OK + MB_ICONERROR); 
	} 
	else
	{
		DWORD proc_id; 
		//** Declares DWORD proc_id which will later hold 
		//** the handle for the process ID which we hook 
		//** the process with 
		GetWindowThreadProcessId(hWnd, &proc_id); 
		//** Gets process ID by window caption, stores 
		//** the recived process id in 'proc_id' 
		//** which we declared earlier with: 
		//** 'DWORD proc_id; 
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_id); 
		//** Opens the process with 
		//** by using the stored ID 
		//** in 'proc_id' 
		if(!hProcess) //** If cannot 'OpenProcess', then it displays the error message 
		{ 
			MessageBox(NULL, "Cannot open process!", "Error!", MB_OK + MB_ICONERROR); 
		}
		else
		{
			//** Below; The data to write to the address which we later state 
			BYTE newdata[]={0x72, 0x65, 0x64, 0x70, 0x6F, 0x74, 0x69, 0x6F, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
			DWORD newdatasize = sizeof(newdata); 
			//** Below; We use WriteProcessMemory to our stored process, 'hProcess' 
			//** and writes 'newdata' to our address '(LPVOID)0x5C6600' 
			if(WriteProcessMemory(hProcess, (LPVOID)0x5C6600, &newdata, newdatasize, NULL)) 
			{ 
				//** If we write the memory successfully, we let the user know 
				MessageBox(NULL, "WriteProcessMemory is a success!", "Success!", MB_OK + MB_ICONINFORMATION); 
				//** Alerts user that it has written 
			} 
			else 
			{ 
				//** If we cant write the memory to our process, we notify 
				//** the user with the following error message; 
				MessageBox(NULL, "Error cannot WriteProcessMemory!", "Error!", MB_OK + MB_ICONERROR); 
			} 
			//** Closes process after we write the memory, 
			//** helps stop memory leaks 
			CloseHandle(hProcess); 
		}
	}
} 
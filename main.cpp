#include<iostream>
#include <windows.h>
#include <stdio.h> 
#include <tchar.h>
#include <strsafe.h>

using namespace std;

int main(int argc,char** argv)
{	
	if(argc > 1)
	{
		cout<< argv[1]<<endl;
		
		LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\rdatapipe");

		// Create a pipe to send data
		HANDLE pipe = CreateNamedPipe(lpszPipename,PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE,1,0,0,0,NULL);

		if (pipe == NULL || pipe == INVALID_HANDLE_VALUE) 
		{
			_tprintf(TEXT("CreateNamedPipe failed, GLE=%d.\n"), GetLastError()); 
			return -1;
		}else{
			 wcout << "Waiting for a client to connect to the pipe..." << endl;
			 // This call blocks until a client process connects to the pipe
			 BOOL result = ConnectNamedPipe(pipe, NULL);
			 if (!result)
			 {
				_tprintf(TEXT("Failed to make connection on named pipe, GLE=%d.\n"), GetLastError());
				CloseHandle(pipe); // close the pipe
				return 1;
			 }else{
				 wcout << "Sending data to pipe..." << endl;
 
				// This call blocks until a client process reads all the data
				DWORD numBytesWritten = 0;
				result = WriteFile(pipe, argv[1], strlen(argv[1])* sizeof(char *), &numBytesWritten, NULL);
 
				if (result) {
					wcout << "Number of bytes sent: " << numBytesWritten << endl;
				} else {
					wcout << "Failed to send data." << endl;
					// look up error code here using GetLastError()
				}
 
				// Close the pipe (automatically disconnects client too)
				CloseHandle(pipe);
 
				wcout << "Done." << endl;
				return 0;
			 }
		}
	}else{
		cout<< "File path is not recieved" << endl;
		return 1;
	}
}

//https://stackoverflow.com/questions/11564148/how-to-get-the-starting-base-address-of-a-process-in-c
#define _CRT_SECURE_NO_WARNINGS
#define UNINITIALIZED 0xFFFFFFFF

#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <TlHelp32.h> //PROCESSENTRY

/* The name of the process */
const char* processName_ = "REPLACETHIS.exe" ; 

void main(void)
{
DWORD  processID_     = NULL ;
DWORD  processBaseAddress_   = UNINITIALIZED;

/* Get the process ID  */
{
    PROCESSENTRY32 processEntry_ ; // Entry into process you wish to inject to
    HANDLE hProcSnapshot_ = NULL ; 
    /* Takes a snapshot of the system's processes */
    hProcSnapshot_ = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) ; //?

    /* While process has not been found, keep looking for it */
    while(!processID_)
    {
        /* If a process on the system exists */
        if(Process32First(hProcSnapshot_, &processEntry_)) //?
        {
            /* Check all processes in the system's processes snapshot */
            do
            {
                /* Compare the name of the process to the one we want */
                if( !strcmp(processEntry_.szExeFile, processName_) ) //?
                {
                    /* Save the processID and break out */
                    processID_ = processEntry_.th32ProcessID ;
                    break ;
                }
            } 
            while(Process32Next(hProcSnapshot_, &processEntry_)) ;
        }

        /* Didnt find process, sleep for a bit */
        if( !processID_ )
        {
            system("CLS") ;
            std::cout << "Make sure " << processName_ << " is running." << std::endl ;
            Sleep(200) ;
        }
    }

    /* Process found */
    std::cout << "Found Process: " << processName_ << std::endl ;
}


/* Find Base Address of process */
{
    HANDLE moduleSnapshotHandle_ = INVALID_HANDLE_VALUE;
    MODULEENTRY32 moduleEntry_;

    /* Take snapshot of all the modules in the process */
    moduleSnapshotHandle_ = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, processID_ );

    /* Snapshot failed */
    if( moduleSnapshotHandle_ == INVALID_HANDLE_VALUE )
    {
        std::cout << "Module Snapshot error" << std::endl ;
        return ;
    }

    /* Size the structure before usage */
    moduleEntry_.dwSize = sizeof( MODULEENTRY32 );

    /* Retrieve information about the first module */
    if( !Module32First( moduleSnapshotHandle_, &moduleEntry_ ) )
    {
        std::cout << "First module not found" << std::endl ;  
        CloseHandle( moduleSnapshotHandle_ );    
        return ;
    }

    /* Find base address */
    while(processBaseAddress_ == UNINITIALIZED)
    {
        /* Find module of the executable */
        do
        {

            /* Compare the name of the process to the one we want */
            if( !strcmp(moduleEntry_.szModule, processName_) ) //?
            {
                /* Save the processID and break out */
                processBaseAddress_ = (unsigned int)moduleEntry_.modBaseAddr ;
                break ;
            }

        } while( Module32Next( moduleSnapshotHandle_, &moduleEntry_ ) );


        if( processBaseAddress_ == UNINITIALIZED )
        {
            system("CLS") ;
            std::cout << "Failed to find module" << processName_ << std::endl ;
            Sleep(200) ;
        }
    }

    /* Found module and base address successfully */
    std::cout << "Base Address: " << std::hex << processBaseAddress_ << std::dec << std::endl ;
    CloseHandle( moduleSnapshotHandle_ );
}
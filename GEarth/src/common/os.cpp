#include "OS.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace COMMON_NAMESPACE
{
    COS::COS()
    {
    }


    double COS::GetMemory()
    {
#ifdef _WIN32
#define ProcessVmCounters 3
#define NT_SUCCESS(x) ((x) >= 0)
        typedef NTSTATUS(NTAPI *pfnZwQueryInformationProcess)(
            IN HANDLE ProcessHandle,
            IN ULONG ProcessInformationClass,
            OUT PVOID ProcessInformation,
            IN ULONG ProcessInformationLength,
            OUT PULONG ReturnLength OPTIONAL
            );

        typedef struct _VM_COUNTERS_EX
        {
            SIZE_T PeakVirtualSize;
            SIZE_T VirtualSize;
            ULONG PageFaultCount;
            SIZE_T PeakWorkingSetSize;
            SIZE_T WorkingSetSize;
            SIZE_T QuotaPeakPagedPoolUsage;
            SIZE_T QuotaPagedPoolUsage;
            SIZE_T QuotaPeakNonPagedPoolUsage;
            SIZE_T QuotaNonPagedPoolUsage;
            SIZE_T PagefileUsage;
            SIZE_T PeakPagefileUsage;
            SIZE_T PrivateUsage;
            SIZE_T PrivateWorkingSetSize;
            ULONGLONG ShaderdCommitUsage;
        }VM_COUNTERS_EX, *PVM_COUNTERS_EX;


        HANDLE handle = GetCurrentProcess();
        HMODULE NtdllModule = GetModuleHandle("ntdll.dll");
        pfnZwQueryInformationProcess NtWow64QueryInformationProcess64 = (pfnZwQueryInformationProcess)GetProcAddress(NtdllModule, "ZwQueryInformationProcess");

        VM_COUNTERS_EX vm = { 0 };
        if (NT_SUCCESS(NtWow64QueryInformationProcess64(handle, ProcessVmCounters, &vm, sizeof(vm), 0)))
        {
            double memory = vm.PrivateWorkingSetSize / 1024.0 / 1024.0;
            return memory;
        }
#else
        return 0.0;
#endif
        return 0.0;
    }

    unsigned int COS::GenerateUID() const
    {
        return 0;
    }

    std::string COS::GenerateUSID() const
    {
        return "0";
    }

    COS::~COS()
    {
    }
}


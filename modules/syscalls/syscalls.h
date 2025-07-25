#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "include/peb.h"
#include "include/shared.h"


typedef unsigned __int64 QWORD;
DWORD g_NtCreateThreadExSSN;
QWORD g_NtCreateThreadExSyscall;
DWORD g_NtCloseSSN;
QWORD g_NtCloseSyscall;
DWORD g_NtWaitForSingleObjectSSN;
QWORD g_NtWaitForSingleObjectSyscall;
DWORD g_NtCreateMutantSSN;
QWORD g_NtCreateMutantSyscall;
DWORD g_NtReleaseMutantSSN;
QWORD g_NtReleaseMutantSyscall;

typedef NTSTATUS (NTAPI *PUSER_THREAD_START_ROUTINE)(
    _In_ PVOID ThreadParameter
);

typedef struct _PS_ATTRIBUTE {
    ULONG_PTR Attribute;
    SIZE_T Size;
    union
    {
        ULONG_PTR Value;
        PVOID ValuePtr;
    };
    PSIZE_T ReturnLength;
} PS_ATTRIBUTE, *PPS_ATTRIBUTE;

typedef struct _PS_ATTRIBUTE_LIST {
    SIZE_T TotalLength;
    PS_ATTRIBUTE Attributes[1];
} PS_ATTRIBUTE_LIST, *PPS_ATTRIBUTE_LIST;

typedef struct _LDR_DATA_TABLE_ENTRY {
    PVOID Reserved1[2];
    LIST_ENTRY InMemoryOrderLinks;
    PVOID Reserved2[2];
    PVOID DllBase;
    PVOID EntryPoint;
    PVOID Reserved3;
    UNICODE_STRING FullDllName;
    BYTE Reserved4[8];
    PVOID Reserved5[3];
    union {
        ULONG CheckSum;
        PVOID Reserved6;
    };
    ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

extern NTSTATUS NtCreateThreadEx(
    _Out_ PHANDLE ThreadHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_ HANDLE ProcessHandle,
    _In_ PUSER_THREAD_START_ROUTINE StartRoutine,
    _In_opt_ PVOID Argument,
    _In_ ULONG CreateFlags, 
    _In_ SIZE_T ZeroBits,
    _In_ SIZE_T StackSize,
    _In_ SIZE_T MaximumStackSize,
    _In_opt_ PPS_ATTRIBUTE_LIST AttributeList
);

extern NTSTATUS NtClose(
    IN HANDLE Handle
);

extern NTSTATUS NtWaitForSingleObject(
    _In_ HANDLE Handle,
    _In_ BOOLEAN Alertable,
    _In_opt_ PLARGE_INTEGER Timeout
);

extern NTSTATUS NtCreateMutant(
    PHANDLE MutantHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    BOOLEAN InitialOwner
);

extern NTSTATUS NtReleaseMutant(
    IN HANDLE MutantHandle,
    OUT PLONG PreviousCount OPTIONAL
);

extern PPEB _getPeb(void);

/**
 * @brief Queries the process ID (PID) by process name.
 *
 * @param processName Pointer to a null-terminated string specifying the process name.
 *                    Must not be NULL.
 * @return DWORD Process ID if the process exists, otherwise returns 0.
 */
DWORD getProcessPID(const char* processName);

/**
 * @brief Retrieves the syscall number and syscall instruction address for a given NT function.
 *
 * @param NtdllHandle Handle to the loaded ntdll.dll module.
 * @param NtFunctionName Pointer to a null-terminated string with the NT function name.
 * @param NtFunctionSSN Pointer to a DWORD that will receive the system call number (SSN).
 * @param NtFunctionSyscall Pointer to a UINT_PTR that will receive the address of the syscall instruction.
 */
VOID IndirectPrelude(HMODULE NtdllHandle, char NtFunctionName[], PDWORD NtFunctionSSN, PUINT_PTR NtFunctionSyscall);

/**
 * @brief Retrieves the base address of ntdll.dll module by parsing the PEB.
 *
 * @return HMODULE Returns the base address of ntdll.dll if found, otherwise returns NULL.
 */
HMODULE getModuleHandle();

/**
 * @brief Retrieves the address of an exported function from a given module.
 *
 * @param module Handle to the module from which the function address will be retrieved.
 * @param target Null-terminated string representing the name of the function to locate.
 *
 * @return UINT_PTR Returns the address of the exported function if found, otherwise returns 0.
 */
UINT_PTR getAddr(HMODULE module, char target[]);

#endif

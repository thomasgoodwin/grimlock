#include "CrashHandler.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dbghelp.h>
#include <iostream>

static void printStackTrace(CONTEXT* ctx)
{
  HANDLE process = GetCurrentProcess();
  HANDLE thread = GetCurrentThread();

  SymInitialize(process, nullptr, TRUE);
  SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);

  STACKFRAME64 frame = {};
  DWORD machineType;

#ifdef _M_X64
  machineType = IMAGE_FILE_MACHINE_AMD64;
  frame.AddrPC.Offset = ctx->Rip;
  frame.AddrPC.Mode = AddrModeFlat;
  frame.AddrFrame.Offset = ctx->Rbp;
  frame.AddrFrame.Mode = AddrModeFlat;
  frame.AddrStack.Offset = ctx->Rsp;
  frame.AddrStack.Mode = AddrModeFlat;
#elif defined(_M_IX86)
  machineType = IMAGE_FILE_MACHINE_I386;
  frame.AddrPC.Offset = ctx->Eip;
  frame.AddrPC.Mode = AddrModeFlat;
  frame.AddrFrame.Offset = ctx->Ebp;
  frame.AddrFrame.Mode = AddrModeFlat;
  frame.AddrStack.Offset = ctx->Esp;
  frame.AddrStack.Mode = AddrModeFlat;
#else
#error "Unsupported architecture for crash handler"
#endif

  char symbolBuf[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char)];
  SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(symbolBuf);
  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
  symbol->MaxNameLen = MAX_SYM_NAME;

  IMAGEHLP_LINE64 line = {};
  line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

  std::cerr << "\n<===== CALL STACK =====>\n";

  for (int i = 0; i < 64; ++i)
  {
    if (!StackWalk64(machineType, process, thread, &frame, ctx,
      nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr)) {
      break;

    }

    if (frame.AddrPC.Offset == 0)
      break;

    std::cerr << "[" << i << "] ";

    DWORD64 symDisp = 0;
    if (SymFromAddr(process, frame.AddrPC.Offset, &symDisp, symbol)) {
      std::cerr << symbol->Name;
      DWORD lineDisp = 0;
      if (SymGetLineFromAddr64(process, frame.AddrPC.Offset, &lineDisp, &line)) {
        std::cerr << "  (" << line.FileName << ":" << line.LineNumber << ")";
      }
    }
    else
    {
      std::cerr << "0x" << std::hex << frame.AddrPC.Offset << std::dec << "  (no symbol)";
    }

    std::cerr << "\n";
  }

  std::cerr << "<=======================>\n";
  std::cerr.flush();

  SymCleanup(process);
}

static LONG WINAPI onUnhandledException(EXCEPTION_POINTERS* info)
{
  const char* name = "Unknown";
  switch (info->ExceptionRecord->ExceptionCode)
  {
  case EXCEPTION_ACCESS_VIOLATION:      name = "Access Violation";        break;
  case EXCEPTION_STACK_OVERFLOW:        name = "Stack Overflow";          break;
  case EXCEPTION_ILLEGAL_INSTRUCTION:   name = "Illegal Instruction";     break;
  case EXCEPTION_INT_DIVIDE_BY_ZERO:    name = "Integer Divide by Zero";  break;
  case EXCEPTION_FLT_DIVIDE_BY_ZERO:    name = "Float Divide by Zero";    break;
  case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: name = "Array Bounds Exceeded";   break;
  case EXCEPTION_DATATYPE_MISALIGNMENT: name = "Data Misalignment";       break;
  }

  std::cerr << "\n*** CRASH: " << name
    << " (code 0x" << std::hex << info->ExceptionRecord->ExceptionCode
    << " at 0x" << info->ExceptionRecord->ExceptionAddress << ")\n" << std::dec;

  printStackTrace(info->ContextRecord);

  return EXCEPTION_EXECUTE_HANDLER;
}

void installCrashHandler()
{
  SetUnhandledExceptionFilter(onUnhandledException);
}

#else
void installCrashHandler() {}
#endif

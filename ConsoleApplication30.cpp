// ============================================================
//  SysFix - Windows 系统修复工具 (CLI)
//  最低支持: Windows 7 (x86 / x64)
//
//  编译 (MSVC, 需以 UTF-8 保存源码并带 BOM):
//    cl /nologo /W3 /O2 /EHsc /utf-8 SysFix.cpp ^
//       /link /SUBSYSTEM:CONSOLE
//
//  编译 (MinGW-w64):
//    g++ -O2 -static -o SysFix.exe SysFix.cpp ^
//        -liphlpapi -ldnsapi -lshell32 -lole32 -loleaut32 -lwbemuuid -luuid
// ============================================================

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601   // Windows 7
#endif
#ifndef WINVER
#define WINVER 0x0601
#endif

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <iphlpapi.h>
#include <windns.h>
#include <netlistmgr.h>
#include <wbemidl.h>
#include <comdef.h>

#include <string>
#include <vector>
#include <cstdio>
#include <cstdarg>
#include <cwctype>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "dnsapi.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "uuid.lib")

// ============================================================
//  语言 / 字符串表
// ============================================================

enum StrId {
    S_TITLE = 0,
    S_SELECT_LANG,
    S_PROMPT_LANG,
    S_INVALID_INPUT,
    S_ADMIN_WARN,
    S_MAIN_MENU_HEADER,
    S_MENU_1,
    S_MENU_2,
    S_MENU_3,
    S_PROMPT_MENU,
    S_PRESS_ENTER,

    S_NET_STEP1,
    S_NET_NO_LINK,
    S_NET_OK,
    S_NET_NO_INTERNET,
    S_NET_STEP2,
    S_DNS_ISSUE,
    S_DNS_FIX_ASK,
    S_DNS_FIXING,
    S_DNS_FIXED,
    S_DNS_FIX_FAILED,
    S_DNS_NOT_ISSUE,
    S_NET_UNKNOWN,

    S_SLOW_STEP1,
    S_TEMP_CLEANED,
    S_SLOW_STEP2,
    S_BIN_CLEANED,
    S_BIN_EMPTY,
    S_BIN_FAIL,

    S_BYE,
    S_COUNT
};

static const wchar_t* g_str[2][S_COUNT] = {
    /* ---------------- English ---------------- */
    {
        L"Windows System Repair Tool",
        L"Please select language / \u8BF7\u9009\u62E9\u8BED\u8A00",
        L"Enter [1/2]: ",
        L"Invalid input, please try again.",
        L"[!] Warning: not running as Administrator. Some repairs may fail.",

        L"===== Main Menu =====",
        L"  1. Repair network problems",
        L"  2. Fix system slowness (clean temp files & recycle bin)",
        L"  0. Exit",
        L"Enter your choice: ",
        L"Press Enter to continue...",

        L"[1/3] Checking network connection...",
        L"    [X] Your Ethernet cable is not plugged in properly, or Wi-Fi is not connected.",
        L"    [OK] Network connection looks normal.",
        L"    [!] Connected, but there is no Internet access.",
        L"[2/3] Testing DNS resolution...",
        L"    [X] DNS resolution failed. Your DNS configuration seems broken.",
        L"    Do you want to set DNS to 8.8.8.8 (preferred) / 114.114.114.114 (alternate)? [Y/n]: ",
        L"    Setting DNS servers...",
        L"    [OK] DNS servers updated. Please reconnect the network or restart your browser.",
        L"    [X] Failed to set DNS. Please run this tool as Administrator.",
        L"    [!] DNS is not the problem. Please check your router and broadband connection.",
        L"    [?] Unable to determine the network state.",

        L"[1/2] Cleaning temporary files...",
        L"    [OK] Removed %d file(s), freed %.2f MB.",
        L"[2/2] Emptying the recycle bin...",
        L"    [OK] Recycle bin emptied.",
        L"    [OK] Recycle bin is already empty.",
        L"    [X] Failed to empty the recycle bin.",

        L"Goodbye!"
    },
    /* ---------------- 中文 ---------------- */
    {
        L"Windows \u7CFB\u7EDF\u4FEE\u590D\u5DE5\u5177",
        L"\u8BF7\u9009\u62E9\u8BED\u8A00 / Please select language",
        L"\u8BF7\u8F93\u5165 [1/2]: ",
        L"\u8F93\u5165\u65E0\u6548\uFF0C\u8BF7\u91CD\u8BD5\u3002",
        L"[!] \u8B66\u544A\uFF1A\u5F53\u524D\u672A\u4EE5\u7BA1\u7406\u5458\u8EAB\u4EFD\u8FD0\u884C\uFF0C\u90E8\u5206\u4FEE\u590D\u53EF\u80FD\u5931\u8D25\u3002",

        L"===== \u4E3B\u83DC\u5355 =====",
        L"  1. \u4FEE\u590D\u7F51\u7EDC\u95EE\u9898",
        L"  2. \u4FEE\u590D\u7CFB\u7EDF\u5361\u987F\uFF08\u6E05\u7406\u4E34\u65F6\u6587\u4EF6\u4E0E\u56DE\u6536\u7AD9\uFF09",
        L"  0. \u9000\u51FA",
        L"\u8BF7\u8F93\u5165\u9009\u9879: ",
        L"\u6309\u56DE\u8F66\u952E\u7EE7\u7EED...",

        L"[1/3] \u6B63\u5728\u68C0\u67E5\u7F51\u7EDC\u8FDE\u63A5...",
        L"    [X] \u60A8\u7684\u7F51\u7EBF\u672A\u63D2\u7A33\u6216\u8005WIFI\u672A\u8FDE\u63A5\u3002",
        L"    [OK] \u7F51\u7EDC\u8FDE\u63A5\u6B63\u5E38\u3002",
        L"    [!] \u5DF2\u8FDE\u63A5\uFF0C\u4F46\u65E0\u6CD5\u8BBF\u95EE Internet\u3002",
        L"[2/3] \u6B63\u5728\u68C0\u6D4B DNS ...",
        L"    [X] DNS \u89E3\u6790\u5931\u8D25\uFF0C\u60A8\u7684 DNS \u914D\u7F6E\u53EF\u80FD\u5B58\u5728\u95EE\u9898\u3002",
        L"    \u662F\u5426\u5C06 DNS \u4FEE\u6539\u4E3A\u9996\u9009 8.8.8.8\uFF0C\u5907\u9009 114.114.114.114 ？[Y/n]: ",
        L"    \u6B63\u5728\u8BBE\u7F6E DNS ...",
        L"    [OK] DNS \u5DF2\u4FEE\u6539\uFF0C\u8BF7\u91CD\u8BD5\u7F51\u7EDC\u6216\u91CD\u542F\u6D4F\u89C8\u5668\u3002",
        L"    [X] DNS \u4FEE\u6539\u5931\u8D25\uFF0C\u8BF7\u4EE5\u7BA1\u7406\u5458\u8EAB\u4EFD\u8FD0\u884C\u672C\u5DE5\u5177\u3002",
        L"    [!] \u4E0D\u662F DNS \u7684\u95EE\u9898\uFF0C\u8BF7\u68C0\u67E5\u8DEF\u7531\u5668\u548C\u5BBD\u5E26\u8FDE\u63A5\u3002",
        L"    [?] \u65E0\u6CD5\u786E\u5B9A\u7F51\u7EDC\u72B6\u6001\u3002",

        L"[1/2] \u6B63\u5728\u6E05\u7406\u4E34\u65F6\u6587\u4EF6...",
        L"    [OK] \u5DF2\u6E05\u7406 %d \u4E2A\u6587\u4EF6\uFF0C\u91CA\u653E %.2f MB\u3002",
        L"[2/2] \u6B63\u5728\u6E05\u7A7A\u56DE\u6536\u7AD9...",
        L"    [OK] \u56DE\u6536\u7AD9\u5DF2\u6E05\u7A7A\u3002",
        L"    [OK] \u56DE\u6536\u7AD9\u5DF2\u7ECF\u662F\u7A7A\u7684\u3002",
        L"    [X] \u6E05\u7A7A\u56DE\u6536\u7AD9\u5931\u8D25\u3002",

        L"\u518D\u89C1\uFF01"
    }
};

static int g_lang = 0;   // 0 = English, 1 = 中文
#define T(id) (g_str[g_lang][(id)])

// ============================================================
//  控制台 I/O
// ============================================================

static HANDLE g_hOut = NULL;
static HANDLE g_hIn = NULL;

static void ConOut(const std::wstring& s)
{
    if (!g_hOut) g_hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD mode = 0;
    if (g_hOut && g_hOut != INVALID_HANDLE_VALUE && GetConsoleMode(g_hOut, &mode))
    {
        DWORD written = 0;
        WriteConsoleW(g_hOut, s.c_str(), (DWORD)s.size(), &written, NULL);
    }
    else
    {
        // 输出被重定向到文件/管道：用 UTF-8 写
        int len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), (int)s.size(),
            NULL, 0, NULL, NULL);
        if (len > 0)
        {
            std::string a((size_t)len, '\0');
            WideCharToMultiByte(CP_UTF8, 0, s.c_str(), (int)s.size(),
                &a[0], len, NULL, NULL);
            fwrite(a.data(), 1, a.size(), stdout);
            fflush(stdout);
        }
    }
}

static void ConOut(const wchar_t* s) { ConOut(std::wstring(s)); }

static void ConOutF(const wchar_t* fmt, ...)
{
    wchar_t buf[2048];
    va_list ap;
    va_start(ap, fmt);
    int n = _vsnwprintf(buf, 2047, fmt, ap);
    va_end(ap);
    if (n < 0) n = 2047;
    buf[n] = L'\0';
    ConOut(std::wstring(buf, (size_t)n));
}

static std::wstring ConIn()
{
    if (!g_hIn) g_hIn = GetStdHandle(STD_INPUT_HANDLE);

    DWORD mode = 0;
    if (g_hIn && g_hIn != INVALID_HANDLE_VALUE && GetConsoleMode(g_hIn, &mode))
    {
        wchar_t buf[512];
        DWORD read = 0;
        if (ReadConsoleW(g_hIn, buf, 511, &read, NULL))
        {
            std::wstring s(buf, read);
            while (!s.empty() && (s[s.size() - 1] == L'\n' || s[s.size() - 1] == L'\r'))
                s.erase(s.size() - 1);
            return s;
        }
        return L"";
    }
    else
    {
        std::string line;
        char c;
        while (fread(&c, 1, 1, stdin) == 1 && c != '\n')
            line.push_back(c);
        if (line.empty()) return L"";
        int len = MultiByteToWideChar(CP_UTF8, 0, line.c_str(), (int)line.size(), NULL, 0);
        std::wstring w((size_t)len, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, line.c_str(), (int)line.size(), &w[0], len);
        return w;
    }
}

// ============================================================
//  工具函数
// ============================================================

static bool IsElevated()
{
    BOOL elevated = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
        TOKEN_ELEVATION te;
        DWORD sz = sizeof(te);
        if (GetTokenInformation(hToken, TokenElevation, &te, sizeof(te), &sz))
            elevated = te.TokenIsElevated;
        CloseHandle(hToken);
    }
    return elevated == TRUE;
}

static bool AskYesNo(const wchar_t* prompt)
{
    ConOut(prompt);
    std::wstring ans = ConIn();
    if (ans.empty()) return true;              // 直接回车 = 是
    wchar_t c = (wchar_t)towlower(ans[0]);
    return (c == L'y' || c == L'1');
}

// 把 "8.8.8.8" 解析成内存中的 DWORD（与 inet_addr 的字节序一致）
static DWORD ParseIPv4(const char* s)
{
    unsigned int a = 0, b = 0, c = 0, d = 0;
    if (sscanf(s, "%u.%u.%u.%u", &a, &b, &c, &d) != 4) return 0;
    return (DWORD)((a & 0xFF) | ((b & 0xFF) << 8) | ((c & 0xFF) << 16) | ((d & 0xFF) << 24));
}

// ============================================================
//  网络诊断
// ============================================================

struct NetStatus {
    bool connected;   // 是否有网络连接（网线 / Wi-Fi）
    bool internet;    // 是否能访问 Internet
    bool ok;          // 查询是否成功
};

static NetStatus GetNetStatus()
{
    NetStatus st = { false, false, false };

    INetworkListManager* pNLM = NULL;
    HRESULT hr = CoCreateInstance(CLSID_NetworkListManager, NULL, CLSCTX_ALL,
        IID_INetworkListManager, (void**)&pNLM);
    if (FAILED(hr) || !pNLM) return st;

    VARIANT_BOOL bConn = VARIANT_FALSE;
    VARIANT_BOOL bNet = VARIANT_FALSE;

    hr = pNLM->get_IsConnected(&bConn);
    if (SUCCEEDED(hr))
    {
        st.connected = (bConn == VARIANT_TRUE);
        hr = pNLM->get_IsConnectedToInternet(&bNet);
        if (SUCCEEDED(hr))
            st.internet = (bNet == VARIANT_TRUE);
        st.ok = true;
    }

    pNLM->Release();
    return st;
}

// 读取系统当前配置的 DNS 服务器
static std::vector<DWORD> GetSystemDnsServers()
{
    std::vector<DWORD> result;

    ULONG size = 0;
    if (GetNetworkParams(NULL, &size) != ERROR_BUFFER_OVERFLOW) return result;

    std::vector<BYTE> buf(size);
    FIXED_INFO* pFi = reinterpret_cast<FIXED_INFO*>(buf.data());
    if (GetNetworkParams(pFi, &size) != ERROR_SUCCESS) return result;

    IP_ADDR_STRING* p = &pFi->DnsServerList;
    while (p)
    {
        DWORD ip = ParseIPv4(p->IpAddress.String);
        if (ip != 0) result.push_back(ip);
        p = p->Next;
    }
    return result;
}

// 用系统 DNS 解析
static bool ResolveWithSystem(const char* host)
{
    PDNS_RECORD pRec = NULL;
    DNS_STATUS s = DnsQuery_A(host, DNS_TYPE_A, DNS_QUERY_STANDARD,
        NULL, &pRec, NULL);
    if (pRec) DnsRecordListFree(pRec, DnsFreeRecordList);
    return (s == ERROR_SUCCESS);
}

// 指定 DNS 服务器解析
static bool ResolveWithServer(const char* host, const char* server)
{
    IP4_ARRAY arr;
    ZeroMemory(&arr, sizeof(arr));
    arr.AddrCount = 1;
    arr.AddrArray[0] = ParseIPv4(server);

    PDNS_RECORD pRec = NULL;
    DNS_STATUS s = DnsQuery_A(host, DNS_TYPE_A,
        DNS_QUERY_BYPASS_CACHE,
        (PVOID)&arr, &pRec, NULL);
    if (pRec) DnsRecordListFree(pRec, DnsFreeRecordList);
    return (s == ERROR_SUCCESS);
}

// 通过 WMI 设置所有已启用适配器的 DNS
static bool SetDnsServers(const wchar_t* dns1, const wchar_t* dns2,
    std::wstring& errMsg)
{
    bool anyOk = false;

    IWbemLocator* pLoc = NULL;
    IWbemServices* pSvc = NULL;
    IEnumWbemClassObject* pEnum = NULL;
    IWbemClassObject* pClass = NULL;
    IWbemClassObject* pInDef = NULL;
    IWbemClassObject* pInParams = NULL;

    HRESULT hr = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (void**)&pLoc);
    if (FAILED(hr)) { errMsg = L"WMI: CoCreateInstance failed"; goto done; }

    hr = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, NULL,
        0, NULL, NULL, &pSvc);
    if (FAILED(hr)) { errMsg = L"WMI: ConnectServer failed"; goto done; }

    hr = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
        RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, EOAC_NONE);
    if (FAILED(hr)) { errMsg = L"WMI: CoSetProxyBlanket failed"; goto done; }

    hr = pSvc->GetObject(_bstr_t(L"Win32_NetworkAdapterConfiguration"),
        0, NULL, &pClass, NULL);
    if (FAILED(hr)) { errMsg = L"WMI: GetObject failed"; goto done; }

    hr = pClass->GetMethod(_bstr_t(L"SetDNSServerSearchOrder"), 0, &pInDef, NULL);
    if (FAILED(hr)) { errMsg = L"WMI: GetMethod failed"; goto done; }

    hr = pInDef->SpawnInstance(0, &pInParams);
    if (FAILED(hr)) { errMsg = L"WMI: SpawnInstance failed"; goto done; }

    {
        SAFEARRAY* psa = SafeArrayCreateVector(VT_BSTR, 0, 2);
        if (!psa) { errMsg = L"WMI: SafeArrayCreateVector failed"; goto done; }

        BSTR b1 = SysAllocString(dns1);
        BSTR b2 = SysAllocString(dns2);
        LONG i0 = 0, i1 = 1;
        SafeArrayPutElement(psa, &i0, b1);
        SafeArrayPutElement(psa, &i1, b2);
        SysFreeString(b1);
        SysFreeString(b2);

        VARIANT v = { 0 };
        VariantInit(&v);
        v.vt = VT_ARRAY | VT_BSTR;
        v.parray = psa;

        hr = pInParams->Put(_bstr_t(L"DNSServerSearchOrder"), 0, &v, 0);
        VariantClear(&v);
        if (FAILED(hr)) { errMsg = L"WMI: Put failed"; goto done; }
    }

    hr = pSvc->ExecQuery(
        _bstr_t(L"WQL"),
        _bstr_t(L"SELECT * FROM Win32_NetworkAdapterConfiguration WHERE IPEnabled = TRUE"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL, &pEnum);
    if (FAILED(hr)) { errMsg = L"WMI: ExecQuery failed"; goto done; }

    {
        IWbemClassObject* pObj = NULL;
        ULONG ret = 0;
        while (pEnum->Next(WBEM_INFINITE, 1, &pObj, &ret) == S_OK)
        {
            // 取出实例的 __PATH
            VARIANT vPath = { 0 };
            VariantInit(&vPath);
            HRESULT hrPath = pObj->Get(_bstr_t(L"__PATH"), 0, &vPath, NULL, NULL);

            IWbemClassObject* pOut = NULL;
            if (SUCCEEDED(hrPath))
            {
                // 注意：改用 pSvc->ExecMethod，并传入实例路径
                hr = pSvc->ExecMethod(vPath.bstrVal,
                    _bstr_t(L"SetDNSServerSearchOrder"),
                    0, NULL, pInParams, &pOut, NULL);
            }

            if (SUCCEEDED(hr) && pOut)
            {
                VARIANT vRet = { 0 };
                VariantInit(&vRet);
                if (SUCCEEDED(pOut->Get(_bstr_t(L"ReturnValue"), 0, &vRet, NULL, NULL)))
                {
                    if (vRet.vt == VT_I4 && vRet.lVal == 0)
                        anyOk = true;
                }
                VariantClear(&vRet);
                pOut->Release();
            }

            VariantClear(&vPath);
            pObj->Release();
            pObj = NULL;
        }
    }

done:
    if (pInParams) pInParams->Release();
    if (pInDef)    pInDef->Release();
    if (pClass)    pClass->Release();
    if (pEnum)     pEnum->Release();
    if (pSvc)      pSvc->Release();
    if (pLoc)      pLoc->Release();

    if (!anyOk && errMsg.empty())
        errMsg = L"No adapter was updated";
    return anyOk;
}

// ------------------------------------------------------------
//  网络修复主流程
// ------------------------------------------------------------
static void DoNetworkRepair()
{
    ConOut(L"\n");
    ConOut(std::wstring(T(S_NET_STEP1)) + L"\n");

    NetStatus st = GetNetStatus();

    if (!st.ok) {
        ConOut(std::wstring(T(S_NET_UNKNOWN)) + L"\n");
        return;
    }

    // 情况一：根本没有连接 —— 网线 / Wi-Fi
    if (!st.connected) {
        ConOut(std::wstring(T(S_NET_NO_LINK)) + L"\n");
        return;
    }

    // 情况二：连接正常且有网
    if (st.internet) {
        ConOut(std::wstring(T(S_NET_OK)) + L"\n");
        return;
    }

    // 情况三：已连接但无 Internet —— 检测 DNS
    ConOut(std::wstring(T(S_NET_NO_INTERNET)) + L"\n");
    ConOut(std::wstring(T(S_NET_STEP2)) + L"\n");

    static const char* kTestHosts[] = { "www.baidu.com", "www.qq.com", "www.microsoft.com" };
    const int kHostCount = 3;

    // 3.1 系统是否配置了 DNS
    std::vector<DWORD> dnsList = GetSystemDnsServers();
    bool dnsConfigured = !dnsList.empty();

    // 3.2 用系统 DNS 解析
    bool sysResolveOk = false;
    for (int i = 0; i < kHostCount && !sysResolveOk; ++i)
        sysResolveOk = ResolveWithSystem(kTestHosts[i]);

    if (sysResolveOk) {
        ConOut(std::wstring(T(S_NET_OK)) + L"\n");
        return;
    }

    // 3.3 用公共 DNS (8.8.8.8) 解析，判断是不是 DNS 的锅
    bool externalResolveOk = false;
    for (int i = 0; i < kHostCount && !externalResolveOk; ++i)
        externalResolveOk = ResolveWithServer(kTestHosts[i], "8.8.8.8");

    bool dnsIsProblem = (!dnsConfigured) || externalResolveOk;

    if (dnsIsProblem)
    {
        ConOut(std::wstring(T(S_DNS_ISSUE)) + L"\n");

        if (!AskYesNo(T(S_DNS_FIX_ASK)))
            return;

        ConOut(std::wstring(T(S_DNS_FIXING)) + L"\n");

        std::wstring err;
        if (SetDnsServers(L"8.8.8.8", L"114.114.114.114", err))
            ConOut(std::wstring(T(S_DNS_FIXED)) + L"\n");
        else
            ConOut(std::wstring(T(S_DNS_FIX_FAILED)) + L"\n");
    }
    else
    {
        // DNS 正常，但依然上不了网 —— 路由器 / 宽带
        ConOut(std::wstring(T(S_DNS_NOT_ISSUE)) + L"\n");
    }
}

// ============================================================
//  系统清理
// ============================================================

static ULONGLONG DeleteDirContents(const std::wstring& dir, int& deleted, int& failed)
{
    ULONGLONG freed = 0;

    std::wstring pattern = dir + L"\\*";
    WIN32_FIND_DATAW fd;
    HANDLE h = FindFirstFileW(pattern.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;

    do
    {
        if (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0)
            continue;

        std::wstring full = dir + L"\\" + fd.cFileName;

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            freed += DeleteDirContents(full, deleted, failed);
            if (!RemoveDirectoryW(full.c_str()))
                ++failed;
        }
        else
        {
            ULONGLONG sz = ((ULONGLONG)fd.nFileSizeHigh << 32) | fd.nFileSizeLow;
            SetFileAttributesW(full.c_str(), FILE_ATTRIBUTE_NORMAL);
            if (DeleteFileW(full.c_str()))
            {
                ++deleted;
                freed += sz;
            }
            else
            {
                ++failed;
            }
        }
    } while (FindNextFileW(h, &fd));

    FindClose(h);
    return freed;
}

static void DoSystemCleanup()
{
    // ---------- 1. 清理临时文件 ----------
    ConOut(L"\n");
    ConOut(std::wstring(T(S_SLOW_STEP1)) + L"\n");

    int deleted = 0, failed = 0;
    ULONGLONG freedBytes = 0;

    wchar_t tmpPath[MAX_PATH + 1] = { 0 };
    if (GetTempPathW(MAX_PATH, tmpPath) > 0)
    {
        std::wstring userTemp(tmpPath);
        while (!userTemp.empty() && userTemp[userTemp.size() - 1] == L'\\')
            userTemp.erase(userTemp.size() - 1);
        if (!userTemp.empty())
            freedBytes += DeleteDirContents(userTemp, deleted, failed);
    }

    // Windows 系统临时目录（普通用户可能没权限，失败会自动忽略）
    {
        wchar_t winDir[MAX_PATH + 1] = { 0 };
        if (GetWindowsDirectoryW(winDir, MAX_PATH) > 0)
        {
            std::wstring sysTemp = std::wstring(winDir) + L"\\Temp";
            freedBytes += DeleteDirContents(sysTemp, deleted, failed);
        }
    }

    ConOutF(T(S_TEMP_CLEANED), deleted, (double)freedBytes / (1024.0 * 1024.0));
    ConOut(L"\n");

    // ---------- 2. 清空回收站 ----------
    ConOut(std::wstring(T(S_SLOW_STEP2)) + L"\n");

    SHQUERYRBINFO rbInfo;
    ZeroMemory(&rbInfo, sizeof(rbInfo));
    rbInfo.cbSize = sizeof(rbInfo);

    bool binEmpty = false;
    if (SUCCEEDED(SHQueryRecycleBinW(NULL, &rbInfo)))
        binEmpty = (rbInfo.i64NumItems == 0 && rbInfo.i64Size == 0);

    if (binEmpty)
    {
        ConOut(std::wstring(T(S_BIN_EMPTY)) + L"\n");
    }
    else
    {
        HRESULT hr = SHEmptyRecycleBinW(NULL, NULL,
            SHERB_NOCONFIRMATION |
            SHERB_NOPROGRESSUI |
            SHERB_NOSOUND);
        if (hr == S_OK)
            ConOut(std::wstring(T(S_BIN_CLEANED)) + L"\n");
        else if (hr == E_UNEXPECTED)
            ConOut(std::wstring(T(S_BIN_EMPTY)) + L"\n");
        else
            ConOut(std::wstring(T(S_BIN_FAIL)) + L"\n");
    }
}

// ============================================================
//  界面
// ============================================================

static void SelectLanguage()
{
    ConOut(L"\n");
    ConOut(L"==========================================\n");
    ConOut(L"   System Repair Tool  /  \u7CFB\u7EDF\u4FEE\u590D\u5DE5\u5177\n");
    ConOut(L"==========================================\n");
    ConOut(L"   1. English\n");
    ConOut(L"   2. \u4E2D\u6587\n");
    ConOut(L"\n");

    for (;;)
    {
        ConOut(L"Select language / \u8BF7\u9009\u62E9\u8BED\u8A00 [1/2]: ");
        std::wstring s = ConIn();
        if (s == L"1") { g_lang = 0; break; }
        if (s == L"2") { g_lang = 1; break; }
        ConOut(L"Invalid input / \u8F93\u5165\u65E0\u6548\uFF0C\u8BF7\u91CD\u8BD5\u3002\n");
    }
}

static void Pause()
{
    ConOut(L"\n");
    ConOut(std::wstring(T(S_PRESS_ENTER)) + L"\n");
    ConIn();
}

static void ShowMenu()
{
    ConOut(L"\n");
    ConOut(std::wstring(T(S_MAIN_MENU_HEADER)) + L"\n");
    ConOut(std::wstring(T(S_MENU_1)) + L"\n");
    ConOut(std::wstring(T(S_MENU_2)) + L"\n");
    ConOut(std::wstring(T(S_MENU_3)) + L"\n");
}

// ============================================================
//  入口
// ============================================================

int main()
{
    // 控制台标题（默认英文，选完语言后更新）
    SetConsoleTitleW(L"SysFix");

    HRESULT hrCo = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (SUCCEEDED(hrCo))
    {
        CoInitializeSecurity(NULL, (DWORD)-1, NULL, NULL,
            RPC_C_AUTHN_LEVEL_DEFAULT,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL, EOAC_NONE, NULL);
    }

    SelectLanguage();

    SetConsoleTitleW(T(S_TITLE));

    // 管理员权限检查
    if (!IsElevated())
    {
        ConOut(L"\n");
        ConOut(std::wstring(T(S_ADMIN_WARN)) + L"\n");
    }

    for (;;)
    {
        ShowMenu();
        ConOut(std::wstring(T(S_PROMPT_MENU)));

        std::wstring choice = ConIn();

        if (choice == L"1")
        {
            DoNetworkRepair();
            Pause();
        }
        else if (choice == L"2")
        {
            DoSystemCleanup();
            Pause();
        }
        else if (choice == L"0" || choice == L"q" || choice == L"Q")
        {
            ConOut(L"\n");
            ConOut(std::wstring(T(S_BYE)) + L"\n");
            break;
        }
        else
        {
            ConOut(std::wstring(T(S_INVALID_INPUT)) + L"\n");
        }
    }

    if (SUCCEEDED(hrCo))
        CoUninitialize();

    return 0;
}
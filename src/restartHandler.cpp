#include "restartHandler.h"

#include <Windows.h>
#include <tlhelp32.h>
#include <shellapi.h>
#include <cstdio>
#include <iostream>

void restartHandler::update() {
    if (listeningForHotkey) {
        for (int vk = 8; vk < 255; vk++) {
            if (GetAsyncKeyState(vk) & 0x8000) {
                if (vk != VK_ESCAPE) {
                    hotkeyVK = vk;
                    hotkeyWasDown = true;
                }
                listeningForHotkey = false;
                break;
            }
        }
        return;
    }

    bool isDown = (GetAsyncKeyState(hotkeyVK) & 0x8000) != 0;
    if (isDown && !hotkeyWasDown) {
        attemptRestart();
    }
    hotkeyWasDown = isDown;

    if (waitingForReappear) {
        double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - lastPollTime).count();

        if (elapsed > 0.25) {
            lastPollTime = std::chrono::steady_clock::now();

            if (isProcessRunning(pendingProcessName)) {
                sendKeyPress(keyCodeToSend);
                waitingForReappear = false;
            }
        }
    }
}

void restartHandler::beginRebind() {
    listeningForHotkey = true;
}

void restartHandler::attemptRestart() {
    std::string name = processNameBuffer;

    unsigned long pid = 0;
    if (!isProcessRunning(name, &pid)) {
        std::cout << "[ERROR] Selected process is not currently running" << "\n";
        return;
    }

    std::string path = getProcessPath(pid);
    if (path.empty()) {
        std::cout << "[ERROR] Process does not belong to an executable??????" << "\n";
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess) {
        TerminateProcess(hProcess, 0);
        WaitForSingleObject(hProcess, 5000);
        CloseHandle(hProcess);
    }

    ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);

    if (sendKeyAfterRestart) {
        pendingProcessName = name;
        waitingForReappear = true;
        lastPollTime = std::chrono::steady_clock::now();
    }
}

bool restartHandler::isProcessRunning(const std::string& name, unsigned long* outPID) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    bool found = false;
    if (Process32First(snapshot, &entry)) {
        do {
            if (_stricmp(entry.szExeFile, name.c_str()) == 0) {
                found = true;
                if (outPID) *outPID = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return found;
}

std::string restartHandler::getProcessPath(unsigned long pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!hProcess) return "";

    char buffer[MAX_PATH];
    DWORD size = MAX_PATH;
    std::string result;

    if (QueryFullProcessImageNameA(hProcess, 0, buffer, &size)) {
        result = buffer;
    }

    CloseHandle(hProcess);
    return result;
}

void restartHandler::sendKeyPress(int vk) {
    INPUT input[2] = {};

    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = (WORD)vk;

    input[1].type = INPUT_KEYBOARD;
    input[1].ki.wVk = (WORD)vk;
    input[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(2, input, sizeof(INPUT));
}

std::string restartHandler::getHotkeyName() const {
    switch (hotkeyVK) {
    case VK_MEDIA_PLAY_PAUSE: return "Media Play/Pause";
    case VK_MEDIA_NEXT_TRACK: return "Media Next";
    case VK_MEDIA_PREV_TRACK: return "Media Previous";
    case VK_MEDIA_STOP:       return "Media Stop";
    case VK_VOLUME_MUTE:      return "Volume Mute";
    case VK_VOLUME_UP:        return "Volume Up";
    case VK_VOLUME_DOWN:      return "Volume Down";
    case VK_SPACE:            return "Space";
    case VK_RETURN:           return "Enter";
    case VK_TAB:              return "Tab";
    case VK_SHIFT:            return "Shift";
    case VK_CONTROL:          return "Ctrl";
    case VK_MENU:             return "Alt";
    }

    if (hotkeyVK >= VK_F1 && hotkeyVK <= VK_F24) return "F" + std::to_string(hotkeyVK - VK_F1 + 1);

    if ((hotkeyVK >= '0' && hotkeyVK <= '9') || (hotkeyVK >= 'A' && hotkeyVK <= 'Z')) return std::string(1, (char)hotkeyVK);

    char buf[16];
    sprintf_s(buf, "VK 0x%02X", hotkeyVK);
    return buf;
}
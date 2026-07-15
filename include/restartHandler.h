#pragma once

#include <string>
#include <chrono>

class restartHandler {
public:
    void update();

    void beginRebind();
    bool isListeningForHotkey() const { return listeningForHotkey; }
    std::string getHotkeyName() const;

public:
    char processNameBuffer[256] = "exampletask.exe";
    bool sendKeyAfterRestart = false;
    int  keyCodeToSend = 0;
    double additionalDelay = 0;


private:
    void attemptRestart();
    bool isProcessRunning(const std::string& name, unsigned long* outPID = nullptr);
    std::string getProcessPath(unsigned long pid);
    void sendKeyPress(int vk);

private:
    int  hotkeyVK = 0x78;
    bool listeningForHotkey = false;
    bool hotkeyWasDown = false;

    bool waitingForReappear = false;
    std::string pendingProcessName;
    std::chrono::steady_clock::time_point lastPollTime;

    bool additionalDelayBool = false;
    std::chrono::steady_clock::time_point additionalDelayStart;
    std::chrono::steady_clock::time_point sinceAdditionalDelay;
};
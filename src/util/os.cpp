#include <firesteel/util/os.hpp>

#include <array>
#include <iostream>

#ifdef FS_PFD
#include <portable-file-dialogs.hpp>
#endif // FS_PFD
#include <firesteel/common.hpp>

namespace OS {
    std::string executeInCmd(const char* cmd) {
        std::array<char, 128> buffer{};
        std::string result;
        std::unique_ptr<FILE, decltype(
#ifdef _WIN32
            & _pclose
#endif // !WIN32
#ifdef __linux__
            & pclose
#endif // !__linux__
            )> pipe(
#ifdef _WIN32
                _popen(cmd, "r"),
#endif // !WIN32
#ifdef __linux__
                popen(cmd, "r"),
#endif // !__linux__
#ifdef _WIN32
                & _pclose
#endif // !WIN32
#ifdef __linux__
                & pclose
#endif // !__linux__
            );
        if (!pipe) {
            LOG_ERRR("Couldn't execute in cmd because popen() failed");
            return "failed";
        }
        while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }
    static void openURL(const char* tUrl) {
#ifdef WIN32
        ShellExecuteA(NULL, "open", tUrl, NULL, NULL, SW_SHOWNORMAL);
#endif // !WIN32
#ifdef __linux__
        system(tUrl);
#endif // !__linux__
    }
    static void copyToClipboard(const std::string& tContents) {
#ifdef WIN32
        if (!OpenClipboard(nullptr)) {
            LOG_ERRR("Couldn't open clipboard");
            return;
        }
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (tContents.length() + 1) * sizeof(char));
        if (!hMem) {
            LOG_ERRR("Couldn't allocate clipboard memory");
            CloseClipboard();
            return;
        }
        char* text = static_cast<char*>(GlobalLock(hMem));
        if (!text) {
            LOG_ERRR("Couldn't allocate clipboard text");
            GlobalFree(hMem);
            CloseClipboard();
            return;
        }
        EmptyClipboard();
        strcpy_s(text, tContents.length() + 1, tContents.c_str());
        GlobalUnlock(hMem);
        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
#endif // WIN32
#ifdef __linux__
        FILE* pipe = popen("xclip -selection clipboard", "w");
        if (!pipe) {
            LOG_ERRR("Couldn't open clipboard");
            return;
        }
        fputs(tContents.c_str(), pipe);
        pclose(pipe);
#endif // __linux__
    }
#ifdef FS_PFD
    std::vector<std::string> fileDialog(const bool& tSave, const bool& tAllowForceful,
        std::string tDefPath, std::vector<std::string>* tFilters, const char* tTitle) {
        //Get path.
        if (tDefPath == "") tDefPath = pfd::path::home().c_str();
        //Get filters.
        std::vector<std::string> filters{ "All Files","*" };
        if (tFilters) filters = *tFilters;
        //Get title.
        if (!tTitle) {
            if (tSave) tTitle = "Save file to...";
            else tTitle = "Open file";
        }
        //Do the operation.
        if (tSave) {
            auto f = pfd::save_file(tTitle, tDefPath, filters, tAllowForceful ? pfd::opt::force_overwrite : pfd::opt::none);
            return { f.result() };
        }
        auto f = pfd::open_file(tTitle, tDefPath, filters, tAllowForceful ? pfd::opt::multiselect : pfd::opt::none);
        return f.result();
    }
    std::string folderDialog(std::string tDefPath, const char* tTitle) {
        //Get path.
        if (tDefPath == "") tDefPath = pfd::path::home().c_str();
        //Get title.
        if (!tTitle) tTitle = "Open folder";
        //Do the operation.
        auto f = pfd::select_folder(tTitle, tDefPath);
        return f.result();
    }
    void messageBox(const char* tContent,
        const MessageBoxChoice& tChoice, const MessageBoxMethods& tMethods, const Icon& tIcon, const char* tTitle) {
        //Do the operation.
        auto m = pfd::message(tTitle, tContent, (pfd::choice)tChoice, (pfd::icon)tIcon);
        //Perfome actions depending on result.
        switch (m.result()) {
        case pfd::button::ok:
            if (tMethods.clickedOk) tMethods.clickedOk();
            break;
        case pfd::button::cancel:
            if (tMethods.clickedCancel) tMethods.clickedCancel();
            break;
        case pfd::button::yes:
            if (tMethods.clickedYes) tMethods.clickedYes();
            break;
        case pfd::button::no:
            if (tMethods.clickedNo) tMethods.clickedNo();
            break;
        case pfd::button::retry:
            if (tMethods.clickedRetry) tMethods.clickedRetry();
            break;
        case pfd::button::abort:
            if (tMethods.clickedOk) tMethods.clickedAbort();
            break;
        case pfd::button::ignore:
            if (tMethods.clickedIgnore) tMethods.clickedIgnore();
            break;
        }
    }
    void notification(const char* tTitle, const char* tContent, const Icon& tIcon) {
        pfd::notify(tTitle, tContent, (pfd::icon)tIcon);
    }
#endif // FS_PFD
}
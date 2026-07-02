#ifndef FS_UTIL_OS_H
#define FS_UTIL_OS_H

#include <vector>
#include <iostream>
#include <string>

namespace OS {
    std::string executeInCmd(const char* cmd);
    static void openURL(const char* tUrl);
    static void copyToClipboard(const std::string& tContents);
#ifdef FS_PFD
    /// Calls system dialog to save or open files.
    /// @param `tSave` determines if dialog will require existing files to open or files to save to.
    /// @param `tAllowForceful` determines if OS allows user to multiselect files in open file dialog or if it forces OS to overwrite files in save file dialog.
    /// @param `tDefPath` determines dialog starting location.
    /// @param `tFilters` determines what files will be shown by files.
    /// @param `tTitle` determines title of file dialog.
    std::vector<std::string> fileDialog(const bool& tSave, const bool& tAllowForceful,
        std::string tDefPath = "", std::vector<std::string>* tFilters = nullptr, const char* tTitle = nullptr);
    /// Calls system dialog to save or open folder.
    /// @param `tDefPath` determines dialog starting location.
    /// @param `tTitle` determines title of folder dialog.
    std::string folderDialog(std::string tDefPath = "", const char* tTitle = nullptr);
    enum MessageBoxChoice {
        MBC_OK = 0,
        MBC_OK_CANCEL,
        MBC_YES_NO,
        MBC_YES_NO_CANCEL,
        MBC_RETRY_CANCEL,
        MBC_ABORT_RETRY_IGNORE
    };
    struct MessageBoxMethods {
        void(*clickedOk)()=nullptr;
        void(*clickedCancel)()=nullptr;
        void(*clickedYes)()=nullptr;
        void(*clickedNo)()=nullptr;
        void(*clickedRetry)()=nullptr;
        void(*clickedAbort)()=nullptr;
        void(*clickedIgnore)()=nullptr;
    };
    enum Icon {
        ICON_INFO = 0,
        ICON_WARNING,
        ICON_ERROR,
        ICON_QUESTION
    };
    /// Calls system message box.
    /// @param `tContent` determines the message.
    /// @param `tChoice` determines choices that are given to user.
    /// @param `tMethods` determines callbacks that will be run depending on user input.
    /// @param `tIcon` determines icon in the message box.
    /// @param `tTitle` determines title of message box.
    void messageBox(const char* tContent,
        const MessageBoxChoice& tChoice = MBC_OK, const MessageBoxMethods& tMethods = {}, const Icon& tIcon = ICON_INFO, const char* tTitle = nullptr);
    /// Calls system notification box.
    /// @param `tTitle` determines title of notification.
    /// @param `tContent` determines the message.
    /// @param `tIcon` determines icon in the notification.
    void notification(const char* tTitle, const char* tContent, const Icon& tIcon = ICON_INFO);
#endif // FS_PFD
}

#endif // !FS_UTIL_OS_H
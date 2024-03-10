#include "xengine/io/os.hpp"

#include <wtypes.h>

namespace XEngine {

	/// <summary>
	/// Opens file dialog and gets file name. Otherwise returns t_default.
	/// </summary>
	/// <param name="t_default">Default return value.</param>
	/// <param name="t_filter">Extension filter.</param>
	/// <returns>File name.</returns>
	std::string OS::open_file_dialog(std::string t_default, std::string t_filter) {
        OPENFILENAME ofn;       //Common dialog box structure.
        char szFile[260]{};     //Buffer for file name.
        HWND hwnd = nullptr;    //Owner window.
        HANDLE hf = nullptr;    //File handle.
        //Initialize OPENFILENAME.
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        //Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
        // use the contents of szFile to initialize itself.
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = t_filter.c_str();
        ofn.nFilterIndex = 0;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        //Display the Open dialog box. 
        if(GetOpenFileName(&ofn) == TRUE)
            return ofn.lpstrFile;
        return t_default;
	}

    void OS::set_window_style(WindowStyle t_style) {
        HWND window_handle = GetForegroundWindow();
        long style = GetWindowLong(window_handle, GWL_STYLE);
        switch (t_style) {
        case XEngine::OS::WS_NO_MAXIMIZE:
            style &= ~WS_MAXIMIZEBOX;
            break;
        case XEngine::OS::WS_NO_MINIMIZE:
            style &= ~WS_MINIMIZEBOX;
            break;
        case XEngine::OS::WS_NO_RESIZE:
            style &= ~WS_THICKFRAME;
            break;
        default:
            break;
        }
        SetWindowLong(window_handle, GWL_STYLE, style);
    }

}
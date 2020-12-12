#include "windowTest.h"

#define FILE_MENU_NEW 1
#define FILE_MENU_ENTER 2
#define FILE_MENU_EXIT 3
#define FILE_MENU_RESET 4

LRESULT CALLBACK WindowsProcedure(HWND, UINT, WPARAM, LPARAM);

int main(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow){
    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND+1;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WindowsProcedure;

    if(!RegisterClassW(&wc)){
        return -1;
    }
    CreateWindowW(L"myWindowClass", L"WordSolver", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 750, 225, 500, 725, NULL, NULL, NULL, NULL);

    MSG msg = {0};
    
    while( GetMessage(&msg, NULL, 0, 0) ){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}

LRESULT CALLBACK WindowsProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp){
    switch (msg){
        case WM_COMMAND:

            switch(wp){
                case FILE_MENU_EXIT:
                    DestroyWindow(hWnd);
                    break;
                case FILE_MENU_NEW:
                    MessageBeep(MB_ICONINFORMATION);
                    break;
                case FILE_MENU_RESET:
                    memset(allWords, 0, sizeof(char)*9999);
                    hOut = CreateWindowW(L"Static", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | WS_BORDER, 150, 240, 200, 750, hWnd, NULL, NULL, NULL);
                    break;
                case FILE_MENU_ENTER:
                    getText();
                    solvePuzzle(sSize, sString, sLength);
                    mbstowcs_s(NULL, lOut, strlen(allWords)+1, allWords, strlen(allWords));
                    //printf("%s\n", allWords);
                    hOut = CreateWindowW(L"Static", lOut, WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | WS_BORDER, 150, 240, 200, 750, hWnd, NULL, NULL, NULL);
                    break;
            }
            break;

        case WM_CREATE:
            AddMenus(hWnd);
            AddControls(hWnd);
            break;
        
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
                return DefWindowProcW(hWnd, msg, wp, lp);
    }


}

void AddMenus(HWND hWnd){

    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_ENTER, "Enter");
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_RESET, "Clear");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL); 
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, "Exit");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hFileMenu, "File");

    SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd){

    //Size
    CreateWindowW(L"Static", L"Enter Size: ", WS_VISIBLE | WS_CHILD, 100, 50, 98, 38, hWnd, NULL, NULL, NULL);
    hSize = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOHSCROLL | WS_BORDER, 200, 50, 148, 38, hWnd, NULL, NULL, NULL);

    //Stringmap
    CreateWindowW(L"Static", L"Enter String: ", WS_VISIBLE | WS_CHILD, 100, 90, 98, 38, hWnd, NULL, NULL, NULL);
    hString = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOHSCROLL | WS_BORDER, 200, 90, 148, 38, hWnd, NULL, NULL, NULL);

    //Length of word
    CreateWindowW(L"Static", L"Length of Word: ", WS_VISIBLE | WS_CHILD, 100, 130, 98, 38, hWnd, NULL, NULL, NULL);
    hLength = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOHSCROLL | WS_BORDER, 200, 130, 148, 38, hWnd, NULL, NULL, NULL);

    CreateWindowW(L"Button", L"Enter", WS_VISIBLE | WS_CHILD, 150, 180, 75, 50, hWnd, (HMENU) FILE_MENU_ENTER, NULL, NULL);
    CreateWindowW(L"Button", L"Clear", WS_VISIBLE | WS_CHILD, 250, 180, 75, 50, hWnd, (HMENU) FILE_MENU_RESET, NULL, NULL);
    

}

void getText(void){
    GetWindowTextW(hSize, lSize, 255);
    GetWindowTextW(hString, lString, 255);
    GetWindowTextW(hLength, lLength, 255);

    wcstombs_s(NULL, sString, wcslen(lString)+1, lString, wcslen(lString)+1);
    sSize = _wtoi(lSize);
    sLength = _wtoi(lLength);

    //printf("board size: %d, String: %s, word length: %d\n", sSize, sString, sLength);
}
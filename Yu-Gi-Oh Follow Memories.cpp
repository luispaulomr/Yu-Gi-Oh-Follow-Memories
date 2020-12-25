// test_hmi_2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "test_hmi_2.h"
#include "CModGame.h"
#include "WindowElementPositions.h"

#include <iostream>
#include <string>
#include <thread>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hMainWindowG;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

std::unique_ptr<std::thread> p_GameThread;
std::unique_ptr<CModGame> p_ModGame;

WindowElementPositions m_WindowPositions;

BOOL setBitmap(HWND hWnd, HWND hsti, const std::string& _filename);

void ClearAllCards(
	HWND hWnd,
	const std::vector<HWND>& h_hand_cards,
	const std::vector<HWND>& h_table_cards,
	const std::vector<std::vector<HWND>>& h_fusion_cards)
{
	/* clear cards */
	for (auto it = h_hand_cards.begin(); it != h_hand_cards.end(); ++it) {

		//std::cout << "[MAIN] HAND CARD " << it - v_hand_cards.begin() << ": " << *it << " " << ModGame.GetCardPathById(*it) << "\n";

		if (!setBitmap(hWnd, *it, std::string())) {

		}

	}

	for (auto it = h_table_cards.begin(); it != h_table_cards.end(); ++it) {

		//std::cout << "[MAIN] TABLE CARD " << it - v_table_cards.begin() << ": " << *it << " " << ModGame.GetCardPathById(*it) << "\n";

		if (!setBitmap(hWnd, *it, std::string())) {

		}

	}

	/* clear fusions */
	for (auto it = h_fusion_cards.begin(); it != h_fusion_cards.end(); ++it) {

		if (!setBitmap(hWnd, it->at(0), std::string())) {

		}

		if (!setBitmap(hWnd, it->at(1), std::string())) {

		}

		if (!setBitmap(hWnd, it->at(2), std::string())) {

		}

	}
}

BOOL setBitmap(HWND hWnd, HWND hsti, const std::string& _filename)
{
	HBITMAP hBitmapG = 0;
	std::string filename = _filename;

	if (filename.empty()) {

		filename = "C:\\Users\\IZZY\\Desktop\\Yu-Gi-Oh-Follow-Memories\\images_bmp\\card_dummy.bmp";

	}

	std::wstring stemp = std::wstring(filename.begin(), filename.end());

	hBitmapG = (HBITMAP)LoadImage(GetModuleHandle(NULL), stemp.c_str(), IMAGE_BITMAP,
				      0, 0, LR_LOADFROMFILE);

	if (!hBitmapG) {
		DWORD err = GetLastError();
		std::wstring errorMsg = L"Failed to LoadImage - '" + stemp + L"', error code (" + std::to_wstring((long long)err) + L")";
		MessageBox(NULL, errorMsg.c_str(), L"WinCanvas::DrawBitmap()", MB_OK);

		PostMessage(hWnd, WM_DESTROY, NULL, NULL);    // Post a message to destroy (shutdown) the program
		return FALSE;
	}

	SendMessage(hsti, STM_SETIMAGE,
		    (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapG);

	return TRUE;
}

void gameLoop(
	HWND hWnd,
	std::vector<HWND> h_hand_cards,
	std::vector<HWND> h_table_cards,
	std::vector<std::vector<HWND>> h_fusion_cards)
{
	p_ModGame = std::unique_ptr<CModGame>(new CModGame(L"ePSXe - Enhanced PSX Emulator", L"ePSXe.exe"));

	std::vector<uint16_t> last_all_cards;
	std::vector<std::vector<struct CCardFusion::Fusion>> last_fusions;
	uint32_t DELAY_MS = 1000;

	while (1) {
		Sleep(DELAY_MS);

		if (p_ModGame->IsGameRunning()) {

			std::cout << "[MAIN] Game is running" << "\n";

			if (p_ModGame->HasDuelStarted()) {
				std::cout << "[MAIN] Duel has started" << "\n";

				auto v_hand_cards = p_ModGame->GetMyCurrentHandCards();
				auto v_table_cards = p_ModGame->GetMyCurrentTableCards();
				std::vector<uint16_t> all_cards = v_hand_cards;

				all_cards.insert(all_cards.end(), v_table_cards.begin(), v_table_cards.end());

				/* check if all_cards changed */

				if (last_all_cards == all_cards) {
					Sleep(DELAY_MS);
					continue;
				}

				for (auto it = v_hand_cards.begin(); it != v_hand_cards.end(); ++it) {

					//std::cout << "[MAIN] HAND CARD " << it - v_hand_cards.begin() << ": " << *it << " " << ModGame.GetCardPathById(*it) << "\n";

					if (!setBitmap(hWnd, h_hand_cards[it - v_hand_cards.begin()], p_ModGame->GetCardPathById(*it))) {

					}

				}

				for (auto it = v_table_cards.begin(); it != v_table_cards.end(); ++it) {

					//std::cout << "[MAIN] TABLE CARD " << it - v_table_cards.begin() << ": " << *it << " " << ModGame.GetCardPathById(*it) << "\n";

					if (!setBitmap(hWnd, h_table_cards[it - v_table_cards.begin()], p_ModGame->GetCardPathById(*it))) {

					}

				}

				last_all_cards = all_cards;

				auto fusions = p_ModGame->GetFusions(all_cards);

				/* clear fusions */
				for (auto it = h_fusion_cards.begin(); it != h_fusion_cards.end(); ++it) {

					for (auto jt = it->begin(); jt != it->end(); ++jt) {

						if (!setBitmap(hWnd, *jt, std::string())) {

						}

					}

				}

				for (auto it = fusions.begin(); it != fusions.end(); ++it) {

					if (!setBitmap(hWnd, h_fusion_cards[it - fusions.begin()][0], p_ModGame->GetCardPathById(it->at(0).card_1))) {

					}

					if (!setBitmap(hWnd, h_fusion_cards[it - fusions.begin()][1], p_ModGame->GetCardPathById(it->at(0).card_2))) {

					}

					if (!setBitmap(hWnd, h_fusion_cards[it - fusions.begin()][2], p_ModGame->GetCardPathById(it->at(0).result))) {

					}

				}

			} else {
				ClearAllCards(hWnd, h_hand_cards, h_table_cards, h_fusion_cards);
			}

		} else {
			std::cout << "[MAIN] Game is not running" << "\n";
			p_ModGame->RetryConnection();

			ClearAllCards(hWnd, h_hand_cards, h_table_cards, h_fusion_cards);
		}
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
		      _In_opt_ HINSTANCE hPrevInstance,
		      _In_ LPWSTR    lpCmdLine,
		      _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TESTHMI2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTHMI2));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTHMI2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TESTHMI2);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hMainWindowG = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
				  CW_USEDEFAULT, 0, 820, 980, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void HandleWMCOMMAND(HWND hWnd, int elementId)
{
	switch (elementId) {
		case static_cast<int>(WindowElements::CB_HEALTH_FREEZE) :
		{
			UINT checked = IsDlgButtonChecked(hWnd, elementId);

			if (checked) {

				CheckDlgButton(hWnd, elementId, BST_UNCHECKED);

			} else {

				CheckDlgButton(hWnd, elementId, BST_CHECKED);

			}
		}

		break;

		case static_cast<int>(WindowElements::CB_HEALTH_SET) :
		{
			UINT checked = IsDlgButtonChecked(hWnd, elementId);

			if (checked) {

				CheckDlgButton(hWnd, elementId, BST_UNCHECKED);

			} else {

				CheckDlgButton(hWnd, elementId, BST_CHECKED);

			}
		}

		default:
			break;

	}
}

std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				     NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
	{
		HWND hCheckBox_set_health = CreateWindowW(L"button", L"Set Health",
					       WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
					       m_WindowPositions.getX(WindowElements::CB_HEALTH_SET),
					       m_WindowPositions.getY(WindowElements::CB_HEALTH_SET),
					       m_WindowPositions.getWidth(WindowElements::CB_HEALTH_SET),
					       m_WindowPositions.getHeight(WindowElements::CB_HEALTH_SET),
					       hWnd,
					       (HMENU)WindowElements::CB_HEALTH_SET,
					       NULL, NULL);

		HWND hCheckBox_freeze_health = CreateWindowW(L"button", L"Freeze Health",
						WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
						m_WindowPositions.getX(WindowElements::CB_HEALTH_FREEZE),
						m_WindowPositions.getY(WindowElements::CB_HEALTH_FREEZE),
						m_WindowPositions.getWidth(WindowElements::CB_HEALTH_FREEZE),
						m_WindowPositions.getHeight(WindowElements::CB_HEALTH_FREEZE),
						hWnd,
						(HMENU)WindowElements::CB_HEALTH_FREEZE,
						NULL, NULL);

		HWND hGroupBox_hand_cards = CreateWindowW(L"button", L"My Hand Cards",
							  WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
							  m_WindowPositions.getX(WindowElements::GB_MY_HAND_CARDS),
							  m_WindowPositions.getY(WindowElements::GB_MY_HAND_CARDS),
							  m_WindowPositions.getWidth(WindowElements::GB_MY_HAND_CARDS),
							  m_WindowPositions.getHeight(WindowElements::GB_MY_HAND_CARDS),
							  hWnd,
							  (HMENU)WindowElements::GB_MY_HAND_CARDS,
							  NULL, NULL);

		HWND hGroupBox_table_cards = CreateWindowW(L"button", L"My Table Cards",
							   WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
							   m_WindowPositions.getX(WindowElements::GB_MY_TABLE_CARDS),
							   m_WindowPositions.getY(WindowElements::GB_MY_TABLE_CARDS),
							   m_WindowPositions.getWidth(WindowElements::GB_MY_TABLE_CARDS),
							   m_WindowPositions.getHeight(WindowElements::GB_MY_TABLE_CARDS),
							   hWnd,
							   (HMENU)WindowElements::GB_MY_TABLE_CARDS,
							   NULL, NULL);

		if (!hCheckBox_set_health) {
			// handle error
			break;
		}

		if (!hCheckBox_freeze_health) {
			// handle error
			break;
		}

		std::vector<HWND> hand_cards;
		std::vector<HWND> table_cards;
		std::vector<std::vector<HWND>> fusion_cards;

		size_t i_begin, i_end;

		i_begin = m_WindowPositions.getElementIndex(WindowElements::MY_HAND_CARD_1);
		i_end = m_WindowPositions.getElementIndex(WindowElements::MY_HAND_CARD_5);

		for (auto i = i_begin; i <= i_end; ++i) {

			HWND hsti = CreateWindowW(L"Static", L"",
						  WS_CHILD | WS_VISIBLE | SS_BITMAP,
						  m_WindowPositions.getX(i),
						  m_WindowPositions.getY(i),
						  m_WindowPositions.getWidth(i),
						  m_WindowPositions.getHeight(i),
						  hWnd,
						  (HMENU)i,
						  NULL, NULL);

			hand_cards.push_back(hsti);
		}

		i_begin = m_WindowPositions.getElementIndex(WindowElements::MY_TABLE_CARD_1);
		i_end = m_WindowPositions.getElementIndex(WindowElements::MY_TABLE_CARD_5);

		for (auto i = i_begin; i <= i_end; ++i) {

			HWND hsti = CreateWindowW(L"Static", L"",
						  WS_CHILD | WS_VISIBLE | SS_BITMAP,
						  m_WindowPositions.getX(i),
						  m_WindowPositions.getY(i),
						  m_WindowPositions.getWidth(i),
						  m_WindowPositions.getHeight(i),
						  hWnd,
						  (HMENU) i,
						  NULL, NULL);

			table_cards.push_back(hsti);
		}

		i_begin = m_WindowPositions.getElementIndex(WindowElements::GB_MY_FUSION_1);
		i_end = m_WindowPositions.getElementIndex(WindowElements::GB_MY_FUSION_8);

		for (auto i = i_begin; i <= i_end; ++i) {

			std::wstring str = L"My Fusion " + std::to_wstring(i + 1 - i_begin);

			HWND hsti = CreateWindowW(L"button", str.c_str(),
						  WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
						  m_WindowPositions.getX(i),
						  m_WindowPositions.getY(i),
						  m_WindowPositions.getWidth(i),
						  m_WindowPositions.getHeight(i),
						  hWnd,
						  (HMENU) i,
						  NULL, NULL);

		}

		i_begin = m_WindowPositions.getElementIndex(WindowElements::MY_FUSION_1_CARD_1);

		for (auto i = 0; i < 8; ++i) {

			std::vector<HWND> cards;

			HWND hsti = CreateWindowW(L"Static", L"",
					     WS_CHILD | WS_VISIBLE | SS_BITMAP,
					     m_WindowPositions.getX(i_begin),
					     m_WindowPositions.getY(i_begin),
					     m_WindowPositions.getWidth(i_begin),
					     m_WindowPositions.getHeight(i_begin),
					     hWnd,
					     (HMENU) i_begin,
					     NULL, NULL);

			cards.push_back(hsti);
			++i_begin;

			hsti = CreateWindowW(L"Static", L"",
					     WS_CHILD | WS_VISIBLE | SS_BITMAP,
					     m_WindowPositions.getX(i_begin),
					     m_WindowPositions.getY(i_begin),
					     m_WindowPositions.getWidth(i_begin),
					     m_WindowPositions.getHeight(i_begin),
					     hWnd,
					     (HMENU) i_begin,
					     NULL, NULL);

			cards.push_back(hsti);
			++i_begin;

			hsti = CreateWindowW(L"Static", L"",
					     WS_CHILD | WS_VISIBLE | SS_BITMAP,
					     m_WindowPositions.getX(i_begin),
					     m_WindowPositions.getY(i_begin),
					     m_WindowPositions.getWidth(i_begin),
					     m_WindowPositions.getHeight(i_begin),
					     hWnd,
					     (HMENU) i_begin,
					     NULL, NULL);

			cards.push_back(hsti);
			++i_begin;

			fusion_cards.push_back(cards);

		}

		i_begin = m_WindowPositions.getElementIndex(WindowElements::L_FUSION_1_PLUS);

		for (auto i = 0; i < 8; ++i) {

			HWND hsti = CreateWindowW(L"Static", L"+",
						  WS_CHILD | WS_VISIBLE | SS_CENTER,
						  m_WindowPositions.getX(i_begin),
						  m_WindowPositions.getY(i_begin),
						  m_WindowPositions.getWidth(i_begin),
						  m_WindowPositions.getHeight(i_begin),
						  hWnd,
						  (HMENU) i_begin,
						  NULL, NULL);
			++i_begin;

			hsti = CreateWindowW(L"Static", L"=",
					     WS_CHILD | WS_VISIBLE | SS_CENTER,
					     m_WindowPositions.getX(i_begin),
					     m_WindowPositions.getY(i_begin),
					     m_WindowPositions.getWidth(i_begin),
					     m_WindowPositions.getHeight(i_begin),
					     hWnd,
					     (HMENU) i_begin,
					     NULL, NULL);
			++i_begin;

		}

		CheckDlgButton(hWnd, static_cast<int>(WindowElements::CB_HEALTH_SET), BST_UNCHECKED);
		CheckDlgButton(hWnd, static_cast<int>(WindowElements::CB_HEALTH_FREEZE), BST_UNCHECKED);

		p_GameThread = std::unique_ptr<std::thread>(new std::thread(gameLoop, hWnd, hand_cards, table_cards, fusion_cards));
	}

	break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId) {
		case static_cast<int>(WindowElements::CB_HEALTH_SET):
		{
			BOOL checked = IsDlgButtonChecked(hWnd, static_cast<int>(WindowElements::CB_HEALTH_SET));

			if (!checked) {
				CheckDlgButton(hWnd, static_cast<int>(WindowElements::CB_HEALTH_SET), BST_CHECKED);

				p_ModGame->SetEnemyCurrentHealth(0);
			} else {
				CheckDlgButton(hWnd, static_cast<int>(WindowElements::CB_HEALTH_SET), BST_UNCHECKED);
			}
		}
			break;
		case IDM_ABOUT:
			DialogBox(hMainWindowG, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		default:
		{
			HandleWMCOMMAND(hWnd, LOWORD(wParam));
		}

		break;

		return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

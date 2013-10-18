#include "include/cef.h"
#include "cefclient.h"
#include "client_handler.h"

#include "mft_app_full_resource.h"

#include "string_util.h"
#include <commdlg.h>
#include <direct.h>
#include <sstream>
#include <Shlwapi.h>
#include <tchar.h>

////////////////////////////////////////////////////////////////////////////////
// include and macro for jump list

//#define NTDDI_VERSION NTDDI_WIN7  // Specifies that the minimum required platform is Windows 7.
//#define WIN32_LEAN_AND_MEAN       // Exclude rarely-used stuff from Windows headers
//#define STRICT_TYPED_ITEMIDS      // Utilize strictly typed IDLists

// Windows Header Files:
#include <psapi.h>
//#include <shlwapi.h>
#include <strsafe.h>

// Header Files for Jump List features
#include <objectarray.h>
#include <shobjidl.h>
#include <propkey.h>
#include <propvarutil.h>
#include <knownfolders.h>
#include <shlobj.h>
#include <shobjidl.h>
/******************************************************************************/

// Add Common Controls to the application manifest because it's required to
// support the default tooltip implementation.
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING	100
#define MAX_URL_LENGTH  255
#define BUTTON_WIDTH	  72
#define URLBAR_HEIGHT	  24

//define langueage (ENG - default)
//#define FR_LNG 1
//#define SPA_LNG 2

// definition of macros which allowes cout console for debug output
//#define ENABLE_COUT_WINDOW

#ifdef ENABLE_COUT_WINDOW
#include "io.h"
#include <fcntl.h>
#endif //ENABLE_COUT_WINDOW

// definition of macros which allowes MessageBrocker usage
#define ENABLE_MESSAGE_BROKER

#ifdef ENABLE_MESSAGE_BROKER
// Header Files for MessageBroker
#include "CMessageBroker.hpp"
// Header Files for MessageBrokerControllerBackend
#include "MessageBrokerControllerBackend.hpp"
// Header Files for TCP server
#include "mb_tcpserver.hpp"
// Header Files for Jsoncpp
#include "json/json.h"
// Header files for threads
#include "system.h"
#endif //ENABLE_MESSAGE_BROKER

//definitions of MCS concepts
//#define MCS_CONCEPT_1   10
//#define MCS_CONCEPT_1_1 1
//#define MCS_CONCEPT_1_2 2
//#define MCS_CONCEPT_1_3 3
//#define MCS_CONCEPT_1_4 4
//#define MCS_CONCEPT_1_5 5

//#define MCS_CONCEPT_2_1 1
//#define MCS_CONCEPT_2_2 2

//#define MCS_CONCEPT_4_1 1

//#define MCS_CONCEPT_5_1 1
//#define MCS_CONCEPT_5_2 2

//#define MCS_CONCEPT_6_1 1
//#define MCS_CONCEPT_6_2 2

//#define MCS_CONCEPT_7_1 1

//prepare windows registry key for application
#ifdef MCS_CONCEPT_1
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_1";
#elif MCS_CONCEPT_1_1
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_1_1";
#elif MCS_CONCEPT_1_2
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_1_2";
#elif MCS_CONCEPT_1_3
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_1_3";
#elif MCS_CONCEPT_1_4
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_1_4";
#elif MCS_CONCEPT_1_5
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_1_5";
#elif MCS_CONCEPT_2_1
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_2_1";
#elif MCS_CONCEPT_2_2
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_2_2";
#elif MCS_CONCEPT_4_1
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_4_1";
#elif MCS_CONCEPT_5_1
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_5_1";
#elif MCS_CONCEPT_5_2
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_5_2";
#elif MCS_CONCEPT_6_1
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_6_1";
#elif MCS_CONCEPT_6_2
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_6_2";
#elif MCS_CONCEPT_7_1
  const LPCSTR gSubKey = "Software\\Ford\\MCS_Concept_7_1";
#elif FR_LNG
  const LPCSTR gSubKey = "Software\\Ford\\Avatar_FR";
#elif SPA_LNG
  const LPCSTR gSubKey = "Software\\Ford\\Avatar_SPA";
#else
  const LPCSTR gSubKey = "Software\\Ford\\Avatar";
#endif

// Global Variables:
HINSTANCE	hInst;				            // current instance
TCHAR		szTitle[MAX_LOADSTRING];	    // The title bar text
TCHAR		szWindowClass[MAX_LOADSTRING];	// the main window class name
char		szWorkingDir[MAX_PATH];		    // The current working directory
HWND		hWnd;                           // Main window HWND
bool		bIsFirstLaunch;					// Indicate if it is the first launch of program
std::string strCurrentVehicle;				// the current vehicle model in settings
bool		bIsNavEnabled;					// value of navigation settings
// The global ClientHandler reference.
extern CefRefPtr<ClientHandler> g_handler;

//The global MessageBroker objects
#ifdef ENABLE_MESSAGE_BROKER
NsMessageBroker::CMessageBroker* pMessageBroker;
NsMessageBroker::TcpServer* server;
NsMessageBroker::CMessageBrokerControllerBackend* pCMessageBrokerControllerBackend;
#endif //ENABLE_MESSAGE_BROKER
///////////////////////////////////////////////////////////////////////////////
// Avatars constants
//define map for vehicles
struct SETTINGS 
{
	int ID_VEHICLE_SETTINGS,
		ID_NAVIGATION_SETTINGS,
		ID_VEHICLE_EDGE,
		ID_VEHICLE_EXPLORER,
		ID_VEHICLE_ESCAPE_2013,
		ID_VEHICLE_F150_2013,
		ID_VEHICLE_FLEX_2013,
		ID_VEHICLE_FOCUS,
		ID_VEHICLE_FUSION_2013,
		ID_VEHICLE_TAURUS_2013,
		ID_VEHICLE_FIESTA;

	SETTINGS ()
	{
		ID_NAVIGATION_SETTINGS = 101;
		ID_VEHICLE_EDGE = 201;
		ID_VEHICLE_EXPLORER = 202;
		ID_VEHICLE_ESCAPE_2013 = 203;
		ID_VEHICLE_F150_2013 = 204;
		ID_VEHICLE_FLEX_2013 = 205;
		ID_VEHICLE_FOCUS = 206;
		ID_VEHICLE_FUSION_2013 = 207;
		ID_VEHICLE_TAURUS_2013 = 208;
		ID_VEHICLE_FIESTA = 209;
	}
	std::string getVehicle(int id)
	{
		if (id == ID_VEHICLE_EDGE)				return "edge";
		else if (id == ID_VEHICLE_EXPLORER)		return "explorer";
		else if (id == ID_VEHICLE_ESCAPE_2013)	return "escape_2013";
		else if (id == ID_VEHICLE_F150_2013)	return "f150_2013";
		else if (id == ID_VEHICLE_FLEX_2013)	return "flex_2013";
		else if (id == ID_VEHICLE_FOCUS)		return "focus";
		else if (id == ID_VEHICLE_FUSION_2013)	return "fusion_2013";
		else if (id == ID_VEHICLE_TAURUS_2013)	return "taurus_2013";
		else if (id == ID_VEHICLE_FIESTA)		return "fiesta";
		else									return "ford";
	}
	int getVehicleId(std::string vehicle)
	{
		if (vehicle.compare("edge") == 0)				return ID_VEHICLE_EDGE;
		else if (vehicle.compare("explorer") == 0)		return ID_VEHICLE_EXPLORER;
		else if (vehicle.compare("escape_2013") == 0)	return ID_VEHICLE_ESCAPE_2013;
		else if (vehicle.compare("f150_2013") == 0)		return ID_VEHICLE_F150_2013;
		else if (vehicle.compare("flex_2013") == 0)		return ID_VEHICLE_FLEX_2013;
		else if (vehicle.compare("focus") == 0)			return ID_VEHICLE_FOCUS;
		else if (vehicle.compare("fusion_2013") == 0)	return ID_VEHICLE_FUSION_2013;
		else if (vehicle.compare("taurus_2013") == 0)	return ID_VEHICLE_TAURUS_2013;
		else if (vehicle.compare("fiesta") == 0)		return ID_VEHICLE_FIESTA;
		return -1;
	}
} Settings;


// Avatar window size
#define WINDOW_WIDTH  807
#define WINDOW_HEIGHT 487

#define ZOOMED_WIDTH 967
#define ZOOMED_HEIGHT 583

// Zoom accelerator events
#define ZOOM_IN_EVENT  (UINT)WM_USER + 100
#define ZOOM_OUT_EVENT (UINT)WM_USER + 101

// Current timeout for splash screen.
#define SPLASH_WINDOW_INITIAL_TIMEOUT 5000
#define SPLASH_WINDOW_TIMEOUT 100

// Avatars constants
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************/

// Creates jump list for avatar app
// TODO(AKandul): we need icons for "zoom in" and "zoom out" elements
BOOL CreateJumpList() {
  ICustomDestinationList *pcdl;
  HRESULT hr = CoCreateInstance(CLSID_DestinationList,
    NULL,CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pcdl));
  if (FAILED(hr)) {
    return FALSE;
  }

  // The cMinSlots and poaRemoved values can be ignored when only a Known Category is being added - those
  // parameters apply only to applications adding custom categories or tasks to the Jump Lists
  UINT cMinSlots;
  IObjectArray *poaRemoved;
  hr = pcdl->BeginList(&cMinSlots, IID_PPV_ARGS(&poaRemoved));
  if (FAILED(hr)) {
    return FALSE;
  }

  IObjectCollection* pObjColl;
  hr = CoCreateInstance(CLSID_EnumerableObjectCollection, NULL,
    CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pObjColl));
  if (FAILED(hr)) {
    return FALSE;
  }

  TCHAR szExePath[MAX_PATH];
  GetModuleFileName ( NULL, szExePath, _countof(szExePath) );

  // Zoom in link
  IShellLink* pZoomInLink;
  hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
    IID_PPV_ARGS(&pZoomInLink));
  if (FAILED(hr)) {
    return FALSE;
  }

  pZoomInLink->SetPath(szExePath);
  pZoomInLink->SetArguments(_T("/+"));
  pZoomInLink->SetDescription(_T("Zoom In"));

  IPropertyStore* pZoomInPropStore;
  hr = pZoomInLink->QueryInterface(IID_IPropertyStore,
    (void**)&pZoomInPropStore);
  if (FAILED(hr)) {
    return FALSE;
  }

  PROPVARIANT pZoomInProp;
  InitPropVariantFromString(L"Zoom In", &pZoomInProp);

  pZoomInPropStore->SetValue(PKEY_Title, pZoomInProp);
  PropVariantClear(&pZoomInProp);

  pZoomInPropStore->Commit();
  pObjColl->AddObject(pZoomInLink);

  // Zoom out link
  IShellLink* pZoomOutLink;
  hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
    IID_PPV_ARGS(&pZoomOutLink));
  if (FAILED(hr)) {
    return FALSE;
  }

  pZoomOutLink->SetPath(szExePath);
  pZoomOutLink->SetArguments(_T("/-"));
  pZoomOutLink->SetDescription(_T("Zoom Out"));

  IPropertyStore* pZoomOutPropStore;
  hr = pZoomOutLink->QueryInterface(IID_IPropertyStore, (void**)&pZoomOutPropStore);
  if (FAILED(hr)) {
    return FALSE;
  }
  
  PROPVARIANT pZoomOutProp;
  InitPropVariantFromString(L"Zoom Out", &pZoomOutProp);

  pZoomOutPropStore->SetValue(PKEY_Title, pZoomOutProp);
  PropVariantClear(&pZoomOutProp);

  pZoomOutPropStore->Commit();
  pObjColl->AddObject(pZoomOutLink);

  IObjectArray* pTasksArray = pObjColl;
  pcdl->AddUserTasks(pTasksArray);
  pcdl->CommitList();

  return TRUE;
}

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

/******************************************************************************/

//-----------------------------------------------------------------------------
// class for JS code injection with first launch indication
class V8Handler : public CefV8Handler
{
public:
	V8Handler() {};
	virtual bool Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) OVERRIDE
	{
    return false;
	}
	// Provide the reference counting implementation for this class.
	IMPLEMENT_REFCOUNTING(MyV8Handler);
};
//-----------------------------------------------------------------------------
void CheckFirstLaunch()
{
	//check in register if it is the first launch of program

	HKEY hKey;
	DWORD firstLaunch = 0;
	DWORD flSize = sizeof(DWORD);
	DWORD keyType = REG_DWORD;

	if (ERROR_SUCCESS == ::RegOpenKeyExA(HKEY_CURRENT_USER,gSubKey, NULL, KEY_ALL_ACCESS, &hKey)) 
	{
		if (ERROR_SUCCESS != ::RegQueryValueExA(hKey, "FirstLaunch", NULL, &keyType, (LPBYTE)&firstLaunch, &flSize))
		{
			firstLaunch = 1;
			::RegSetValueEx(hKey, _T("FirstLaunch"), 0, REG_DWORD, (LPBYTE)&firstLaunch, sizeof(DWORD));
		}
		bIsFirstLaunch = firstLaunch == 0 ? false : true;
		RegCloseKey(hKey);
	}

	//Create an instance of my CefV8Handler object.
	CefRefPtr<CefV8Handler> handler = new V8Handler();
	//create JS extension
	std::string extensionCode =	bIsFirstLaunch ? 
								"var win_firstlaunch = true;" : 
								"var win_firstlaunch = false;";

	// Register the extension.
	CefRegisterExtension("v8/win_firstlaunch", extensionCode, handler);

}
//-----------------------------------------------------------------------------
void CheckNavigationSettings()
{
	HKEY hKey;
	DWORD hasMaps = 0;
	DWORD hmSize = sizeof(DWORD);
	DWORD keyType = REG_DWORD;

	if (ERROR_SUCCESS == ::RegOpenKeyExA(HKEY_CURRENT_USER,gSubKey, NULL, KEY_ALL_ACCESS, &hKey)) 
	{
		if (ERROR_SUCCESS != ::RegQueryValueExA(hKey, "HasMaps", NULL, &keyType, (LPBYTE)&hasMaps, &hmSize))
		{
			hasMaps = 1;
			::RegSetValueEx(hKey, _T("HasMaps"), 0, REG_DWORD, (LPBYTE)&hasMaps, hmSize);
		}
		bIsNavEnabled = hasMaps == 0 ? false : true;
		RegCloseKey(hKey);
	}
}
//-----------------------------------------------------------------------------
void CheckVehicleSettings()
{
	HKEY hKey;
	DWORD vehicleId = -1;
	DWORD idSize = sizeof(DWORD);
	DWORD keyType = REG_DWORD;

	if (ERROR_SUCCESS == ::RegOpenKeyExA(HKEY_CURRENT_USER,gSubKey, NULL, KEY_ALL_ACCESS, &hKey)) 
	{
		if (ERROR_SUCCESS != ::RegQueryValueExA(hKey, "Vehicle", NULL, &keyType, (LPBYTE)&vehicleId, &idSize))
		{
			vehicleId = -1;
			::RegSetValueEx(hKey, _T("Vehicle"), 0, REG_DWORD, (LPBYTE)&vehicleId, idSize);
		}
		strCurrentVehicle = Settings.getVehicle(vehicleId);
		RegCloseKey(hKey);
	}
}
//-----------------------------------------------------------------------------
std::string getInstallationPath()
{
	//Getting installation path

	HKEY hKey;
	char installationPath[MAX_PATH] = {0};
	DWORD installationPathSize = MAX_PATH;
	DWORD keyType = REG_SZ;

	if (ERROR_SUCCESS == ::RegOpenKeyExA(HKEY_CURRENT_USER,
			gSubKey, NULL, KEY_QUERY_VALUE, &hKey)) 
	{
		if (ERROR_SUCCESS != ::RegQueryValueExA(hKey, "InstallationPath", 
			NULL, &keyType, (LPBYTE)installationPath, &installationPathSize))
		{
			::MessageBoxA(hWnd, "Installation path not found.",
				"Ford Avatar", MB_ICONERROR | MB_OK);
			return 0;
		}
	} 
	else 
	{
		::MessageBoxA(hWnd, "Installation path not found.",
			"Ford Avatar", MB_ICONERROR | MB_OK);
		return 0;
	}

	return installationPath;

}
//------------------------------------------------------------------------------------
void SaveFirstLaunchState()
{
	HKEY hKey;
	DWORD firstLaunch = 0;
	DWORD flSize = sizeof(DWORD);
	DWORD keyType = REG_DWORD;

	
	if (ERROR_SUCCESS == ::RegOpenKeyExA(HKEY_CURRENT_USER, gSubKey, NULL, KEY_ALL_ACCESS, &hKey)) 
	{
		::RegSetValueEx(hKey, _T("FirstLaunch"), 0, REG_DWORD, (LPBYTE)&firstLaunch, flSize);
		bIsFirstLaunch = false;
		RegCloseKey(hKey);
	}

}
//------------------------------------------------------------------------------------
void SaveNavigationState()
{
	HKEY hKey;
	DWORD hasMaps = bIsNavEnabled ? 1 : 0;
	DWORD hmSize = sizeof(DWORD);
	DWORD keyType = REG_DWORD;

	
	if (ERROR_SUCCESS == ::RegOpenKeyExA(HKEY_CURRENT_USER, gSubKey, NULL, KEY_ALL_ACCESS, &hKey)) 
	{
		::RegSetValueEx(hKey, _T("HasMaps"), 0, REG_DWORD, (LPBYTE)&hasMaps, hmSize);
		RegCloseKey(hKey);
	}

}
//------------------------------------------------------------------------------------
void SaveVehicleState()
{
	HKEY hKey;
	DWORD vehicleID = Settings.getVehicleId(strCurrentVehicle);
	DWORD idSize = sizeof(DWORD);
	DWORD keyType = REG_DWORD;

	if (ERROR_SUCCESS == ::RegOpenKeyExA(HKEY_CURRENT_USER, gSubKey, NULL, KEY_ALL_ACCESS, &hKey)) 
	{
		::RegSetValueEx(hKey, _T("Vehicle"), 0, REG_DWORD, (LPBYTE)&vehicleID, idSize);
		RegCloseKey(hKey);
	}
}
//------------------------------------------------------------------------------------
void SetWindowZoom(int windowWidth,
                   int windowHeight,
                   double zoomlevel) 
{
  int captionHeight = GetSystemMetrics(SM_CYCAPTION);
  
  ::ShowWindow(hWnd, SW_HIDE);
  ::SetWindowPos(hWnd, 0, 0, 0, windowWidth, windowHeight + captionHeight, 0);
  ::SetWindowPos(g_handler->GetBrowserHwnd(), 0, 0, 0, windowWidth, windowHeight + captionHeight, 0);
  g_handler->GetBrowser()->SetZoomLevel(zoomlevel);
}

// -----------------------------------------------------------------------------
void ToCenterScreen(int nCmdShow) {
  // Set avatar window at center screen
  RECT rc;
  ::GetWindowRect(hWnd, &rc);
  int xPos = (::GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
  int yPos = (::GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
  ::SetWindowPos(hWnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE );

  ::ShowWindow(hWnd, nCmdShow);
  ::UpdateWindow(hWnd);
}
// -----------------------------------------------------------------------------
bool setNavigationSetting(bool value)
{
	bIsNavEnabled = value;
	return true;
}
// -----------------------------------------------------------------------------
bool setCurrentVehicle(int id)
{
	strCurrentVehicle = Settings.getVehicle(id);
	return true;
}
// -----------------------------------------------------------------------------
bool setVehicleSetting(std::string value)
{
	bool res = false;
	if (value.compare(Settings.getVehicle(Settings.ID_VEHICLE_EDGE)) == 0)
	{
		setCurrentVehicle(Settings.ID_VEHICLE_EDGE);
	} 
	else if (value.compare(Settings.getVehicle(Settings.ID_VEHICLE_EXPLORER)) == 0)
	{
		setCurrentVehicle(Settings.ID_VEHICLE_EXPLORER);
	} 
	else if (value.compare(Settings.getVehicle(Settings.ID_VEHICLE_ESCAPE_2013)) == 0)
	{
		setCurrentVehicle(Settings.ID_VEHICLE_ESCAPE_2013);
	} 
	else if (value.compare(Settings.getVehicle(Settings.ID_VEHICLE_F150_2013)) == 0)
	{
		setCurrentVehicle(Settings.ID_VEHICLE_F150_2013);
	} 
	else if (value.compare(Settings.getVehicle(Settings.ID_VEHICLE_FLEX_2013)) == 0)
	{
		setCurrentVehicle(Settings.ID_VEHICLE_FLEX_2013);
	} 
	else if (value.compare(Settings.getVehicle(Settings.ID_VEHICLE_FOCUS)) == 0)
	{
		setCurrentVehicle(Settings.ID_VEHICLE_FOCUS);
	} 
	else if (value.compare(Settings.getVehicle(Settings.ID_VEHICLE_FUSION_2013)) == 0)
	{
		setCurrentVehicle(Settings.ID_VEHICLE_FUSION_2013);
	} 
	else if (value.compare(Settings.getVehicle(Settings.ID_VEHICLE_TAURUS_2013)) == 0)
	{
		setCurrentVehicle(Settings.ID_VEHICLE_TAURUS_2013);
	}
	else if (value.compare(Settings.getVehicle(Settings.ID_VEHICLE_FIESTA)) == 0)
	{
		setCurrentVehicle(Settings.ID_VEHICLE_FIESTA);
	} 
	return res;
}
// -----------------------------------------------------------------------------
DWORD WINAPI SplashThreadProc(LPVOID lpParameter) {
  g_handler->SetAppIsLoaded(false);
  ::Sleep(SPLASH_WINDOW_INITIAL_TIMEOUT);//show splash screen
  while(!g_handler->GetAppIsLoaded())
	  ::Sleep(SPLASH_WINDOW_TIMEOUT); //Waiting for loading of content
  
  ::ShowWindow((HWND)lpParameter, SW_HIDE); // Hide or close splash window here.
  return 0;
}
// -----------------------------------------------------------------------------
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR lpCmdLine,
                      int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  ::CoInitialize(NULL);

  // Retrieve the current working directory.
  if(_getcwd(szWorkingDir, MAX_PATH) == NULL)
    szWorkingDir[0] = 0;

  CefSettings settings;
  settings.log_severity = LOGSEVERITY_DISABLE;
  settings.multi_threaded_message_loop = true;

  CefInitialize(settings);

  // Initialize global strings
  ::LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

//load szWindowClass depends on MFT/MCS Concept
#ifdef MCS_CONCEPT_1
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_1, szWindowClass, MAX_LOADSTRING);
#elif MCS_CONCEPT_1_1
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_1_1, szWindowClass, MAX_LOADSTRING);
#elif MCS_CONCEPT_1_2
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_1_2, szWindowClass, MAX_LOADSTRING);
#elif MCS_CONCEPT_1_3
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_1_3, szWindowClass, MAX_LOADSTRING);
#elif MCS_CONCEPT_1_4
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_1_4, szWindowClass, MAX_LOADSTRING);
#elif MCS_CONCEPT_1_5
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_1_5, szWindowClass, MAX_LOADSTRING);
#elif MCS_CONCEPT_2_1
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_2_1, szWindowClass, MAX_LOADSTRING);
#elif MCS_CONCEPT_2_2
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_2_2, szWindowClass, MAX_LOADSTRING);
#elif MCS_CONCEPT_4_1
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_4_1, szWindowClass, MAX_LOADSTRING);
#elif MCS_CONCEPT_5_1
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_5_1, szWindowClass, MAX_LOADSTRING);
#elif MCS_CONCEPT_5_2
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_5_2, szWindowClass, MAX_LOADSTRING);
#elif MCS_CONCEPT_6_1
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_6_1, szWindowClass, MAX_LOADSTRING);
#elif MCS_CONCEPT_6_2
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_6_2, szWindowClass, MAX_LOADSTRING);
#elif MCS_CONCEPT_7_1
  ::LoadString(hInstance, IDC_CEFCLIENT_MCS_7_1, szWindowClass, MAX_LOADSTRING);
#elif FR_LNG
  ::LoadString(hInstance, IDC_CEFCLIENT_FR, szWindowClass, MAX_LOADSTRING);
#elif SPA_LNG
  ::LoadString(hInstance, IDC_CEFCLIENT_SPA, szWindowClass, MAX_LOADSTRING);
#else
  ::LoadString(hInstance, IDC_CEFCLIENT, szWindowClass, MAX_LOADSTRING);
#endif
  
  MyRegisterClass(hInstance);

  // Send message from Windows 7 jump list
  HWND found = ::FindWindow(szWindowClass, 0);
  if (::IsWindow(found)) {
    if (_tcsstr(lpCmdLine, _T("/+"))) {
      ::SendMessage(found, ZOOM_IN_EVENT, 0, 0);
    } else if (_tcsstr(lpCmdLine, _T("/-"))) {
      ::SendMessage(found, ZOOM_OUT_EVENT, 0, 0);
    }
    
    return FALSE;
  }

  // Perform application initialization
  if (!InitInstance (hInstance, nCmdShow))
    return FALSE;

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_AVATAR));
  MSG msg;
  int result = 0;

  // Run the application message loop.
  while (::GetMessage(&msg, NULL, 0, 0)) {
    if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
  }

  result = (int)msg.wParam;
  CefShutdown();
  ::CoUninitialize();
  return 0;
}

// -----------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEX wcex;

  wcex.cbSize		= sizeof(WNDCLASSEX);
  wcex.style		= CS_HREDRAW | CS_VREDRAW ;
  wcex.lpfnWndProc	= WndProc;
  wcex.cbClsExtra	= 0;
  wcex.cbWndExtra	= 0;
  wcex.hInstance	= hInstance;
  wcex.hCursor		= ::LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszClassName	= szWindowClass;

  wcex.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MFT_APP_ICON_AVATAR_FULL));
  wcex.hIconSm = ::LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MFT_APP_ICON_AVATAR_FULL));

  return ::RegisterClassEx(&wcex);
}

// -----------------------------------------------------------------------------
#ifdef ENABLE_COUT_WINDOW
void SetStdOutToNewConsole()
{
    // allocate a console for this app
    AllocConsole();

    // redirect unbuffered STDOUT to the console
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    int fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT);
    FILE *fp = _fdopen( fileDescriptor, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );

    // give the console window a nicer title
    SetConsoleTitle(L"Debug Output");

    // give the console window a bigger buffer size
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if ( GetConsoleScreenBufferInfo(consoleHandle, &csbi) )
    {
        COORD bufferSize;
        bufferSize.X = csbi.dwSize.X;
        bufferSize.Y = 9999;
        SetConsoleScreenBufferSize(consoleHandle, bufferSize);
    }
}
#endif
// -----------------------------------------------------------------------------
DWORD WINAPI MessageBrokerThreadProc(LPVOID lpParameter)
{
  if (NULL != pMessageBroker)
  {
    pMessageBroker->MethodForThread(0);
  }
  return 0;
}
DWORD WINAPI TCPServerThreadProc(LPVOID lpParameter)
{
  if (NULL != server)
  {
    server->MethodForThread(0);
  }
  return 0;
}
DWORD WINAPI MessageBrokerControllerBackendThreadProc(LPVOID lpParameter)
{
  if (NULL != pCMessageBrokerControllerBackend)
  {
    pCMessageBrokerControllerBackend->MethodForReceiverThread(0);
  }
  return 0;
}

// -----------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance,
                  int nCmdShow) {
  hInst = hInstance; // Store instance handle in our global variable
  int captionHeight = GetSystemMetrics(SM_CYCAPTION);

#ifdef ENABLE_COUT_WINDOW
  SetStdOutToNewConsole();
#endif //ENABLE_COUT_WINDOW

  // instantiate MessageBroker here
#ifdef ENABLE_MESSAGE_BROKER
  pMessageBroker = NsMessageBroker::CMessageBroker::getInstance();
  if (!pMessageBroker)
  {
    //TODO: Error handler if MessageBroker hasn't been instantiated.
  } else
  {
    // create a server
#ifdef FR_LNG
	int port = 8088;
#elif SPA_LNG
	int port = 8089;
#else
	int port = 8086;
#endif
    server = new NsMessageBroker::TcpServer(std::string("127.0.0.1"), port, pMessageBroker);
    if (!server)
    {
      //TODO: Error handler if server hasn't been instantiated.
    } else
    {
      // create a CMessageBrokerControllerBackend component
      pCMessageBrokerControllerBackend = new NsMessageBroker::CMessageBrokerControllerBackend(std::string("127.0.0.1"), port);
      if (!pCMessageBrokerControllerBackend)
      {
        delete server;
        server = 0;
        //TODO: Error handler if pCMessageBrokerControllerBackend hasn't been instantiated.
      } else
      {
        if (!server->Bind() || !server->Listen())
        {
          delete server;
          server = 0;
          delete pCMessageBrokerControllerBackend;
          pCMessageBrokerControllerBackend = 0;
          // TODO: TCP server Connect or Listen error 
        } else
        {
          if (!pCMessageBrokerControllerBackend->Connect())
          {
            delete server;
            server = 0;
            delete pCMessageBrokerControllerBackend;
            pCMessageBrokerControllerBackend = 0;
            //TODO: Failed to connect to the server
          } else
          {
            // create thread for tcp server
            ::CreateThread(
                              NULL,
                              0,
                              TCPServerThreadProc,
                              NULL,
                              NULL,
                              NULL);

            // start MessageBroker
            pMessageBroker->startMessageBroker(server);

            // create thread for MessageBroker
            ::CreateThread(
                              NULL,
                              0,
                              MessageBrokerThreadProc,
                              NULL,
                              NULL,
                              NULL);

            // create thread for MessageBrokerControllerBackend reciever
            ::CreateThread(
                              NULL,
                              0,
                              MessageBrokerControllerBackendThreadProc,
                              NULL,
                              NULL,
                              NULL);

            // register MessageBrokerControllerBackend
            pCMessageBrokerControllerBackend->registerController(1);
          }
        }
      }
    }
  }
#endif //ENABLE_MESSAGE_BROKER
  
  hWnd = ::CreateWindowEx(0, szWindowClass, 0,
    WS_POPUP | WS_SYSMENU | WS_CAPTION, 
	CW_USEDEFAULT, CW_USEDEFAULT,
    WINDOW_WIDTH, WINDOW_HEIGHT + captionHeight, NULL, NULL, hInstance, NULL);

  //EditWindowSystemMenu();
  //check settings
  CheckNavigationSettings();
  setNavigationSetting(bIsNavEnabled);
  CheckVehicleSettings();
  setVehicleSetting(strCurrentVehicle);

  if (!::IsWindow(hWnd))
    return FALSE;

  ToCenterScreen(nCmdShow);

  return TRUE;
}
//------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, 
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam) {
	static WNDPROC editWndOldProc = NULL;

  // Callback for the main window
  switch (message) {
    case WM_CREATE:
      {
        //check if it is the first launch of program
		CheckFirstLaunch();

		// Create the single static handler class instance
        g_handler = new ClientHandler();
        g_handler->SetMainHwnd(hWnd);

        // Create the child windows used for navigation
        RECT rect;
        int x = 0;

        ::GetClientRect(hWnd, &rect);
        CefWindowInfo info;
        CefBrowserSettings settings;

        // Initialize window info to the defaults for a child window
        info.SetAsChild(hWnd, rect);

        char buffer[MAX_PATH] = {0};
		HRESULT hr = NULL;
		hr = SHGetFolderPathA(0, CSIDL_APPDATA, 0, 0, buffer);

#ifdef FR_LNG
			::PathAppendA(buffer, "Ford\\Guide MyFord Touch\\prototype\\index.html");
#elif SPA_LNG
			::PathAppendA(buffer, "Ford\\Guis de MyFord Touch\\prototype\\index.html");
#else
			::PathAppendA(buffer, "Ford\\MyFord Touch Guide\\prototype\\index.html");
#endif
//		}
        // Getting installation path
        ///////////////////////////////////////////////////////////////////////

        // Create the new child browser window
        CefBrowser::CreateBrowser(info,
          static_cast<CefRefPtr<CefClient> >(g_handler),
          buffer, settings);

		///////////////////////////////////////////////////////////////////////
        // Show splash screen.
		int captionHeight = GetSystemMetrics(SM_CYCAPTION);
  
        // Create static control for splash image.
        HWND splashWnd = ::CreateWindowEx(0, _T("STATIC"), _T(""),
          SS_REALSIZEIMAGE | SS_BITMAP | WS_CHILD | WS_VISIBLE,
          0, 0, WINDOW_WIDTH, WINDOW_HEIGHT + captionHeight, hWnd, (HMENU)-1, NULL, NULL);

        // Set new window style.
        ::SetClassLong(splashWnd, GCL_STYLE, CS_HREDRAW | CS_VREDRAW);

        // Move window to top.
        ::SetWindowPos(splashWnd, HWND_TOPMOST, 0, 0, 800, 480,
          SWP_NOSIZE | SWP_SHOWWINDOW);

        // Load splash bitmap from resource.
#ifdef FR_LNG
		HBITMAP bitmap;
        bitmap = (HBITMAP)::LoadImage(GetModuleHandle(NULL),
          MAKEINTRESOURCE(IDB_MFT_APP_SPLASH_AVATAR_FULL_FR), IMAGE_BITMAP, 
          0, 0, LR_DEFAULTCOLOR);
#elif SPA_LNG
		HBITMAP bitmap;
        bitmap = (HBITMAP)::LoadImage(GetModuleHandle(NULL),
          MAKEINTRESOURCE(IDB_MFT_APP_SPLASH_AVATAR_FULL_SPA), IMAGE_BITMAP, 
          0, 0, LR_DEFAULTCOLOR);
#else
		HBITMAP bitmap;
        bitmap = (HBITMAP)::LoadImage(GetModuleHandle(NULL),
          MAKEINTRESOURCE(IDB_MFT_APP_SPLASH_AVATAR_FULL), IMAGE_BITMAP, 
          0, 0, LR_DEFAULTCOLOR);
#endif
        if (NULL != bitmap) {
          // Show splash image.
          ::SendMessage(splashWnd, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP,
            (LPARAM)bitmap);
        }

        // We waiting here...
        ::CreateThread(NULL, 0, SplashThreadProc, 
          (LPVOID)splashWnd, NULL, NULL);

        // Show splash screen.
        ///////////////////////////////////////////////////////////////////////
      }
      return 0;
    case WM_SETFOCUS:
		if(g_handler.get() && g_handler->GetBrowserHwnd()) {
        // Pass focus to the browser window
        ::PostMessage(g_handler->GetBrowserHwnd(), WM_SETFOCUS, wParam, NULL);
      }
      return 0;
    case WM_CLOSE:
      if (g_handler.get())
      {
        CefRefPtr<CefBrowser> browser = g_handler->GetBrowser();
        if (browser.get())
        {
          // Let the browser window know we are about to destroy it.
          browser->ParentWindowWillClose();
        }
      }
	  //save first launch state
	  SaveFirstLaunchState();
	  //save settings
	  SaveNavigationState();
	  SaveVehicleState();
      break;

    case WM_SHOWWINDOW:
      {
#ifdef ENABLE_MESSAGE_BROKER
        if (pCMessageBrokerControllerBackend)
        {
          if (0 != wParam)
          {
            pCMessageBrokerControllerBackend->onAppStatusChanged(true);
          } else
          {
            pCMessageBrokerControllerBackend->onAppStatusChanged(false);
          }
        }
#endif //ENABLE_MESSAGE_BROKER
        break;
      }
    case WM_SIZE:
      {
#ifdef ENABLE_MESSAGE_BROKER
        if (pCMessageBrokerControllerBackend)
        {
          pCMessageBrokerControllerBackend->onFullScreenChanged(SIZE_MAXIMIZED == wParam);
        }
#endif //ENABLE_MESSAGE_BROKER
        break;
      }
    case WM_DESTROY:
      // The frame window has exited
      ::PostQuitMessage(0);
      return 0;
    case ZOOM_IN_EVENT:
      SetWindowZoom(ZOOMED_WIDTH, ZOOMED_HEIGHT, 1);
      ToCenterScreen(SW_SHOW);
      break;
    case ZOOM_OUT_EVENT:
      SetWindowZoom(WINDOW_WIDTH, WINDOW_HEIGHT, 0);
      ToCenterScreen(SW_SHOW);
      break;
    }

  return ::DefWindowProc(hWnd, message, wParam, lParam);
}

// -----------------------------------------------------------------------------
std::string AppGetWorkingDirectory()
{
  return szWorkingDir;
}



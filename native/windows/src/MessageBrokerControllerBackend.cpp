/**
 * \file MessageBrokerControllerBackend.cpp
 * \brief MessageBroker Controller for Backend.
 * \author AKara
 */

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <Shellapi.h>
#include <tchar.h>
#include <string.h>

#include "MessageBrokerControllerBackend.hpp"

#include "DebugHelper.h"

extern bool			bIsFirstLaunch;					// Indicate if it is the first launch of program
extern std::string	strCurrentVehicle;				// the current vehicle model in settings
extern bool			bIsNavEnabled;					// value of navigation settings
extern bool	setVehicleSetting(std::string value);
extern bool setNavigationSetting(bool value);
extern std::string	getInstallationPath();

#define BUFSIZE 80

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);

namespace NsMessageBroker
{
  CMessageBrokerControllerBackend::CMessageBrokerControllerBackend(const std::string& address, unsigned short port):
CMessageBrokerController(address, port, std::string("Backend"))
  {
  }

  CMessageBrokerControllerBackend::~CMessageBrokerControllerBackend()
  {
  }

  void CMessageBrokerControllerBackend::processRequest(Json::Value& root)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::processRequest()\n"));
    if (getControllersName() == getDestinationComponentName(root))
    {
      Json::Value response;
      response["jsonrpc"] = root["jsonrpc"];
      response["id"] = root["id"];
      if ("isFirstStart" == getMethodName(root))
      {
        isFirstStart(response);
      } else if ("isFullScreen" == getMethodName(root))
      {
        isFullScreen(response);
      } else if ("getWindowSize" == getMethodName(root))
      {
        getWindowSize(response);
      } else if ("getWindowDensity" == getMethodName(root))
      {
        getWindowDensity(response);
      } else if ("getOSInfo" == getMethodName(root))
      {
        getOSInfo(response);
      } else if ("getVehicleModel" == getMethodName(root))
      {
        getVehicleModel(response);
      } else if ("hasMaps" == getMethodName(root))
      {
        hasMaps(response);
      } else if ("setHasMaps" == getMethodName(root))
      {
		Json::Value& params = root["params"];
		bool maps = params["hasMaps"].asBool();
		setHasMaps(response,maps);
      } else if ("setVehicleModel" == getMethodName(root))
      {
		Json::Value& params = root["params"];
		std::string vehicle = params["vehicle"].asString();
		setVehicleModel(response,vehicle);
      } else if ("logToOS" == getMethodName(root))
      {
        Json::Value params = root["params"];
        if (params.isMember("message") && params["message"].isString())
        {
          logToOS(params, response);
        } else
        {
          DBG_MSG_ERROR(("Wrong logToOS parameter!\n"));
          Json::Value err;
          err["code"] = NsMessageBroker::INVALID_REQUEST;
          err["message"] = "Wrong logToOS parameter.";
          response["error"] = err;
        }
      } else if ("sendSupportEmail" == getMethodName(root))
	  {
		  sendSupportEmail(response);
	  } else if ("openEULA" == getMethodName(root))
	  {
		  openEula(response);
	  } else
      {
        DBG_MSG_ERROR(("Method has not been found!\n"));
        Json::Value err;
        err["code"] = NsMessageBroker::INVALID_REQUEST;
        err["message"] = "Method has not been found.";
        response["error"] = err;
      }
      sendJsonMessage(response);
    } else
    {
      DBG_MSG_ERROR(("Wrong message destination!\n"));
    }
  }

  void CMessageBrokerControllerBackend::processNotification(Json::Value& root)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::processNotification()\n"));
    root=root;//to prevent compiler warning
  }

  void CMessageBrokerControllerBackend::processResponse(std::string method, Json::Value& root)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::processResponse()\n"));
    root=root;//to prevent compiler warning
    method=method;//to prevent compiler warning
  }

  void CMessageBrokerControllerBackend::onFullScreenChanged(bool isFullScreen)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::onFullScreenChanged()\n"));
    Json::Value request, params;
    request["jsonrpc"] = "2.0";
    request["method"] = "Backend.onFullScreenChanged";
    params["isFullScreen"] = isFullScreen;
    request["params"] = params;
    sendJsonMessage(request);
  }

  void CMessageBrokerControllerBackend::sendNavSettingsNotification(bool navIsEnabled)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::sendNavSettingsNotification()\n"));
    Json::Value notification, params;
    notification["jsonrpc"] = "2.0";
    notification["method"] = "Backend.onHasMapsChanged";
    params["hasMaps"] = navIsEnabled;
    notification["params"] = params;
    sendJsonMessage(notification);
  }

  void CMessageBrokerControllerBackend::sendVehicleSettingsNotification(std::string vehicle)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::sendVehicleSettingsNotification()\n"));
    Json::Value notification, params;
    notification["jsonrpc"] = "2.0";
    notification["method"] = "Backend.onVehicleChanged";
    params["vehicle"] = vehicle;
    notification["params"] = params;
    sendJsonMessage(notification);
  }

  void CMessageBrokerControllerBackend::onAppStatusChanged(bool appBecameActive)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::onAppStatusChanged()\n"));
    Json::Value request, params;
    request["jsonrpc"] = "2.0";
    request["method"] = "Backend.onAppStatusChanged";
    params["appBecameActive"] = appBecameActive;
    request["params"] = params;
    sendJsonMessage(request);
  }

  void CMessageBrokerControllerBackend::isFirstStart(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::isFirstStart()\n"));
    Json::Value res;
    res["isFirstStart"] = bIsFirstLaunch;
    response["result"] = res;
  }

  void CMessageBrokerControllerBackend::isFullScreen(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::isFullScreen()\n"));
    Json::Value res;
    res["isFullScreen"] = false;
    response["result"] = res;
  }

  void CMessageBrokerControllerBackend::getWindowSize(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::getWindowSize()\n"));
    Json::Value res;
    res["width"] = 800;
    res["height"] = 480;
    response["result"] = res;
  }

  void CMessageBrokerControllerBackend::getWindowDensity(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::getWindowDensity()\n"));
    Json::Value res;
    res["windowDensity"] = 1;
    response["result"] = res;
  }

  void CMessageBrokerControllerBackend::getOSInfo(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::getOSInfo()\n"));
    Json::Value res;
    prepareOSInfo(response);
  }

  void CMessageBrokerControllerBackend::prepareOSInfo(Json::Value& response)
  {
    OSVERSIONINFOEX osvi;
    SYSTEM_INFO si;
    PGNSI pGNSI;
    BOOL bOsVersionInfoEx;

    std::string OSType = "";
    std::string OSVersion = "";
    Json::Value res;
    res["osType"] = OSType;
    res["osVersion"] = OSVersion;
    res["isNativeApplication"] = true;
    response["result"] = res;

    ZeroMemory(&si, sizeof(SYSTEM_INFO));
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

    // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
    // If that fails, try using the OSVERSIONINFO structure.

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
    {
      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
        return;
    }

    // Call GetNativeSystemInfo if supported
    // or GetSystemInfo otherwise.

    pGNSI = (PGNSI) GetProcAddress(
      GetModuleHandle(TEXT("kernel32.dll")), 
      "GetNativeSystemInfo");
    if(NULL != pGNSI)
      pGNSI(&si);
    else GetSystemInfo(&si);

    switch (osvi.dwPlatformId)
    {
      // Test for the Windows NT product family.

    case VER_PLATFORM_WIN32_NT:

      // Test for the specific product.

      if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )
      {
        if( osvi.wProductType == VER_NT_WORKSTATION )
        {
          printf ("Windows Vista ");
          OSType += "Windows Vista ";
        } else
        {
          printf ("Windows Server \"Longhorn\" " );
          OSType += "Windows Server \"Longhorn\" ";
        }
      }

      if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
      {
        if( GetSystemMetrics(SM_SERVERR2) )
        {
          printf( "Microsoft Windows Server 2003 \"R2\" ");
          OSType += "Microsoft Windows Server 2003 \"R2\" ";
        } else if( osvi.wProductType == VER_NT_WORKSTATION &&
          si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
        {
          printf( "Microsoft Windows XP Professional x64 Edition ");
          OSType += "Microsoft Windows XP Professional x64 Edition ";
        } else
        {
          printf ("Microsoft Windows Server 2003, ");
          OSType += "Microsoft Windows Server 2003, ";
        }
      }

      if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
      {
        printf ("Microsoft Windows XP ");
        OSType += "Microsoft Windows XP ";
      }

      if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
      {
        printf ("Microsoft Windows 2000 ");
        OSType += "Microsoft Windows 2000 ";
      }

      if ( osvi.dwMajorVersion <= 4 )
      {
        printf ("Microsoft Windows NT ");
        OSType += "Microsoft Windows NT ";
      }

      // Test for specific product on Windows NT 4.0 SP6 and later.
      if( bOsVersionInfoEx )
      {
        // Test for the workstation type.
        if ( osvi.wProductType == VER_NT_WORKSTATION &&
          si.wProcessorArchitecture!=PROCESSOR_ARCHITECTURE_AMD64)
        {
          if( osvi.dwMajorVersion == 4 )
          {
            printf ( "Workstation 4.0 " );
            OSType += "Workstation 4.0 ";
          } else if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
          {
            printf ( "Home Edition " );
            OSType += "Home Edition ";
          } else
          {
            printf ( "Professional " );
            OSType += "Professional ";
          }
        }

        // Test for the server type.
        else if ( osvi.wProductType == VER_NT_SERVER || 
          osvi.wProductType == VER_NT_DOMAIN_CONTROLLER )
        {
          if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==2)
          {
            if ( si.wProcessorArchitecture ==
              PROCESSOR_ARCHITECTURE_IA64 )
            {
              if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
              {
                printf ( "Datacenter Edition for Itanium-based Systems" );
                OSType += "Datacenter Edition for Itanium-based Systems";
              } else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
              {
                printf ( "Enterprise Edition for Itanium-based Systems" );
                OSType += "Enterprise Edition for Itanium-based Systems";
              }
            }

            else if ( si.wProcessorArchitecture ==
              PROCESSOR_ARCHITECTURE_AMD64 )
            {
              if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
              {
                printf ( "Datacenter x64 Edition " );
                OSType += "Datacenter x64 Edition ";
              } else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
              {
                printf ( "Enterprise x64 Edition " );
                OSType += "Enterprise x64 Edition ";
              } else
              {
                printf( "Standard x64 Edition " );
                OSType += "Standard x64 Edition ";
              }
            }

            else
            {
              if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
              {
                printf ( "Datacenter Edition " );
                OSType += "Datacenter Edition ";
              } else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
              {
                printf ( "Enterprise Edition " );
                OSType += "Enterprise Edition ";
              } else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
              {
                printf ( "Web Edition " );
                OSType += "Web Edition ";
              } else
              {
                printf ( "Standard Edition " );
                OSType += "Standard Edition ";
              }
            }
          }
          else if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==0)
          {
            if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
            {
              printf ( "Datacenter Server " );
              OSType += "Datacenter Server ";
            } else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
            {
              printf ( "Advanced Server " );
              OSType += "Advanced Server ";
            } else
            {
              printf ( "Server " );
              OSType += "Server ";
            }
          }
          else  // Windows NT 4.0 
          {
            if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
            {
              printf ("Server 4.0, Enterprise Edition " );
              OSType += "Server 4.0, Enterprise Edition " ;
            } else
            {
              printf ( "Server 4.0 " );
              OSType += "Server 4.0 ";
            }
          }
        }
      }
      // Test for specific product on Windows NT 4.0 SP5 and earlier
      else  
      {
        HKEY hKey;
        TCHAR szProductType[BUFSIZE];
        DWORD dwBufLen=BUFSIZE*sizeof(TCHAR);
        LONG lRet;

        lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
          TEXT("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"), 0, KEY_QUERY_VALUE, &hKey );
        if( lRet != ERROR_SUCCESS )
          return;

        lRet = RegQueryValueEx( hKey, TEXT("ProductType"),
          NULL, NULL, (LPBYTE) szProductType, &dwBufLen);
        RegCloseKey( hKey );

        if( (lRet != ERROR_SUCCESS) ||
          (dwBufLen > BUFSIZE*sizeof(TCHAR)) )
          return;

        if ( lstrcmpi( TEXT("WINNT"), szProductType) == 0 )
        {
          printf( "Workstation " );
          OSType += "Workstation ";
        }
        if ( lstrcmpi( TEXT("LANMANNT"), szProductType) == 0 )
        {
          printf( "Server " );
          OSType += "Server ";
        }
        if ( lstrcmpi( TEXT("SERVERNT"), szProductType) == 0 )
        {
          printf( "Advanced Server " );
          OSType += "Advanced Server ";
        }
        printf( "%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion );
        char outstr[255];
        int size = sprintf(outstr, "%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion );
        if (size>0)
        {
          OSType += std::string(outstr, size);
        }
      }

      // Display service pack (if any) and build number.

      if( osvi.dwMajorVersion == 4 && 
        lstrcmpi( osvi.szCSDVersion, TEXT("Service Pack 6") ) == 0 )
      { 
        HKEY hKey;
        LONG lRet;

        // Test for SP6 versus SP6a.
        lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
          TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"), 0, KEY_QUERY_VALUE, &hKey );
        if( lRet == ERROR_SUCCESS )
        {
          printf( "Service Pack 6a (Build %d)\n", osvi.dwBuildNumber & 0xFFFF );
          char outstr[255];
          int size = sprintf(outstr, "Service Pack 6a (Build %d)\n", osvi.dwBuildNumber & 0xFFFF );
          if (size>0)
          {
            OSVersion += std::string(outstr, size);
          }
        }

        else // Windows NT 4.0 prior to SP6a
        {
          _tprintf( TEXT("%s (Build %d)\n"), osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
          char outstr[255];
          int size = sprintf(outstr, "%S (Build %d)", osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
          if (size>0)
          {
            OSVersion += std::string(outstr, size);
          }
        }

        RegCloseKey( hKey );
      }
      else // not Windows NT 4.0 
      {
        _tprintf( TEXT("%s (Build %d)\n"), osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
        char outstr[255];
        int size = sprintf(outstr, "%S (Build %d)", osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
        if (size>0 && size<255)
        {

          OSVersion += std::string(outstr, size);
        }
      }

      break;

      // Test for the Windows Me/98/95.
    case VER_PLATFORM_WIN32_WINDOWS:

      if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
      {
        printf ("Microsoft Windows 95 ");
        OSType += "Microsoft Windows 95 ";
        if (osvi.szCSDVersion[1]=='C' || osvi.szCSDVersion[1]=='B')
        {
          printf("OSR2 " );
          OSType += "OSR2 ";
        }
      } 

      if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
      {
        printf ("Microsoft Windows 98 ");
        OSType += "Microsoft Windows 98 ";
        if ( osvi.szCSDVersion[1]=='A' || osvi.szCSDVersion[1]=='B')
        {
          printf("SE " );
          OSType += "SE ";
        }
      } 

      if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
      {
        printf ("Microsoft Windows Millennium Edition\n");
        OSType += "Microsoft Windows Millennium Edition";
      } 
      break;

    case VER_PLATFORM_WIN32s:
      printf ("Microsoft Win32s\n");
      OSType += "Microsoft Win32s";
      break;
    }
    res["osType"] = OSType;
    res["osVersion"] = OSVersion;
    response["result"] = res;
  }

  void CMessageBrokerControllerBackend::hasMaps(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::hasMaps()\n"));
    Json::Value res;
	res["hasMaps"] = bIsNavEnabled;
    response["result"] = res;
  }

  void CMessageBrokerControllerBackend::getVehicleModel(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::getVehicleModel()\n"));
    Json::Value res;
	res["vehicle"] = strCurrentVehicle;
    response["result"] = res;
  }

  void CMessageBrokerControllerBackend::setHasMaps(Json::Value& response, bool value)
  {
	DBG_MSG(("CMessageBrokerControllerBackend::setHasMaps()\n"));
	if(setNavigationSetting(value))
		response["result"] = "OK";
	else
		response["result"] = "!!! ERROR while navigation setting";
  }
  
  void CMessageBrokerControllerBackend::setVehicleModel(Json::Value& response, std::string value)
  {
	DBG_MSG(("CMessageBrokerControllerBackend::setVehicleModel()\n"));
	if(setVehicleSetting(value))
		response["result"] = "OK";
	else
		response["result"] = "!!! ERROR while vehicle setting";
  }

  void CMessageBrokerControllerBackend::logToOS(Json::Value& params, Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::logToOS()\n"));
    printf("logToOS: %s", params["message"].asString().c_str());
    response["result"] = "OK";
  }

  void CMessageBrokerControllerBackend::sendSupportEmail(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::sendSupportEmail()\n"));
	int h = (int)ShellExecute(NULL, _T("open"), 
		_T("mailto:mftguide@ford.com ? subject=subject of email & body=Sent from Windows OS"), 
		NULL , NULL, SW_SHOWNORMAL);
	printf( "\nopen Eula %i\n", h );
    response["result"] = "OK";
  }

   void CMessageBrokerControllerBackend::openEula(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::sendSupportEmail()\n"));
	// get installation path
	std::string path = getInstallationPath();
	path.append("\\license.txt");
	std::wstring wPath = toStringW(path.c_str());
	int h = (int)ShellExecute(NULL, _T("open"), 
		LPCWSTR(wPath.c_str()), 
		NULL , NULL, SW_SHOWNORMAL);
	printf( "\nopen Eula %i\n", h );
    response["result"] = "OK";
  }

std::wstring  CMessageBrokerControllerBackend::toStringW( const std::string& strText )
{
	std::wstring wstrResult;
	wstrResult.resize( strText.length() );
	typedef std::codecvt<wchar_t, char, mbstate_t> widecvt;
	std::locale locGlob;
	std::locale::global( locGlob );
	const widecvt& cvt( std::use_facet<widecvt>( locGlob ) );
	mbstate_t State;
	const char* cTemp;
	wchar_t* wTemp;
	cvt.in( State, 
			&strText[0], 
			&strText[0] + strText.length(), 
			cTemp,
			(wchar_t*)&wstrResult[0], 
			&wstrResult[0] + wstrResult.length(), 
			wTemp );
	return wstrResult;
}

} /* namespace NsMessageBroker */

#ifndef __UILIB_H__
#define __UILIB_H__
#pragma once

// Suppress warnings
#ifdef _MSC_VER
#pragma warning (disable : 4819) // chars not representable in current code page.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif // _MSC_VER

// Export/Import macros
#ifdef UILIB_STATIC
#define DUILIB_API
#else
#if defined(UILIB_EXPORTS)
# if defined(_MSC_VER)
#   define DUILIB_API __declspec(dllexport)
# else
#   define DUILIB_API
# endif
#else
# if defined(_MSC_VER)
#   define DUILIB_API __declspec(dllimport)
# else
#   define DUILIB_API
# endif
#endif
#endif
#define UILIB_COMDAT __declspec(selectany)

// Manifest dependency
#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

// System headers
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stddef.h>
#include <richedit.h>
#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>
#include <malloc.h>
#include <gdiplus.h>

// Module switches
// NOTE: Modules off by default; uncomment to enable.
#define DUIMOD_ACTIVEX              // CActiveXUI
#define DUIMOD_ANIMATION            // CUIAnimation
#define DUIMOD_ANIMATIONTABLAYOUT   // CAnimationTabLayoutUI (auto-enables ANIMATION)
#define DUIMOD_FADEBUTTON           // CFadeButtonUI (auto-enables ANIMATION)
#define DUIMOD_GIFANIM              // CGifAnimUI
#define DUIMOD_COLORPALETTE         // CColorPaletteUI
#define DUIMOD_HOTKEY               // CHotKeyUI
#define DUIMOD_IPADDRESS            // CIPAddressUI
#define DUIMOD_IPADDRESSEX          // CIPAddressExUI
#define DUIMOD_ROLLTEXT             // CRollTextUI
#define DUIMOD_LOADING              // CLoadingUI
#define DUIMOD_RING                 // CRingUI
#define DUIMOD_TRAYICON             // CTrayIconUI
#define DUIMOD_WEBBROWSER           // CWebBrowserUI

// Module dependencies auto-enabled.
#ifdef DUIMOD_ANIMATIONTABLAYOUT
#ifndef DUIMOD_ANIMATION
#define DUIMOD_ANIMATION
#endif
#endif
#ifdef DUIMOD_FADEBUTTON
#ifndef DUIMOD_ANIMATION
#define DUIMOD_ANIMATION
#endif
#endif

#include "Utils/Utils.h"
#include "Utils/unzip.h"
#include "Utils/VersionHelpers.h"
#include "Core/UIMarkup.h"
#include "Utils/UIShadow.h"
#include "Utils/UIDelegate.h"
#include "Utils/DragDropImpl.h"
#include "Utils/DPI.h"

#include "Core/UIDefine.h"
#include "Core/UIResourceManager.h"
#include "Core/UIManager.h"
#include "Core/UIBase.h"
#include "Core/ControlFactory.h"
#include "Core/UIControl.h"
#include "Core/UIContainer.h"
#include "Core/UIDlgBuilder.h"
#include "Core/UIRender.h"
#include "Utils/WinImplBase.h"

#include "Layout/UIHorizontalLayout.h"
#include "Layout/UIVerticalLayout.h"
#include "Layout/UITabLayout.h"
#include "Layout/UITileLayout.h"
#include "Layout/UIChildLayout.h"
#include "Control/UILabel.h"
#include "Control/UIText.h"
#include "Control/UIButton.h"
#include "Control/UIOption.h"
#include "Control/UIEdit.h"
#include "Control/UIRichEdit.h"
#include "Control/UIList.h"
#include "Control/UIListEx.h"
#include "Control/UITreeView.h"
#include "Control/UICombo.h"
#include "Control/UIComboBox.h"
#include "Control/UIDateTime.h"
#include "Control/UIProgress.h"
#include "Control/UISlider.h"
#include "Control/UIScrollBar.h"
#include "Control/UIGroupBox.h"
#include "Control/UIMenu.h"
#include "Control/UIPageControl.h"

// Switchable modules
#ifdef DUIMOD_ACTIVEX
#include "Control/UIActiveX.h"
#endif
#ifdef DUIMOD_ANIMATION
#include "Control/UIAnimation.h"
#endif
#ifdef DUIMOD_ANIMATIONTABLAYOUT
#include "Layout/UIAnimationTabLayout.h"
#endif
#ifdef DUIMOD_GIFANIM
#include "Control/UIGifAnim.h"
#endif
#ifdef DUIMOD_COLORPALETTE
#include "Control/UIColorPalette.h"
#endif
#ifdef DUIMOD_HOTKEY
#include "Control/UIHotKey.h"
#endif
#ifdef DUIMOD_IPADDRESS
#include "Control/UIIPAddress.h"
#endif
#ifdef DUIMOD_IPADDRESSEX
#include "Control/UIIPAddressEx.h"
#endif
#ifdef DUIMOD_ROLLTEXT
#include "Control/UIRollText.h"
#endif
#ifdef DUIMOD_FADEBUTTON
#include "Control/UIFadeButton.h"
#endif
#ifdef DUIMOD_LOADING
#include "Control/UILoading.h"
#endif
#ifdef DUIMOD_RING
#include "Control/UIRing.h"
#endif
#ifdef DUIMOD_TRAYICON
#include "Utils/TrayIcon.h"
#endif
#ifdef DUIMOD_WEBBROWSER
#include "Control/UIWebBrowser.h"
#endif

// Link libraries
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "GdiPlus.lib")
#pragma comment(lib, "Imm32.lib")

#endif // __UILIB_H__

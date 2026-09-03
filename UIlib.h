#ifndef __UILIB_H__
#define __UILIB_H__
#pragma once

// ===========================================================================
// 导出/导入宏
// ===========================================================================
#ifdef UILIB_STATIC
#define UILIB_API
#else
#if defined(UILIB_EXPORTS)
# if defined(_MSC_VER)
#   define UILIB_API __declspec(dllexport)
# else
#   define UILIB_API
# endif
#else
# if defined(_MSC_VER)
#   define UILIB_API __declspec(dllimport)
# else
#   define UILIB_API
# endif
#endif
#endif
#define UILIB_COMDAT __declspec(selectany)

// 抑制警告
#pragma warning(disable:4505)
#pragma warning(disable:4251)
#pragma warning(disable:4189)
#pragma warning(disable:4121)
#pragma warning(disable:4100)

// 清单依赖
#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

// ===========================================================================
// 系统头文件
// ===========================================================================
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

// ===========================================================================
// 模块开关
// 注意：所有模块默认关闭，取消下方对应模块的注释符即可启用
// ===========================================================================
#define DUIMOD_ACTIVEX              // CActiveXUI ActiveX容器
#define DUIMOD_ANIMATION            // CUIAnimation 动画基类（FADEBUTTON/TABLAYOUT 自动联动）
#define DUIMOD_ANIMATIONTABLAYOUT   // CAnimationTabLayoutUI 动画页切换（自动启用 ANIMATION）
#define DUIMOD_COLORPALETTE         // CColorPaletteUI 调色板
#define DUIMOD_FADEBUTTON           // CFadeButtonUI 渐变按钮（自动启用 ANIMATION）
#define DUIMOD_GIFANIM              // CGifAnimUI GIF动画
#define DUIMOD_HOTKEY               // CHotKeyUI 热键
#define DUIMOD_IPADDRESS            // CIPAddressUI IP地址
#define DUIMOD_IPADDRESSEX          // CIPAddressExUI 扩展IP地址
#define DUIMOD_LISTEX               // CListExUI 扩展列表（自动启用 COMBOBOX + RICHEDIT）
#define DUIMOD_LOADING              // CLoadingUI 加载动画
#define DUIMOD_PAGECONTROL          // CPageControlUI 分页
#define DUIMOD_PROGRESS             // CProgressUI 进度条
#define DUIMOD_RICHEDIT             // CRichEditUI 富文本
#define DUIMOD_RING                 // CRingUI 环形
#define DUIMOD_ROLLTEXT             // CRollTextUI 滚动文字
#define DUIMOD_SLIDER               // CSliderUI 滑动条（自动启用 PROGRESS）
#define DUIMOD_TRAYICON             // CTrayIconUI 托盘图标
#define DUIMOD_TREEVIEW             // CTreeViewUI 树控件

// ===========================================================================
// 模块依赖联动（自动启用依赖模块，无需手动开启）
// ===========================================================================
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

#ifdef DUIMOD_LISTEX
#ifndef DUIMOD_RICHEDIT
#define DUIMOD_RICHEDIT
#endif
#endif

#ifdef DUIMOD_SLIDER
#ifndef DUIMOD_PROGRESS
#define DUIMOD_PROGRESS
#endif
#endif

// ===========================================================================
// 工具及核心头文件
// ===========================================================================
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

// ===========================================================================
// 核心模块
// ===========================================================================
#include "Layout/UIChildLayout.h"
#include "Layout/UIHorizontalLayout.h"
#include "Layout/UIVerticalLayout.h"
#include "Layout/UITabLayout.h"
#include "Layout/UITileLayout.h"
#include "Control/UILabel.h"
#include "Control/UIButton.h"
#include "Control/UIList.h"
#include "Control/UICombo.h"
#include "Control/UIComboBox.h"
#include "Control/UIDateTime.h"
#include "Control/UIEdit.h"
#include "Control/UIGroupBox.h"
#include "Control/UIMenu.h"
#include "Control/UIOption.h"
#include "Control/UIScrollBar.h"
#include "Control/UIText.h"

// ===========================================================================
// 开关模块（条件包含，由上方开关控制）
// ===========================================================================
#ifdef DUIMOD_ACTIVEX
#include "Control/UIActiveX.h"
#endif
#ifdef DUIMOD_ANIMATION
#include "Control/UIAnimation.h"
#endif
#ifdef DUIMOD_ANIMATIONTABLAYOUT
#include "Layout/UIAnimationTabLayout.h"
#endif
#ifdef DUIMOD_COLORPALETTE
#include "Control/UIColorPalette.h"
#endif
#ifdef DUIMOD_FADEBUTTON
#include "Control/UIFadeButton.h"
#endif
#ifdef DUIMOD_GIFANIM
#include "Control/UIGifAnim.h"
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
#ifdef DUIMOD_LISTEX
#include "Control/UIListEx.h"
#endif
#ifdef DUIMOD_LOADING
#include "Control/UILoading.h"
#endif
#ifdef DUIMOD_PAGECONTROL
#include "Control/UIPageControl.h"
#endif
#ifdef DUIMOD_PROGRESS
#include "Control/UIProgress.h"
#endif
#ifdef DUIMOD_RICHEDIT
#include "Control/UIRichEdit.h"
#endif
#ifdef DUIMOD_RING
#include "Control/UIRing.h"
#endif
#ifdef DUIMOD_ROLLTEXT
#include "Control/UIRollText.h"
#endif
#ifdef DUIMOD_SLIDER
#include "Control/UISlider.h"
#endif
#ifdef DUIMOD_TRAYICON
#include "Utils/TrayIcon.h"
#endif
#ifdef DUIMOD_TREEVIEW
#include "Control/UITreeView.h"
#endif

// ===========================================================================
// 链接库
// ===========================================================================
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "GdiPlus.lib")
#pragma comment(lib, "Imm32.lib")

#endif // __UILIB_H__

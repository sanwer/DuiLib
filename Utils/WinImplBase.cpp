#include "StdAfx.h"

namespace DuiLib
{
	//////////////////////////////////////////////////////////////////////////
	//
	LPBYTE WindowImplBase::m_lpResourceZIPBuffer = NULL;

	DUI_BEGIN_MESSAGE_MAP(WindowImplBase, CNotifyPump)
		DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
	DUI_END_MESSAGE_MAP()

	void WindowImplBase::OnFinalMessage(HWND hWnd)
	{
		m_pm.RemovePreMessageFilter(this);
		m_pm.RemoveNotifier(this);
		m_pm.ReapObjects(m_pm.GetRoot());
	}

	LRESULT WindowImplBase::ResponseDefaultKeyEvent(WPARAM wParam)
	{
		if (wParam == VK_RETURN)
		{
			return FALSE;
		}
		else if (wParam == VK_ESCAPE)
		{
			Close();
			return TRUE;
		}

		return FALSE;
	}

	UINT WindowImplBase::GetClassStyle() const
	{
		return CS_DBLCLKS;
	}

	UILIB_RESOURCETYPE WindowImplBase::GetResourceType() const
	{
		return UILIB_FILE;
	}

	CDuiString WindowImplBase::GetZIPFileName() const
	{
		return _T("");
	}

	LPCTSTR WindowImplBase::GetResourceID() const
	{
		return _T("");
	}

	CDuiString WindowImplBase::GetSkinFolder()
	{
		return _T("");
	}

	CControlUI* WindowImplBase::CreateControl(LPCTSTR pstrClass)
	{
		return NULL;
	}

	LPCTSTR WindowImplBase::QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType)
	{
		return NULL;
	}

	LRESULT WindowImplBase::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/)
	{
		if (uMsg == WM_KEYDOWN)
		{
			switch (wParam)
			{
			case VK_RETURN:
			case VK_ESCAPE:
				return ResponseDefaultKeyEvent(wParam);
			default:
				break;
			}
		}
		return FALSE;
	}

	LRESULT WindowImplBase::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

#if defined(WIN32) && !defined(UNDER_CE)
	LRESULT WindowImplBase::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if (::IsIconic(*this)) bHandled = FALSE;
		return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT WindowImplBase::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LPRECT pRect = NULL;

		if (wParam == TRUE)
		{
			LPNCCALCSIZE_PARAMS pParam = (LPNCCALCSIZE_PARAMS)lParam;
			pRect = &pParam->rgrc[0];
		}
		else
		{
			pRect = (LPRECT)lParam;
		}

		if (::IsZoomed(m_hWnd))
		{
			MONITORINFO oMonitor = {};
			oMonitor.cbSize = sizeof(oMonitor);
			::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
			CDuiRect rcWork = oMonitor.rcWork;
			CDuiRect rcMonitor = oMonitor.rcMonitor;
			rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

			pRect->right = pRect->left + rcWork.GetWidth();
			pRect->bottom = pRect->top + rcWork.GetHeight();
			return WVR_REDRAW;
		}

		return 0;
	}

	LRESULT WindowImplBase::OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 0;
	}

	LRESULT WindowImplBase::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

		CControlUI* pMaxBtn = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
		CControlUI* pRestoreBtn = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
		if (pMaxBtn && pMaxBtn->IsVisible()) {
			RECT rcBtn = pMaxBtn->GetPos();
			if (::PtInRect(&rcBtn, pt)) {
				return HTMAXBUTTON;
			}
		}
		else if (pRestoreBtn && pRestoreBtn->IsVisible()) {
			RECT rcBtn = pRestoreBtn->GetPos();
			if (::PtInRect(&rcBtn, pt)) {
				return HTMAXBUTTON;
			}
		}

		if (!::IsZoomed(*this))
		{
			RECT rcSizeBox = m_pm.GetSizeBox();
			if (pt.y < rcClient.top + rcSizeBox.top)
			{
				if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
				if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
				return HTTOP;
			}
			else if (pt.y > rcClient.bottom - rcSizeBox.bottom)
			{
				if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
				if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
				return HTBOTTOM;
			}

			if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
		}

		RECT rcCaption = m_pm.GetCaptionRect();
		if (0 > rcCaption.bottom)
		{
			rcCaption.bottom = rcClient.bottom;
		}

		if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom)
		{
			CControlUI* pControl = m_pm.FindControl(pt);
			if (pControl && _tcsicmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 &&
				_tcsicmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
				_tcsicmp(pControl->GetClass(), DUI_CTR_TEXT) != 0)
				return HTCAPTION;
		}

		return HTCLIENT;
	}

	LRESULT WindowImplBase::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;

		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
		CDuiRect rcMonitor = oMonitor.rcMonitor;
		rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

		lpMMI->ptMaxPosition.x = rcWork.left;
		lpMMI->ptMaxPosition.y = rcWork.top;

		lpMMI->ptMaxTrackSize.x = m_pm.GetMaxInfo().cx == 0 ? rcWork.GetWidth() : m_pm.GetMaxInfo().cx;
		lpMMI->ptMaxTrackSize.y = m_pm.GetMaxInfo().cy == 0 ? rcWork.GetHeight() : m_pm.GetMaxInfo().cy;

		lpMMI->ptMinTrackSize.x = m_pm.GetMinInfo().cx;
		lpMMI->ptMinTrackSize.y = m_pm.GetMinInfo().cy;

		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
#endif

	LRESULT WindowImplBase::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SIZE szRoundCorner = m_pm.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
		if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
			CDuiRect rcWnd;
			::GetWindowRect(*this, &rcWnd);
			rcWnd.Offset(-rcWnd.left, -rcWnd.top);
			rcWnd.right++; rcWnd.bottom++;
			HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
			::SetWindowRgn(*this, hRgn, TRUE);
			::DeleteObject(hRgn);

			if (m_pm.IsValid() && wParam == SIZE_RESTORED) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if (pControl) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if (pControl) pControl->SetVisible(false);
			}
		}
#endif
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (wParam == SC_CLOSE)
		{
			bHandled = TRUE;
			SendMessage(WM_CLOSE);
			return 0;
		}
#if defined(WIN32) && !defined(UNDER_CE)
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if (::IsZoomed(*this) != bZoomed)
		{
			CControlUI* pbtnMax = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));         // max button
			CControlUI* pbtnRestore = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn"))); // restore button

			// toggle status of max and restore button
			if (pbtnMax && pbtnRestore)
			{
				pbtnMax->SetVisible(TRUE == bZoomed);
				pbtnRestore->SetVisible(FALSE == bZoomed);
			}
		}
#else
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
		return lRes;
	}

	LRESULT WindowImplBase::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
			rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

		m_pm.Init(m_hWnd);
		m_pm.AddPreMessageFilter(this);

		CDialogBuilder builder;
		CDuiString strResourcePath = m_pm.GetResourcePath();
		if (strResourcePath.IsEmpty())
		{
			strResourcePath = m_pm.GetInstancePath();
			strResourcePath += GetSkinFolder().GetData();
		}
		m_pm.SetResourcePath(strResourcePath.GetData());

		switch (GetResourceType())
		{
		case UILIB_ZIP:
			m_pm.SetResourceZip(GetZIPFileName().GetData(), true);
			break;
		case UILIB_ZIPRESOURCE:
		{
			HRSRC hResource = ::FindResource(m_pm.GetResourceDll(), GetResourceID(), _T("ZIPRES"));
			if (hResource == NULL)
				return 0L;
			DWORD dwSize = 0;
			HGLOBAL hGlobal = ::LoadResource(m_pm.GetResourceDll(), hResource);
			if (hGlobal == NULL)
			{
#if defined(WIN32) && !defined(UNDER_CE)
				::FreeResource(hResource);
#endif
				return 0L;
			}
			dwSize = ::SizeofResource(m_pm.GetResourceDll(), hResource);
			if (dwSize == 0)
				return 0L;
			m_lpResourceZIPBuffer = new BYTE[dwSize];
			if (m_lpResourceZIPBuffer != NULL)
			{
				::CopyMemory(m_lpResourceZIPBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
			}
#if defined(WIN32) && !defined(UNDER_CE)
			::FreeResource(hResource);
#endif
			m_pm.SetResourceZip(m_lpResourceZIPBuffer, dwSize);
		}
		break;
		}

		CControlUI* pRoot = NULL;
		if (GetResourceType() == UILIB_RESOURCE)
		{
			STRINGorID xml(_ttoi(GetSkinFile().GetData()));
			pRoot = builder.Create(xml, _T("xml"), this, &m_pm);
		}
		else
			pRoot = builder.Create(GetSkinFile().GetData(), (UINT)0, this, &m_pm);
		ASSERT(pRoot);
		if (pRoot == NULL)
		{
			MessageBox(NULL, _T("Failed to load resource file!"), _T("Duilib"), MB_OK | MB_ICONERROR);
			return -1;
		}
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		InitWindow();
		return 0;
	}

	LRESULT WindowImplBase::OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch (uMsg)
		{
		case WM_CREATE:         lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:          lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:        lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
#if defined(WIN32) && !defined(UNDER_CE)
		case WM_NCACTIVATE:     lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:     lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:        lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:      lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_NCLBUTTONDBLCLK: {
			if (wParam == HTMAXBUTTON) {
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				::ScreenToClient(m_hWnd, &pt);
				LPARAM param = MAKELPARAM(pt.x, pt.y);
				if (uMsg == WM_NCLBUTTONDOWN) {
					::SendMessage(m_hWnd, WM_LBUTTONDOWN, wParam, param);
				}
				else if (uMsg == WM_NCLBUTTONUP) {
					::SendMessage(m_hWnd, WM_LBUTTONUP, wParam, param);
				}
				return 0;
			}
			bHandled = FALSE;
			break;
		}
		case WM_NCMOUSEHOVER:
		case WM_NCMOUSELEAVE:
		case WM_NCMOUSEMOVE: {
			if (wParam == HTMAXBUTTON) {
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				::ScreenToClient(m_hWnd, &pt);
				LPARAM param = MAKELPARAM(pt.x, pt.y);
				if (uMsg == WM_NCMOUSEHOVER) {
					//::SendMessage(m_hWnd, WM_MOUSEHOVER, wParam, param);
				}
				else if (uMsg == WM_NCMOUSELEAVE) {
					//::SendMessage(m_hWnd, WM_MOUSELEAVE, wParam, param);
				}
				else {
					::SendMessage(m_hWnd, WM_MOUSEMOVE, wParam, param);
				}
				return 0;
			}
			bHandled = FALSE;
			break;
		}
		case WM_GETMINMAXINFO:  lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_MOUSEWHEEL:     lRes = OnMouseWheel(uMsg, wParam, lParam, bHandled); break;
#endif
		case WM_SIZE:           lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_CHAR:           lRes = OnChar(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:     lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_KEYDOWN:        lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
		case WM_KILLFOCUS:      lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
		case WM_SETFOCUS:       lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
		case WM_LBUTTONUP:      lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled); break;
		case WM_LBUTTONDOWN:    lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
		case WM_MOUSEMOVE:      lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
		case WM_MOUSEHOVER:     lRes = OnMouseHover(uMsg, wParam, lParam, bHandled); break;
		default:                bHandled = FALSE; break;
		}
		if (bHandled) return lRes;

		lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
		if (bHandled) return lRes;

		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes))
			return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	LRESULT WindowImplBase::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LONG WindowImplBase::GetStyle()
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;

		return styleValue;
	}

	void WindowImplBase::OnClick(TNotifyUI& msg)
	{
		CDuiString sCtrlName = msg.pSender->GetName();
		if (sCtrlName == _T("closebtn")) {
			Close();
			return;
		}
		else if (sCtrlName == _T("minbtn")) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return;
		}
		else if (sCtrlName == _T("maxbtn")) {
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			return;
		}
		else if (sCtrlName == _T("restorebtn")) {
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
			return;
		}
		return;
	}

	void WindowImplBase::Notify(TNotifyUI& msg)
	{
		return CNotifyPump::NotifyPump(msg);
	}
}

#include "StdAfx.h"
#include "UILinearLayout.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CLinearLayoutUI)

    CLinearLayoutUI::CLinearLayoutUI()
        : m_bHorizontal(true)
        , m_iSepSize(0)
        , m_uButtonState(0)
        , m_bImmMode(false)
	{
		ptLastMouse.x = ptLastMouse.y = 0;
		::ZeroMemory(&m_rcNewPos, sizeof(m_rcNewPos));
	}

	LPCTSTR CLinearLayoutUI::GetClass() const
	{
		return _T("LinearLayoutUI");
	}

	LPVOID CLinearLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, DUI_CTR_LINEARLAYOUT) == 0) return static_cast<CLinearLayoutUI*>(this);
		return CContainerUI::GetInterface(pstrName);
	}

	UINT CLinearLayoutUI::GetControlFlags() const
	{
		if (IsEnabled() && m_iSepSize != 0) return UIFLAG_SETCURSOR;
		return 0;
	}

	void CLinearLayoutUI::SetHorizontal(bool bHorizontal)
	{
		if (m_bHorizontal == bHorizontal) return;
		m_bHorizontal = bHorizontal;
		NeedParentUpdate();
	}

	bool CLinearLayoutUI::IsHorizontal() const
	{
		return m_bHorizontal;
	}

	void CLinearLayoutUI::SetSepSize(int iSize)
	{
		if (m_iSepSize == iSize) return;
		m_iSepSize = iSize;
		NeedParentUpdate();
	}

	int CLinearLayoutUI::GetSepSize() const
	{
		if (m_pManager) return m_pManager->GetDPIObj()->Scale(m_iSepSize);
		return m_iSepSize;
	}

	void CLinearLayoutUI::SetSepImmMode(bool bImmediately)
	{
		if (m_bImmMode == bImmediately) return;
		if ((m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode && m_pManager != NULL) {
			m_pManager->RemovePostPaint(this);
		}
		m_bImmMode = bImmediately;
	}

	bool CLinearLayoutUI::IsSepImmMode() const
	{
		return m_bImmMode;
	}

	void CLinearLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcsicmp(pstrName, _T("orientation")) == 0) {
			if (_tcsicmp(pstrValue, _T("h")) == 0 || _tcsicmp(pstrValue, _T("horizontal")) == 0)
				SetHorizontal(true);
			else if (_tcsicmp(pstrValue, _T("v")) == 0 || _tcsicmp(pstrValue, _T("vertical")) == 0)
				SetHorizontal(false);
		}
		else if (_tcsicmp(pstrName, _T("sep")) == 0 || _tcsicmp(pstrName, _T("sepsize")) == 0
			|| _tcsicmp(pstrName, _T("sepwidth")) == 0 || _tcsicmp(pstrName, _T("sepheight")) == 0) {
			SetSepSize(_ttoi(pstrValue));
		}
		else if (_tcsicmp(pstrName, _T("sepimm")) == 0) {
			SetSepImmMode(_tcsicmp(pstrValue, _T("true")) == 0);
		}
		else {
			CContainerUI::SetAttribute(pstrName, pstrValue);
		}
	}

	void CLinearLayoutUI::DoEvent(TEventUI& event)
	{
		if (m_iSepSize != 0) {
			if (event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
			{
				RECT rcSeparator = GetThumbRect(false);
				if (::PtInRect(&rcSeparator, event.ptMouse)) {
					m_uButtonState |= UISTATE_CAPTURED;
					ptLastMouse = event.ptMouse;
					m_rcNewPos = m_rcItem;
					if (!m_bImmMode && m_pManager) m_pManager->AddPostPaint(this);
					return;
				}
			}
			if (event.Type == UIEVENT_BUTTONUP)
			{
				if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
					m_uButtonState &= ~UISTATE_CAPTURED;
					m_rcItem = m_rcNewPos;
					if (!m_bImmMode && m_pManager) m_pManager->RemovePostPaint(this);
					NeedParentUpdate();
					return;
				}
			}
			if (event.Type == UIEVENT_MOUSEMOVE)
			{
				if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
					// 根据方向计算偏移
					LONG delta = 0;
					if (m_bHorizontal) {
						delta = event.ptMouse.x - ptLastMouse.x;
						ptLastMouse = event.ptMouse;
						RECT rc = m_rcNewPos;
						if (m_iSepSize >= 0) {
							if (delta > 0 && event.ptMouse.x < m_rcNewPos.right - m_iSepSize) return;
							if (delta < 0 && event.ptMouse.x > m_rcNewPos.right) return;
							rc.right += delta;
							if (rc.right - rc.left <= GetMinWidth()) {
								if (m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth()) return;
								rc.right = rc.left + GetMinWidth();
							}
							if (rc.right - rc.left >= GetMaxWidth()) {
								if (m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth()) return;
								rc.right = rc.left + GetMaxWidth();
							}
						}
						else {
							if (delta > 0 && event.ptMouse.x < m_rcNewPos.left) return;
							if (delta < 0 && event.ptMouse.x > m_rcNewPos.left - m_iSepSize) return;
							rc.left += delta;
							if (rc.right - rc.left <= GetMinWidth()) {
								if (m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth()) return;
								rc.left = rc.right - GetMinWidth();
							}
							if (rc.right - rc.left >= GetMaxWidth()) {
								if (m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth()) return;
								rc.left = rc.right - GetMaxWidth();
							}
						}
						CDuiRect rcInvalidate = GetThumbRect(true);
						m_rcNewPos = rc;
						m_cxyFixed.cx = (m_pManager != NULL) ? m_pManager->GetDPIObj()->Scale(m_rcNewPos.right - m_rcNewPos.left) : m_rcNewPos.right - m_rcNewPos.left;
						if (m_bImmMode) {
							m_rcItem = m_rcNewPos;
							NeedParentUpdate();
						}
						else {
							rcInvalidate.Join(GetThumbRect(true));
							rcInvalidate.Join(GetThumbRect(false));
							if (m_pManager) m_pManager->Invalidate(rcInvalidate);
						}
					}
					else { // 垂直方向
						delta = event.ptMouse.y - ptLastMouse.y;
						ptLastMouse = event.ptMouse;
						RECT rc = m_rcNewPos;
						if (m_iSepSize >= 0) {
							if (delta > 0 && event.ptMouse.y < m_rcNewPos.bottom + m_iSepSize) return;
							if (delta < 0 && event.ptMouse.y > m_rcNewPos.bottom) return;
							rc.bottom += delta;
							if (rc.bottom - rc.top <= GetMinHeight()) {
								if (m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight()) return;
								rc.bottom = rc.top + GetMinHeight();
							}
							if (rc.bottom - rc.top >= GetMaxHeight()) {
								if (m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight()) return;
								rc.bottom = rc.top + GetMaxHeight();
							}
						}
						else {
							if (delta > 0 && event.ptMouse.y < m_rcNewPos.top) return;
							if (delta < 0 && event.ptMouse.y > m_rcNewPos.top + m_iSepSize) return;
							rc.top += delta;
							if (rc.bottom - rc.top <= GetMinHeight()) {
								if (m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight()) return;
								rc.top = rc.bottom - GetMinHeight();
							}
							if (rc.bottom - rc.top >= GetMaxHeight()) {
								if (m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight()) return;
								rc.top = rc.bottom - GetMaxHeight();
							}
						}
						CDuiRect rcInvalidate = GetThumbRect(true);
						m_rcNewPos = rc;
						m_cxyFixed.cy = (m_pManager != NULL) ? m_pManager->GetDPIObj()->Scale(m_rcNewPos.bottom - m_rcNewPos.top) : m_rcNewPos.bottom - m_rcNewPos.top;
						if (m_bImmMode) {
							m_rcItem = m_rcNewPos;
							NeedParentUpdate();
						}
						else {
							rcInvalidate.Join(GetThumbRect(true));
							rcInvalidate.Join(GetThumbRect(false));
							if (m_pManager) m_pManager->Invalidate(rcInvalidate);
						}
					}
					return;
				}
			}
			if (event.Type == UIEVENT_SETCURSOR)
			{
				RECT rcSeparator = GetThumbRect(false);
				if (IsEnabled() && ::PtInRect(&rcSeparator, event.ptMouse)) {
					::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(m_bHorizontal ? IDC_SIZEWE : IDC_SIZENS)));
					return;
				}
			}
		}
		CContainerUI::DoEvent(event);
	}

	void CLinearLayoutUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		rc = m_rcItem;

		RECT rcInset = GetInset();
		rc.left += rcInset.left;
		rc.top += rcInset.top;
		rc.right -= rcInset.right;
		rc.bottom -= rcInset.bottom;
		if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
		if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

		if (m_items.GetSize() == 0) {
			ProcessScrollBar(rc, 0, 0);
			return;
		}

		int iChildPadding = GetChildPadding();
		SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
		// 与原HorizontalLayout/VerticalLayout保持一致：水平布局只补竖向滚动条范围，垂直布局两者都补
		if (m_bHorizontal) {
			if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
				szAvailable.cy += m_pVerticalScrollBar->GetScrollRange();
		}
		else {
			if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
				szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();
			if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
				szAvailable.cy += m_pVerticalScrollBar->GetScrollRange();
		}

		// 如果水平布局，计算总宽度和可伸缩项；垂直布局计算总高度
		int nAdjustables = 0;
		int nEstimateNum = 0;
		int cyNeeded = 0;      // 垂直方向所需高度（水平布局中取最大，垂直布局中累加）
		int cxNeeded = 0;      // 水平方向所需宽度（水平布局中累加，垂直布局中取最大）
		int fixedNeeded = 0;   // 水平布局：固定宽度之和；垂直布局：固定高度之和

		SIZE szControlAvailable;
		int iControlMaxWidth = 0, iControlMaxHeight = 0;

		for (int it1 = 0; it1 < m_items.GetSize(); it1++) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
			if (!pControl->IsVisible()) continue;
			if (pControl->IsFloat()) continue;

			RECT rcPadding = pControl->GetPadding();
			szControlAvailable = szAvailable;
			if (m_bHorizontal) {
				szControlAvailable.cy -= rcPadding.top + rcPadding.bottom;
				iControlMaxWidth = pControl->GetFixedWidth() > 0 ? pControl->GetFixedWidth() : pControl->GetMaxWidth();
				iControlMaxHeight = pControl->GetFixedHeight() > 0 ? pControl->GetFixedHeight() : pControl->GetMaxHeight();
				if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
				if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;
				SIZE sz = pControl->EstimateSize(szControlAvailable);
				if (sz.cx == 0) nAdjustables++;
				else {
					if (sz.cx < pControl->GetMinWidth()) sz.cx = pControl->GetMinWidth();
					if (sz.cx > pControl->GetMaxWidth()) sz.cx = pControl->GetMaxWidth();
				}
				fixedNeeded += sz.cx + rcPadding.left + rcPadding.right;
				sz.cy = MAX(sz.cy, 0);
				if (sz.cy < pControl->GetMinHeight()) sz.cy = pControl->GetMinHeight();
				if (sz.cy > pControl->GetMaxHeight()) sz.cy = pControl->GetMaxHeight();
				cyNeeded = MAX(cyNeeded, sz.cy + rcPadding.top + rcPadding.bottom);
			}
			else { // 垂直布局
				szControlAvailable.cx -= rcPadding.left + rcPadding.right;
				iControlMaxWidth = pControl->GetFixedWidth() > 0 ? pControl->GetFixedWidth() : pControl->GetMaxWidth();
				iControlMaxHeight = pControl->GetFixedHeight() > 0 ? pControl->GetFixedHeight() : pControl->GetMaxHeight();
				if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
				if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;
				SIZE sz = pControl->EstimateSize(szControlAvailable);
				if (sz.cy == 0) nAdjustables++;
				else {
					if (sz.cy < pControl->GetMinHeight()) sz.cy = pControl->GetMinHeight();
					if (sz.cy > pControl->GetMaxHeight()) sz.cy = pControl->GetMaxHeight();
				}
				fixedNeeded += sz.cy + rcPadding.top + rcPadding.bottom;
				sz.cx = MAX(sz.cx, 0);
				if (sz.cx < pControl->GetMinWidth()) sz.cx = pControl->GetMinWidth();
				if (sz.cx > pControl->GetMaxWidth()) sz.cx = pControl->GetMaxWidth();
				cxNeeded = MAX(cxNeeded, sz.cx + rcPadding.left + rcPadding.right);
			}
			nEstimateNum++;
		}

		int needed = 0;
		if (m_bHorizontal) {
			fixedNeeded += (nEstimateNum - 1) * iChildPadding;
			needed = fixedNeeded;
		}
		else {
			fixedNeeded += (nEstimateNum - 1) * iChildPadding;
			needed = fixedNeeded;
		}

		// 计算扩展大小
		int expand = 0;
		if (nAdjustables > 0) {
			if (m_bHorizontal)
				expand = MAX(0, (szAvailable.cx - fixedNeeded) / nAdjustables);
			else
				expand = MAX(0, (szAvailable.cy - fixedNeeded) / nAdjustables);
		}

		// 开始定位
		SIZE szRemaining = szAvailable;
		int iPos = 0;
		if (m_bHorizontal) {
			iPos = rc.left;
			if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
				iPos -= m_pHorizontalScrollBar->GetScrollPos();
			else {
				// 水平对齐（当无可伸缩项时）
				if (nAdjustables <= 0) {
					UINT iChildAlign = GetChildAlign();
					if (iChildAlign == DT_CENTER) iPos += (szAvailable.cx - fixedNeeded) / 2;
					else if (iChildAlign == DT_RIGHT) iPos += (szAvailable.cx - fixedNeeded);
				}
			}
		}
		else {
			iPos = rc.top;
			if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
				iPos -= m_pVerticalScrollBar->GetScrollPos();
			else {
				if (nAdjustables <= 0) {
					UINT iChildVAlign = GetChildVAlign();
					if (iChildVAlign == DT_VCENTER) iPos += (szAvailable.cy - fixedNeeded) / 2;
					else if (iChildVAlign == DT_BOTTOM) iPos += (szAvailable.cy - fixedNeeded);
				}
			}
		}

		int iAdjustable = 0;
		int iEstimate = 0;
		int fixedRemaining = fixedNeeded;

		for (int it2 = 0; it2 < m_items.GetSize(); it2++) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
			if (!pControl->IsVisible()) continue;
			if (pControl->IsFloat()) {
				SetFloatPos(it2);
				continue;
			}

			iEstimate += 1;
			RECT rcPadding = pControl->GetPadding();
			SIZE sz = { 0, 0 };

			if (m_bHorizontal) {
				szRemaining.cx -= rcPadding.left;
				szControlAvailable = szRemaining;
				szControlAvailable.cy -= rcPadding.top + rcPadding.bottom;
				iControlMaxWidth = pControl->GetFixedWidth() > 0 ? pControl->GetFixedWidth() : pControl->GetMaxWidth();
				iControlMaxHeight = pControl->GetFixedHeight() > 0 ? pControl->GetFixedHeight() : pControl->GetMaxHeight();
				if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
				if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;

				fixedRemaining -= rcPadding.left + rcPadding.right;
				if (iEstimate > 1) fixedRemaining -= iChildPadding;

				SIZE szEst = pControl->EstimateSize(szControlAvailable);
				if (szEst.cx == 0) {
					iAdjustable++;
					sz.cx = expand;
					if (iAdjustable == nAdjustables) {
						sz.cx = MAX(0, szRemaining.cx - rcPadding.right - fixedRemaining);
					}
					if (sz.cx < pControl->GetMinWidth()) sz.cx = pControl->GetMinWidth();
					if (sz.cx > pControl->GetMaxWidth()) sz.cx = pControl->GetMaxWidth();
				}
				else {
					sz.cx = szEst.cx;
					if (sz.cx < pControl->GetMinWidth()) sz.cx = pControl->GetMinWidth();
					if (sz.cx > pControl->GetMaxWidth()) sz.cx = pControl->GetMaxWidth();
					fixedRemaining -= sz.cx;
				}

				sz.cy = pControl->GetMaxHeight();
				if (sz.cy == 0) sz.cy = szAvailable.cy - rcPadding.top - rcPadding.bottom;
				if (sz.cy < 0) sz.cy = 0;
				if (sz.cy > szControlAvailable.cy) sz.cy = szControlAvailable.cy;
				if (sz.cy < pControl->GetMinHeight()) sz.cy = pControl->GetMinHeight();

				// 垂直对齐
				int iPosY = 0;
				UINT iChildVAlign = GetChildVAlign();
				if (iChildVAlign == DT_VCENTER) {
					iPosY = (rc.bottom + rc.top) / 2;
					if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) {
						iPosY += m_pVerticalScrollBar->GetScrollRange() / 2 - m_pVerticalScrollBar->GetScrollPos();
					}
					iPosY -= sz.cy / 2;
				}
				else if (iChildVAlign == DT_BOTTOM) {
					iPosY = rc.bottom;
					if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) {
						iPosY += m_pVerticalScrollBar->GetScrollRange() - m_pVerticalScrollBar->GetScrollPos();
					}
					iPosY -= rcPadding.bottom + sz.cy;
				}
				else {
					iPosY = rc.top;
					if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) {
						iPosY -= m_pVerticalScrollBar->GetScrollPos();
					}
					iPosY += rcPadding.top;
				}
				RECT rcCtrl = { iPos + rcPadding.left, iPosY, iPos + sz.cx + rcPadding.left, iPosY + sz.cy };
				pControl->SetPos(rcCtrl, false);

				iPos += sz.cx + iChildPadding + rcPadding.left + rcPadding.right;
				cxNeeded += sz.cx + rcPadding.left + rcPadding.right;
				szRemaining.cx -= sz.cx + iChildPadding + rcPadding.right;
			}
			else { // 垂直布局
				szRemaining.cy -= rcPadding.top;
				szControlAvailable = szRemaining;
				szControlAvailable.cx -= rcPadding.left + rcPadding.right;
				iControlMaxWidth = pControl->GetFixedWidth() > 0 ? pControl->GetFixedWidth() : pControl->GetMaxWidth();
				iControlMaxHeight = pControl->GetFixedHeight() > 0 ? pControl->GetFixedHeight() : pControl->GetMaxHeight();
				if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
				if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;

				fixedRemaining -= rcPadding.top + rcPadding.bottom;
				if (iEstimate > 1) fixedRemaining -= iChildPadding;

				SIZE szEst = pControl->EstimateSize(szControlAvailable);
				if (szEst.cy == 0) {
					iAdjustable++;
					sz.cy = expand;
					if (iAdjustable == nAdjustables) {
						sz.cy = MAX(0, szRemaining.cy - rcPadding.bottom - fixedRemaining);
					}
					if (sz.cy < pControl->GetMinHeight()) sz.cy = pControl->GetMinHeight();
					if (sz.cy > pControl->GetMaxHeight()) sz.cy = pControl->GetMaxHeight();
				}
				else {
					sz.cy = szEst.cy;
					if (sz.cy < pControl->GetMinHeight()) sz.cy = pControl->GetMinHeight();
					if (sz.cy > pControl->GetMaxHeight()) sz.cy = pControl->GetMaxHeight();
					fixedRemaining -= sz.cy;
				}

				// 与原VerticalLayout一致：宽度取估算结果(跨轴)
				sz.cx = MAX(szEst.cx, 0);
				if (sz.cx == 0) sz.cx = szAvailable.cx - rcPadding.left - rcPadding.right;
				if (sz.cx > szControlAvailable.cx) sz.cx = szControlAvailable.cx;
				if (sz.cx < pControl->GetMinWidth()) sz.cx = pControl->GetMinWidth();

				// 水平对齐
				int iPosX = 0;
				UINT iChildAlign = GetChildAlign();
				if (iChildAlign == DT_CENTER) {
					iPosX = (rc.right + rc.left) / 2;
					if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) {
						iPosX += m_pHorizontalScrollBar->GetScrollRange() / 2 - m_pHorizontalScrollBar->GetScrollPos();
					}
					iPosX -= sz.cx / 2;
				}
				else if (iChildAlign == DT_RIGHT) {
					iPosX = rc.right;
					if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) {
						iPosX += m_pHorizontalScrollBar->GetScrollRange() - m_pHorizontalScrollBar->GetScrollPos();
					}
					iPosX -= rcPadding.right + sz.cx;
				}
				else {
					iPosX = rc.left;
					if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) {
						iPosX -= m_pHorizontalScrollBar->GetScrollPos();
					}
					iPosX += rcPadding.left;
				}
				RECT rcCtrl = { iPosX, iPos + rcPadding.top, iPosX + sz.cx, iPos + sz.cy + rcPadding.top };
				pControl->SetPos(rcCtrl, false);

				iPos += sz.cy + iChildPadding + rcPadding.top + rcPadding.bottom;
				cyNeeded += sz.cy + rcPadding.top + rcPadding.bottom;
				szRemaining.cy -= sz.cy + iChildPadding + rcPadding.bottom;
			}
		}

		// 计算总需求
		if (m_bHorizontal) {
			cxNeeded += (nEstimateNum - 1) * iChildPadding;
		}
		else {
			cyNeeded += (nEstimateNum - 1) * iChildPadding;
		}

		ProcessScrollBar(rc, cxNeeded, cyNeeded);
	}

	void CLinearLayoutUI::DoPostPaint(HDC hDC, const RECT& rcPaint)
	{
		if ((m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode) {
			RECT rcSeparator = GetThumbRect(true);
			CRenderEngine::DrawColor(hDC, rcSeparator, 0xAA000000);
		}
	}

	RECT CLinearLayoutUI::GetThumbRect(bool bUseNew) const
	{
		RECT rc;
		if ((m_uButtonState & UISTATE_CAPTURED) != 0 && bUseNew) {
			rc = m_rcNewPos;
		}
		else {
			rc = m_rcItem;
		}

		if (m_bHorizontal) {
			if (m_iSepSize >= 0)
				return CDuiRect(rc.right - m_iSepSize, rc.top, rc.right, rc.bottom);
			else
				return CDuiRect(rc.left, rc.top, rc.left - m_iSepSize, rc.bottom);
		}
		else {
			if (m_iSepSize >= 0)
				return CDuiRect(rc.left, MAX(rc.bottom - m_iSepSize, rc.top), rc.right, rc.bottom);
			else
				return CDuiRect(rc.left, rc.top, rc.right, MIN(rc.top - m_iSepSize, rc.bottom));
		}
	}
} // namespace DuiLib
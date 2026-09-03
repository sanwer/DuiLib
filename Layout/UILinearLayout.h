#ifndef __UILINEARLAYOUT_H__
#define __UILINEARLAYOUT_H__
#pragma once

namespace DuiLib
{
	class UILIB_API CLinearLayoutUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CLinearLayoutUI)
	public:
		CLinearLayoutUI();

		virtual LPCTSTR GetClass() const;
		virtual LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetHorizontal(bool bHorizontal); // true=水平，false=垂直
		bool IsHorizontal() const;

		void SetSepSize(int iSize);           // 分隔条宽度/高度
		int GetSepSize() const;
		void SetSepImmMode(bool bImmediately);
		bool IsSepImmMode() const;

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoEvent(TEventUI& event);

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void DoPostPaint(HDC hDC, const RECT& rcPaint);

		// 获取分隔条矩形（用于绘制和命中测试）
		RECT GetThumbRect(bool bUseNew = false) const;

	protected:
		bool m_bHorizontal;                   // true=水平，false=垂直
		int m_iSepSize;                       // 分隔条宽度/高度
		UINT m_uButtonState;
		POINT ptLastMouse;
		RECT m_rcNewPos;
		bool m_bImmMode;                      // 是否即时更新
	};
} // namespace DuiLib

#endif // __UILINEARLAYOUT_H__
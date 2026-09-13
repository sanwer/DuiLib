#ifndef __UITILELAYOUT_H__
#define __UITILELAYOUT_H__

#pragma once

namespace DuiLib
{
	class DUILIB_API CTileLayoutUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CTileLayoutUI)
	public:
		CTileLayoutUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetPos(RECT rc, bool bNeedInvalidate = true);

		SIZE GetItemSize() const;
		void SetItemSize(SIZE szItem);
		int GetColumns() const;
		void SetColumns(int nCols);

		// Extra column spacing offset (added to childpadding, >= 0)
		int GetChildHPadding() const;
		void SetChildHPadding(int iPadding);

		// Extra row spacing offset (added to childpadding, >= 0)
		int GetChildVPadding() const;
		void SetChildVPadding(int iPadding);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	protected:
		SIZE m_szItem;
		int m_nColumns;
		int m_iChildHPadding;
		int m_iChildVPadding;
	};
}
#endif // __UITILELAYOUT_H__

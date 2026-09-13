#ifndef __UIIPADDRESS_H__
#define __UIIPADDRESS_H__

#pragma once

//Add attribute dtstyle to this control

namespace DuiLib
{
	class CIPAddressWnd;

	// Date/time picker control
	class DUILIB_API CIPAddressUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CIPAddressUI)

		friend class CIPAddressWnd;
	public:
		CIPAddressUI();
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		DWORD GetIP();
		void SetIP(DWORD dwIP);

		void SetReadOnly(bool bReadOnly);
		bool IsReadOnly() const;

		void UpdateText();

		void DoEvent(TEventUI& event);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	protected:
		DWORD m_dwIP;
		bool m_bReadOnly;
		int m_nIPUpdateFlag;

		CIPAddressWnd* m_pWindow;
	};
}
#endif // __UIIPADDRESS_H__

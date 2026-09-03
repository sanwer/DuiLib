#include "StdAfx.h"
#include "ControlFactory.h"

namespace DuiLib
{
	CControlFactory::CControlFactory()
	{
		INNER_REGISTER_DUICONTROL(CControlUI);
		INNER_REGISTER_DUICONTROL(CContainerUI);
		INNER_REGISTER_DUICONTROL(CButtonUI);
		INNER_REGISTER_DUICONTROL(CCheckBoxUI);
		INNER_REGISTER_DUICONTROL(CComboBoxUI);
		INNER_REGISTER_DUICONTROL(CComboUI);
		INNER_REGISTER_DUICONTROL(CDateTimeUI);
		INNER_REGISTER_DUICONTROL(CEditUI);
		INNER_REGISTER_DUICONTROL(CGroupBoxUI);
		INNER_REGISTER_DUICONTROL(CLabelUI);
		INNER_REGISTER_DUICONTROL(CListContainerElementUI);
		INNER_REGISTER_DUICONTROL(CListHeaderItemUI);
		INNER_REGISTER_DUICONTROL(CListHeaderUI);
		INNER_REGISTER_DUICONTROL(CListLabelElementUI);
		INNER_REGISTER_DUICONTROL(CListTextElementUI);
		INNER_REGISTER_DUICONTROL(CListUI);
		INNER_REGISTER_DUICONTROL(CMenuElementUI);
		INNER_REGISTER_DUICONTROL(CMenuUI);
		INNER_REGISTER_DUICONTROL(COptionUI);
		INNER_REGISTER_DUICONTROL(CTextUI);
		INNER_REGISTER_DUICONTROL(CChildLayoutUI);
		INNER_REGISTER_DUICONTROL(CHorizontalLayoutUI);
		INNER_REGISTER_DUICONTROL(CVerticalLayoutUI);
		INNER_REGISTER_DUICONTROL(CTabLayoutUI);
		INNER_REGISTER_DUICONTROL(CTileLayoutUI);

#ifdef DUIMOD_ACTIVEX
		INNER_REGISTER_DUICONTROL(CActiveXUI);
#endif
#ifdef DUIMOD_ANIMATIONTABLAYOUT
		INNER_REGISTER_DUICONTROL(CAnimationTabLayoutUI);
#endif
#ifdef DUIMOD_GIFANIM
		INNER_REGISTER_DUICONTROL(CGifAnimUI);
#endif
#ifdef DUIMOD_IPADDRESS
		INNER_REGISTER_DUICONTROL(CIPAddressUI);
#endif
#ifdef DUIMOD_IPADDRESSEX
		INNER_REGISTER_DUICONTROL(CIPAddressExUI);
#endif
#ifdef DUIMOD_PROGRESS
		INNER_REGISTER_DUICONTROL(CProgressUI);
#endif
#ifdef DUIMOD_RICHEDIT
		INNER_REGISTER_DUICONTROL(CRichEditUI);
#endif
		INNER_REGISTER_DUICONTROL(CScrollBarUI);
#ifdef DUIMOD_SLIDER
		INNER_REGISTER_DUICONTROL(CSliderUI);
#endif
#ifdef DUIMOD_TREEVIEW
		INNER_REGISTER_DUICONTROL(CTreeNodeUI);
		INNER_REGISTER_DUICONTROL(CTreeViewUI);
#endif
#ifdef DUIMOD_ROLLTEXT
		INNER_REGISTER_DUICONTROL(CRollTextUI);
#endif
#ifdef DUIMOD_COLORPALETTE
		INNER_REGISTER_DUICONTROL(CColorPaletteUI);
#endif
#ifdef DUIMOD_LISTEX
		INNER_REGISTER_DUICONTROL(CListExUI);
		INNER_REGISTER_DUICONTROL(CListContainerHeaderItemUI);
		INNER_REGISTER_DUICONTROL(CListTextExtElementUI);
#endif
#ifdef DUIMOD_HOTKEY
		INNER_REGISTER_DUICONTROL(CHotKeyUI);
#endif
#ifdef DUIMOD_FADEBUTTON
		INNER_REGISTER_DUICONTROL(CFadeButtonUI);
#endif
#ifdef DUIMOD_RING
		INNER_REGISTER_DUICONTROL(CRingUI);
#endif
#ifdef DUIMOD_LOADING
		INNER_REGISTER_DUICONTROL(CLoadingUI);
#endif
#ifdef DUIMOD_PAGECONTROL
		INNER_REGISTER_DUICONTROL(CPageControlUI);
#endif

		INNER_REGISTER_DUICONTROL_EX(DUI_CTR_BOX, CContainerUI);
		INNER_REGISTER_DUICONTROL_EX(DUI_CTR_CHILD_BOX, CChildLayoutUI);
		INNER_REGISTER_DUICONTROL_EX(DUI_CTR_HBOX, CHorizontalLayoutUI);
		INNER_REGISTER_DUICONTROL_EX(DUI_CTR_VBOX, CVerticalLayoutUI);
		INNER_REGISTER_DUICONTROL_EX(DUI_CTR_TAB_BOX, CTabLayoutUI);
		INNER_REGISTER_DUICONTROL_EX(DUI_CTR_TILE_BOX, CTileLayoutUI);
	}

	CControlFactory::~CControlFactory()
	{
	}

	CControlUI* CControlFactory::CreateControl(CDuiString strClassName)
	{
		strClassName.MakeLower();
		MAP_DUI_CTRATECLASS::iterator iter = m_mapControl.find(strClassName);
		if ( iter == m_mapControl.end()) {
			return NULL;
		}
		else {
			return (CControlUI*) (iter->second());
		}
	}

	void CControlFactory::RegistControl(CDuiString strClassName, CreateClass pFunc)
	{
		strClassName.MakeLower();
		m_mapControl.insert(MAP_DUI_CTRATECLASS::value_type(strClassName, pFunc));
	}

	CControlFactory* CControlFactory::GetInstance()
	{
		static CControlFactory* pInstance = new CControlFactory;
		return pInstance;
	}

	void CControlFactory::Release()
	{
		delete this;
	}
}

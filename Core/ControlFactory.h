#ifndef __CONTROL_FACTORY_H__
#define __CONTROL_FACTORY_H__
#pragma once

#include <map>
namespace DuiLib
{
	typedef CControlUI* (*CreateClass)();

	//Key normalization: GetClass() accepts tag ("Button") or class name ("ButtonUI") style, unified as "CButtonUI"
	inline CDuiString MakeControlKey(LPCTSTR pstrClassName)
	{
		CDuiString sKey;
		CDuiString sName = pstrClassName;
		if (sName.GetLength() > 2 && sName.Right(2) == _T("UI"))
			sKey.Format(_T("C%s"), sName.GetData());
		else
			sKey.Format(_T("C%sUI"), sName.GetData());
		return sKey;
	}

	struct LessNoCaseCDuiString
	{
		bool operator()(const CDuiString& _Left, const CDuiString& _Right) const
		{
			return _tcsicmp(_Left.GetData(), _Right.GetData()) < 0;
		}
	};

	typedef std::map<CDuiString, CreateClass, LessNoCaseCDuiString> MAP_DUI_CTRATECLASS;

	class DUILIB_API CControlFactory
	{
	public:
		CControlUI* CreateControl(LPCTSTR pstrClassName);
		CControlUI* CreateControl(CDuiString strClassName);
		void RegistControl(CDuiString strClassName, CreateClass pFunc);

		static CControlFactory* GetInstance();
		void Release();

	private:
		CControlFactory();
		virtual ~CControlFactory();

	private:
		MAP_DUI_CTRATECLASS m_mapControl;
	};

#define DECLARE_DUICONTROL(class_name)\
public:\
	static CControlUI* CreateControl();

#define IMPLEMENT_DUICONTROL(class_name)\
	CControlUI* class_name::CreateControl()\
	{\
		return new class_name;\
	}

#define REGIST_DUICONTROL(class_name)\
	CControlFactory::GetInstance()->RegistControl(_T(#class_name), (CreateClass)class_name::CreateControl);

#define INNER_REGISTER_DUICONTROL(class_name)\
{\
	class_name _chkIns;\
	CDuiString sRegClassName = MakeControlKey(_chkIns.GetClass());\
	ASSERT(_tcscmp(sRegClassName, _T(#class_name)) == 0);\
	RegistControl(sRegClassName, (CreateClass)class_name::CreateControl);\
}


#define INNER_REGISTER_DUICONTROL_EX(str_class_name,class_type)\
	{\
		CDuiString sRegClassName;\
		sRegClassName.Format(_T("C%sUI"), str_class_name); \
		RegistControl(sRegClassName, (CreateClass)class_type::CreateControl);\
	}
}

#endif //__CONTROL_FACTORY_H__

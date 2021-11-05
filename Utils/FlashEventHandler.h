/*
	�������ڣ�	2012/11/05 15:09:48
	���ߣ�			daviyang35@gmail.com
	������	FlashEventHandler
*/
#ifndef __FLASH_EVENT_HANDLER_H__
#define __FLASH_EVENT_HANDLER_H__
#pragma once
//#include <ExDisp.h>

namespace DuiLib
{
	class CFlashEventHandler
	{
	public:
		CFlashEventHandler() {}
		~CFlashEventHandler() {}

		virtual ULONG STDMETHODCALLTYPE Release( void ) { return S_OK;}
		virtual ULONG STDMETHODCALLTYPE AddRef( void ) { return S_OK;}

		virtual HRESULT OnReadyStateChange ( long newState )
		{
			return S_OK;
		}

		virtual HRESULT OnProgress (long percentDone )
		{
			return S_OK;
		}

		virtual HRESULT FSCommand ( LPCTSTR command, LPCTSTR args )
		{
			return S_OK;
		}
		
		virtual HRESULT FlashCall ( LPCTSTR request )
		{
			return S_OK;
		}
	};
}

#endif // __FLASH_EVENT_HANDLER_H__

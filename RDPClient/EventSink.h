#ifndef _EVENTSINK_H_
#define _EVENTSINK_H_
#include <rdpencomapi.h>

#define override

typedef void(*typeOnConnectionFailed)();
typedef void(*typeOnConnectionEstablished)();


class EventSink : public _IRDPSessionEvents {
private:
	typeOnConnectionFailed OnConnectionFailed = NULL;
	typeOnConnectionEstablished OnConnectionEstablished = NULL;

public:
	EventSink() {
	}
	~EventSink() {
	}

	// IUnknown
	virtual HRESULT STDMETHODCALLTYPE override QueryInterface(
		REFIID iid, void**ppvObject) {
		*ppvObject = 0;
		if (iid == IID_IUnknown || iid == IID_IDispatch || iid == __uuidof(_IRDPSessionEvents))
			*ppvObject = this;
		if (*ppvObject)
		{
			((IUnknown*)(*ppvObject))->AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	virtual ULONG STDMETHODCALLTYPE override AddRef(void) {
		return 0;
	}

	virtual ULONG STDMETHODCALLTYPE override Release(void) {
		return 0;
	}


	// IDispatch
	virtual HRESULT STDMETHODCALLTYPE override GetTypeInfoCount(
		__RPC__out UINT *pctinfo) {
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE override GetTypeInfo(
		UINT iTInfo,
		LCID lcid,
		__RPC__deref_out_opt ITypeInfo **ppTInfo) {
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE override GetIDsOfNames(
		__RPC__in REFIID riid,
		__RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
		UINT cNames,
		LCID lcid,
		__RPC__out_ecount_full(cNames) DISPID *rgDispId) {
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE override EventSink::Invoke(
		DISPID dispIdMember,
		REFIID riid,
		LCID lcid,
		WORD wFlags,
		DISPPARAMS FAR* pDispParams,
		VARIANT FAR* pVarResult,
		EXCEPINFO FAR* pExcepInfo,
		unsigned int FAR* puArgErr) {

		switch (dispIdMember) {
		case DISPID_RDPSRAPI_EVENT_ON_VIEWER_CONNECTFAILED:
			if(OnConnectionFailed)
				OnConnectionFailed();
			break;
		case DISPID_RDPSRAPI_EVENT_ON_VIEWER_CONNECTED:
			if(OnConnectionEstablished)
				OnConnectionEstablished();
			break;
		}
		return S_OK;
	}

	void SetEventFunction(
		typeOnConnectionFailed f1, 
		typeOnConnectionEstablished f2)
	{
		OnConnectionFailed = f1;
		OnConnectionEstablished = f2;
	}
};


#endif //_EVENTSINK_H_
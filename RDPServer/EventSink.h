#ifndef _EVENTSINK_H_
#define _EVENTSINK_H_
#include <RdpEncomAPI.h>

#define override

/*Event function prototypes*/
typedef void (*typeOnAttendeeConnected)(IDispatch *pAttendee);
typedef void (*typeOnAttendeeDisconnected)(IDispatch *pAttendee);
typedef void (*typeOnControlLevelChangeRequest)(IDispatch  *pAttendee, CTRL_LEVEL RequestedLevel);


class EventSink : public _IRDPSessionEvents
{
private:
	typeOnAttendeeConnected OnAttendeeConnected = NULL;
	typeOnAttendeeDisconnected OnAttendeeDisconnected = NULL;
	typeOnControlLevelChangeRequest OnControlLevelChangeRequest = NULL;

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
		case DISPID_RDPSRAPI_EVENT_ON_ATTENDEE_CONNECTED:
			if(OnAttendeeConnected)
				OnAttendeeConnected(pDispParams->rgvarg[0].pdispVal);
			break;
		case DISPID_RDPSRAPI_EVENT_ON_ATTENDEE_DISCONNECTED:
			if(OnAttendeeDisconnected)
				OnAttendeeDisconnected(pDispParams->rgvarg[0].pdispVal);
			break;
		case DISPID_RDPSRAPI_EVENT_ON_CTRLLEVEL_CHANGE_REQUEST:
			if(OnControlLevelChangeRequest)
				OnControlLevelChangeRequest(pDispParams->rgvarg[1].pdispVal, (CTRL_LEVEL)pDispParams->rgvarg[0].intVal);
			break;
		}
		return S_OK;
	}

	void SetEventFunction(
		typeOnAttendeeConnected f1, 
		typeOnAttendeeDisconnected f2, 
		typeOnControlLevelChangeRequest f3)
	{
		OnAttendeeConnected = f1;
		OnAttendeeDisconnected = f2;
		OnControlLevelChangeRequest = f3;
	}
};


#endif //_EVENTSINK_H_
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include "EventSink.h"

#define MAX_ATTENDEE 1

IRDPSRAPISharingSession *session = NULL;
IRDPSRAPIInvitationManager *invitationManager = NULL;
IRDPSRAPIInvitation *invitation = NULL;
IRDPSRAPIAttendeeManager *attendeeManager = NULL;
IRDPSRAPIAttendee *attendee = NULL;

IConnectionPointContainer* picpc = NULL;
IConnectionPoint* picp = NULL;
EventSink ev;


int ConnectEvent(IUnknown* Container, REFIID riid, IUnknown* Advisor, IConnectionPointContainer** picpc, IConnectionPoint** picp)
{
	HRESULT hr = 0;
	unsigned long tid = 0;
	IConnectionPointContainer* icpc = 0;
	IConnectionPoint* icp = 0;
	*picpc = 0;
	*picp = 0;
	Container->QueryInterface(IID_IConnectionPointContainer, (void **)&icpc);
	if (icpc)
	{
		*picpc = icpc;
		icpc->FindConnectionPoint(riid, &icp);
		if (icp)
		{
			*picp = icp;
			hr = icp->Advise(Advisor, &tid);
		}
	}
	return tid;
}

void Disconnect()
{
	if (session)
	{
		session->Close();
		session->Release();
		session = NULL;
	}
}

void OnAttendeeConnected(IDispatch *pAttendee)
{
	BSTR remoteName;
	IRDPSRAPIAttendee *pRDPAtendee;
	pAttendee->QueryInterface(__uuidof(IRDPSRAPIAttendee), (void**)&pRDPAtendee);
	pRDPAtendee->put_ControlLevel(CTRL_LEVEL::CTRL_LEVEL_VIEW);
	if(pRDPAtendee->get_RemoteName(&remoteName) == S_OK);
		printf("%S Connected.\n", remoteName);
}

void OnAttendeeDisconnected(IDispatch *pAttendee)
{
	BSTR remoteName;
	IRDPSRAPIAttendee *pRDPAtendee;
	IRDPSRAPIAttendeeDisconnectInfo *info;
	ATTENDEE_DISCONNECT_REASON reason;
	pAttendee->QueryInterface(__uuidof(IRDPSRAPIAttendeeDisconnectInfo), (void**)&info);
	if (info->get_Reason(&reason) == S_OK)
	{
		switch (reason)
		{
		case ATTENDEE_DISCONNECT_REASON_APP:
			break;
		case ATTENDEE_DISCONNECT_REASON_ERR:
			break;
		case ATTENDEE_DISCONNECT_REASON_CLI:
			break;
		default:
			break;
		}
	}
	if (info->get_Attendee(&pRDPAtendee) == S_OK)
	{
		if (pRDPAtendee->get_RemoteName(&remoteName) == S_OK);
			printf("%S Disconnected.\n", remoteName);
	}
	pAttendee->Release();
	picp = 0;
	picpc = 0;
	//Disconnect();
}

void OnControlLevelChangeRequest(IDispatch  *pAttendee, CTRL_LEVEL RequestedLevel)
{
	IRDPSRAPIAttendee *pRDPAtendee;
	pAttendee->QueryInterface(__uuidof(IRDPSRAPIAttendee), (void**)&pRDPAtendee);
	if (pRDPAtendee->put_ControlLevel(RequestedLevel) == S_OK)
	{
		switch (RequestedLevel)
		{
		case CTRL_LEVEL_NONE:
			printf("Control set to CTRL_LEVEL_NONE.\n");
			break;
		case CTRL_LEVEL_VIEW:
			printf("Control set to CTRL_LEVEL_VIEW.\n");
			break;
		case CTRL_LEVEL_INTERACTIVE:
			printf("Control set to CTRL_LEVEL_INTERACTIVE.\n");
			break;
		}
	}
}

void StartServer()
{
	TCHAR hostname[260];
	DWORD dwSize = 260;
	GetComputerName(hostname, &dwSize);
	lstrcat(hostname, _T(".xml"));

	CoInitialize(NULL);

	if (CoCreateInstance(__uuidof(RDPSession), NULL, CLSCTX_INPROC_SERVER, __uuidof(IRDPSRAPISharingSession), (void**)&session) != S_OK)
	{
		printf("CoCreateInstance failed with err=%d.\n", GetLastError());
		return;
	}

	ConnectEvent((IUnknown*)session, __uuidof(_IRDPSessionEvents), (IUnknown*)&ev, &picpc, &picp);

	if (session->Open() != S_OK)
	{
		printf("session->Open failed with err=%d.\n", GetLastError());
		return;
	}

	if (session->get_Invitations(&invitationManager) != S_OK)
	{
		printf("session->get_Invitations failed with err=%d.\n", GetLastError());
		return;
	}

	if (invitationManager->CreateInvitation(L"WinPresenter", L"PresentationGroup", L"", MAX_ATTENDEE, &invitation) != S_OK)
	{
		printf("invitationManager->CreateInvitation failed with err=%d.\n", GetLastError());
		return;
	}

	ev.SetEventFunction(OnAttendeeConnected, OnAttendeeDisconnected, OnControlLevelChangeRequest);

	FILE *invite = _tfopen(hostname, _T("w"));
	if (invite)
	{
		BSTR inviteString;
		if (invitation->get_ConnectionString(&inviteString) == S_OK)
		{
			fwprintf_s(invite, L"%ws", inviteString);
			SysFreeString(inviteString);
		}
		fclose(invite);
	}

	if (session->get_Attendees(&attendeeManager) == S_OK)
	{
		printf("Start ok.\n");
		printf("Watting for attendees.\n");
	}
}

int _tmain(int argc, TCHAR* argv[])
{
	MSG Msg;

	StartServer();

	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return 1;
}
#pragma once
#include <afxwin.h>
#include <afxsock.h>

class CRobotControl;

class RSocket : public CSocket
{
public:
	RSocket(){}
	virtual ~RSocket(){}
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	CRobotControl *par;
};
#include "stdafx.h"
#include "RSocket.h"
#include "RobotControl.h"

void RSocket::OnClose(int nErrorCode){ par->OnClose(); }
void RSocket::OnReceive(int nErrorCode){ par->OnReceive(); }
void RSocket::OnAccept(int nErrorCode){ par->OnAccept(); }

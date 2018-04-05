#include "stdafx.h"
#include "RobotControl.h"
#include "RobotControlDlg.h"

void RSocket::OnClose(int nErrorCode){ 
	if (nErrorCode == 0){
		par->OnClose();
	}
}

void RSocket::OnReceive(int nErrorCode){ 
	if (nErrorCode == 0){
		par->OnReceive();
	}
}

void RSocket::OnAccept(int nErrorCode){ 
	if (nErrorCode == 0){
		par->OnAccept();
	}
}

CRobotControl::CRobotControl()
	: nPort(6006)
	, A1(0)
	, A2(-90)
	, A3(+90)
	, A4(0)
	, A5(0)
	, A6(0)
	, RecvMode(0)
	, A1Recv(0)
	, A2Recv(0)
	, A3Recv(0)
	, A4Recv(0)
	, A5Recv(0)
	, A6Recv(0)
	, XBase(0)
	, YBase(0)
	, ZBase(0)
	, ABase(0)
	, BBase(0)
	, CBase(0)
	, VelBase(0.1)
	, AccBase(0.5)
	, SBase(6)
	, TBase(50)
	, axis(AXIS)
	, pDlg(NULL)
{
	if (!AfxSocketInit()){
		AfxMessageBox(L"Socket Init Error!");
	}
}


CRobotControl::~CRobotControl()
{
	pDlg = NULL;
}

void CRobotControl::Create(CRobotControlDlg* p){
	RobotCommond[60] = '/';
	RobotCommond[61] = '%';
	RobotCommond[62] = '/';
	RobotCommond[63] = '%';

	SendError = false;
	IsLinked = false;

	m_ServerSocket.par = this;
	m_ClientSocket.par = this;

	pDlg = p;
	if (pDlg){

	}
}

void CRobotControl::OnClose(){
	m_ClientSocket.Close();
}

bool CRobotControl::Link(){
	bool success = false;
	if (m_ServerSocket.Create(nPort)){
		//TRACE("创建套接字，绑定端口，绑IP成功\n");
		if (m_ServerSocket.Listen()){
			//TRACE("监听成功");
			success = true;
		}
	}
	return success;
}

void CRobotControl::LinkOff(){
	char GetCloseMsg[64] = { 0 };
	IsLinked = false;
	m_ClientSocket.Send(GetCloseMsg, COMMANDLENGTH);
	RecvMode = 0;
	OnClose();
	m_ServerSocket.Close();
}

void CRobotControl::OnAccept(){
	m_ServerSocket.Accept(m_ClientSocket); // 对m_ClientSocket进行初始化 以方便与客户端收发信息
}

void CRobotControl::OnReceive(){
	char buf[256] = { 0 };
	if (m_ClientSocket.Receive(buf, 256) > 0){
		if ((buf[0] == '%') && (buf[1] == '%') && (buf[2] == '%') && (buf[3] == '%') && 
			(buf[4] == '%') && (buf[5] == '%') && (buf[6] == '%') && (buf[7] == '%') &&
			(buf[8] == '%') && (buf[9] == '%')){
			SendError = true;
			return;
		}
		//char Recvs[256];
		switch (RecvMode)
		{
		case 0:
			break;
		case 1:
			memcpy_s(&PA1, 48, buf, 24);
			A1Recv = PA1.A1;
			A2Recv = PA1.A2;
			A3Recv = PA1.A3;
			A4Recv = PA1.A4;
			A5Recv = PA1.A5;
			A6Recv = PA1.A6;

			memcpy_s(&PP1, 24, buf + 24, 24);
			XRecv = PP1.X;
			YRecv = PP1.Y;
			ZRecv = PP1.Z;
			ARecv = PP1.A;
			BRecv = PP1.B;
			CRecv = PP1.C;

			IsLinked = true;
			pDlg->UiConnectedLock(IsLinked);
			DataToDlg();
			break;
		default:
			break;
		}
	}
}

void CRobotControl::SetAxis(int ax){
	axis = AXISSYS(ax);
}

void CRobotControl::SetPort(int port){
	nPort = port;
}

void CRobotControl::AddA1(){
	switch (axis)
	{
	case CRobotControl::AXIS:
		RobotCommond[0] = 11;//A1+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::BASE:
		RobotCommond[0] = 23;//A1+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::TOOL:
		RobotCommond[0] = 35;//A1+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	default:
		break;
	}
}

void CRobotControl::DecA1(){
	switch (axis)
	{
	case CRobotControl::AXIS:
		RobotCommond[0] = 12;//A1-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::BASE:
		RobotCommond[0] = 24;//A1-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::TOOL:
		RobotCommond[0] = 36;//A1-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	default:
		break;
	}

}

void CRobotControl::AddA2(){
	switch (axis)
	{
	case CRobotControl::AXIS:
		RobotCommond[0] = 13;//A2+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::BASE:
		RobotCommond[0] = 25;//A2+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::TOOL:
		RobotCommond[0] = 37;//A2+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	default:
		break;
	}

}

void CRobotControl::DecA2(){
	switch (axis)
	{
	case CRobotControl::AXIS:
		RobotCommond[0] = 14;//A2-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::BASE:
		RobotCommond[0] = 26;//A2-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::TOOL:
		RobotCommond[0] = 38;//A2-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	default:
		break;
	}

}

void CRobotControl::AddA3(){
	switch (axis)
	{
	case CRobotControl::AXIS:
		RobotCommond[0] = 15;//A3+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::BASE:
		RobotCommond[0] = 27;//A3+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::TOOL:
		RobotCommond[0] = 39;//A3+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	default:
		break;
	}

}

void CRobotControl::DecA3(){
	switch (axis)
	{
	case CRobotControl::AXIS:
		RobotCommond[0] = 16;//A3-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::BASE:
		RobotCommond[0] = 28;//A3-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::TOOL:
		RobotCommond[0] = 40;//A3-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	default:
		break;
	}

}

void CRobotControl::AddA4(){
	switch (axis)
	{
	case CRobotControl::AXIS:
		RobotCommond[0] = 17;//A4+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::BASE:
		RobotCommond[0] = 29;//A4+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::TOOL:
		RobotCommond[0] = 41;//A4+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	default:
		break;
	}

}

void CRobotControl::DecA4(){
	switch (axis)
	{
	case CRobotControl::AXIS:
		RobotCommond[0] = 18;//A4-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::BASE:
		RobotCommond[0] = 30;//A4-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::TOOL:
		RobotCommond[0] = 42;//A4-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	default:
		break;
	}

}

void CRobotControl::AddA5(){
	switch (axis)
	{
	case CRobotControl::AXIS:
		RobotCommond[0] = 19;//A5+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::BASE:
		RobotCommond[0] = 31;//A5+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::TOOL:
		RobotCommond[0] = 43;//A5+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	default:
		break;
	}

}

void CRobotControl::DecA5(){
	switch (axis)
	{
	case CRobotControl::AXIS:
		RobotCommond[0] = 20;//A5-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::BASE:
		RobotCommond[0] = 32;//A5-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::TOOL:
		RobotCommond[0] = 44;//A5-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	default:
		break;
	}

}

void CRobotControl::AddA6(){
	switch (axis)
	{
	case CRobotControl::AXIS:
		RobotCommond[0] = 21;//A6+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::BASE:
		RobotCommond[0] = 33;//A6+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::TOOL:
		RobotCommond[0] = 45;//A6+
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	default:
		break;
	}

}

void CRobotControl::DecA6(){
	switch (axis)
	{
	case CRobotControl::AXIS:
		RobotCommond[0] = 22;//A6-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::BASE:
		RobotCommond[0] = 34;//A6-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	case CRobotControl::TOOL:
		RobotCommond[0] = 46;//A6-
		RobotCommond[1] = 0;
		RobotCommond[2] = 0;
		RobotCommond[3] = 0;
		m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
		break;
	default:
		break;
	}

}

void CRobotControl::StopMove(){
	RecvMode = 1;
	RobotCommond[0] = 10;
	RobotCommond[1] = 0;
	RobotCommond[2] = 0;
	RobotCommond[3] = 0;
	m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
}

void CRobotControl::MoveToAxisMark(){
	DataFromDlg();

	float sendf[6];
	sendf[0] = A1;
	sendf[1] = A2;
	sendf[2] = A3;
	sendf[3] = A4;
	sendf[4] = A5;
	sendf[5] = A6;

	memcpy_s(RobotCommond + 4, 60, sendf, 24);
	RobotCommond[0] = 1;
	RobotCommond[1] = 0;
	RobotCommond[2] = 0;
	RobotCommond[3] = 0;
	RobotCommond[28] = 0;
	RobotCommond[29] = 0;
	RobotCommond[60] = '/';
	RobotCommond[61] = '%';
	RobotCommond[62] = '/';
	RobotCommond[63] = '%';

	RecvMode = 1;
	m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);

}

void CRobotControl::MoveToBaseMark(){
	DataFromDlg();

	float sendf[6];
	sendf[0] = VelBase;
	sendf[1] = AccBase;
	E6POS XP1 = {XBase, YBase, ZBase, ABase, BBase, CBase};

	memcpy_s(RobotCommond + 4, 60, &XP1, 24);
	memcpy_s(RobotCommond + 28, 36, sendf, 8);
	RobotCommond[0] = 3;
	RobotCommond[1] = 0;
	RobotCommond[2] = 0;
	RobotCommond[3] = 0;

	RecvMode = 1;
	m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
}

void CRobotControl::GetCurrentMark(){
	RecvMode = 1;
	RobotCommond[0] = 10;
	RobotCommond[1] = 0;
	RobotCommond[2] = 0;
	RobotCommond[3] = 0;
	RobotCommond[60] = '/';
	RobotCommond[61] = '%';
	RobotCommond[62] = '/';
	RobotCommond[63] = '%';
	m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
}

void CRobotControl::GetCurrentBaseMark(){
	RobotCommond[0] = 30;
	RobotCommond[1] = 0;
	RobotCommond[2] = 0;
	RobotCommond[3] = 0;
	RobotCommond[60] = '/';
	RobotCommond[61] = '%';
	RobotCommond[62] = '/';
	RobotCommond[63] = '%';
	m_ClientSocket.Send(RobotCommond, COMMANDLENGTH);
	RecvMode = 1;
}

void CRobotControl::ResetAxisMark(){
	A1 = 0;
	A2 = -90;
	A3 = 90;
	A4 = 0;
	A5 = 0;
	A6 = 0;
	if (pDlg){
		pDlg->m_A1axis = A1;
		pDlg->m_A2axis = A2;
		pDlg->m_A3axis = A3;
		pDlg->m_A4axis = A4;
		pDlg->m_A5axis = A5;
		pDlg->m_A6axis = A6;
		pDlg->UpdateData(false);
	}
}

void CRobotControl::ResetBaseMark(){
	/*ABase = 0;
	BBase = 0;
	CBase = 0;
	XBase = 0;
	YBase = 0;
	ZBase = 0;
	AccBase = 0.5;
	VelBase = 0.1;*/
	if (pDlg){
		ABase = pDlg->m_Astatus;
		BBase = pDlg->m_Bstatus;
		CBase = pDlg->m_Cstatus;
		XBase = pDlg->m_Xstatus;
		YBase = pDlg->m_Ystatus;
		ZBase = pDlg->m_Zstatus;
		AccBase = 0.5;
		VelBase = 0.1;

		pDlg->m_acc = AccBase;
		pDlg->m_vec = VelBase;
		pDlg->m_ABase = ABase;
		pDlg->m_BBase = BBase;
		pDlg->m_CBase = CBase;
		pDlg->m_XBase = XBase;
		pDlg->m_YBase = YBase;
		pDlg->m_ZBase = ZBase;
		pDlg->UpdateData(false);
	}
}

void CRobotControl::DataFromDlg(){
	if (pDlg){
		VelBase = pDlg->m_vec;
		AccBase = pDlg->m_acc;
		XBase = pDlg->m_XBase;
		YBase = pDlg->m_YBase;
		ZBase = pDlg->m_ZBase;
		ABase = pDlg->m_ABase;
		BBase = pDlg->m_BBase;
		CBase = pDlg->m_CBase;

		A1 = pDlg->m_A1axis;
		A2 = pDlg->m_A2axis;
		A3 = pDlg->m_A3axis;
		A4 = pDlg->m_A4axis;
		A5 = pDlg->m_A5axis;
		A6 = pDlg->m_A6axis;
	}
}

void CRobotControl::DataToDlg(){
	if (pDlg){
		CString cs;
		pDlg->m_A1status = A1Recv;
		cs.Format(L"%7.3f", A1Recv);
		pDlg->GetDlgItem(IDC_EDIT8)->SetWindowTextW(cs);
		pDlg->m_A2status = A2Recv;
		cs.Format(L"%7.3f", A2Recv);
		pDlg->GetDlgItem(IDC_EDIT9)->SetWindowTextW(cs);
		pDlg->m_A3status = A3Recv;
		cs.Format(L"%7.3f", A3Recv);
		pDlg->GetDlgItem(IDC_EDIT10)->SetWindowTextW(cs);
		pDlg->m_A4status = A4Recv;
		cs.Format(L"%7.3f", A4Recv);
		pDlg->GetDlgItem(IDC_EDIT11)->SetWindowTextW(cs);
		pDlg->m_A5status = A5Recv;
		cs.Format(L"%7.3f", A5Recv);
		pDlg->GetDlgItem(IDC_EDIT12)->SetWindowTextW(cs);
		pDlg->m_A6status = A6Recv;
		cs.Format(L"%7.3f", A6Recv);
		pDlg->GetDlgItem(IDC_EDIT13)->SetWindowTextW(cs);
		pDlg->m_Xstatus = XRecv;
		cs.Format(L"%7.3f", XRecv);
		pDlg->GetDlgItem(IDC_EDIT14)->SetWindowTextW(cs);
		pDlg->m_Ystatus = YRecv;
		cs.Format(L"%7.3f", YRecv);
		pDlg->GetDlgItem(IDC_EDIT15)->SetWindowTextW(cs);
		pDlg->m_Zstatus = ZRecv;
		cs.Format(L"%7.3f", ZRecv);
		pDlg->GetDlgItem(IDC_EDIT16)->SetWindowTextW(cs);
		pDlg->m_Astatus = ARecv;
		cs.Format(L"%7.3f", ARecv);
		pDlg->GetDlgItem(IDC_EDIT17)->SetWindowTextW(cs);
		pDlg->m_Bstatus = BRecv;
		cs.Format(L"%7.3f", BRecv);
		pDlg->GetDlgItem(IDC_EDIT18)->SetWindowTextW(cs);
		pDlg->m_Cstatus = CRecv;
		cs.Format(L"%7.3f", CRecv);
		pDlg->GetDlgItem(IDC_EDIT19)->SetWindowTextW(cs);
	}
}
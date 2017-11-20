
#include "stdafx.h"
#include "DlgVideo.h"
#include "afxdialogex.h"


// CDlgVideo �Ի���
int CALLBACK __EasyPlayerCallBack(EASY_CALLBACK_TYPE_ENUM callbackType, int channelId, void *userPtr, int mediaType, char *pbuf, EASY_FRAME_INFO *frameInfo);
IMPLEMENT_DYNAMIC(CDlgVideo, CDialogEx)

CDlgVideo::CDlgVideo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVideo::IDD, pParent)
{
	m_WindowId	=	-1;
	m_ChannelId	=	-1;
	sourceMultiplex	=	0;
	bDrag		=	false;

	m_BrushBtn = ::CreateSolidBrush(DIALOG_BASE_BACKGROUND_COLOR);
	m_BrushEdt = ::CreateSolidBrush(RGB(0xef,0xef,0xef));
	m_BrushStatic = ::CreateSolidBrush(DIALOG_BASE_BACKGROUND_COLOR);

	InitialComponents();
}

CDlgVideo::~CDlgVideo()
{
	DeleteObject(m_BrushBtn);
	DeleteObject(m_BrushEdt);
	DeleteObject(m_BrushStatic);
}

void CDlgVideo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgVideo, CDialogEx)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, &CDlgVideo::OnBnClickedButtonPreview)
	ON_BN_CLICKED(IDC_CHECK_OSD, &CDlgVideo::OnBnClickedCheckOsd)
	ON_WM_HSCROLL()
	ON_WM_RBUTTONUP()
	ON_MESSAGE(WM_RECORDING_CMPLETE, OnRecordingComplete)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgVideo ��Ϣ��������
LRESULT CDlgVideo::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_PAINT == message || WM_SIZE==message)
	{
		UpdateComponents();
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}



BOOL CDlgVideo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CreateComponents();

	SetBackgroundColor(RGB(0x7c,0x7c,0x7c));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CDlgVideo::DestroyWindow()
{
	DeleteComponents();

	return CDialogEx::DestroyWindow();
}



void CDlgVideo::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ

	HWND hWnd = ::GetParent(GetSafeHwnd());
	::PostMessageW(hWnd, WM_WINDOW_MAXIMIZED, m_WindowId, 0);

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CDlgVideo::OnLButtonDown(UINT nFlags, CPoint point)
{
	bDrag = true;

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgVideo::OnLButtonUp(UINT nFlags, CPoint point)
{
	bDrag = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgVideo::OnMouseMove(UINT nFlags, CPoint point)
{
	if (bDrag)
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		if (! rcClient.IsRectEmpty())
		{
			int nX = (int)(((float)point.x / (float)rcClient.Width() * 100.0f));
			int nY = (int)(((float)point.y / (float)rcClient.Height() * 100.0f));

			TRACE("X: %d\tY: %d\n", nX, nY);
		}
	}


	CDialogEx::OnMouseMove(nFlags, point);
}

void	CDlgVideo::SetWindowId(int _windowId)	
{
	m_WindowId = _windowId;

	//if (NULL != pDlgRender)	pDlgRender->SetChannelId(m_WindowId);

	if (m_WindowId == 0)
	{
		//if (NULL != pEdtURL)		pEdtURL->SetWindowText(TEXT("rtsp://121.15.129.227"));
		//if (NULL != pEdtURL)		pEdtURL->SetWindowText(TEXT("rtsp://192.168.1.100"));
	}
}
void	CDlgVideo::SetURL(char *url)
{
	wchar_t wszURL[128] = {0};
	MByteToWChar(url, wszURL, sizeof(wszURL)/sizeof(wszURL[0]));
	if (NULL != pEdtURL)		pEdtURL->SetWindowText(wszURL);
}

void	CDlgVideo::SetShownToScale(int shownToScale)
{
	if (m_ChannelId > 0)
	{
		//Player_SetShownToScale(m_ChannelId, shownToScale);
		libEasyPlayerPro_SetScaleDisplay(playerHandle, m_ChannelId, shownToScale, RGB(0x26,0x26,0x26));
	}
}
void	CDlgVideo::SetMultiplex(unsigned char multiplex)
{
	sourceMultiplex = multiplex;
}
void	CDlgVideo::InitialComponents()
{
	pDlgRender	=	NULL;
	pEdtURL		=	NULL;
	//pEdtUsername=	NULL;
	//pEdtPassword=	NULL;
	pChkOSD		=	NULL;
	pChkTCP		=	NULL;
	pSliderCache=	NULL;
	pBtnPreview	=	NULL;
}

void	CDlgVideo::CreateComponents()
{
	if (NULL == pDlgRender)
	{
		pDlgRender = new CDlgRender();
		pDlgRender->Create(IDD_DIALOG_RENDER, this);
		pDlgRender->ShowWindow(SW_SHOW);
	}

	__CREATE_WINDOW(pEdtURL,		CEdit,		IDC_EDIT_RTSP_URL);
	//__CREATE_WINDOW(pEdtUsername,	CEdit,		IDC_EDIT_USERNAME);
	//__CREATE_WINDOW(pEdtPassword,	CEdit,		IDC_EDIT_PASSWORD);
	__CREATE_WINDOW(pChkOSD,		CButton,	IDC_CHECK_OSD);
	__CREATE_WINDOW(pChkTCP,		CButton,	IDC_CHECK_TCP);
	__CREATE_WINDOW(pSliderCache,	CSliderCtrl,IDC_SLIDER_CACHE);
	__CREATE_WINDOW(pBtnPreview,	CButton,	IDC_BUTTON_PREVIEW);


	//if (NULL != pEdtUsername)	pEdtUsername->SetWindowText(TEXT("admin"));
	//if (NULL != pEdtPassword)	pEdtPassword->SetWindowText(TEXT("12345"));
	if (NULL != pSliderCache)	pSliderCache->SetRange(1, 20);
	if (NULL != pSliderCache)	pSliderCache->SetPos(3);
	if (NULL != pChkOSD)		pChkOSD->SetCheck(1);
	if (NULL != pChkTCP)		pChkTCP->SetCheck(1);

	if (NULL != pBtnPreview)		pBtnPreview->SetWindowText(TEXT("Play"));
}
void	CDlgVideo::UpdateComponents()
{
	CRect	rcClient;
	GetClientRect(&rcClient);
	if (rcClient.IsRectEmpty())		return;

	CRect	rcRender;
	rcRender.SetRect(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom-20);
	__MOVE_WINDOW(pDlgRender, rcRender);
	if (NULL != pDlgRender)		pDlgRender->Invalidate();

	CRect	rcURL;
	rcURL.SetRect(rcClient.left, rcRender.bottom+2, rcClient.right-200, rcClient.bottom);
	__MOVE_WINDOW(pEdtURL, rcURL);
	if (NULL != pEdtURL)		pEdtURL->Invalidate();

	CRect	rcOSD;
	rcOSD.SetRect(rcURL.right+2, rcURL.top, rcURL.right+2+45, rcURL.bottom);
	__MOVE_WINDOW(pChkOSD, rcOSD);
	if (NULL != pChkOSD)		pChkOSD->Invalidate();
	CRect	rcTCP;
	rcTCP.SetRect(rcOSD.right+2, rcOSD.top, rcOSD.right+2+45, rcOSD.bottom);
	__MOVE_WINDOW(pChkTCP, rcTCP);
	CRect	rcCache;
	rcCache.SetRect(rcTCP.right+2, rcTCP.top, rcTCP.right+2+60, rcTCP.bottom);
	__MOVE_WINDOW(pSliderCache, rcCache);
	if (NULL != pSliderCache)		pSliderCache->Invalidate();

	CRect	rcPreview;
	rcPreview.SetRect(rcCache.right+2, rcURL.top, rcClient.right-3, rcURL.bottom);
	__MOVE_WINDOW(pBtnPreview, rcPreview);
	if (NULL != pBtnPreview)		pBtnPreview->Invalidate();
}
void	CDlgVideo::DeleteComponents()
{
	if (m_ChannelId > 0)
	{
		libEasyPlayerPro_CloseStream(playerHandle, m_ChannelId);
		m_ChannelId = -1;
	}
	__DELETE_WINDOW(pDlgRender);
}

void CDlgVideo::OnBnClickedButtonPreview()
{
	if (NULL == playerHandle)
	{
		libEasyPlayerPro_Create(&playerHandle, 128);
	}

	if (m_ChannelId > 0)
	{
		//Player_CloseStream(m_ChannelId);
		libEasyPlayerPro_StopPlayStream(playerHandle, m_ChannelId);
		libEasyPlayerPro_CloseStream(playerHandle, m_ChannelId);
		m_ChannelId = -1;

		if (NULL != pDlgRender)
		{
			pDlgRender->SetChannelId(m_ChannelId);
			pDlgRender->ResetChannel();
		}

		if (NULL != pDlgRender)			pDlgRender->Invalidate();
		if (NULL != pBtnPreview)		pBtnPreview->SetWindowText(TEXT("Play"));
	}
	else
	{
		wchar_t wszURL[256] = {0};
		if (NULL != pEdtURL)	pEdtURL->GetWindowTextW(wszURL, sizeof(wszURL));
		if (wcslen(wszURL) < 1)		return;

		char szURL[256] = {0};
		WCharToMByte(wszURL, szURL, sizeof(szURL)/sizeof(szURL[0]));

/*
		wchar_t wszUsername[32] = {0};
		wchar_t wszPassword[32] = {0};
		if (NULL != pEdtUsername)	pEdtUsername->GetWindowText(wszUsername, sizeof(wszUsername));
		if (NULL != pEdtPassword)	pEdtPassword->GetWindowText(wszPassword, sizeof(wszPassword));


		char szUsername[32] = {0};
		char szPassword[32] = {0};
		WCharToMByte(wszUsername, szUsername, sizeof(szUsername)/sizeof(szUsername[0]));
		WCharToMByte(wszPassword, szPassword, sizeof(szPassword)/sizeof(szPassword[0]));
*/
		int	rtpOverTcp = 0x00;
		if (NULL != pChkTCP)		rtpOverTcp = pChkTCP->GetCheck();

		HWND hWnd = NULL;
		if (NULL != pDlgRender)	hWnd = pDlgRender->GetSafeHwnd();

		EASY_CHANNEL_SOURCE_TYPE_ENUM		sourceType = EASY_CHANNEL_SOURCE_TYPE_RTSP;
		if (0 == strncmp(szURL, "rtsp", 4))	sourceType = EASY_CHANNEL_SOURCE_TYPE_RTSP;
		else if (0 == strncmp(szURL, "rtmp", 4))	sourceType = EASY_CHANNEL_SOURCE_TYPE_RTMP;
		else if (0 == strncmp(szURL, "http", 4))	sourceType = EASY_CHANNEL_SOURCE_TYPE_HLS;
		else if (0 == strncmp(szURL, "file", 4))	sourceType = EASY_CHANNEL_SOURCE_TYPE_FILE;

		int queueSize = 1024 * 1024 * 2;		//2MB
		if (sourceType == EASY_CHANNEL_SOURCE_TYPE_HLS)		queueSize = 1024 * 1024 * 5;		//5MB

		m_ChannelId = libEasyPlayerPro_OpenStream(playerHandle, sourceType, szURL, 
												rtpOverTcp, 
												MEDIA_TYPE_VIDEO|MEDIA_TYPE_AUDIO|MEDIA_TYPE_EVENT,
											__EasyPlayerCallBack, this, 0x01, 0x00, queueSize, sourceMultiplex);
		

		if (m_ChannelId > 0)
		{
			//libEasyPlayerPro_StartPlayStream(playerHandle, m_ChannelId, hWnd, RenderFormat);
			libEasyPlayerPro_StartPlayStream(playerHandle, m_ChannelId, hWnd, RenderFormat);

			int iPos = pSliderCache->GetPos();
			libEasyPlayerPro_SetPlayFrameCache(playerHandle, m_ChannelId, iPos);		//���û���
			//libEasyPlayerPro_StartPlaySound(playerHandle, m_ChannelId);				//��������
			if (NULL != pDlgRender)	pDlgRender->SetChannelId(m_ChannelId);

			OnBnClickedCheckOsd();

			if (NULL != pBtnPreview)		pBtnPreview->SetWindowText(TEXT("Stop"));
		}
	}
}



void CDlgVideo::OnBnClickedCheckOsd()
{
	int nShow = 0x00;

	if (NULL != pChkOSD)		nShow = pChkOSD->GetCheck();

	if (m_ChannelId > 0)
	{
		libEasyPlayerPro_ShowStatisticalInfo(playerHandle, m_ChannelId, nShow);
	}
}


void CDlgVideo::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( NULL != pScrollBar && NULL != pSliderCache &&
		pSliderCache->GetDlgCtrlID() == pScrollBar->GetDlgCtrlID())
	{
		int iPos = pSliderCache->GetPos();
		
		if (m_ChannelId > 0)
		{
			libEasyPlayerPro_SetPlayFrameCache(playerHandle, m_ChannelId, iPos);
		}
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CDlgVideo::OnRButtonUp(UINT nFlags, CPoint point)
{


	CDialogEx::OnRButtonUp(nFlags, point);
}

void CDlgVideo::OnMouseWheel(short zDelta, CPoint pt)
{
	if (NULL != pDlgRender)	pDlgRender->OnMouseWheel(zDelta, pt);
}




LRESULT CDlgVideo::OnRecordingComplete(WPARAM wParam, LPARAM lParam)
{
	if (NULL != pDlgRender)		pDlgRender->SetRecordingFlag(0);

	return 0;
}



int CALLBACK __EasyPlayerCallBack(EASY_CALLBACK_TYPE_ENUM callbackType, int channelId, void *userPtr, int mediaType, char *pbuf, EASY_FRAME_INFO *frameInfo)
{
	CDlgVideo	*pLiveVideo = (CDlgVideo *)userPtr;

	if (callbackType == EASY_TYPE_CONNECTING)
	{
		OutputDebugString(TEXT("EASY_TYPE_CONNECTING...\n"));
	}
	else if (callbackType == EASY_TYPE_CONNECTED)
	{
		OutputDebugString(TEXT("EASY_TYPE_CONNECTED.\n"));
	}
	else if (callbackType == EASY_TYPE_DISCONNECT)
	{
		OutputDebugString(TEXT("EASY_TYPE_DISCONNECT.\n"));
	}
	else if (callbackType == EASY_TYPE_RECONNECT)
	{
		OutputDebugString(TEXT("EASY_TYPE_RECONNECT.\n"));
	}
	else if (callbackType == EASY_TYPE_FILE_DURATION)
	{
		wchar_t wszLog[128] = {0};
		wsprintf(wszLog, TEXT("��ʱ��: %u\n"), frameInfo->timestamp_sec);
		OutputDebugString(wszLog);
	}
	else if (callbackType == EASY_TYPE_CODEC_DATA)
	{
		if (mediaType == MEDIA_TYPE_SDP)
		{
		}
		else if (mediaType == MEDIA_TYPE_CODEC_INFO)
		{
		}
		else if (mediaType == MEDIA_TYPE_VIDEO)
		{
			wchar_t wszLog[128] = {0};
			wsprintf(wszLog, TEXT("����ʱ��: %u\n"), frameInfo->timestamp_sec);
			OutputDebugString(wszLog);
		}
		//else if (mediaType == 


		//_TRACE(TRACE_LOG_WARNING, "[ch%d] type[%d] channelId[%d] mediaType[%d]\n", pLiveVideo->channelId, callbackType, channelId, mediaType);

		

#ifdef _DEBUG1
		if (mediaType == 0x01)
		{
			static int iH264FrameNo = 0;
			char sztmp[128] = {0};
			sprintf(sztmp, "C:\\test\\h264\\%d.txt", frameInfo->length);//iH264FrameNo++, frameInfo->length);
			FILE *f = fopen(sztmp, "wb");
			if (NULL != f)
			{
				fwrite(pbuf, 1, frameInfo->length, f);
				fclose(f);
			}
		}
#endif
	}
	else if (callbackType == EASY_TYPE_DECODE_DATA)
	{
		//_TRACE(TRACE_LOG_DEBUG, "��������[ch%d]type[%d] channelId[%d] mediaType[%d] [%d x %d] framesize[%d]\n",  pLiveVideo->channelId,
			//	callbackType, channelId, mediaType, frameInfo->width, frameInfo->height, frameInfo->length);
	}
	else if (callbackType == EASY_TYPE_SNAPSHOT)
	{
		//_TRACE(TRACE_LOG_DEBUG, "ץ��ͼƬ[ch%d] %s. filename:%s\n",  channelId, mediaType==1?"�ɹ�":"ʧ��", pbuf);
		if (mediaType == MEDIA_TYPE_VIDEO)		OutputDebugString(TEXT("ץ��ͼƬ�ɹ�\n"));
		else if (mediaType == MEDIA_TYPE_EVENT)		OutputDebugString(TEXT("ץ��ͼƬʧ��\n"));
	}
	else if (callbackType == EASY_TYPE_RECORDING)
	{
		if (mediaType == MEDIA_TYPE_VIDEO)		OutputDebugString(TEXT("¼��ɹ�\n"));
		else if (mediaType == MEDIA_TYPE_EVENT)		OutputDebugString(TEXT("¼��ʧ��\n"));

		pLiveVideo->PostMessageW(WM_RECORDING_CMPLETE, MEDIA_TYPE_VIDEO == mediaType ? 0 : -1, 0);
	}
	else if (callbackType == EASY_TYPE_INSTANT_REPLAY_RECORDING)
	{
		if (mediaType == MEDIA_TYPE_VIDEO)		OutputDebugString(TEXT("��ʱ�ط�¼��ɹ�\n"));
		else if (mediaType == MEDIA_TYPE_EVENT)		OutputDebugString(TEXT("��ʱ�ط�¼��ʧ��\n"));
	}

	return 0;
}




HBRUSH CDlgVideo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	switch (nCtlColor)
	{
	case CTLCOLOR_BTN:	//��ť
		{
			return m_BrushBtn;
		}
		break;
		case CTLCOLOR_EDIT:	//�༭��
		{
			//pDC->SelectObject(&fontText);
			pDC->SetTextColor(DIALOG_BASE_TEXT_COLOR);
			return m_BrushEdt;
		}
		break;
	case CTLCOLOR_STATIC:	
		{
			pDC->SetBkColor(DIALOG_BASE_BACKGROUND_COLOR);
			pDC->SetTextColor(DIALOG_BASE_TEXT_COLOR);
			return m_BrushStatic;
		}
		break;
	default:
		break;
	}


	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
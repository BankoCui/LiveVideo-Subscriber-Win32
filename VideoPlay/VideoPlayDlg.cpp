
// VideoPlayDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoPlay.h"
#include "VideoPlayDlg.h"
#include "afxdialogex.h"

#include "Utils.h"
#include "AvPlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVideoPlayDlg �Ի���
CVideoPlayDlg::CVideoPlayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoPlayDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	av_player_ = NULL;
	rgb_data_  = NULL;
	h264_file_ = NULL;
	rgb_file_  = NULL;
}

void CVideoPlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVideoPlayDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CVideoPlayDlg ��Ϣ�������

BOOL CVideoPlayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	StartLivePlay();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVideoPlayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVideoPlayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVideoPlayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/************************************************************************/
/*                        StartLivePlay                                 */
/************************************************************************/
void CVideoPlayDlg::StartLivePlay()
{
	//char* url = "rtmp://192.168.61.129:1935/live/123";
	char* url = "rtmp://localhost/live/stream";

	av_player_ = new AvPlayer();
	av_player_->InitObserver(this);
	av_player_->StartPlayRtmp(url);
}

void CVideoPlayDlg::CreateBitmapInfo(BITMAPINFO* pSrcBitmapInfo, BITMAPINFO* pDstBitmapInfo, int width, int height)
{
	if (pSrcBitmapInfo)
	{
		memcpy(pDstBitmapInfo, pSrcBitmapInfo, sizeof(BITMAPINFO));
	}
	else
	{
		DWORD dwBmpHdr = sizeof(BITMAPINFO);
		pDstBitmapInfo->bmiHeader.biClrImportant = 0;
		pDstBitmapInfo->bmiHeader.biSize = dwBmpHdr;
		pDstBitmapInfo->bmiHeader.biSizeImage = 0;
		pDstBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
		pDstBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
		pDstBitmapInfo->bmiHeader.biClrUsed = 0;
		pDstBitmapInfo->bmiHeader.biPlanes = 1;
		pDstBitmapInfo->bmiHeader.biWidth = width;
	}
	pDstBitmapInfo->bmiHeader.biHeight = -1*height;
	pDstBitmapInfo->bmiHeader.biCompression = BI_RGB;
	pDstBitmapInfo->bmiHeader.biBitCount = 24;
}

void CVideoPlayDlg::DisplayPitcure(const BITMAPINFO bitmapinfo, int width, int height)
{
	CRect rect;
	GetClientRect(rect);
	CDC* pDC = GetDC();
	HDC pHDC = pDC->GetSafeHdc();

	LONG image_w = width;
	LONG image_h = height;

	LONG canvas_x = 0;
	LONG canvas_y = 0;
	LONG canvas_w = 0;
	LONG canvas_h = 0;

	float rect_aspect_ratio = rect.Width()*1.0f / rect.Height();
	float image_aspect_ratio = image_w*1.0f /image_h;
	if (rect_aspect_ratio > image_aspect_ratio)
	{
		canvas_h = rect.Height();
		canvas_w = canvas_h * image_w / image_h;
		canvas_x = (rect.Width() - canvas_w) / 2;
	}
	else
	{
		canvas_w = rect.Width();
		canvas_h = canvas_w * image_h/image_w;
		canvas_y = (rect.Height() - canvas_h) / 2;
	}

	SetStretchBltMode(pHDC, HALFTONE);
	StretchDIBits(pHDC,
		canvas_x, canvas_y, canvas_w, canvas_h,
		0, 0, image_w , image_h,
		(CONST VOID*)rgb_data_, &bitmapinfo, DIB_RGB_COLORS, SRCCOPY);

	ReleaseDC(pDC);
}

void CVideoPlayDlg::OnRawData(const char* data, int length)
{

}

void CVideoPlayDlg::OnVideoData(const unsigned char* y, const unsigned char* u, const unsigned char* v, int width, int height)
{
	unsigned char* yuv[3];
	yuv[0] = (unsigned char* )y;
	yuv[1] = (unsigned char* )u;
	yuv[2] = (unsigned char* )v;

	if (NULL == rgb_data_)
	{
		rgb_data_ = new unsigned char[width*height*3];
	}
	YUV420p_to_RGB24(yuv, rgb_data_, width, height);

#if 0
	if (rgb_file_ == NULL)
	{
		rgb_file_ = fopen("play_test.rgb", "wb");
	}
	fwrite(rgb, width*height*3, 1, mRgbFile);
	fflush(mRgbFile);
#endif

	// ��ʹ��GDI��ȾRGB����ʱ��
	// ע��BMP��y�����Ƿ��Ŵ洢�ģ���˸߶ȱ�������һ����ֵ������ʹͼ��������ʾ����
	BITMAPINFO bitmap_info;
	CreateBitmapInfo(NULL, &bitmap_info, width, height);
	change_endian_picture_rgb(rgb_data_, width, height, 24);
	DisplayPitcure(bitmap_info, width, height);
}

void CVideoPlayDlg::OnAudioData(const char* data, int length)
{

}

void CVideoPlayDlg::OnPlayerEvent(int code, const char* content)
{

}

void CVideoPlayDlg::OnDestroy()
{
	__super::OnDestroy();

	if (av_player_)
	{
		av_player_->UninitObserver();
		delete av_player_;
		av_player_ = NULL;
	}
	if (rgb_data_)
	{
		delete[] rgb_data_;
		rgb_data_ = NULL;
	}
}

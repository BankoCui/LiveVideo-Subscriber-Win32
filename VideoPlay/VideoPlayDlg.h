
// VideoPlayDlg.h : ͷ�ļ�
//

#pragma once


#include "AvPlayerObserver.h"

class AvPlayer; // ����������

// CVideoPlayDlg �Ի���
class CVideoPlayDlg : public CDialogEx, public AvPlayerObserver
{
// ����
public:
	CVideoPlayDlg(CWnd* pParent = NULL);	// ��׼���캯��

	virtual void OnRawData(const char* data, int length);
	virtual void OnVideoData(const unsigned char* y, const unsigned char* u, const unsigned char* v, int width, int height);
	virtual void OnAudioData(const char* data, int length);
	virtual void OnPlayerEvent(int code, const char* content);

	afx_msg void OnDestroy();

// �Ի�������
	enum { IDD = IDD_VIDEOPLAY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void StartLivePlay();
	void CreateBitmapInfo(BITMAPINFO* pSrcBitmapInfo, BITMAPINFO* pDstBitmapInfo, int width, int height);
	void DisplayPitcure(const BITMAPINFO bitmapinfo, int width, int height);

	// player
	AvPlayer* av_player_;

	// render with rgb data
	unsigned char* rgb_data_;

	// dump data with file
	FILE* h264_file_;
	FILE* rgb_file_;	
};

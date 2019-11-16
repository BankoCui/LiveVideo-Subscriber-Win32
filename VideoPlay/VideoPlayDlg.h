
// VideoPlayDlg.h : 头文件
//

#pragma once


#include "AvPlayerObserver.h"

class AvPlayer; // 声明引用类

// CVideoPlayDlg 对话框
class CVideoPlayDlg : public CDialogEx, public AvPlayerObserver
{
// 构造
public:
	CVideoPlayDlg(CWnd* pParent = NULL);	// 标准构造函数

	virtual void OnRawData(const char* data, int length);
	virtual void OnVideoData(const unsigned char* y, const unsigned char* u, const unsigned char* v, int width, int height);
	virtual void OnAudioData(const char* data, int length);
	virtual void OnPlayerEvent(int code, const char* content);

	afx_msg void OnDestroy();

// 对话框数据
	enum { IDD = IDD_VIDEOPLAY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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

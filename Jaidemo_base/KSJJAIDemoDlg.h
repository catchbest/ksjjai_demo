
// KSJJAIDemoDlg.h : 头文件
//

#pragma once

#include <Jai_Factory.h>
#define MAX_CAMERAS             16
//#define VERSION2  

#define NODE_NAME_WIDTH         (int8_t*)"Width"
#define NODE_NAME_HEIGHT        (int8_t*)"Height"
#define NODE_NAME_PIXELFORMAT   (int8_t*)"PixelFormat"

#ifdef VERSION2
#define NODE_NAME_EXPOSURE      (int8_t*)"ExposureTimeRaw"
#define NODE_NAME_GAIN          (int8_t*)"GainRaw"
#else
#define NODE_NAME_EXPOSURE      (int8_t*)"ExposureTime"
#define NODE_NAME_GAIN          (int8_t*)"Gain"
#endif

#define NODE_NAME_ACQSTART			(int8_t*)"AcquisitionStart"
#define NODE_NAME_ACQSTOP			(int8_t*)"AcquisitionStop"
#define NODE_NAME_TRIGGERMODE		(int8_t*)"TriggerMode"
#define NODE_NAME_TRIGGERSOURCE     (int8_t*)"TriggerSource"
#define NODE_NAME_ACQUISITIONMODE   (int8_t*)"AcquisitionMode"
#define NODE_NAME_ACQUISITIONSTART  (int8_t*)"TriggerSoftware"
#define NODE_NAME_SAVE			    (int8_t*)"UserSetSave"
#define NODE_NAME_LOAD			    (int8_t*)"UserSetLoad"
#define NODE_NAME_PXIELFORMAT       (int8_t*)"PixelFormat"

#define NODE_NAME_ACQUISITIONFRAMERATEENABLE   (int8_t*)"AcquisitionFrameRateEnable"
#define NODE_NAME_ACQUISITIONFRAMERATE   (int8_t*)"AcquisitionFrameRate"
// CKSJJAIDemoDlg 对话框
class CKSJJAIDemoDlg : public CDialogEx
{
// 构造
public:
	CKSJJAIDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_KSJJAIDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	BOOL OpenFactoryAndCamera();
	void CloseFactoryAndCamera();
	FACTORY_HANDLE  m_hFactory;             // Factory Handle
	CAM_HANDLE      m_hCam[MAX_CAMERAS];    // Camera Handles
	VIEW_HANDLE     m_hView[MAX_CAMERAS];   // View Window handles
	THRD_HANDLE     m_hThread[MAX_CAMERAS]; // Stream handles
	int8_t          m_sCameraId[MAX_CAMERAS][J_CAMERA_ID_SIZE]; // Camera IDs
	int8_t			m_sIP[MAX_CAMERAS][J_CAMERA_INFO_SIZE];
	void StreamCBFunc1(J_tIMAGE_INFO * pAqImageInfo);
	void StreamCBFunc2(J_tIMAGE_INFO * pAqImageInfo);
	int64_t     m_nCaptureWidth;
	int64_t		m_nCaptureHeight;
	J_tIMAGE_INFO m_ImageBufferInfo;
	int64_t m_PixelType;
// 实现
protected:
	HICON m_hIcon;
	bool  m_bReqAck;
	int   m_CameraCount;
	int	  m_CameraCurSel;

	int   m_nCount;
	bool  m_bImage;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboCamera();
	afx_msg void OnEnChangeEditExposure();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnDestroy();
	void UpdateUi();
	afx_msg void OnEnChangeEditGain();
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditHeight();
	afx_msg void OnCbnSelchangeComboTriggermode();
	afx_msg void OnCbnSelchangeComboTriggersource();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnCbnSelchangeComboCapture();
	afx_msg void OnBnClickedButtonSoftware();
	afx_msg void OnBnClickedCheckFramerate();
	afx_msg void OnEnChangeEditFramerate();
};


// KSJJAIDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KSJJAIDemo.h"
#include "KSJJAIDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern int plc_open();
extern void plc_close();
extern int plc_readly_check();
extern int plc_write_xy_data(DWORD x1, DWORD y1, DWORD x2, DWORD y2);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CKSJJAIDemoDlg 对话框



CKSJJAIDemoDlg::CKSJJAIDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKSJJAIDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for (int i = 0; i < MAX_CAMERAS; i++)
	{
		m_hCam[i] = NULL;
		m_hView[i] = NULL;
		m_hThread[i] = NULL;
	}
	m_CameraCurSel = 0;
	m_pRGBBuffer1 = NULL;
	m_pRGBBuffer2 = NULL;
	m_ImageBufferInfo.pImageBuffer = NULL;
	m_ImageBufferInfo2.pImageBuffer = NULL;
}

void CKSJJAIDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PREVIEWWND, m_SnapStatic);
	DDX_Control(pDX, IDC_STATIC_PREVIEWWND2, m_SnapStatic2);
}

BEGIN_MESSAGE_MAP(CKSJJAIDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_EXPOSURE, &CKSJJAIDemoDlg::OnEnChangeEditExposure)
	ON_BN_CLICKED(IDC_BUTTON_START, &CKSJJAIDemoDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CKSJJAIDemoDlg::OnBnClickedButtonStop)
	ON_CBN_SELCHANGE(IDC_COMBO_CAMERA, &CKSJJAIDemoDlg::OnCbnSelchangeComboCamera)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_GAIN, &CKSJJAIDemoDlg::OnEnChangeEditGain)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CKSJJAIDemoDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CKSJJAIDemoDlg::OnBnClickedButtonLoad)
	ON_MESSAGE(WM_SETPIC, &CKSJJAIDemoDlg::OnMsgSetPIC)
	ON_MESSAGE(WM_SETPIC2, &CKSJJAIDemoDlg::OnMsgSetPIC2)
	ON_BN_CLICKED(IDC_BUTTON_START2, &CKSJJAIDemoDlg::OnBnClickedButtonStart2)
	ON_BN_CLICKED(IDC_BUTTON_STOP2, &CKSJJAIDemoDlg::OnBnClickedButtonStop2)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGERMODE, &CKSJJAIDemoDlg::OnCbnSelchangeComboTriggermode)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGERSOURCE, &CKSJJAIDemoDlg::OnCbnSelchangeComboTriggersource)
	ON_CBN_SELCHANGE(IDC_COMBO_CAPTURE, &CKSJJAIDemoDlg::OnCbnSelchangeComboCapture)
	ON_BN_CLICKED(IDC_BUTTON_SOFTWARE, &CKSJJAIDemoDlg::OnBnClickedButtonSoftware)
END_MESSAGE_MAP()


// CKSJJAIDemoDlg 消息处理程序

BOOL CKSJJAIDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	BOOL retval;

	CComboBox    *pComboBox = NULL;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMODE);
	CString strTriggerMode[2] = { _T("off"), _T("on") };
	for (int i = 0; i < 2; i++)
	{
		pComboBox->AddString(strTriggerMode[i]);
	}

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERSOURCE);
	CString strTriggerSource[2] = { _T("Trigger Software"), _T("Line 1") };
	for (int i = 0; i < 2; i++)
	{
		pComboBox->AddString(strTriggerSource[i]);
	}

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CAPTURE);
	CString strCapture[2] = { _T("SingleFrame"), _T("Continuous") };
	for (int i = 0; i < 2; i++)
	{
		pComboBox->AddString(strCapture[i]);
	}

	// Open factory & camera
	retval = OpenFactoryAndCamera();
	if (retval)
	{
		CComboBox    *pComboBox = NULL;
		pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CAMERA);
		for (int i = 0; i < m_CameraCount;i++)
		{
			pComboBox->AddString(CString((char*)m_sCameraId[i]));
		}

		pComboBox->SetCurSel(m_CameraCurSel);
		UpdateUi();
		//OnCbnSelchangeComboCamera();
	}

	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP2)->EnableWindow(FALSE);
	InitializeCriticalSection(&m_cs);
	InitializeCriticalSection(&m_cs2);
	m_hSetCaptureFovEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hSetCaptureFovEvent2 = CreateEvent(NULL, TRUE, FALSE, NULL);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL CKSJJAIDemoDlg::OpenFactoryAndCamera()
{
	J_STATUS_TYPE   retval;
	uint32_t        iSize;
	uint32_t        iNumDev;
	bool8_t         bHasChange;

	// Open factory
	retval = J_Factory_Open((int8_t*)"", &m_hFactory);
	if (retval != J_ST_SUCCESS)
	{
		AfxMessageBox(CString("Could not open factory!"));
		return FALSE;
	}
	TRACE("Opening factory succeeded\n");

	// Update camera list
	retval = J_Factory_UpdateCameraList(m_hFactory, &bHasChange);
	if (retval != J_ST_SUCCESS)
	{
		AfxMessageBox(CString("Could not update camera list!"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	TRACE("Updating camera list succeeded\n");

	// Get the number of Cameras
	retval = J_Factory_GetNumOfCameras(m_hFactory, &iNumDev);
	if (retval != J_ST_SUCCESS)
	{
		AfxMessageBox(CString("Could not get the number of cameras!"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	if (iNumDev == 0)
	{
		AfxMessageBox(CString("There is no camera!"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	TRACE("%d cameras were found\n", iNumDev);

	// We only want to get MAX_CAMERAS cameras connected at a time
	// and we assume that iNumDev is the actual camera count*2 because we assume
	// that we have 2 drivers active (SockerDriver+FilerDriver)

	bool bFdUse = false;
	int	iValidCamera = 0;

	for (int i = 0; i < (int)iNumDev; i++)
	{
		// Get camera IDs
		iSize = J_CAMERA_ID_SIZE;
		m_sCameraId[iValidCamera][0] = 0;
		retval = J_Factory_GetCameraIDByIndex(m_hFactory, i, m_sCameraId[iValidCamera], &iSize);
		if (retval != J_ST_SUCCESS)
		{
			AfxMessageBox(CString("Could not get the camera ID!"), MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
		TRACE("Camera ID[%d]: %s\n", i, m_sCameraId[iValidCamera]);

		if (0 == strncmp("TL=>GevTL , INT=>FD", (char*)m_sCameraId[iValidCamera], 19))
		{ // FD
			bFdUse = true;
			// Open camera
			retval = J_Camera_Open(m_hFactory, m_sCameraId[iValidCamera], &m_hCam[iValidCamera]);
			if (retval != J_ST_SUCCESS)
			{
				AfxMessageBox(CString("Could not open the camera!"), MB_OK | MB_ICONEXCLAMATION);
				return FALSE;
			}
			iValidCamera++;
			TRACE("Opening camera %d succeeded\n", iValidCamera);
		}
		else
		{ // SD
			if (bFdUse == false)
			{
				// Open camera
				retval = J_Camera_Open(m_hFactory, m_sCameraId[iValidCamera], &m_hCam[iValidCamera]);
				if (retval != J_ST_SUCCESS)
				{
					AfxMessageBox(CString("Could not open the camera!"), MB_OK | MB_ICONEXCLAMATION);
					return FALSE;
				}
				iValidCamera++;
				TRACE("Opening camera %d succeeded\n", iValidCamera);
			}
		}
		if (iValidCamera >= MAX_CAMERAS)
			break;
	}
	m_CameraCount = min(iValidCamera, MAX_CAMERAS);

	return TRUE;
}

void CKSJJAIDemoDlg::CloseFactoryAndCamera()
{
	for (int i = 0; i < MAX_CAMERAS; i++)
	{
		if (m_hCam[i])
		{
			// Close camera
			J_Camera_Close(m_hCam[i]);
			m_hCam[i] = NULL;
			TRACE("Closed camera %d\n", i);
		}
	}

	if (m_hFactory)
	{
		// Close factory
		J_Factory_Close(m_hFactory);
		m_hFactory = NULL;
		TRACE("Closed factory\n");
	}
}

void CKSJJAIDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKSJJAIDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CKSJJAIDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKSJJAIDemoDlg::OnCbnSelchangeComboCamera()
{
	CComboBox    *pComboBox = NULL;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CAMERA);
	m_CameraCurSel = pComboBox->GetCurSel();
	UpdateUi();
}

void CKSJJAIDemoDlg::OnBnClickedButtonStart()
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	J_STATUS_TYPE   retval;
	int64_t int64Val;
	int64_t pixelFormat;
	int     bpp = 0;
	SIZE	ViewSize;
	POINT	TopLeft;
	
	if (m_hCam[0])
	{
		SetEvent(m_hSetCaptureFovEvent);
		// Get Width from the camera
		retval = J_Camera_GetValueInt64(m_hCam[0], NODE_NAME_WIDTH, &int64Val);
		ViewSize.cx = (LONG)int64Val;     // Set window size cx
		//ViewSize.cx = 1280;
		// Get Height from the camera
		retval = J_Camera_GetValueInt64(m_hCam[0], NODE_NAME_HEIGHT, &int64Val);
		ViewSize.cy = (LONG)int64Val;     // Set window size cy
		//ViewSize.cy = 1024;
		// Set window position
		TopLeft.x = 10;
		TopLeft.y = 10 + (0*(ViewSize.cy + 25));

		// Get pixelformat from the camera
		retval = J_Camera_GetValueInt64(m_hCam[0], NODE_NAME_PIXELFORMAT, &int64Val);
		pixelFormat = int64Val;

		// Calculate number of bits (not bytes) per pixel using macro
		bpp = J_BitsPerPixel(pixelFormat);

		// Open stream
		//if (m_CameraCurSel == 0)
		{
			retval = J_Image_OpenStream(m_hCam[0], 0, reinterpret_cast<J_IMG_CALLBACK_OBJECT>(this), reinterpret_cast<J_IMG_CALLBACK_FUNCTION>(&CKSJJAIDemoDlg::StreamCBFunc1), &m_hThread[0], (ViewSize.cx*ViewSize.cy*bpp) / 8);
			if (retval != J_ST_SUCCESS) {
				AfxMessageBox(CString("Could not open stream!"), MB_OK | MB_ICONEXCLAMATION);
				OnBnClickedButtonStop();
				return;
			}
			TRACE("Opening stream succeeded\n");
		}
		// Start Acquision
		retval = J_Camera_ExecuteCommand(m_hCam[0], NODE_NAME_ACQSTART);
	}
}


void CKSJJAIDemoDlg::OnBnClickedButtonStop()
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	J_STATUS_TYPE retval;

	if (m_hCam[0])
	{
		// Stop Acquision
		if (m_hCam[0]) {
			retval = J_Camera_ExecuteCommand(m_hCam[0], NODE_NAME_ACQSTOP);
		}

		EnterCriticalSection(&m_cs);

		if (m_hThread[0])
		{
			// Close stream
			retval = J_Image_CloseStream(m_hThread[0]);
			m_hThread[0] = NULL;
			TRACE("Closed stream\n");
		}

		LeaveCriticalSection(&m_cs);
		ResetEvent(m_hSetCaptureFovEvent);
	}

}

//--------------------------------------------------------------------------------------------------
// StreamCBFunc
//--------------------------------------------------------------------------------------------------
void CKSJJAIDemoDlg::StreamCBFunc1(J_tIMAGE_INFO * pAqImageInfo)
{
	PostMessage(WM_SETPIC, 0, (LPARAM)pAqImageInfo);
}

void CKSJJAIDemoDlg::StreamCBFunc2(J_tIMAGE_INFO * pAqImageInfo)
{
	PostMessage(WM_SETPIC2, 0, (LPARAM)pAqImageInfo);
}

LRESULT CKSJJAIDemoDlg::OnMsgSetPIC(WPARAM wParam, LPARAM lParam)
{
	if (WAIT_OBJECT_0 != WaitForSingleObject(m_hSetCaptureFovEvent, 0)) return 0;
	J_STATUS_TYPE iResult;
	EnterCriticalSection(&m_cs);
	J_tIMAGE_INFO* pAqImageInfo = (J_tIMAGE_INFO*)lParam;
	if (pAqImageInfo->iPixelType >= 0x1080008)
	{
		// We need to allocate the conversion buffer once
		if (m_ImageBufferInfo.pImageBuffer == NULL)
		{
			iResult = J_Image_Malloc(pAqImageInfo, &m_ImageBufferInfo);
			if (GC_ERR_SUCCESS != iResult)
			{
				OutputDebugString(_T("Error with J_Image_Malloc in CStreamThread::StreamProcess.\n"));
				return 0;
			}
		}

		// Converts from RAW to internal image before applying the Green-compensation algorithm.
		iResult = J_Image_FromRawToImageEx(pAqImageInfo, &m_ImageBufferInfo, BAYER_STANDARD_MULTI);
		if (GC_ERR_SUCCESS != iResult)
		{
			OutputDebugString(_T("Error with J_Image_FromRawToImageEx in CStreamThread::StreamProcess.\n"));
			return 0;
		}

		m_SnapStatic.LoadImage(m_ImageBufferInfo.pImageBuffer, m_nCaptureWidth, m_nCaptureHeight, 24);
	}
	else
	{
		m_SnapStatic.LoadImage(pAqImageInfo->pImageBuffer, m_nCaptureWidth, m_nCaptureHeight, 8);
	}
	
	LeaveCriticalSection(&m_cs);
	return 0;
}

LRESULT CKSJJAIDemoDlg::OnMsgSetPIC2(WPARAM wParam, LPARAM lParam)
{
	if (WAIT_OBJECT_0 != WaitForSingleObject(m_hSetCaptureFovEvent2, 0)) return 0;
	J_STATUS_TYPE iResult;
	EnterCriticalSection(&m_cs2);
	J_tIMAGE_INFO* pAqImageInfo = (J_tIMAGE_INFO*)lParam;
	if (pAqImageInfo->iPixelType >= 0x1080008)
	{
		// We need to allocate the conversion buffer once
		if (m_ImageBufferInfo2.pImageBuffer == NULL)
		{
			iResult = J_Image_Malloc(pAqImageInfo, &m_ImageBufferInfo2);
			if (GC_ERR_SUCCESS != iResult)
			{
				OutputDebugString(_T("Error with J_Image_Malloc in CStreamThread::StreamProcess.\n"));
				return 0;
			}
		}

		// Converts from RAW to internal image before applying the Green-compensation algorithm.
		iResult = J_Image_FromRawToImageEx(pAqImageInfo, &m_ImageBufferInfo2, BAYER_STANDARD_MULTI);
		if (GC_ERR_SUCCESS != iResult)
		{
			OutputDebugString(_T("Error with J_Image_FromRawToImageEx in CStreamThread::StreamProcess.\n"));
			return 0;
		}

		m_SnapStatic2.LoadImage(m_ImageBufferInfo2.pImageBuffer, m_nCaptureWidth2, m_nCaptureHeight2, 24);
	}
	else
	{
		m_SnapStatic2.LoadImage(pAqImageInfo->pImageBuffer, m_nCaptureWidth2, m_nCaptureHeight2, 8);
	}

	LeaveCriticalSection(&m_cs2);
	return 0;
}

void CKSJJAIDemoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	OnBnClickedButtonStop();
	OnBnClickedButtonStop2();
	CloseFactoryAndCamera();
	DeleteCriticalSection(&m_cs);
	DeleteCriticalSection(&m_cs2);
	CloseHandle(m_hSetCaptureFovEvent);
	CloseHandle(m_hSetCaptureFovEvent2);

	if (m_pRGBBuffer1 != NULL)
	{
		delete[] m_pRGBBuffer1;
		m_pRGBBuffer1 = NULL;
	}

	if (m_pRGBBuffer2 != NULL)
	{
		delete[] m_pRGBBuffer2;
		m_pRGBBuffer2 = NULL;
	}
}


void CKSJJAIDemoDlg::UpdateUi()
{
	J_STATUS_TYPE   retval;
	NODE_HANDLE hNode;
	int64_t int64Val, max, min;
	double dMax, dMin, dValue;
	TCHAR szTemp[32] = { 0 };
	retval = J_Camera_GetNodeByName(m_hCam[m_CameraCurSel], NODE_NAME_EXPOSURE, &hNode);
	if (retval == J_ST_SUCCESS)
	{
		retval = J_Node_GetMinInt64(hNode, &min);
		retval = J_Node_GetMaxInt64(hNode, &max);
		J_Node_GetValueInt64(hNode, FALSE, &int64Val);

		CSpinButtonCtrl * pSpinCtrl = (CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_EXPOSURE);
		pSpinCtrl->SetRange32(min, max);
		pSpinCtrl->SetPos32(int64Val);
	}

	retval = J_Camera_GetNodeByName(m_hCam[m_CameraCurSel], NODE_NAME_GAIN, &hNode);
	if (retval == J_ST_SUCCESS)
	{
		J_Node_GetValueDouble(hNode, FALSE, &dValue);
		_stprintf(szTemp, _T("%3f"), dValue);
		GetDlgItem(IDC_EDIT_GAIN)->SetWindowText(szTemp);
	}

	retval = J_Camera_GetNodeByName(m_hCam[0], NODE_NAME_WIDTH, &hNode);
	if (retval == J_ST_SUCCESS)
	{
		retval = J_Node_GetMaxInt64(hNode, &m_nCaptureWidth);
		J_Camera_SetValueInt64(m_hCam[0], NODE_NAME_WIDTH, m_nCaptureWidth);
	}

	retval = J_Camera_GetNodeByName(m_hCam[0], NODE_NAME_HEIGHT, &hNode);
	if (retval == J_ST_SUCCESS)
	{
		retval = J_Node_GetMaxInt64(hNode, &m_nCaptureHeight);
		J_Camera_SetValueInt64(m_hCam[0], NODE_NAME_HEIGHT, m_nCaptureHeight);
	}

	retval = J_Camera_GetNodeByName(m_hCam[1], NODE_NAME_WIDTH, &hNode);
	if (retval == J_ST_SUCCESS)
	{
		retval = J_Node_GetMaxInt64(hNode, &m_nCaptureWidth2);
		J_Camera_SetValueInt64(m_hCam[1], NODE_NAME_WIDTH, m_nCaptureWidth2);
	}

	retval = J_Camera_GetNodeByName(m_hCam[1], NODE_NAME_HEIGHT, &hNode);
	if (retval == J_ST_SUCCESS)
	{
		retval = J_Node_GetMaxInt64(hNode, &m_nCaptureHeight2);
		J_Camera_SetValueInt64(m_hCam[1], NODE_NAME_HEIGHT, m_nCaptureHeight2);
	}

	retval = J_Camera_GetNodeByName(m_hCam[m_CameraCurSel], NODE_NAME_WIDTH, &hNode);
	if (retval == J_ST_SUCCESS)
	{
		retval = J_Node_GetMinInt64(hNode, &min);
		retval = J_Node_GetMaxInt64(hNode, &max);
		J_Node_GetValueInt64(hNode, FALSE, &int64Val);

		CSpinButtonCtrl * pSpinCtrl = (CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_WIDTH);
		pSpinCtrl->SetRange32(min, max);
		pSpinCtrl->SetPos32(int64Val);
	}

	retval = J_Camera_GetNodeByName(m_hCam[m_CameraCurSel], NODE_NAME_HEIGHT, &hNode);
	if (retval == J_ST_SUCCESS)
	{
		retval = J_Node_GetMinInt64(hNode, &min);
		retval = J_Node_GetMaxInt64(hNode, &max);
		J_Node_GetValueInt64(hNode, FALSE, &int64Val);

		CSpinButtonCtrl * pSpinCtrl = (CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_HEIGHT);
		pSpinCtrl->SetRange32(min, max);
		pSpinCtrl->SetPos32(int64Val);
	}

	retval = J_Camera_GetNodeByName(m_hCam[m_CameraCurSel], NODE_NAME_TRIGGERMODE, &hNode);
	if (retval == J_ST_SUCCESS)
	{
		CComboBox    *pComboBox = NULL;
		pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMODE);

		J_Node_GetValueInt64(hNode, FALSE, &int64Val);
		pComboBox->SetCurSel(int64Val);
	}

	retval = J_Camera_GetNodeByName(m_hCam[m_CameraCurSel], NODE_NAME_TRIGGERSOURCE, &hNode);
	if (retval == J_ST_SUCCESS)
	{
		CComboBox    *pComboBox = NULL;
		pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERSOURCE);

		J_Node_GetValueInt64(hNode, FALSE, &int64Val);
		pComboBox->SetCurSel(int64Val);
	}

	retval = J_Camera_GetNodeByName(m_hCam[m_CameraCurSel], NODE_NAME_ACQUISITIONMODE, &hNode);
	if (retval == J_ST_SUCCESS)
	{
		CComboBox    *pComboBox = NULL;
		pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CAPTURE);

		J_Node_GetValueInt64(hNode, FALSE, &int64Val);
		if (int64Val == 2)
		{
			int64Val--;
		}

		pComboBox->SetCurSel(int64Val);
	}

	if (m_CameraCount >=1)
	{
		if (m_pRGBBuffer1 != NULL)
		{
			delete[] m_pRGBBuffer1;
			m_pRGBBuffer1 = NULL;
		}

		m_pRGBBuffer1 = new BYTE[m_nCaptureWidth * m_nCaptureHeight * 3];
	}

	if (m_CameraCount >= 2)
	{
		if (m_pRGBBuffer2 != NULL)
		{
			delete[] m_pRGBBuffer2;
			m_pRGBBuffer2 = NULL;
		}

		m_pRGBBuffer2 = new BYTE[m_nCaptureWidth2 * m_nCaptureHeight2 * 3];
	}
}

void CKSJJAIDemoDlg::OnEnChangeEditExposure()
{
	if (m_CameraCurSel == -1) return;
	int nValue = GetDlgItemInt(IDC_EDIT_EXPOSURE);
	J_Camera_SetValueInt64(m_hCam[m_CameraCurSel], NODE_NAME_EXPOSURE, nValue);
}

void CKSJJAIDemoDlg::OnEnChangeEditGain()
{
	if (m_CameraCurSel == -1) return;
	TCHAR szTemp[32] = { 0 };
	GetDlgItem(IDC_EDIT_GAIN)->GetWindowText(szTemp, 32);
	float fValue = atof(szTemp);
	J_Camera_SetValueDouble(m_hCam[m_CameraCurSel], NODE_NAME_GAIN, fValue);
}


void CKSJJAIDemoDlg::OnBnClickedButtonSave()
{
	if (m_CameraCurSel == -1) return;
	J_Camera_ExecuteCommand(m_hCam[m_CameraCurSel], NODE_NAME_SAVE);
}


void CKSJJAIDemoDlg::OnBnClickedButtonLoad()
{
	if (m_CameraCurSel == -1) return;
	J_Camera_ExecuteCommand(m_hCam[m_CameraCurSel], NODE_NAME_LOAD);
	UpdateUi();
}


void CKSJJAIDemoDlg::OnBnClickedButtonStart2()
{
	GetDlgItem(IDC_BUTTON_START2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP2)->EnableWindow(TRUE);
	J_STATUS_TYPE   retval;
	int64_t int64Val;
	int64_t pixelFormat;
	int     bpp = 0;
	SIZE	ViewSize;
	POINT	TopLeft;

	if (m_hCam[1])
	{
		SetEvent(m_hSetCaptureFovEvent2);
		// Get Width from the camera
		retval = J_Camera_GetValueInt64(m_hCam[1], NODE_NAME_WIDTH, &int64Val);
		ViewSize.cx = (LONG)int64Val;     // Set window size cx
		//ViewSize.cx = 1280;
		// Get Height from the camera
		retval = J_Camera_GetValueInt64(m_hCam[1], NODE_NAME_HEIGHT, &int64Val);
		ViewSize.cy = (LONG)int64Val;     // Set window size cy
		//ViewSize.cy = 1024;
		// Set window position
		TopLeft.x = 10;
		TopLeft.y = 10 + (1*(ViewSize.cy + 25));

		// Get pixelformat from the camera
		retval = J_Camera_GetValueInt64(m_hCam[1], NODE_NAME_PIXELFORMAT, &int64Val);
		pixelFormat = int64Val;

		// Calculate number of bits (not bytes) per pixel using macro
		bpp = J_BitsPerPixel(pixelFormat);

		// Open stream
		{
			retval = J_Image_OpenStream(m_hCam[1], 0, reinterpret_cast<J_IMG_CALLBACK_OBJECT>(this), reinterpret_cast<J_IMG_CALLBACK_FUNCTION>(&CKSJJAIDemoDlg::StreamCBFunc2), &m_hThread[1], (ViewSize.cx*ViewSize.cy*bpp) / 8);
			if (retval != J_ST_SUCCESS) {
				AfxMessageBox(CString("Could not open stream!"), MB_OK | MB_ICONEXCLAMATION);
				OnBnClickedButtonStop();
				return;
			}
			TRACE("Opening stream succeeded\n");
		}
		// Start Acquision
		retval = J_Camera_ExecuteCommand(m_hCam[1], NODE_NAME_ACQSTART);
	}
}


void CKSJJAIDemoDlg::OnBnClickedButtonStop2()
{
	GetDlgItem(IDC_BUTTON_START2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP2)->EnableWindow(FALSE);
	J_STATUS_TYPE retval;

	if (m_hCam[1])
	{
		// Stop Acquision
		if (m_hCam[1]) {
			retval = J_Camera_ExecuteCommand(m_hCam[1], NODE_NAME_ACQSTOP);
		}

		EnterCriticalSection(&m_cs2);

		if (m_hThread[1])
		{
			// Close stream
			retval = J_Image_CloseStream(m_hThread[1]);
			m_hThread[1] = NULL;
			TRACE("Closed stream\n");
		}

		LeaveCriticalSection(&m_cs2);
		ResetEvent(m_hSetCaptureFovEvent2);
	}
}


void CKSJJAIDemoDlg::OnCbnSelchangeComboTriggermode()
{
	if (m_CameraCurSel == -1) return;
	CComboBox    *pComboBox = NULL;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMODE);
	int nValue = pComboBox->GetCurSel();
	J_Camera_SetValueInt64(m_hCam[m_CameraCurSel], NODE_NAME_TRIGGERMODE, nValue);
}


void CKSJJAIDemoDlg::OnCbnSelchangeComboTriggersource()
{
	if (m_CameraCurSel == -1) return;
	CComboBox    *pComboBox = NULL;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERSOURCE);
	int nValue = pComboBox->GetCurSel();
	J_Camera_SetValueInt64(m_hCam[m_CameraCurSel], NODE_NAME_TRIGGERSOURCE, nValue);
}


void CKSJJAIDemoDlg::OnCbnSelchangeComboCapture()
{
	if (m_CameraCurSel == -1) return;
	CComboBox    *pComboBox = NULL;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CAPTURE);
	int nValue = pComboBox->GetCurSel();
	if (nValue != 0)
	{
		nValue++;
	}

	J_Camera_SetValueInt64(m_hCam[m_CameraCurSel], NODE_NAME_ACQUISITIONMODE, nValue);
}


void CKSJJAIDemoDlg::OnBnClickedButtonSoftware()
{
	J_Camera_ExecuteCommand(m_hCam[m_CameraCurSel], NODE_NAME_ACQUISITIONSTART);
}

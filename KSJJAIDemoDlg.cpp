
// KSJJAIDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KSJJAIDemo.h"
#include "KSJJAIDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	m_CameraCurSel = -1;
}

void CKSJJAIDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &CKSJJAIDemoDlg::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &CKSJJAIDemoDlg::OnEnChangeEditHeight)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGERMODE, &CKSJJAIDemoDlg::OnCbnSelchangeComboTriggermode)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGERSOURCE, &CKSJJAIDemoDlg::OnCbnSelchangeComboTriggersource)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CKSJJAIDemoDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CKSJJAIDemoDlg::OnBnClickedButtonLoad)
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
	}

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
	OnBnClickedButtonStop();
}

void CKSJJAIDemoDlg::OnBnClickedButtonStart()
{
	if (m_CameraCurSel == -1) return;

	J_STATUS_TYPE   retval;
	int64_t int64Val;
	int64_t pixelFormat;
	int     bpp = 0;
	SIZE	ViewSize;
	POINT	TopLeft;

	
	if (m_hCam[m_CameraCurSel])
	{
		// Get Width from the camera
		retval = J_Camera_GetValueInt64(m_hCam[m_CameraCurSel], NODE_NAME_WIDTH, &int64Val);
		ViewSize.cx = (LONG)int64Val;     // Set window size cx

		// Get Height from the camera
		retval = J_Camera_GetValueInt64(m_hCam[m_CameraCurSel], NODE_NAME_HEIGHT, &int64Val);
		ViewSize.cy = (LONG)int64Val;     // Set window size cy

		// Set window position
		TopLeft.x = 10;
		TopLeft.y = 10 + (m_CameraCurSel*(ViewSize.cy + 25));

		// Get pixelformat from the camera
		retval = J_Camera_GetValueInt64(m_hCam[m_CameraCurSel], NODE_NAME_PIXELFORMAT, &int64Val);
		pixelFormat = int64Val;

		// Calculate number of bits (not bytes) per pixel using macro
		bpp = J_BitsPerPixel(pixelFormat);

		// Open view window
		retval = J_Image_OpenViewWindow(_T("Image View Window"), &TopLeft, &ViewSize, &m_hView[m_CameraCurSel]);
		if (retval != J_ST_SUCCESS) {
			AfxMessageBox(CString("Could not open view window!"), MB_OK | MB_ICONEXCLAMATION);
		}
		TRACE("Opening view window succeeded\n");

		// Open stream
		if (m_CameraCurSel == 0)
		{
			retval = J_Image_OpenStream(m_hCam[m_CameraCurSel], 0, reinterpret_cast<J_IMG_CALLBACK_OBJECT>(this), reinterpret_cast<J_IMG_CALLBACK_FUNCTION>(&CKSJJAIDemoDlg::StreamCBFunc1), &m_hThread[m_CameraCurSel], (ViewSize.cx*ViewSize.cy*bpp) / 8);
			if (retval != J_ST_SUCCESS) {
				AfxMessageBox(CString("Could not open stream!"), MB_OK | MB_ICONEXCLAMATION);
				OnBnClickedButtonStop();
				return;
			}
			TRACE("Opening stream succeeded\n");
		}
		else if (m_CameraCurSel == 1)
		{
			retval = J_Image_OpenStream(m_hCam[m_CameraCurSel], 0, reinterpret_cast<J_IMG_CALLBACK_OBJECT>(this), reinterpret_cast<J_IMG_CALLBACK_FUNCTION>(&CKSJJAIDemoDlg::StreamCBFunc2), &m_hThread[m_CameraCurSel], (ViewSize.cx*ViewSize.cy*bpp) / 8);
			if (retval != J_ST_SUCCESS) {
				AfxMessageBox(CString("Could not open stream!"), MB_OK | MB_ICONEXCLAMATION);
				OnBnClickedButtonStop();
				return;
			}
			TRACE("Opening stream succeeded\n");
		}
		// Start Acquision
		retval = J_Camera_ExecuteCommand(m_hCam[m_CameraCurSel], NODE_NAME_ACQSTART);
	}
	

	//EnableControls(TRUE, TRUE);
}

//--------------------------------------------------------------------------------------------------
// StreamCBFunc
//--------------------------------------------------------------------------------------------------
void CKSJJAIDemoDlg::StreamCBFunc1(J_tIMAGE_INFO * pAqImageInfo)
{
	// Update timestamp
	CString timestamp;
	timestamp.Format(_T("%016llx"), pAqImageInfo->iTimeStamp);
	if (m_hView[0])
	{
		// Shows image
		J_Image_ShowImage(m_hView[0], pAqImageInfo);
		//J_Image_SaveFileEx(pAqImageInfo, _T("c:\\Camera1.bmp"), J_FF_BMP);
	}
}

//--------------------------------------------------------------------------------------------------
// StreamCBFunc
//--------------------------------------------------------------------------------------------------
void CKSJJAIDemoDlg::StreamCBFunc2(J_tIMAGE_INFO * pAqImageInfo)
{
	// Update timestamp
	CString timestamp;
	timestamp.Format(_T("%016llx"), pAqImageInfo->iTimeStamp);

	if (m_hView[1])
	{
		// Shows image
		J_Image_ShowImage(m_hView[1], pAqImageInfo);
		//J_Image_SaveFileEx(pAqImageInfo, _T("c:\\Camera2.bmp"), J_FF_BMP);
	}
}


void CKSJJAIDemoDlg::OnBnClickedButtonStop()
{
	if (m_CameraCurSel == -1) return;

	J_STATUS_TYPE retval;

	if (m_hCam[m_CameraCurSel])
	{
		// Stop Acquision
		if (m_hCam[m_CameraCurSel]) {
			retval = J_Camera_ExecuteCommand(m_hCam[m_CameraCurSel], NODE_NAME_ACQSTOP);
		}

		if (m_hThread[m_CameraCurSel])
		{
			// Close stream
			retval = J_Image_CloseStream(m_hThread[m_CameraCurSel]);
			m_hThread[m_CameraCurSel] = NULL;
			TRACE("Closed stream\n");
		}

		if (m_hView[m_CameraCurSel])
		{
			// Close view window
			retval = J_Image_CloseViewWindow(m_hView[m_CameraCurSel]);
			m_hView[m_CameraCurSel] = NULL;
			TRACE("Closed view window\n");
		}
	}
	
}


void CKSJJAIDemoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	OnBnClickedButtonStop();
	CloseFactoryAndCamera();
}


void CKSJJAIDemoDlg::UpdateUi()
{
	J_STATUS_TYPE   retval;
	NODE_HANDLE hNode;
	int64_t int64Val, max, min;

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

#ifdef VERSION2
	retval = J_Camera_GetNodeByName(m_hCam[m_CameraCurSel], NODE_NAME_GAIN, &hNode);
	if (retval == J_ST_SUCCESS)
	{
		retval = J_Node_GetMinInt64(hNode, &min);
		retval = J_Node_GetMaxInt64(hNode, &max);
		J_Node_GetValueInt64(hNode, FALSE, &int64Val);

		CSpinButtonCtrl * pSpinCtrl = (CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_GAIN);
		pSpinCtrl->SetRange32(min, max);
		pSpinCtrl->SetPos32(int64Val);
	}
#else
	double dValue;
	TCHAR szTemp[32] = { 0 };
	retval = J_Camera_GetNodeByName(m_hCam[m_CameraCurSel], NODE_NAME_GAIN, &hNode);
	if (retval == J_ST_SUCCESS)
	{
		J_Node_GetValueDouble(hNode, FALSE, &dValue);
		_stprintf_s(szTemp, _T("%3f"), dValue);
		GetDlgItem(IDC_EDIT_GAIN)->SetWindowText(szTemp);
	}
#endif

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
#ifdef VERSION2
	int nValue = GetDlgItemInt(IDC_EDIT_GAIN);
	J_Camera_SetValueInt64(m_hCam[m_CameraCurSel], NODE_NAME_GAIN, nValue);
#else
	TCHAR szTemp[32] = { 0 };
	GetDlgItem(IDC_EDIT_GAIN)->GetWindowText(szTemp, 32);
	float fValue = atof(szTemp);
	J_Camera_SetValueDouble(m_hCam[m_CameraCurSel], NODE_NAME_GAIN, fValue);
#endif
}

void CKSJJAIDemoDlg::OnEnChangeEditWidth()
{
	if (m_CameraCurSel == -1) return;
	int nValue = GetDlgItemInt(IDC_EDIT_WIDTH);
	J_Camera_SetValueInt64(m_hCam[m_CameraCurSel], NODE_NAME_WIDTH, nValue);
}

void CKSJJAIDemoDlg::OnEnChangeEditHeight()
{
	if (m_CameraCurSel == -1) return;
	int nValue = GetDlgItemInt(IDC_EDIT_HEIGHT);
	J_Camera_SetValueInt64(m_hCam[m_CameraCurSel], NODE_NAME_HEIGHT, nValue);
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

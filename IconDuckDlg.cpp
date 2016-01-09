#include "stdafx.h"
#include "IconDuck.h"
#include "IconDuckDlg.h"
#include "UnicodeTextFile.h"

#define MIN_ICON_SIZE 16
#define MAX_ICON_SIZE 256

CIconDuckDlg::CIconDuckDlg(CWnd* pParent)
	: CDialogEx(CIconDuckDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_app_title = L"IconDuck";
  m_hDisplayIcon = NULL;
}

CIconDuckDlg::~CIconDuckDlg()
{
  if (NULL != m_hDisplayIcon)
    ::DestroyIcon(m_hDisplayIcon);
}

void CIconDuckDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_INFO, m_info);
  DDX_Control(pDX, IDC_BROWSE, m_browse);
  DDX_Control(pDX, IDC_DISPLAY, m_display);
  DDX_Control(pDX, IDC_PATH, m_path);
  DDX_Control(pDX, IDC_ICONSIZE, m_size);
  DDX_Control(pDX, IDC_SPIN, m_spin);
  DDX_Control(pDX, IDC_PREVIEW, m_preview);
  DDX_Control(pDX, IDC_GROUP, m_group);
}

BOOL CIconDuckDlg::PreTranslateMessage(MSG* pMsg)
{
  if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE))
    return TRUE;
  return CDialogEx::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CIconDuckDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDC_BROWSE, &CIconDuckDlg::OnClickedBrowse)
  ON_BN_CLICKED(IDC_DISPLAY, &CIconDuckDlg::OnClickedDisplay)
END_MESSAGE_MAP()

BOOL CIconDuckDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

  SetWindowText(m_app_title);

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

  HDC hDC = ::GetDC(NULL);
  m_dpi = abs(::GetDeviceCaps(hDC, LOGPIXELSX));
  ::ReleaseDC(NULL, hDC);
  m_scale = MulDiv(m_dpi, 100, 96);

  CString strInfo;
  strInfo += GetOperatingSystem();
  strInfo += L"\r\n";
  strInfo += GetVideoAdapter();
  strInfo += L"\r\n";
  strInfo += L"\r\n";
  strInfo += GetScreenDPI();
  strInfo += L"\r\n";
  strInfo += GetDpiScale();
  strInfo += L"\r\n";
  strInfo += GetDpiScaleFactor();
  strInfo += L"\r\n";
  strInfo += L"\r\n";
  strInfo += GetNormalIconSize();
  strInfo += L"\r\n";
  strInfo += GetSmallIconSize();
  m_info.SetWindowText(strInfo);

  CString strSize;
  strSize.Format(L"%d", MIN_ICON_SIZE);
  m_size.SetWindowText(strSize);
  m_spin.SetRange(MIN_ICON_SIZE, MAX_ICON_SIZE);

  m_path.SetFocus();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIconDuckDlg::OnPaint()
{
	if (IsIconic())
	{
    CPaintDC dc(this);
    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = ::GetSystemMetrics(SM_CXICON);
    int cyIcon = ::GetSystemMetrics(SM_CYICON);
		CRect rect;
    GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
  else
  {
    CDialogEx::OnPaint();
  }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIconDuckDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CIconDuckDlg::GetOperatingSystem()
{
  CString str;
  HINSTANCE hInstance = ::ShellExecute(0, L"open", L"cmd.exe", L"/c ver > %temp%\\ver.txt", L"", SW_HIDE);
  if ((int)hInstance > 32)
  {
    CUnicodeTextFile file;
    wchar_t szPath[MAX_PATH];
    GetTempPath(MAX_PATH, szPath);
    CString strTemp(szPath);
    strTemp += "\\ver.txt";
    if (file.OpenToRead(strTemp))
    {
      while (file.ReadString(str))
      {
        str.Trim();
        if (!str.IsEmpty())
          break;
      }
      file.Close();
    }
    ::DeleteFile(strTemp);
  }
  return str;
}

CString CIconDuckDlg::GetVideoAdapter()
{
  CString str;
  IDirect3D9* direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);
  if (0 != direct3D9) 
  {
    D3DADAPTER_IDENTIFIER9 adapterIdentifier;
    HRESULT hr = direct3D9->GetAdapterIdentifier(0, 0, &adapterIdentifier);
    if (SUCCEEDED(hr)) 
      str = adapterIdentifier.Description;
    direct3D9->Release();
  }
  return str;
}

int CIconDuckDlg::Scale(int x)
{
  return MulDiv(x, m_scale, 100);
}

CString CIconDuckDlg::GetScreenDPI()
{
  CString str;
  str.Format(L"Display DPI when application started: %d", m_dpi);
  return str;
}

CString CIconDuckDlg::GetDpiScale()
{
  CString str;
  str.Format(L"DPI scale: %d%%", m_scale);
  return str;
}

CString CIconDuckDlg::GetDpiScaleFactor()
{
  CString str;
  str.Format(L"DPI scale factor: %g\n", m_scale / 100.0);
  return str;
}

CString CIconDuckDlg::GetNormalIconSize()
{
  CString str;
  str.Format(L"Normal (32-pixel) icons are now %d pixel\n", ::GetSystemMetrics(SM_CXICON));
  return str;
}

CString CIconDuckDlg::GetSmallIconSize()
{
  CString str;
  str.Format(L"Small (16-pixel) icons are now %d pixel\n", ::GetSystemMetrics(SM_CXSMICON));
  return str;
}


void CIconDuckDlg::OnClickedBrowse()
{
  wchar_t szFilter[] = L"Icon Files(*.ico)|*.ico||";
  CFileDialog dialog(TRUE, L"ico", L"*.ico", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, this);
  if (dialog.DoModal() == IDOK)
  {
    CString filename = dialog.GetPathName();
    m_path.SetWindowText(filename);
  }
}

void CIconDuckDlg::OnClickedDisplay()
{
  CString strPath;
  m_path.GetWindowText(strPath);
  strPath.Trim();
  
  if (strPath.IsEmpty())
  {
    MessageBox(L"Please specify an icon file.", m_app_title, MB_ICONEXCLAMATION | MB_OK);
    m_path.SetFocus();
    m_path.SetSel(0, -1);
    return;
  }

  if (!PathFileExists(strPath))
  {
    MessageBox(L"Icon file not found.", m_app_title, MB_ICONEXCLAMATION | MB_OK);
    m_path.SetFocus();
    m_path.SetSel(0, -1);
    return;
  }

  CString strSize;
  m_size.GetWindowText(strSize);
  strSize.Trim();

  if (strSize.IsEmpty())
  {
    MessageBox(L"Please enter an icon size.", m_app_title, MB_ICONEXCLAMATION | MB_OK);
    m_size.SetFocus();
    m_size.SetSel(0, -1);
    return;
  }

  int nSize = MIN_ICON_SIZE;
  bool rc = ParseValue(strSize, nSize);
  if (!rc)
  {
    m_size.SetFocus();
    m_size.SetSel(0, -1);
    return;
  }

  if (NULL != m_hDisplayIcon)
    ::DestroyIcon(m_hDisplayIcon);
  m_hDisplayIcon = NULL;

  int nIconSize = Scale(nSize);
  HRESULT hr = ::LoadIconWithScaleDown(NULL, strPath, nIconSize, nIconSize, &m_hDisplayIcon);
  if (SUCCEEDED(hr))
  {
    CString strMsg;
    strMsg.Format(L"%d-pixel icon loaded with LoadIconWithScaleDown", nIconSize);
    m_group.SetWindowText(strMsg);
  }
  else
  {
    m_hDisplayIcon = (HICON)::LoadImage(NULL, strPath, IMAGE_ICON, nIconSize, nIconSize, LR_LOADFROMFILE);
    if (NULL != m_hDisplayIcon)
    {
      CString strMsg;
      strMsg.Format(L"%d-pixel icon loaded with LoadImage", nIconSize);
      m_group.SetWindowText(strMsg);
    }
  }

  if (NULL == m_hDisplayIcon)
  {
    MessageBox(L"Unable to load icon.", m_app_title, MB_ICONEXCLAMATION | MB_OK);
    return;
  }

  m_preview.SetIcon(m_hDisplayIcon);
  Invalidate();
}

bool CIconDuckDlg::ParseValue(const wchar_t* pszString, int& nValue)
{
  if (0 == pszString || 0 == pszString[0])
    return false;

  CString str1(pszString);
  str1.TrimLeft();
  str1.TrimRight();
  if (str1.IsEmpty())
    return false;

  if (str1.Find('.') != -1)
  {
    MessageBox(L"Please enter an icon size.", m_app_title, MB_ICONEXCLAMATION | MB_OK);
    return false;
  }

  double d = _wtof(str1);
  if (d < MIN_ICON_SIZE || d > MAX_ICON_SIZE)
  {
    CString strMsg;
    strMsg.Format(L"Please enter a number between %d and %d.", MIN_ICON_SIZE, MAX_ICON_SIZE);
    MessageBox(strMsg, m_app_title, MB_ICONEXCLAMATION | MB_OK);
    return false;
  }

  int n = (int)_wtof(str1);

  CString str2;
  str2.Format(L"%d", n);
  if (str1.CompareNoCase(str2))
  {
    MessageBox(L"Please enter an icon size.", m_app_title, MB_ICONEXCLAMATION | MB_OK);
    return false;
  }

  nValue = n;

  return true;
}
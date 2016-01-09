#pragma once

class CIconDuckDlg : public CDialogEx
{
public:
	CIconDuckDlg(CWnd* pParent = NULL);
  virtual ~CIconDuckDlg();

	enum { IDD = IDD_ICONDUCK_DIALOG };
  CEdit m_info;
  CEdit m_path;
  CEdit m_size;
  CSpinButtonCtrl m_spin;
  CButton m_browse;
  CButton m_display;
  CStatic m_preview;
  CStatic m_group;

  int Scale(int x);
  CString GetOperatingSystem();
  CString GetVideoAdapter();
  CString GetScreenDPI();
  CString GetDpiScale();
  CString GetDpiScaleFactor();
  CString GetNormalIconSize();
  CString GetSmallIconSize();

  bool ParseValue(const wchar_t* string, int& value);

protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnClickedBrowse();
  afx_msg void OnClickedDisplay();
  DECLARE_MESSAGE_MAP()

private:
  HICON m_hIcon;
  int m_dpi;
  int m_scale;
  CString m_app_title;
  HICON m_hDisplayIcon;
};

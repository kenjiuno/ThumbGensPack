
#include <afxwin.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlcom.h>
#include <tchar.h>
#include <shobjidl.h>
#include <algorithm>

#include <ximage.h>

namespace Ut {
	CString GetTempDir() {
		TCHAR tc[MAX_PATH] = {0};
		ATLVERIFY(0 != GetTempPath(MAX_PATH, tc));
		return tc;
	}
	CString Combine(CString s, CString t) {
		TCHAR tc[MAX_PATH] = {0};
		ATLVERIFY(NULL != PathCombine(tc, s, t));
		return tc;
	}
	CString NewGuidStr() {
		HRESULT hr;
		GUID guid;
		ATLVERIFY(SUCCEEDED(hr = CoCreateGuid(&guid)));
		WCHAR wc[MAX_PATH] = {0};
		ATLVERIFY(0 != StringFromGUID2(guid, wc, MAX_PATH));
		return wc;
	}
	CString GetExe() {
		TCHAR tc[MAX_PATH] = {0};
		ATLVERIFY(0 != GetModuleFileName(GetModuleHandle(NULL), tc, MAX_PATH));
		return tc;
	}
	CString GetDir(CString fp) {
		TCHAR tc[MAX_PATH] = {0};
		_tcscpy_s(tc, fp);
		ATLVERIFY(PathRemoveFileSpec(tc));
		return tc;
	}
	CString GetFileName(CString fp) {
		return PathFindFileName(fp);
	}
};

int _tmain(int argc, TCHAR **argv) {
	if (!AfxWinInit(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW))
		return 1;
	if (argc < 5) {
		_fputts(_T("pdf2bmp input.pdf output.bmp cx cy (flags) (iCurPage)\n"), stderr);
		return 1;
	}
	int cx = _ttoi(argv[3]);
	int cy = _ttoi(argv[4]);
	int flags = argc >= 6 ? _ttoi(argv[5]) : 0;
	int iCurPage = (argc >= 7) ? _ttoi(argv[6]) : 1;
	bool useOrigSize = 0 != (flags & IEIFLAG_ORIGSIZE);
	bool noBorder = 0 != (flags & IEIFLAG_NOBORDER);
	CString fpTmp = Ut::Combine(Ut::GetTempDir(), Ut::NewGuidStr());
	CString strApp = Ut::Combine(Ut::GetDir(Ut::GetExe()), _T("pdftoppm.exe"));
	CString strCmd;
	strCmd.Format(_T(" -jpeg -f %u -l %u -scale-to %d \"%s\" \"%s\"")
		, iCurPage
		, iCurPage
		, std::min<int>(cx, cy)
		, argv[1]
		, fpTmp
		);
	CString strCwd = Ut::GetDir(Ut::GetExe());
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags |= STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	BOOL f = CreateProcess(
		strApp,
		const_cast<LPTSTR>(static_cast<LPCTSTR>(strCmd)),
		NULL,
		NULL,
		false,
		0,
		NULL,
		strCwd,
		&si,
		&pi
		);
	if (!f)
		return 1;

	WaitForSingleObject(pi.hThread, INFINITE);
	CloseHandle(pi.hThread);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);

	CFileFind ff;
	BOOL fNext = ff.FindFile(fpTmp + "*.jpg");
	while (fNext) {
		fNext = ff.FindNextFile();

		CString fpJpeg = ff.GetFilePath();

		CxImage picIn;
		if (picIn.Load(fpJpeg, CXIMAGE_FORMAT_JPG)) {
			if (useOrigSize) {
				VERIFY(picIn.Save(argv[2], CXIMAGE_FORMAT_BMP));
			}
			else {
				CxImage picOut;
				RGBQUAD bkClr = {255,255,255, 0};
				if (picIn.Expand(cx, cy, bkClr, &picOut)) {
					VERIFY(picOut.Save(argv[2], CXIMAGE_FORMAT_BMP));
				}
			}
		}
		VERIFY(DeleteFile(fpJpeg));
	}

	return 0;
}
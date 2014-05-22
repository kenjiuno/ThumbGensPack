// Gene.h : CGene の宣言

#pragma once
#include "resource.h"       // メイン シンボル

#include "CmdThumbGen.h"

// CGene

class ATL_NO_VTABLE CGene :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CGene, &CLSID_Gene>,
	public IPersistFile,
	public IExtractImage2,
	public IRunnableTask,
	public IInitializeWithFile,
	public IInitializeWithStream,
	public IInitializeWithItem,
	//public IThumbnailProvider,
	public IGene,
	public ISetPage4ThumbnailProvider
{
public:
	CGene()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GENE)

BEGIN_COM_MAP(CGene)
	COM_INTERFACE_ENTRY(IPersist)
	COM_INTERFACE_ENTRY(IPersistFile)
	COM_INTERFACE_ENTRY(IExtractImage)
	COM_INTERFACE_ENTRY(IExtractImage2)
	COM_INTERFACE_ENTRY(IInitializeWithFile)
	//COM_INTERFACE_ENTRY(IInitializeWithStream)
	COM_INTERFACE_ENTRY(IInitializeWithItem)
	COM_INTERFACE_ENTRY(IThumbnailProvider)
	COM_INTERFACE_ENTRY(IRunnableTask)
	COM_INTERFACE_ENTRY(ISetPage4ThumbnailProvider)

	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pFTM)

	COM_INTERFACE_ENTRY(IGene)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	CComPtr<IUnknown> m_pFTM;
	CAtlArray<CString> m_aryfptmp;

	HRESULT FinalConstruct()
	{
		HRESULT hr;
		if (FAILED(hr = CoCreateFreeThreadedMarshaler(static_cast<IPersist *>(this), &m_pFTM)))
			return hr;

		iCurPage = 0;
		return S_OK;
	}

	void FinalRelease()
	{
		m_pFTM.Release();

		for (size_t x=0, cx=m_aryfptmp.GetCount(); x<cx; x++) {
			DeleteFile(m_aryfptmp[x]);
		}
		m_aryfptmp.RemoveAll();
	}

protected:
	DWORD iCurPage;

public:
	// IPersist
		virtual HRESULT STDMETHODCALLTYPE GetClassID( 
			/* [out] */ CLSID *pClassID)
		{
			if (pClassID == NULL)
				return E_POINTER;

			*pClassID = GetObjectCLSID();
			return S_OK;
		}

	// IPersistFile
		virtual HRESULT STDMETHODCALLTYPE IsDirty( void)
		{
			return S_FALSE;
		}
		
		virtual HRESULT STDMETHODCALLTYPE Load( 
			/* [in] */ LPCOLESTR pszFileName,
			/* [in] */ DWORD dwMode)
		{
			ObjectLock lck(this);

			if (0 != (dwMode & (STGM_CREATE|STGM_CONVERT|STGM_TRANSACTED|STGM_DELETEONRELEASE)))
				return STG_E_INVALIDPARAMETER;

			m_targetPath = pszFileName;
			m_state = Waiting;

			return S_OK;
		}

		CString m_targetPath;
		
		virtual HRESULT STDMETHODCALLTYPE Save( 
			/* [unique][in] */ LPCOLESTR pszFileName,
			/* [in] */ BOOL fRemember) 
		{
			return E_NOTIMPL;
		}
		
		virtual HRESULT STDMETHODCALLTYPE SaveCompleted( 
			/* [unique][in] */ LPCOLESTR pszFileName)
		{
			return S_OK;
		}
		
		virtual HRESULT STDMETHODCALLTYPE GetCurFile( 
			/* [out] */ LPOLESTR *ppszFileName)
		{
			ObjectLock lck(this);

			if (ppszFileName == NULL)
				return E_POINTER;

			*ppszFileName = NULL;

			if (m_targetPath.IsEmpty())
				return S_FALSE;

			*ppszFileName = SUt::CopyW(CT2W(m_targetPath));

			return (*ppszFileName != NULL) ? S_OK : E_OUTOFMEMORY;
		}

		class SUt {
		public:
			static LPWSTR CopyW(LPCWSTR pcwSrc) {
				LPWSTR pcwRet = NULL;
				if (pcwSrc != NULL) {
					size_t cb = wcslen(pcwSrc + 1) << 1;
					LPWSTR pcwRet = reinterpret_cast<LPWSTR>(CoTaskMemAlloc(cb));
					if (pcwRet != NULL) {
						memcpy(pcwRet, pcwSrc, cb);
					}
				}
				return pcwRet;
			}
		};

		SIZE m_size;
		DWORD m_flags;

    // IInitializeWithStream : public IUnknown
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [annotation][in] */ 
            __in  IStream *pstream,
            /* [annotation][in] */ 
            __in  DWORD grfMode)
		{
			if (pstream == NULL)
				return E_POINTER;

			CString strTempFile;
			HRESULT hr;
			if (FAILED(hr = RUt2::GetTempFilePath3(strTempFile)))
				return hr;

			STATSTG ss;
			if (FAILED(hr = pstream->Stat(&ss, STATFLAG_DEFAULT)))
				return hr;

			strTempFile += PathFindExtension(ss.pwcsName);

			m_aryfptmp.Add(strTempFile);

			BYTE buff[4096];
			CAtlFile f;
			if (SUCCEEDED(hr = f.Create(strTempFile, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS))) {
				while (true) {
					ULONG cbRead = 0;
					if (FAILED(hr = pstream->Read(buff, 4096, &cbRead)))
						break;
					if (cbRead == 0)
						break;
					if (FAILED(hr = f.Write(buff, cbRead)))
						return hr;
				}
				f.Close();
				return Initialize(strTempFile, grfMode);
			}
			return hr;
		}

    // IInitializeWithItem : public IUnknown
    public:
        virtual HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ __RPC__in_opt IShellItem *psi,
            /* [in] */ DWORD grfMode)
		{
			ObjectLock lck(this);

			HRESULT hr;

			LPWSTR pszName = NULL;
			if (FAILED(hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszName)))
				return hr;

			CString strName = pszName;

			CoTaskMemFree(pszName);

			return Initialize(strName, grfMode);
		}

	// IInitializeWithFile : public IUnknown
	public:
		virtual HRESULT STDMETHODCALLTYPE Initialize( 
			/* [string][in] */ __RPC__in_string LPCWSTR pszFilePath,
			/* [in] */ DWORD grfMode)
		{
			ObjectLock lck(this);

			if (pszFilePath == NULL)
				return E_POINTER;

			m_targetPath = pszFilePath;
			m_state = Waiting;

			return S_OK;
		}

	// IExtractImage
		virtual HRESULT STDMETHODCALLTYPE GetLocation( 
			/* [size_is][out] */ LPWSTR pszPathBuffer,
			/* [in] */ DWORD cch,
			/* [unique][out][in] */ DWORD *pdwPriority,
			/* [in] */ const SIZE *prgSize,
			/* [in] */ DWORD dwRecClrDepth,
			/* [out][in] */ DWORD *pdwFlags)
		{
			ObjectLock lck(this);

			if (pszPathBuffer != NULL) {
				wcsncpy_s(pszPathBuffer, cch, CT2W(m_targetPath), cch);
			}
			if (pdwPriority == NULL)
				return E_POINTER;
			if (prgSize == NULL)
				return E_POINTER;
			if (pdwFlags == NULL)
				return E_POINTER;

			m_size = *prgSize;
			m_flags = *pdwFlags;

			if (0 != (m_flags & IEIFLAG_ASYNC))
				return E_PENDING;
			return S_OK;
		}

		typedef enum {
			Waiting, Running, Done,
		} State;

		State m_state;
		
		virtual HRESULT STDMETHODCALLTYPE Extract( 
			/* [out] */ HBITMAP *phBmpThumbnail)
		{
			ObjectLock lck(this);
			HRESULT hr;
			int errc;

			if (phBmpThumbnail == NULL)
				return E_POINTER;

			if (m_targetPath.IsEmpty())
				return E_FAIL;

			LPCTSTR pszExt = PathFindExtension(m_targetPath);
			if (pszExt == NULL)
				pszExt = _T(".");
			CString strCmdlForm;
			if (FAILED(hr = RUt::GetCommandLineForm(pszExt, strCmdlForm)))
				return hr;
			CString strTempFile;
			if (FAILED(hr = RUt::GetTempFilePath2(strTempFile)))
				return hr;
			DelTmp dt(strTempFile);

			TCHAR tcWorkdir[MAX_PATH] = {0};
			if (0 == GetTempPath(MAX_PATH, tcWorkdir))
				return errc = GetLastError(), HRESULT_FROM_WIN32(errc);

			DWORD_PTR pvArgs[] = {
				reinterpret_cast<DWORD_PTR>(static_cast<LPCTSTR>(m_targetPath)),
				reinterpret_cast<DWORD_PTR>(static_cast<LPCTSTR>(strTempFile)),
				m_size.cx,
				m_size.cy,
				m_flags,
				0,
				0,
				0,
				0,
				0,
			};

			TCHAR szCmdRun[2000 +1] = {0};
			if (0 == (FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY, strCmdlForm, 0, 0, szCmdRun, 2000, (va_list *)pvArgs)))
				return errc = GetLastError(), HRESULT_FROM_WIN32(errc);

			STARTUPINFO si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_SHOWMINNOACTIVE;

			PROCESS_INFORMATION pi;
			ZeroMemory(&pi, sizeof(pi));

			BOOL fCreated = CreateProcess(
				NULL,
				szCmdRun,
				NULL,
				NULL,
				false,
				0 |IDLE_PRIORITY_CLASS |CREATE_NO_WINDOW,
				NULL,
				tcWorkdir,
				&si,
				&pi
				);
			if (!fCreated)
				return errc = GetLastError(), HRESULT_FROM_WIN32(errc);

			m_state = Running;

			WaitForSingleObject(pi.hProcess, INFINITE);

			m_state = Done;

			DWORD errlv = 0;
			if (!GetExitCodeProcess(pi.hProcess, &errlv))
				errlv = 1;

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			HBITMAP hbm = NULL;

			if (errlv == 0) {
				hbm = reinterpret_cast<HBITMAP>(LoadImage(NULL, strTempFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE|LR_VGACOLOR));
			}

			*phBmpThumbnail = hbm;
			return (*phBmpThumbnail != NULL) ? S_OK : E_FAIL;
		}

		class DelTmp {
		protected:
			CString fpTmp;
		public:
			DelTmp(CString fpTmp): fpTmp(fpTmp) {

			}
			~DelTmp() {
				ATLVERIFY(DeleteFile(fpTmp));
			}
		};

		class RUt2 {
		public:
			static HRESULT GetTempFilePath3(CString &pStr) {
				int errc;
				TCHAR szDir[MAX_PATH] = {0};
				if (0 == GetTempPath(MAX_PATH, szDir))
					return errc = GetLastError(), HRESULT_FROM_WIN32(errc);
				GUID tmpId;
				HRESULT hr;
				if (FAILED(hr = CoCreateGuid(&tmpId)))
					return hr;
				CString strId;
				for (int x = 0; x < 16; x++)
					strId.AppendFormat(_T("%02x"), ((PBYTE)&tmpId)[x]);
				TCHAR szPath[MAX_PATH] = {0};
				PathCombine(szPath, szDir, CW2T(strId));

				pStr = szPath;
				return S_OK;
			}
		};

		class RUt {
		public:
			static HRESULT GetTempFilePath(CString &pStr) {
				int errc;
				TCHAR szDir[MAX_PATH] = {0};
				if (0 == GetTempPath(MAX_PATH, szDir))
					return errc = GetLastError(), HRESULT_FROM_WIN32(errc);
				TCHAR szPath[MAX_PATH] = {0};
				if (0 == GetTempFileName(szDir, _T("tmp"), 0, szPath))
					return errc = GetLastError(), HRESULT_FROM_WIN32(errc);

				pStr = szPath;
				return S_OK;
			}

			static HRESULT GetTempFilePath2(CString &pStr) {
				int errc;
				TCHAR szDir[MAX_PATH] = {0};
				if (0 == GetTempPath(MAX_PATH, szDir))
					return errc = GetLastError(), HRESULT_FROM_WIN32(errc);
				GUID tmpId;
				HRESULT hr;
				if (FAILED(hr = CoCreateGuid(&tmpId)))
					return hr;
				WCHAR wcId[MAX_PATH] = {0};
				if (0 == StringFromGUID2(tmpId, wcId, MAX_PATH))
					return E_OUTOFMEMORY;
				TCHAR szPath[MAX_PATH] = {0};
				PathCombine(szPath, szDir, CW2T(wcId));

				pStr = szPath;
				return S_OK;
			}

			static HRESULT GetCommandLineForm(LPCTSTR pszExt, CString &pStr, LPCTSTR pszVerb = NULL) {
				LONG lerr;
				{
					CRegKey rkExt;
					CString strKey; strKey.Format(_T("Software\\HIRAOKA HYPERS TOOLS, Inc.\\CmdThumbGen\\FileExts\\%s")
						, pszExt);
					if (0 == (lerr = rkExt.Open(HKEY_CURRENT_USER, strKey, KEY_READ))) {
						TCHAR szRet[256 +1] = {0};
						ULONG cchRet = 256;
						if (0 == (lerr = rkExt.QueryStringValue(pszVerb, szRet, &cchRet))) {
							pStr = CString(szRet, cchRet);
							return NOERROR;
						}
					}
				}

				{
					CRegKey rkExt;
					CString strKey; strKey.Format(_T("Software\\HIRAOKA HYPERS TOOLS, Inc.\\CmdThumbGen\\FileExts\\%s")
						, pszExt);
					if (0 == (lerr = rkExt.Open(HKEY_LOCAL_MACHINE, strKey, KEY_READ))) {
						TCHAR szRet[256 +1] = {0};
						ULONG cchRet = 256;
						if (0 == (lerr = rkExt.QueryStringValue(pszVerb, szRet, &cchRet))) {
							pStr = CString(szRet, cchRet);
							return NOERROR;
						}
					}
				}

				return E_NOT_SET;
			}
		};

	// IExtractImage2
		virtual HRESULT STDMETHODCALLTYPE GetDateStamp( 
			/* [out] */ FILETIME *pDateStamp)
		{
			ObjectLock lck(this);
			HRESULT hr;
			int errc;
			CAtlFile f;
			if (SUCCEEDED(hr = f.Create(m_targetPath, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING))) {
				if (GetFileTime(f, NULL, NULL, pDateStamp)) {
					f.Close();
					return S_OK;
				}
				else { errc = GetLastError(), hr = HRESULT_FROM_WIN32(errc); }
			}
			return hr;
		}

	// IRunnableTask
		virtual HRESULT STDMETHODCALLTYPE Run( void)
		{
			return NOERROR;
		}
		
		virtual HRESULT STDMETHODCALLTYPE Kill( 
			/* [annotation][in] */ 
			__in  BOOL bWait)
		{
			return E_NOTIMPL;
		}
		
		virtual HRESULT STDMETHODCALLTYPE Suspend( void)
		{
			return E_NOTIMPL;
		}
		
		virtual HRESULT STDMETHODCALLTYPE Resume( void)
		{
			return E_NOTIMPL;
		}
		
		virtual ULONG STDMETHODCALLTYPE IsRunning( void)
		{
			switch (m_state) {
				case Running:
					return IRTIR_TASK_RUNNING;
				case Done:
					return IRTIR_TASK_FINISHED;
			}

			return IRTIR_TASK_NOT_RUNNING;
		}

	// IThumbnailProvider : public IUnknown
	public:
		virtual HRESULT STDMETHODCALLTYPE GetThumbnail( 
			/* [in] */ UINT cx,
			/* [out] */ __RPC__deref_out_opt HBITMAP *phbmp,
			/* [out] */ __RPC__out WTS_ALPHATYPE *pdwAlpha)
		{
			ObjectLock lck(this);
			HRESULT hr;
			int errc;

			if (phbmp == NULL)
				return E_POINTER;

			if (m_targetPath.IsEmpty())
				return E_FAIL;

			LPCTSTR pszExt = PathFindExtension(m_targetPath);
			if (pszExt == NULL)
				pszExt = _T(".");
			CString strCmdlForm;
			if (FAILED(hr = RUt::GetCommandLineForm(pszExt, strCmdlForm, (iCurPage == 0) ? NULL : _T("MP"))))
				return hr;
			CString strTempFile;
			if (FAILED(hr = RUt::GetTempFilePath2(strTempFile)))
				return hr;
			DelTmp dt(strTempFile);

			TCHAR tcWorkdir[MAX_PATH] = {0};
			if (0 == GetTempPath(MAX_PATH, tcWorkdir))
				return errc = GetLastError(), HRESULT_FROM_WIN32(errc);

			// if iCurPage == 0, output first page with verb "".
			// if iCurPage == 1, output first page with verb "MP".
			// if iCurPage == 2, output second page with verb "MP".
			// if iCurPage == 3, output third page with verb "MP".

			DWORD_PTR pvArgs[] = {
				reinterpret_cast<DWORD_PTR>(static_cast<LPCTSTR>(m_targetPath)),
				reinterpret_cast<DWORD_PTR>(static_cast<LPCTSTR>(strTempFile)),
				cx,
				cx,
				IEIFLAG_ORIGSIZE, // m_flags
				iCurPage,
				0,
				0,
				0,
				0,
			};

			TCHAR szCmdRun[2000 +1] = {0};
			if (0 == (FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY, strCmdlForm, 0, 0, szCmdRun, 2000, (va_list *)pvArgs)))
				return errc = GetLastError(), HRESULT_FROM_WIN32(errc);

			STARTUPINFO si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_SHOWMINNOACTIVE;

			PROCESS_INFORMATION pi;
			ZeroMemory(&pi, sizeof(pi));

			BOOL fCreated = CreateProcess(
				NULL,
				szCmdRun,
				NULL,
				NULL,
				false,
				0 |IDLE_PRIORITY_CLASS |CREATE_NO_WINDOW,
				NULL,
				tcWorkdir,
				&si,
				&pi
				);
			if (!fCreated)
				return errc = GetLastError(), HRESULT_FROM_WIN32(errc);

			m_state = Running;

			WaitForSingleObject(pi.hProcess, INFINITE);

			m_state = Done;

			DWORD errlv = 0;
			if (!GetExitCodeProcess(pi.hProcess, &errlv))
				errlv = 1;

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			HBITMAP hbm = NULL;

			if (errlv == 0) {
				hbm = reinterpret_cast<HBITMAP>(LoadImage(NULL, strTempFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE|LR_VGACOLOR));

			}

			if (pdwAlpha != NULL) {
				*pdwAlpha = WTSAT_UNKNOWN;

				if (hbm != NULL) {
					DIBSECTION dib;
					ZeroMemory(&dib, sizeof(dib));
					if (GetObject(hbm, sizeof(dib), &dib) == sizeof(dib)) {
						switch (dib.dsBmih.biBitCount) {
							case 32:
								*pdwAlpha = WTSAT_ARGB;
								break;
							default:
								*pdwAlpha = WTSAT_RGB;
								break;
						}
					}
				}
			}

			*phbmp = hbm;
			return (*phbmp != NULL) ? S_OK : E_FAIL;
		}

    // ISetPage4ThumbnailProvider : public IUnknown
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPageCount( 
            /* [out] */ DWORD *pcPages)
		{
			ObjectLock lck(this);
			HRESULT hr;
			int errc;

			if (pcPages == NULL)
				return E_POINTER;

			if (m_targetPath.IsEmpty())
				return E_FAIL;

			LPCTSTR pszExt = PathFindExtension(m_targetPath);
			if (pszExt == NULL)
				pszExt = _T(".");
			CString strCmdlForm;
			if (FAILED(hr = RUt::GetCommandLineForm(pszExt, strCmdlForm, _T("GPC"))))
				return hr;
			CString strTempFile;
			if (FAILED(hr = RUt::GetTempFilePath2(strTempFile)))
				return hr;
			DelTmp dt(strTempFile);

			TCHAR tcWorkdir[MAX_PATH] = {0};
			if (0 == GetTempPath(MAX_PATH, tcWorkdir))
				return errc = GetLastError(), HRESULT_FROM_WIN32(errc);

			DWORD_PTR pvArgs[] = {
				reinterpret_cast<DWORD_PTR>(static_cast<LPCTSTR>(m_targetPath)),
				reinterpret_cast<DWORD_PTR>(static_cast<LPCTSTR>(strTempFile)), // width for output
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
			};

			TCHAR szCmdRun[2000 +1] = {0};
			if (0 == (FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY, strCmdlForm, 0, 0, szCmdRun, 2000, (va_list *)pvArgs)))
				return errc = GetLastError(), HRESULT_FROM_WIN32(errc);

			STARTUPINFO si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_SHOWMINNOACTIVE;

			PROCESS_INFORMATION pi;
			ZeroMemory(&pi, sizeof(pi));

			BOOL fCreated = CreateProcess(
				NULL,
				szCmdRun,
				NULL,
				NULL,
				false,
				0 |IDLE_PRIORITY_CLASS |CREATE_NO_WINDOW,
				NULL,
				tcWorkdir,
				&si,
				&pi
				);
			if (!fCreated)
				return errc = GetLastError(), HRESULT_FROM_WIN32(errc);

			WaitForSingleObject(pi.hProcess, INFINITE);

			DWORD errlv = 0;
			if (!GetExitCodeProcess(pi.hProcess, &errlv))
				errlv = 1;

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			if (errlv == 0) {
				CString text = FUt::ReadFile(strTempFile, 1000);
				if (text.GetLength() != 0) {
					*pcPages = _ttoi(text);
					return S_OK;
				}
				return E_UNEXPECTED;
			}

			return E_FAIL;
		}
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetActivePage( 
            /* [in] */ DWORD iPage)
		{
			iCurPage = iPage;
			return S_OK;
		}

		class FUt {
		public:
			static CString ReadFile(LPCTSTR pszTemp, int cbMaxRead, LPCTSTR pszDef = NULL) {
				FILE *f = NULL;
				if (0 == _tfopen_s(&f, pszTemp, _T("rb"))) {
					CHeapPtr<BYTE> buff;
					if (buff.AllocateBytes(cbMaxRead +1)) {
						size_t r = fread(buff, 1, cbMaxRead, f);
						if (r != 0) {
							buff[r] = 0;
							fclose(f);
							return CString(CA2T(reinterpret_cast<LPSTR>(static_cast<LPBYTE>(buff))));
						}
					}
					fclose(f);
				}
				return pszDef;
			}
		};
};

OBJECT_ENTRY_AUTO(__uuidof(Gene), CGene)

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

	HBITMAP DecodePicture(CString filePath);

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
			HRESULT hr;

			if (pstream == NULL)
				return E_POINTER;

			STATSTG stStat = {0};
			if (FAILED(hr = pstream->Stat(&stStat, STATFLAG_DEFAULT)))
				return hr;

			TempFile tempFile(stStat.pwcsName);
			CString tempFilePath = tempFile.Detach();

			m_aryfptmp.Add(tempFilePath);

			BYTE buff[4096];
			CAtlFile f;
			if (SUCCEEDED(hr = f.Create(tempFilePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS))) {
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
				return Initialize(tempFilePath, grfMode);
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
			int errCode;

			if (phBmpThumbnail == NULL) {
				return E_POINTER;
			}

			if (m_targetPath.IsEmpty()) {
				return E_FAIL;
			}

			LPCTSTR pszExt = PathFindExtension(m_targetPath);
			if (pszExt == NULL) {
				pszExt = _T(".");
			}
			GetFileExtsReg fileExt(pszExt);
			if (!fileExt.valid) {
				return E_FAIL;
			}
			TempFile tempFile(fileExt.suffix);
			if (!tempFile.valid) {
				return E_FAIL;
			}

			TCHAR tcWorkdir[MAX_PATH] = {0};
			if (0 == GetTempPath(MAX_PATH, tcWorkdir)) {
				errCode = GetLastError();
				return HRESULT_FROM_WIN32(errCode);
			}

			DWORD_PTR pvArgs[] = {
				reinterpret_cast<DWORD_PTR>(static_cast<LPCTSTR>(m_targetPath)),
				reinterpret_cast<DWORD_PTR>(static_cast<LPCTSTR>(tempFile.filePath)),
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
			if (0 == (FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY, fileExt.commandLine, 0, 0, szCmdRun, 2000, (va_list *)pvArgs))) {
				errCode = GetLastError();
				return HRESULT_FROM_WIN32(errCode);
			}

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
			if (!fCreated) {
				errCode = GetLastError();
				return HRESULT_FROM_WIN32(errCode);
			}

			m_state = Running;

			WaitForSingleObject(pi.hProcess, INFINITE);

			m_state = Done;

			DWORD errlv = 0;
			if (!GetExitCodeProcess(pi.hProcess, &errlv)) {
				errlv = 1;
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			HBITMAP hbm = NULL;

			if (errlv == 0) {
				hbm = DecodePicture(tempFile.filePath);
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

		struct TempFile {
			CString filePath;
			bool valid;

			TempFile(CString suffix): valid(false) {
				TCHAR szDir[MAX_PATH] = {0};
				if (0 != GetTempPath(MAX_PATH, szDir)) {
					GUID tmpId;
					HRESULT hr;
					if (SUCCEEDED(hr = CoCreateGuid(&tmpId))) {
						CString strId;
						strId.AppendFormat(_T("%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X")
							, 0U +tmpId.Data1
							, 0U +tmpId.Data2
							, 0U +tmpId.Data3
							, 0U +tmpId.Data4[0]
							, 0U +tmpId.Data4[1]
							, 0U +tmpId.Data4[2]
							, 0U +tmpId.Data4[3]
							, 0U +tmpId.Data4[4]
							, 0U +tmpId.Data4[5]
							, 0U +tmpId.Data4[6]
							, 0U +tmpId.Data4[7]
							);
						TCHAR szPath[MAX_PATH] = {0};
						if (NULL != PathCombine(szPath, szDir, CW2T(strId))) {
							if (0 == _tcscat_s(szPath, suffix)) {
								valid = true;
								filePath = szPath;
								return;
							}
						}
					}
				}
			}

			CString Detach() {
				CString ret = filePath;
				filePath.Empty();
				valid = false;
				return ret;
			}

			~TempFile() {
				if (valid) {
					ATLVERIFY(DeleteFile(filePath));
				}
			}
		};

		struct GetFileExtsReg {
			CString commandLine;
			CString suffix;
			bool valid;

			GetFileExtsReg(LPCTSTR pszExt, LPCTSTR pszVerb = NULL) {
				CString strKey;
				strKey.Format(_T("Software\\HIRAOKA HYPERS TOOLS, Inc.\\CmdThumbGen\\FileExts\\%s"), pszExt);

				LONG errCode;
				bool has = false;
				if (!has) {
					// HKCU
					CRegKey rkExt;
					if (0 == (errCode = rkExt.Open(HKEY_CURRENT_USER, strKey, KEY_READ))) {
						TCHAR szRet[256 +1] = {0};
						ULONG cchRet;

						cchRet = 256;
						if (0 == (errCode = rkExt.QueryStringValue(pszVerb, szRet, &cchRet))) {
							commandLine = CString(szRet, cchRet);
							has = true;

							cchRet = 256;
							if (0 == (errCode = rkExt.QueryStringValue(_T("Suffix"), szRet, &cchRet))) {
								suffix = CString(szRet, cchRet);
							}
						}
					}
				}

				if (!has) {
					// HKLM
					CRegKey rkExt;
					if (0 == (errCode = rkExt.Open(HKEY_LOCAL_MACHINE, strKey, KEY_READ))) {
						TCHAR szRet[256 +1] = {0};
						ULONG cchRet;

						cchRet = 256;
						if (0 == (errCode = rkExt.QueryStringValue(pszVerb, szRet, &cchRet))) {
							commandLine = CString(szRet, cchRet);
							has = true;

							cchRet = 256;
							if (0 == (errCode = rkExt.QueryStringValue(_T("Suffix"), szRet, &cchRet))) {
								suffix = CString(szRet, cchRet);
							}
						}
					}
				}

				valid = has;
			}
		};

	// IExtractImage2
		virtual HRESULT STDMETHODCALLTYPE GetDateStamp( 
			/* [out] */ FILETIME *pDateStamp)
		{
			ObjectLock lck(this);
			HRESULT hr;
			int errCode;
			CAtlFile f;
			if (SUCCEEDED(hr = f.Create(m_targetPath, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING))) {
				if (GetFileTime(f, NULL, NULL, pDateStamp)) {
					f.Close();
					return S_OK;
				}
				else {
					errCode = GetLastError();
					hr = HRESULT_FROM_WIN32(errCode);
				}
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
			int errCode;

			if (phbmp == NULL) {
				return E_POINTER;
			}

			if (m_targetPath.IsEmpty()) {
				return E_FAIL;
			}

			LPCTSTR pszExt = PathFindExtension(m_targetPath);
			if (pszExt == NULL) {
				pszExt = _T(".");
			}
			GetFileExtsReg fileExt(pszExt, (iCurPage == 0) ? NULL : _T("MP"));
			if (!fileExt.valid) {
				return E_FAIL;
			}
			TempFile tempFile(fileExt.suffix);
			if (!tempFile.valid) {
				return E_FAIL;
			}

			TCHAR tcWorkdir[MAX_PATH] = {0};
			if (0 == GetTempPath(MAX_PATH, tcWorkdir)) {
				errCode = GetLastError();
				return HRESULT_FROM_WIN32(errCode);
			}

			// if iCurPage == 0, output first page with verb "".
			// if iCurPage == 1, output first page with verb "MP".
			// if iCurPage == 2, output second page with verb "MP".
			// if iCurPage == 3, output third page with verb "MP".

			DWORD_PTR pvArgs[] = {
				reinterpret_cast<DWORD_PTR>(static_cast<LPCTSTR>(m_targetPath)),
				reinterpret_cast<DWORD_PTR>(static_cast<LPCTSTR>(tempFile.filePath)),
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
			if (0 == (FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY, fileExt.commandLine, 0, 0, szCmdRun, 2000, (va_list *)pvArgs))) {
				errCode = GetLastError();
				return HRESULT_FROM_WIN32(errCode);
			}

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
			if (!fCreated) {
				errCode = GetLastError();
				return HRESULT_FROM_WIN32(errCode);
			}

			m_state = Running;

			WaitForSingleObject(pi.hProcess, INFINITE);

			m_state = Done;

			DWORD errlv = 0;
			if (!GetExitCodeProcess(pi.hProcess, &errlv)) {
				errlv = 1;
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			HBITMAP hbm = NULL;

			if (errlv == 0) {
				hbm = DecodePicture(tempFile.filePath);
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
			int errCode;

			if (pcPages == NULL) {
				return E_POINTER;
			}

			if (m_targetPath.IsEmpty()) {
				return E_FAIL;
			}

			LPCTSTR pszExt = PathFindExtension(m_targetPath);
			if (pszExt == NULL) {
				pszExt = _T(".");
			}
			GetFileExtsReg fileExt(pszExt, _T("GPC"));
			if (!fileExt.valid) {
				return E_FAIL;
			}
			TempFile tempFile(fileExt.suffix);
			if (!tempFile.valid) {
				return E_FAIL;
			}

			TCHAR tcWorkdir[MAX_PATH] = {0};
			if (0 == GetTempPath(MAX_PATH, tcWorkdir)) {
				errCode = GetLastError();
				return HRESULT_FROM_WIN32(errCode);
			}

			DWORD_PTR pvArgs[] = {
				reinterpret_cast<DWORD_PTR>(static_cast<LPCTSTR>(m_targetPath)),
				reinterpret_cast<DWORD_PTR>(static_cast<LPCTSTR>(tempFile.filePath)), // width for output
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
			if (0 == (FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY, fileExt.commandLine, 0, 0, szCmdRun, 2000, (va_list *)pvArgs))) {
				errCode = GetLastError();
				return HRESULT_FROM_WIN32(errCode);
			}

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
			if (!fCreated) {
				errCode = GetLastError();
				return HRESULT_FROM_WIN32(errCode);
			}

			WaitForSingleObject(pi.hProcess, INFINITE);

			DWORD errlv = 0;
			if (!GetExitCodeProcess(pi.hProcess, &errlv)) {
				errlv = 1;
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			if (errlv == 0) {
				CString text = FUt::ReadFile(tempFile.filePath, 1000);
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

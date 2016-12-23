// Gene.cpp : CGene の実装

#include "stdafx.h"
#include "Gene.h"

// CGene

HBITMAP CGene::DecodePicture(CString filePath) {
	HRESULT hr;
	HBITMAP bitmapHandle = NULL;
#if WIC
	if (bitmapHandle == NULL) {
		// Windows Imaging Component and Direct2D Image Viewer Win32 Sample\C++\WicViewerD2D.cpp
		CComPtr<IWICImagingFactory> pWICFactory;
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pWICFactory)
			);
		if (SUCCEEDED(hr)) {
			CComPtr<IWICBitmapDecoder> pDecoder;
			hr = pWICFactory->CreateDecoderFromFilename(
				filePath,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnDemand,
				&pDecoder
				);

			if (SUCCEEDED(hr)) {
				CComPtr<IWICBitmapFrameDecode> pFrame;
				hr = pDecoder->GetFrame(0, &pFrame);
				if (SUCCEEDED(hr)) {
					CComPtr<IWICFormatConverter> pConvertedSourceBitmap;
					hr = pWICFactory->CreateFormatConverter(&pConvertedSourceBitmap);
					if (SUCCEEDED(hr)) {
						hr = pConvertedSourceBitmap->Initialize(
							pFrame,
							GUID_WICPixelFormat32bppBGRA,
							WICBitmapDitherTypeNone,
							NULL,
							0.f,
							WICBitmapPaletteTypeCustom
							);
						if (SUCCEEDED(hr)) {
							UINT cx = 0, cy = 0;
							hr = pConvertedSourceBitmap->GetSize(&cx, &cy);
							if (SUCCEEDED(hr)) {
								struct CompatibleDCHandle {
									HDC hMemDC;

									CompatibleDCHandle(): hMemDC(CreateCompatibleDC(NULL)) { }

									~CompatibleDCHandle() {
										if (hMemDC != NULL) {
											DeleteDC(hMemDC);
										}
									}
								};
								CompatibleDCHandle compatDC;
								BITMAPINFO bi = {
									sizeof(BITMAPINFOHEADER),
									(LONG)cx,
									-(LONG)cy,
									1,
									32,
									BI_RGB,
									4 * cx * cy
								};
								struct DIBSectionHandle {
									HBITMAP bitmapHandle;
									void *pvBits;

									DIBSectionHandle(CompatibleDCHandle &compatDC, BITMAPINFO &bi) {
										bitmapHandle = CreateDIBSection(
											compatDC.hMemDC, &bi, DIB_RGB_COLORS, &pvBits, NULL, 0
											);
									}

									~DIBSectionHandle() {
										if (bitmapHandle != NULL) {
											DeleteObject(bitmapHandle);
										}
									}

									HBITMAP Detach() {
										HBITMAP hbm = bitmapHandle;
										bitmapHandle = NULL;
										return hbm;
									}

									bool Has() const {
										return bitmapHandle != NULL;
									}
								};
								DIBSectionHandle bitmap(compatDC, bi);
								if (bitmap.Has()) {
									WICRect bitmapRect = {0, 0, cx, cy};
									hr = pConvertedSourceBitmap->CopyPixels(
										&bitmapRect,
										4 * cx,
										bi.bmiHeader.biSizeImage,
										reinterpret_cast<PBYTE>(bitmap.pvBits)
										);
									if (SUCCEEDED(hr)) {
										bitmapHandle = bitmap.Detach();
									}
								}
							}
						}
					}
				}
			}
		}
	}
#endif
#if 1 // GDI_LOADIMAGE
	if (bitmapHandle == NULL) {
		bitmapHandle = reinterpret_cast<HBITMAP>(LoadImage(NULL, filePath, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE|LR_VGACOLOR));
	}
#endif
	return bitmapHandle;
}

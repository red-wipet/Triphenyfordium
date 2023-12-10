#include <windows.h>
#include <tchar.h>
#include <ctime>
#include <iostream>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <time.h>
#define M_PI   3.14159265358979323846264338327950288
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	//These HSL functions was made by Wipet, credits to him!
	//OBS: I used it in 3 payloads

	//Btw ArTicZera created HSV functions, but it sucks unfortunatelly
	//So I didn't used in this malware.

	HSL rgb2hsl(RGBQUAD rgb)
	{
		HSL hsl;

		BYTE r = rgb.rgbRed;
		BYTE g = rgb.rgbGreen;
		BYTE b = rgb.rgbBlue;

		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;

		FLOAT rgbMin = min(min(_r, _g), _b);
		FLOAT rgbMax = max(max(_r, _g), _b);

		FLOAT fDelta = rgbMax - rgbMin;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f)
		{
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
		RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) { //Credits to Void_/GetMBR
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}

DWORD WINAPI fallingscreen(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int b; // [rsp+C0h] [rbp-10h]
	int time; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 255;
	g = 255;
	b = 255;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left;
		pt[0].y = rect.top + 10 * time;
		pt[1].x = rect.right - 2 * time;
		pt[1].y = rect.top;
		pt[2].x = 2 * time + rect.left;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		brush = CreateSolidBrush(((unsigned __int8)g << 8) | (unsigned __int8)r | ((unsigned __int8)b << 16));
		SelectObject(hdc, brush);
		w = 0;
		h = 0;
		StretchBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, sw, sh, 0xC000CAu);
		DeleteObject(brush);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 26)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 30)
				time = 0;
		}
	}
}
DWORD WINAPI payload2(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int b; // [rsp+C0h] [rbp-10h]
	int time; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 255;
	g = 255;
	b = 255;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left;
		pt[0].y = rect.top + 99 * time;
		pt[1].x = rect.right - 9 * time;
		pt[1].y = rect.top;
		pt[2].x = 2 * time + rect.left;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		brush = CreateSolidBrush(((unsigned __int8)g << 8) | (unsigned __int8)r | ((unsigned __int8)b << 16));
		SelectObject(hdc, brush);
		w = 0;
		h = 0;
		StretchBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, sw, sh, 0xC000CAu);
		DeleteObject(brush);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 26)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 30)
				time = 0;
		}
	}
}
DWORD WINAPI payload3(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int b; // [rsp+C0h] [rbp-10h]
	int time; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 255;
	g = 255;
	b = 255;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left;
		pt[0].y = rect.top + 1 * time;
		pt[1].x = rect.right - 9 * time;
		pt[1].y = rect.top;
		pt[2].x = 2 * time + rect.left;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		brush = CreateSolidBrush(((unsigned __int8)g << 8) | (unsigned __int8)r | ((unsigned __int8)b << 16));
		SelectObject(hdc, brush);
		w = 0;
		h = 0;
		StretchBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, sw, sh, 0xC000CAu);
		DeleteObject(brush);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 26)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 30)
				time = 0;
		}
	}
}
DWORD WINAPI payload4(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int b; // [rsp+C0h] [rbp-10h]
	int time; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 255;
	g = 255;
	b = 255;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left;
		pt[0].y = rect.top + 5 * time;
		pt[1].x = rect.right - 9 * time;
		pt[1].y = rect.top;
		pt[2].x = 2 * time + rect.left;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		brush = CreateSolidBrush(((unsigned __int8)g << 8) | (unsigned __int8)r | ((unsigned __int8)b << 16));
		SelectObject(hdc, brush);
		w = 0;
		h = 0;
		StretchBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, sw, sh, 0xC000CAu);
		DeleteObject(brush);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 26)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 30)
				time = 0;
		}
	}
}
DWORD WINAPI payload5(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int b; // [rsp+C0h] [rbp-10h]
	int time; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 255;
	g = 255;
	b = 255;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left;
		pt[0].y = rect.top + 5 * time;
		pt[1].x = rect.right - 5 * time;
		pt[1].y = rect.top;
		pt[2].x = 9 * time + rect.left;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		brush = CreateSolidBrush(((unsigned __int8)g << 8) | (unsigned __int8)r | ((unsigned __int8)b << 16));
		SelectObject(hdc, brush);
		w = 0;
		h = 0;
		StretchBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, sw, sh, 0xC000CAu);
		DeleteObject(brush);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 26)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 30)
				time = 0;
		}
	}
}
DWORD WINAPI payload6(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int time; // [rsp+C0h] [rbp-10h]
	int b; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 150;
	g = 255;
	b = 75;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left + 99;
		pt[0].y = rect.top;
		pt[1].x = rect.right;
		pt[1].y = rect.top;
		pt[2].x = rect.left;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		w = 0;
		h = 40;
		StretchBlt(hdc, 0, 0, sw, sh + 40, hdcMem, 0, 20, sw, sh, 0xC000CAu);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 19)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 23)
				time = 0;
		}
	}
}
DWORD WINAPI payload7(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int time; // [rsp+C0h] [rbp-10h]
	int b; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 150;
	g = 255;
	b = 75;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left + 99;
		pt[0].y = rect.top - 99;
		pt[1].x = rect.right;
		pt[1].y = rect.top;
		pt[2].x = rect.left;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		w = 0;
		h = 40;
		StretchBlt(hdc, 0, 0, sw, sh + 40, hdcMem, 0, 20, sw, sh, 0xC000CAu);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 19)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 23)
				time = 0;
		}
	}
}
DWORD WINAPI payload8(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int time; // [rsp+C0h] [rbp-10h]
	int b; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 150;
	g = 255;
	b = 75;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left - 99;
		pt[0].y = rect.top + 99;
		pt[1].x = rect.right;
		pt[1].y = rect.top;
		pt[2].x = rect.left;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		w = 0;
		h = 40;
		StretchBlt(hdc, 0, 0, sw, sh + 40, hdcMem, 0, 20, sw, sh, 0xC000CAu);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 19)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 23)
				time = 0;
		}
	}
}
DWORD WINAPI payload9(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int time; // [rsp+C0h] [rbp-10h]
	int b; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 150;
	g = 255;
	b = 75;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left - 99;
		pt[0].y = rect.top - 99;
		pt[1].x = rect.right - 99;
		pt[1].y = rect.top + 99;
		pt[2].x = rect.left;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		w = 0;
		h = 40;
		StretchBlt(hdc, 0, 0, sw, sh + 40, hdcMem, 0, 20, sw, sh, 0xC000CAu);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 19)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 23)
				time = 0;
		}
	}
}
DWORD WINAPI payload10(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int time; // [rsp+C0h] [rbp-10h]
	int b; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 150;
	g = 255;
	b = 75;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left - 99;
		pt[0].y = rect.top - 99;
		pt[1].x = rect.right - 99;
		pt[1].y = rect.top - 99;
		pt[2].x = rect.left;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		w = 0;
		h = 40;
		StretchBlt(hdc, 0, 0, sw, sh + 40, hdcMem, 0, 20, sw, sh, 0xC000CAu);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 19)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 23)
				time = 0;
		}
	}
}
DWORD WINAPI payload11(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int time; // [rsp+C0h] [rbp-10h]
	int b; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 150;
	g = 255;
	b = 75;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left - 99;
		pt[0].y = rect.top - 99;
		pt[1].x = rect.right - 99;
		pt[1].y = rect.top + 99;
		pt[2].x = rect.left - 99;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		w = 0;
		h = 40;
		StretchBlt(hdc, 0, 0, sw, sh + 40, hdcMem, 0, 20, sw, sh, 0xC000CAu);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 19)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 23)
				time = 0;
		}
	}
}
DWORD WINAPI zoom(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int time; // [rsp+C0h] [rbp-10h]
	int b; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 150;
	g = 255;
	b = 75;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left - 99;
		pt[0].y = rect.top - 99;
		pt[1].x = rect.right + 99;
		pt[1].y = rect.top - 99;
		pt[2].x = rect.left - 99;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		w = 0;
		h = 40;
		StretchBlt(hdc, 0, 0, sw, sh + 40, hdcMem, 0, 20, sw, sh, 0xC000CAu);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 19)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 23)
				time = 0;
		}
	}
}
DWORD WINAPI payload13(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int time; // [rsp+C0h] [rbp-10h]
	int b; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 150;
	g = 255;
	b = 75;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left - 99;
		pt[0].y = rect.top + 99;
		pt[1].x = rect.right - 99;
		pt[1].y = rect.top - 99;
		pt[2].x = rect.left - 99;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		w = 0;
		h = 40;
		StretchBlt(hdc, 0, 0, sw, sh + 40, hdcMem, 0, 20, sw, sh, 0xC000CAu);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 19)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 23)
				time = 0;
		}
	}
}
DWORD WINAPI payload14(LPVOID lpParam) {
	HWND DesktopWindow; // rax
	tagPOINT pt[3]; // [rsp+60h] [rbp-70h] BYREF
	RECT rect; // [rsp+80h] [rbp-50h] BYREF
	int h; // [rsp+90h] [rbp-40h]
	int w; // [rsp+94h] [rbp-3Ch]
	HBRUSH brush; // [rsp+98h] [rbp-38h]
	HBITMAP bm; // [rsp+A0h] [rbp-30h]
	int sh; // [rsp+A8h] [rbp-28h]
	int sw; // [rsp+ACh] [rbp-24h]
	HDC hdcMem; // [rsp+B0h] [rbp-20h]
	HDC hdc; // [rsp+B8h] [rbp-18h]
	int time; // [rsp+C0h] [rbp-10h]
	int b; // [rsp+C4h] [rbp-Ch]
	int g; // [rsp+C8h] [rbp-8h]
	int r; // [rsp+CCh] [rbp-4h]

	r = 150;
	g = 255;
	b = 75;
	time = 0;
	while (1)
	{
		++time;
		hdc = GetDC(0i64);
		hdcMem = CreateCompatibleDC(hdc);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		DesktopWindow = GetDesktopWindow();
		GetWindowRect(DesktopWindow, &rect);
		pt[0].x = rect.left + 99;
		pt[0].y = rect.top - 99;
		pt[1].x = rect.right - 99;
		pt[1].y = rect.top - 99;
		pt[2].x = rect.left - 99;
		pt[2].y = rect.bottom;
		PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0i64, 0, 0);
		w = 0;
		h = 40;
		StretchBlt(hdc, 0, 0, sw, sh + 40, hdcMem, 0, 20, sw, sh, 0xC000CAu);
		DeleteObject(hdcMem);
		DeleteObject(bm);
		ReleaseDC(0i64, hdc);
		Sleep(1u);
		while (time > 19)
		{
			Sleep(1u);
			++time;
			InvalidateRect(0i64, 0i64, 0);
			if (time == 23)
				time = 0;
		}
	}
}

int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	if (MessageBoxW(NULL, L"Warning!!!\r\n"
		"This software is safe to run on your real computer, also\r\n"
		"it won't damage your system and don't overwrite your bootloader or MBR\r\n"
		"(Master boot record)", L"MALWARE WARNING!!!", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"          LAST WARNING          \r\nNOT FOR EPILEPSEY", L"THIS IS YOUR LAST WARNING!!!", MB_YESNO | MB_ICONWARNING) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
			HANDLE thread1 = CreateThread(0, 0, fallingscreen, 0, 0, 0);
			Sleep(15000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			InvalidateRect(0, 0, 0);
			HANDLE thread2 = CreateThread(0, 0, payload2, 0, 0, 0);
			Sleep(15000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			InvalidateRect(0, 0, 0);
			HANDLE thread3 = CreateThread(0, 0, payload3, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			InvalidateRect(0, 0, 0);
			Sleep(10);
			HANDLE thread4 = CreateThread(0, 0, payload4, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			InvalidateRect(0, 0, 0);
			HANDLE thread5 = CreateThread(0, 0, payload5, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			InvalidateRect(0, 0, 0);
			HANDLE thread6 = CreateThread(0, 0, payload6, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			InvalidateRect(0, 0, 0);
			HANDLE thread7 = CreateThread(0, 0, payload7, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			InvalidateRect(0, 0, 0);
			HANDLE thread8 = CreateThread(0, 0, payload8, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread8, 0);
			CloseHandle(thread8);
			InvalidateRect(0, 0, 0);
			HANDLE thread9 = CreateThread(0, 0, payload9, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread9, 0);
			CloseHandle(thread9);
			InvalidateRect(0, 0, 0);
			HANDLE thread10 = CreateThread(0, 0, payload10, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread10, 0);
			CloseHandle(thread10);
			InvalidateRect(0, 0, 0);
			HANDLE thread11 = CreateThread(0, 0, payload11, 0, 0, 0);
			Sleep(15000);
			TerminateThread(thread11, 0);
			CloseHandle(thread11);
			InvalidateRect(0, 0, 0);
			HANDLE thread12 = CreateThread(0, 0, zoom, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread12, 0);
			CloseHandle(thread12);
			InvalidateRect(0, 0, 0);
			Sleep(10);
			HANDLE thread13 = CreateThread(0, 0, payload13, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread13, 0);
			CloseHandle(thread13);
			InvalidateRect(0, 0, 0);
			HANDLE thread14 = CreateThread(0, 0, payload14, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread14, 0);
			CloseHandle(thread14);
			InvalidateRect(0, 0, 0);
		}
	}
}
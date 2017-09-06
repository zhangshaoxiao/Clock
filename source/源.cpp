/*--------------------------------------
CLOCK.C -- Analog Clock Program
(c)xxxxxxx,2017
--------------------------------------*/

#include<Windows.h>
#include <math.h>
#include<time.h>
#include<stdio.h>
#include<cstring>
#include<string.h>
#include<string>
#define ID_TIMER    1  //��ʱ��
#define TWOPI       (2 * 3.14159) //Բ����

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //�ص�����

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
	//Win main��������������ʾһ������
{
	static TCHAR szAppName[] = TEXT("Clock");
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Analog Clock"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	 //����Ϊ���������ֵ���Զ���ʼ��
	ShowWindow(hwnd, iCmdShow);   //�˺��������ƺõĴ�������Ļ����ʾ����
	UpdateWindow(hwnd);  //��Ϊ��ʱ�ӣ���Ҫ��̬ˢ��ҳ�棬�˺�������ÿ��ˢ�´��岢�ػ�

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}                            //ѭ����ѭ��������Ϣ�������˴��ǽ��ն�ʱ����Ϣ
	return msg.wParam;
}
void DisplayDigit(HDC hdc, int iNumber) //��ʾ�����ӵ�ĳ������
{
	static BOOL  fSevenSegment[10][7] = {
		1, 1, 1, 0, 1, 1, 1,     // 0
		0, 0, 1, 0, 0, 1, 0,     // 1
		1, 0, 1, 1, 1, 0, 1,     // 2
		1, 0, 1, 1, 0, 1, 1,     // 3
		0, 1, 1, 1, 0, 1, 0,     // 4
		1, 1, 0, 1, 0, 1, 1,     // 5
		1, 1, 0, 1, 1, 1, 1,     // 6
		1, 0, 1, 0, 0, 1, 0,     // 7
		1, 1, 1, 1, 1, 1, 1,     // 8
		1, 1, 1, 1, 0, 1, 1 };  // 9             
	                      //���ﶨ���˵����ӣ� �����7�ι���ʾ0-9 ��0��ʾ����ʾĳ�ιܣ�1��ʾ��ʾĳ�ι�
	static POINT ptSegment[7][6] = {                                 //����Ҫ6��7�ε��ӹ�
		7,  6,  11,  2,  31,  2,  35,  6,  31, 10,  11, 10,
		6,  7,  10, 11,  10, 31,   6, 35,   2, 31,   2, 11,
		36,  7,  40, 11,  40, 31,  36, 35,  32, 31,  32, 11,
		7, 36,  11, 32,  31, 32,  35, 36,  31, 40,  11, 40,
		6, 37,  10, 41,  10, 61,   6, 65,   2, 61,   2, 41,
		36, 37,  40, 41,  40, 61,  36, 65,  32, 61,  32, 41,
		7, 66,  11, 62,  31, 62,  35, 66,  31, 70,  11, 70 };//����6�����֣�ÿ��������Ҫ��7�ιܱ�ʾ
	int          iSeg;

	for (iSeg = 0; iSeg < 7; iSeg++)
		if (fSevenSegment[iNumber][iSeg])
			Polygon(hdc, ptSegment[iSeg], 6);   //���ƶ���Σ�����ÿһ���ܣ��Ǹ������Σ���ʹ�õ����� ѭ����ʾ�����ε���ĳ�ιܣ���Ϊ�Ӿ�����ЧӦ�����Կ���
	                                          //���е��ӹ�������
}

void DisplayTwoDigits(HDC hdc, int iNumber, BOOL fSuppress)  //��ʾ��λ��
{
	if (!fSuppress || (iNumber / 10 != 0))
		DisplayDigit(hdc, iNumber / 10);

	OffsetWindowOrgEx(hdc, -42, 0, NULL);
	DisplayDigit(hdc, iNumber % 10);
	OffsetWindowOrgEx(hdc, -42, 0, NULL);
}

void DisplayColon(HDC hdc)   //��ʾð��
{
	POINT ptColon[2][4] = { 2,  21,  6,  17,  10, 21,  6, 25,
		2,  51,  6,  47,  10, 51,  6, 55 };

	Polygon(hdc, ptColon[0], 4);
	Polygon(hdc, ptColon[1], 4);

	OffsetWindowOrgEx(hdc, -12, 0, NULL);
}

void DisplayTime(HDC hdc, BOOL f24Hour, BOOL fSuppress)  //��ʾʱ��ĺ���
{
	SYSTEMTIME st;

	GetLocalTime(&st);

	if (f24Hour)
		DisplayTwoDigits(hdc, st.wHour, fSuppress);
	else
		DisplayTwoDigits(hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress);
	                 //��Сʱ��������12ʱ�ƻ���24ʱ��
	DisplayColon(hdc);   //��ð��
	DisplayTwoDigits(hdc, st.wMinute, FALSE);  //�ٻ�����
	DisplayColon(hdc);   //�ٻ�ð��
	DisplayTwoDigits(hdc, st.wSecond, FALSE);   //����
}
void SetIsotropic(HDC hdc, int cxClient, int cyClient)      //���õ����ӵ�λ�úʹ�С
{
	SetMapMode(hdc, MM_ISOTROPIC);
	SetWindowExtEx(hdc, 1000, 1000, NULL);
	SetViewportExtEx(hdc, cxClient / 2, -cyClient / 2, NULL);
	SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
}

void RotatePoint(POINT pt[], int iNum, int iAngle)  //Ѱ��ʱ�����ĵ�
{
	int   i;
	POINT ptTemp;

	for (i = 0; i < iNum; i++)
	{
		ptTemp.x = (int)(pt[i].x * cos(TWOPI * iAngle / 360) +
			pt[i].y * sin(TWOPI * iAngle / 360));

		ptTemp.y = (int)(pt[i].y * cos(TWOPI * iAngle / 360) -
			pt[i].x * sin(TWOPI * iAngle / 360));

		pt[i] = ptTemp;
	}                                                           
}

void DrawClock(HDC hdc)               //��ʱ�ӱ���
{
	int   iAngle;
	POINT pt[3];

	for (iAngle = 0; iAngle < 360; iAngle += 6)   //6�Ȼ�һ���㣬��60����
	{
		pt[0].x = 0;
		pt[0].y = 900;

		RotatePoint(pt, 1, iAngle);

		pt[2].x = pt[2].y = iAngle % 5 ? 33 : 100;

		pt[0].x -= pt[2].x / 2;
		pt[0].y -= pt[2].y / 2;

		pt[1].x = pt[0].x + pt[2].x;
		pt[1].y = pt[0].y + pt[2].y;
		                                     //���ϼ���Ϊ���ݵ�ĽǶȼ���������
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));  //���û�����ɫΪ��ɫ

		Ellipse(hdc, pt[0].x, pt[0].y, pt[1].x, pt[1].y);   //���㺯��
	}
}

void DrawHands(HDC hdc, SYSTEMTIME * pst, BOOL fChange)   //������
{    
	static POINT pt[3][5] = { 0, -150, 100, 0, 0, 600, -100, 0, 0, -150,
		0, -200,  50, 0, 0, 800,  -50, 0, 0, -200,
		0,    0,   0, 0, 0,   0,    0, 0, 0,  800 };    //���������룬ÿ�������Ǹ�����Ρ���ʼ�����ǵ����λ��
	int          i, iAngle[3];
	POINT        ptTemp[3][5];

	iAngle[0] = (pst->wHour * 30) % 360 + pst->wMinute / 2;
	iAngle[1] = pst->wMinute * 6;
	iAngle[2] = pst->wSecond * 6;
	                                                    //��ʼ��ʱ���֡����ֵ
	memcpy(ptTemp, pt, sizeof(pt));

	for (i = fChange ? 0 : 2; i < 3; i++)
	{
		RotatePoint(ptTemp[i], 5, iAngle[i]);              //����ʱ�����еĵ�

		Polyline(hdc, ptTemp[i], 5);                        //��ʱ�����룬���ǻ�3������Σ��ѻ��Ƶĵ���������ok
	}
	/*
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char *year = (char *)sys.wYear;
	char *month = (char *)sys.wMonth;
	char *day = (char *)sys.wDay;
	char *weekday = (char *)sys.wDayOfWeek;
	char date[128];
	strcpy_s(date, "��ǰ���ڣ�");
	strcat_s(date, year);
	strcat_s(date, month);
	TextOut(hdc, 2, 2, date, 20);
	*/
	
	/*
	time_t nowtime;
	nowtime = time(NULL); //��ȡ����ʱ��  
	char tmp[64];
	tm TM;
	localtime_s(&TM, &nowtime);
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A �����%j�� %z",&TM );
	
	TextOut(hdc, 2, 2, tmp, 20);          //��ʾ�ַ���
	*/
	
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR         szBuffer[2];
	static BOOL   f24Hour, fSuppress;
	static int        cxClient, cyClient;
	static SYSTEMTIME stPrevious;
	BOOL              fChange;
	HDC               hdc;
	PAINTSTRUCT       ps;
	SYSTEMTIME        st;
	static HBRUSH hBrushRed;
	switch (message)
	{
	case WM_CREATE:
		hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
		SetTimer(hwnd, ID_TIMER, 1000, NULL);
		GetLocalTime(&st);
		stPrevious = st;
		return 0;

	case WM_SETTINGCHANGE:
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2);
		f24Hour = (szBuffer[0] == '1');

		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2);
		fSuppress = (szBuffer[0] == '0');

		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);     //��ȡ����ߴ�
		cyClient = HIWORD(lParam);
		return 0;

	case WM_TIMER:
		GetLocalTime(&st);

		fChange = st.wHour != stPrevious.wHour ||
			st.wMinute != stPrevious.wMinute;

		hdc = GetDC(hwnd);

		SetIsotropic(hdc, cxClient, cyClient);

		SelectObject(hdc, GetStockObject(WHITE_PEN));
		DrawHands(hdc, &stPrevious, fChange);

		SelectObject(hdc, GetStockObject(BLACK_PEN));
		DrawHands(hdc, &st, TRUE);

		ReleaseDC(hwnd, hdc);
		                       //��hdc�л�ʱ�ӣ��涨ʱ������ˢ��λ��
		stPrevious = st;
		/*---------------------�ָ���--------------------------*/
		InvalidateRect(hwnd, NULL, TRUE);  //�ػ����棬�ɴ˺�������ˢ��ҳ�棬��֤ҳ���Ƕ�̬��
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SetIsotropic(hdc, cxClient, cyClient);
		DrawClock(hdc);
		DrawHands(hdc, &stPrevious, TRUE);     //��ʱ��
		SetMapMode(hdc, MM_ISOTROPIC);
		SetWindowExtEx(hdc, 500, 500, NULL);//���ý����С
		SetViewportExtEx(hdc, cxClient, cyClient, NULL);   //��������

		/*---------------------�ָ���--------------------------*/

		SetWindowOrgEx(hdc, 138, 36, NULL);
		SetViewportOrgEx(hdc, cxClient / 8, cyClient / 8, NULL);
		SelectObject(hdc, GetStockObject(NULL_PEN));
		SelectObject(hdc, hBrushRed);

		DisplayTime(hdc, f24Hour, fSuppress);
		                        //��������
		 
		                  //����Ϊ��ʾ����ʱ�������
		time_t nowtime;
		nowtime = time(NULL); //��ȡ����ʱ��  
		char tmp[64];
		for (int no = 0; no < 64; no++)
		{
			tmp[no] = '\0';
		}
		tm TM;
		localtime_s(&TM, &nowtime);   //��nowtimeת����TM��
		strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A �����%j�� %z", &TM);  //��ʽ��ʱ��
		SetTextColor(hdc, 0x9F); //����������ɫ
		
		TextOut(hdc, -250, cyClient -100, tmp, 42);          //��ʾ�ַ���

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd, ID_TIMER);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

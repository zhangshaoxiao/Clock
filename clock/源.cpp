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
#define ID_TIMER    1  //定时器
#define TWOPI       (2 * 3.14159) //圆周率

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //回调函数

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
	//Win main函数，利用它显示一个窗体
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
	 //以上为窗体的属性值的自动初始化
	ShowWindow(hwnd, iCmdShow);   //此函数将绘制好的窗体在屏幕上显示出来
	UpdateWindow(hwnd);  //因为是时钟，需要动态刷新页面，此函数用做每秒刷新窗体并重绘

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}                            //循环，循环接受消息并处理，此处是接收定时器消息
	return msg.wParam;
}
void DisplayDigit(HDC hdc, int iNumber) //显示电子钟的某个数字
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
	                      //这里定义了电子钟， 如何用7段管显示0-9 ，0表示不显示某段管，1表示显示某段管
	static POINT ptSegment[7][6] = {                                 //共需要6个7段电子管
		7,  6,  11,  2,  31,  2,  35,  6,  31, 10,  11, 10,
		6,  7,  10, 11,  10, 31,   6, 35,   2, 31,   2, 11,
		36,  7,  40, 11,  40, 31,  36, 35,  32, 31,  32, 11,
		7, 36,  11, 32,  31, 32,  35, 36,  31, 40,  11, 40,
		6, 37,  10, 41,  10, 61,   6, 65,   2, 61,   2, 41,
		36, 37,  40, 41,  40, 61,  36, 65,  32, 61,  32, 41,
		7, 66,  11, 62,  31, 62,  35, 66,  31, 70,  11, 70 };//共有6个数字，每个数字需要用7段管表示
	int          iSeg;

	for (iSeg = 0; iSeg < 7; iSeg++)
		if (fSevenSegment[iNumber][iSeg])
			Polygon(hdc, ptSegment[iSeg], 6);   //绘制多边形（就是每一条管，是个六边形），使得电子钟 循环显示，依次点亮某段管，因为视觉暂留效应，所以看到
	                                          //所有电子管是亮的
}

void DisplayTwoDigits(HDC hdc, int iNumber, BOOL fSuppress)  //显示两位数
{
	if (!fSuppress || (iNumber / 10 != 0))
		DisplayDigit(hdc, iNumber / 10);

	OffsetWindowOrgEx(hdc, -42, 0, NULL);
	DisplayDigit(hdc, iNumber % 10);
	OffsetWindowOrgEx(hdc, -42, 0, NULL);
}

void DisplayColon(HDC hdc)   //显示冒号
{
	POINT ptColon[2][4] = { 2,  21,  6,  17,  10, 21,  6, 25,
		2,  51,  6,  47,  10, 51,  6, 55 };

	Polygon(hdc, ptColon[0], 4);
	Polygon(hdc, ptColon[1], 4);

	OffsetWindowOrgEx(hdc, -12, 0, NULL);
}

void DisplayTime(HDC hdc, BOOL f24Hour, BOOL fSuppress)  //显示时间的函数
{
	SYSTEMTIME st;

	GetLocalTime(&st);

	if (f24Hour)
		DisplayTwoDigits(hdc, st.wHour, fSuppress);
	else
		DisplayTwoDigits(hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress);
	                 //画小时，区分是12时制还是24时制
	DisplayColon(hdc);   //画冒号
	DisplayTwoDigits(hdc, st.wMinute, FALSE);  //再画分钟
	DisplayColon(hdc);   //再画冒号
	DisplayTwoDigits(hdc, st.wSecond, FALSE);   //画秒
}
void SetIsotropic(HDC hdc, int cxClient, int cyClient)      //设置电子钟的位置和大小
{
	SetMapMode(hdc, MM_ISOTROPIC);
	SetWindowExtEx(hdc, 1000, 1000, NULL);
	SetViewportExtEx(hdc, cxClient / 2, -cyClient / 2, NULL);
	SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
}

void RotatePoint(POINT pt[], int iNum, int iAngle)  //寻找时钟中心点
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

void DrawClock(HDC hdc)               //画时钟表盘
{
	int   iAngle;
	POINT pt[3];

	for (iAngle = 0; iAngle < 360; iAngle += 6)   //6度画一个点，共60个点
	{
		pt[0].x = 0;
		pt[0].y = 900;

		RotatePoint(pt, 1, iAngle);

		pt[2].x = pt[2].y = iAngle % 5 ? 33 : 100;

		pt[0].x -= pt[2].x / 2;
		pt[0].y -= pt[2].y / 2;

		pt[1].x = pt[0].x + pt[2].x;
		pt[1].y = pt[0].y + pt[2].y;
		                                     //以上计算为根据点的角度计算点的坐标
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));  //设置画笔颜色为黑色

		Ellipse(hdc, pt[0].x, pt[0].y, pt[1].x, pt[1].y);   //画点函数
	}
}

void DrawHands(HDC hdc, SYSTEMTIME * pst, BOOL fChange)   //画表针
{    
	static POINT pt[3][5] = { 0, -150, 100, 0, 0, 600, -100, 0, 0, -150,
		0, -200,  50, 0, 0, 800,  -50, 0, 0, -200,
		0,    0,   0, 0, 0,   0,    0, 0, 0,  800 };    //共三个表针，每个表针是个五边形。初始化它们的相对位置
	int          i, iAngle[3];
	POINT        ptTemp[3][5];

	iAngle[0] = (pst->wHour * 30) % 360 + pst->wMinute / 2;
	iAngle[1] = pst->wMinute * 6;
	iAngle[2] = pst->wSecond * 6;
	                                                    //初始化时、分、秒的值
	memcpy(ptTemp, pt, sizeof(pt));

	for (i = fChange ? 0 : 2; i < 3; i++)
	{
		RotatePoint(ptTemp[i], 5, iAngle[i]);              //画出时钟所有的点

		Polyline(hdc, ptTemp[i], 5);                        //画时分秒针，就是画3个五边形，把绘制的点连起来就ok
	}
	/*
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char *year = (char *)sys.wYear;
	char *month = (char *)sys.wMonth;
	char *day = (char *)sys.wDay;
	char *weekday = (char *)sys.wDayOfWeek;
	char date[128];
	strcpy_s(date, "当前日期：");
	strcat_s(date, year);
	strcat_s(date, month);
	TextOut(hdc, 2, 2, date, 20);
	*/
	
	/*
	time_t nowtime;
	nowtime = time(NULL); //获取日历时间  
	char tmp[64];
	tm TM;
	localtime_s(&TM, &nowtime);
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A 本年第%j天 %z",&TM );
	
	TextOut(hdc, 2, 2, tmp, 20);          //显示字符串
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
		cxClient = LOWORD(lParam);     //获取窗体尺寸
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
		                       //向hdc中画时钟，随定时器不断刷新位置
		stPrevious = st;
		/*---------------------分割线--------------------------*/
		InvalidateRect(hwnd, NULL, TRUE);  //重画界面，由此函数不断刷新页面，保证页面是动态的
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SetIsotropic(hdc, cxClient, cyClient);
		DrawClock(hdc);
		DrawHands(hdc, &stPrevious, TRUE);     //画时钟
		SetMapMode(hdc, MM_ISOTROPIC);
		SetWindowExtEx(hdc, 500, 500, NULL);//设置界面大小
		SetViewportExtEx(hdc, cxClient, cyClient, NULL);   //其他配置

		/*---------------------分割线--------------------------*/

		SetWindowOrgEx(hdc, 138, 36, NULL);
		SetViewportOrgEx(hdc, cxClient / 8, cyClient / 8, NULL);
		SelectObject(hdc, GetStockObject(NULL_PEN));
		SelectObject(hdc, hBrushRed);

		DisplayTime(hdc, f24Hour, fSuppress);
		                        //画电子钟
		 
		                  //下面为显示文字时间和日期
		time_t nowtime;
		nowtime = time(NULL); //获取日历时间  
		char tmp[64];
		for (int no = 0; no < 64; no++)
		{
			tmp[no] = '\0';
		}
		tm TM;
		localtime_s(&TM, &nowtime);   //把nowtime转换到TM中
		strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A 本年第%j天 %z", &TM);  //格式化时间
		SetTextColor(hdc, 0x9F); //设置字体颜色
		
		TextOut(hdc, -250, cyClient -100, tmp, 42);          //显示字符串

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd, ID_TIMER);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

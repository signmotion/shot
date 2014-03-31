/**
* Пример работы с библиотекой Shot.
* Для компании Ubisoft.
*
* Запускается приложение из консоли командой "shot СКОРОСТЬ УГОЛ [УСКОРЕНИЕ]".
* Значения даются в системе СИ, угол броска - в градусах.
* Пример: shot 100 70 10
*
* Тело начинает движение с координатами (0, 0). Визуализация - с левого
* нижнего угла окна.
*
* @author Андрей Сырокомский, +38 050 335-16-18
*/

#include <assert.h>
#include <iostream>
#include <iomanip>
#include <memory>
#include <sstream>
#include <ctime>
#include <windows.h>

#include "Shot.h"


/**
* Размеры окна визуализации.
*/
static const int WINDOW_WIDTH  = 1000;
static const int WINDOW_HEIGHT = 500;


/**
* Это - простая демонстраци возможностей класса Shot. Поэтому, чтобы
* не раздувать проект, просто укажем общие переменные в глобальном
* пространстве имён.
*/
std::shared_ptr< shot::Shot >  shotPtr;
shot::real_t timestep = 0.0f;
shot::real_t timelive = 0.0f;




/**
* Собственно, сами функции моделирования / визуализации.
*/
void pulse();
void draw( HWND wnd );




/**
* Для логгирования.
*/
std::ostream& shotLog = std::cout;
std::string ticklog();




/**
* Исп. для работы с GDI.
*/
LRESULT CALLBACK wndProc(
    HWND wnd, UINT message, WPARAM wparam, LPARAM lparam
);




int WINAPI WinMain(
    HINSTANCE instance,
    HINSTANCE prevInstance,
    LPSTR cmdLine,
    int cmdShow
) {
    using namespace shot;

#ifdef _DEBUG
    shotLog << ticklog() << "Start\n";
#endif

    setlocale( LC_ALL, "Russian" );
    setlocale( LC_NUMERIC, "C" );


#ifdef _DEBUG
    shotLog << ticklog() << "Регистрируем окно визуализации\n";
#endif

    WNDCLASSEX ex;
    ex.cbSize        = sizeof( WNDCLASSEX );
    ex.style         = 0;
    ex.lpfnWndProc   = wndProc;
    ex.cbClsExtra    = 0;
    ex.cbWndExtra    = 0;
    ex.hInstance     = instance;
    ex.hIcon         = LoadIcon( instance, MAKEINTRESOURCE( IDI_APPLICATION ) );
    ex.hCursor       = LoadCursor( nullptr, IDC_UPARROW );
    ex.hbrBackground = (HBRUSH) GetStockObject( WHITE_BRUSH );
    ex.lpszMenuName  = nullptr;
    ex.lpszClassName = "win32app";
    ex.hIconSm = LoadIcon( ex.hInstance, MAKEINTRESOURCE( IDI_APPLICATION ) );

    if ( !RegisterClassEx( &ex ) ) {
        MessageBox( nullptr, "(!) Register class failed.", "Shot", 0 );
        return -1;
    }


#ifdef _DEBUG
    shotLog << ticklog() << "Создаём окно визуализации\n";
#endif

    auto wnd = CreateWindow(
        "win32app", "",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr,
        nullptr,
        instance,
        nullptr
    );

    if ( !wnd )  {
        MessageBox( nullptr, "(!) Create window failed.", "Shot", 0 );
        return -1;
    }




    // инициируем модель
#ifdef _DEBUG
    shotLog << ticklog() << "Инициируем модель\n";
#endif

    try {
        shotPtr = Shot::valueOf( cmdLine );

    } catch ( const shot::Exception& ex ) {
        const auto s = ex.what();
        shotLog << ticklog() << "(!) " << s << "\n";
        SendMessage( wnd, WM_DESTROY, 0, 0 );
        return -1;
    }

    // находим подходящий шаг времени
#ifdef _DEBUG
    shotLog << ticklog() << "Находим подходящий шаг времени\n";
#endif

    static const real_t PRECISION = 500.0f;
    static const real_t timestepVelocity =
        abs( shotPtr->velocityAbs() ) / PRECISION;
    static const real_t timestepGravity =
        abs( shotPtr->gravity() ) / PRECISION;
    timestep = min( timestepVelocity, timestepGravity );

#ifdef _DEBUG
    shotLog << ticklog() << "Выбран шаг времени " << timestep << " с\n";
#endif




    // моделируем, визуализируем
#ifdef _DEBUG
    shotLog << ticklog() << "Запускаем модель\n";
#endif

    ShowWindow( wnd, cmdShow );
    UpdateWindow( wnd );




    MSG msg;
    while ( GetMessage( &msg, nullptr, 0, 0 ) ) {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

#ifdef _DEBUG
    shotLog << ticklog() << "Stop\n";
#endif

    return static_cast< int >( msg.wParam );
}








LRESULT CALLBACK wndProc(
    HWND wnd, UINT message, WPARAM wparam, LPARAM lparam
) {
    switch ( message ) {
        case WM_CREATE:
            SetTimer( wnd, 0, 10, 0 );
            return 0;

        case WM_TIMER:
            pulse();
            InvalidateRect( wnd, nullptr, false );
            UpdateWindow( wnd );
            return 0;

        case WM_PAINT:
            draw( wnd );
            return 0;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( wnd, message, wparam, lparam);
}








void pulse() {

    assert( shotPtr
        && "Необходимо инициализировать модель до вызова этого метода." );

    timelive += timestep;
    shotPtr->pulse( timestep );
}




void draw( HWND wnd ) {

    assert( shotPtr
        && "Необходимо инициализировать модель до вызова этого метода." );

    PAINTSTRUCT ps;
    auto hdc = BeginPaint( wnd, &ps );

    // заботимся о рисовании относительно центра окна
#if 0
    @todo maybe Отодвигаемся (увеличиваем масштаб), если тело вылетело
          за пределы окна.
    SetGraphicsMode( hdc, GM_ADVANCED );
    SetMapMode( hdc, MM_ANISOTROPIC );
    static shot::real_t scaleK = 1.0;
    auto ex = static_cast< shot::real_t >( WINDOW_WIDTH  ) * scaleK;
    auto ey = static_cast< shot::real_t >( WINDOW_HEIGHT ) * scaleK;
    if ( !shotPtr->inner( ex, ey ) ) {
        scaleK *= 1.1f;
        ex = static_cast< shot::real_t >( WINDOW_WIDTH  ) * scaleK;
        ey = static_cast< shot::real_t >( WINDOW_HEIGHT ) * scaleK;
        InvalidateRect( wnd, nullptr, true );
    }

    const auto iex = static_cast< int >( ex );
    const auto iey = static_cast< int >( ey );
    SetWindowOrgEx( hdc,  -iex / 2,  -iey / 2,  nullptr );
    SetWindowExtEx( hdc,   iex,       iey,      nullptr );
    RECT rc;
    GetClientRect( wnd, &rc );
    SetViewportExtEx( hdc, rc.right, rc.bottom, nullptr );
#endif

    RECT rc;
    GetClientRect( wnd, &rc );
    SetWindowOrgEx( hdc,  0,  -rc.bottom,  nullptr );
    SetViewportExtEx( hdc, rc.right, rc.bottom, nullptr );


    // рисуем
    const auto coord = shotPtr->coord();
    static const auto color = RGB( 0, 0, 0 );
    const int x = static_cast< int >( coord.x );
    const int y = -static_cast< int >( coord.y );
    SetPixel( hdc, x, y, color );

    EndPaint( wnd, &ps );


    // в заголовке окна покажем информацию о сост. модели
    std::ostringstream title;
    title << "Тело брошено под углом к горизонту    " <<
        std::fixed << std::setprecision( 1 ) <<
        timelive << " с  " <<
        shotPtr->velocityAbs() << " м/с  " <<
        coord
    ;
    SetWindowText( wnd,  title.str().c_str() );


#ifdef _DEBUG
    // ведём лог
    {
        std::ostringstream s;
        s <<
            std::fixed << std::setprecision( 5 ) <<
            timelive << " с\t" <<
            shotPtr->velocityAbs() << " м/с\t" <<
            coord
        ;
        shotLog << ticklog() << s.str() << std::endl;
    }
#endif
}




inline std::string ticklog() {
    std::ostringstream ss;
    ss << GetTickCount() << "\t";
    return ss.str();
}

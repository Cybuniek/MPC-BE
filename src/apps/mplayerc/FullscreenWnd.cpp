#include "stdafx.h"
#include "MPC-BE.h" // Główny nagłówek aplikacji
#include "FullscreenWnd.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Statyczna zmienna pomocnicza do przechowywania stanu pełnoekranowego dla tej instancji
static bool s_bFullScreen = false;

IMPLEMENT_DYNAMIC(CFullscreenWnd, CWnd)

CFullscreenWnd::CFullscreenWnd(CMainFrame* pMainFrame)
    : m_pMainFrame(pMainFrame)
    , m_hCursor(NULL)
    , m_bCursorVisible(false)
    , m_bTrackingMouseLeave(false)
{
}

CFullscreenWnd::~CFullscreenWnd()
{
}

BEGIN_MESSAGE_MAP(CFullscreenWnd, CWnd)
    ON_WM_CREATE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_ERASEBKGND()
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()

int CFullscreenWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_hCursor = ::LoadCursorW(NULL, IDC_ARROW);
    m_bCursorVisible = false;
    m_bTrackingMouseLeave = false;

    // Usunięcie ramki okna dla trybu pełnoekranowego.
    lpCreateStruct->style &= ~WS_BORDER;
    return 0;
}

void CFullscreenWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    CWnd::OnLButtonDown(nFlags, point);
}

void CFullscreenWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    CWnd::OnLButtonUp(nFlags, point);
}

void CFullscreenWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bTrackingMouseLeave) {
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0 };
        _TrackMouseEvent(&tme);
        m_bTrackingMouseLeave = true;
    }
    CWnd::OnMouseMove(nFlags, point);
}

void CFullscreenWnd::OnMouseLeave()
{
    m_bTrackingMouseLeave = false;
    CWnd::OnMouseLeave();
}

BOOL CFullscreenWnd::OnEraseBkgnd(CDC* pDC)
{
    // Zapobieganie migotaniu
    return FALSE;
}

BOOL CFullscreenWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_bCursorVisible) {
        ::SetCursor(m_hCursor);
    } else {
        ::SetCursor(NULL);
    }
    return TRUE;
}

void CFullscreenWnd::ShowCursor(bool bVisible)
{
    if (m_bCursorVisible != bVisible) {
        m_bCursorVisible = bVisible;
        PostMessageW(WM_SETCURSOR, 0, 0);
    }
}

void CFullscreenWnd::SetCursor(LPCWSTR lpCursorName)
{
    m_hCursor = ::LoadCursorW(NULL, lpCursorName);
    m_bCursorVisible = true;
    PostMessageW(WM_SETCURSOR, 0, 0);
}

bool CFullscreenWnd::IsWindow() const
{
    return m_hWnd != NULL;
}

// Nowa metoda, która wywołuje metodę ToggleControls w głównym oknie
void CFullscreenWnd::ToggleControls(bool bVisible)
{
    if (m_pMainFrame) {
        m_pMainFrame->ToggleControls(bVisible);
    }
}

// Override PreTranslateMessage do obsługi klawisza F11.
// Po naciśnięciu F11 przełączamy stan pełnoekranowy i odpowiednio widoczność elementów sterujących.
BOOL CFullscreenWnd::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F11)
    {
        // Przełączamy tryb: gdy aktualnie tryb okienkowy (s_bFullScreen == false),
        // wyświetlamy kontrolki; gdy pełnoekranowy, ukrywamy.
        ToggleControls(!s_bFullScreen);
        s_bFullScreen = !s_bFullScreen;
        return TRUE;
    }
    return CWnd::PreTranslateMessage(pMsg);
}

LRESULT CFullscreenWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_COMMAND:
            if (m_pMainFrame) {
                m_pMainFrame->PostMessageW(message, wParam, lParam);
            }
            break;
        case WM_NCACTIVATE:
            if (!wParam && m_pMainFrame && !m_pMainFrame->CanShowDialog()) {
                return 0;
            }
            break;
    }
    return CWnd::WindowProc(message, wParam, lParam);
}

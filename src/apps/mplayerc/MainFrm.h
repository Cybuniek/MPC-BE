#ifndef MAINFRM_H
#define MAINFRM_H

#include <afxwin.h>
#include "PlayerPlaylistBar.h"
#include "FlyBar.h"
#include "PreView.h"

class CMainFrame : public CFrameWnd {
    DECLARE_DYNAMIC(CMainFrame)
public:
    CMainFrame();
    virtual ~CMainFrame();

    // Metoda, która przełącza widoczność elementów sterowania
    void ToggleControls(bool bVisible);

protected:
    DECLARE_MESSAGE_MAP()

public:
    // Inne już istniejące elementy interfejsu
    CPlayerPlaylistBar m_wndPlaylistBar;
    CFlyBar            m_wndFlyBar;
    CPreView           m_wndPreView;

private:
    // Zmienna określająca, czy aktualnie jesteśmy w trybie pełnoekranowym.
    // Jeśli false – widok okienkowy (kontrolki ukryte), true – pełnoekranowy (kontrolki widoczne)
    bool m_bFullScreen;
};

#endif // MAINFRM_H

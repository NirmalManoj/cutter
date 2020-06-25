#ifndef DECOMPILERCONTEXTMENU_H
#define DECOMPILERCONTEXTMENU_H

#include "core/Cutter.h"
#include <QMenu>
#include <QKeySequence>

class DecompilerContextMenu : public QMenu
{
    Q_OBJECT

public:
    DecompilerContextMenu(QWidget *parent, MainWindow *mainWindow);
    ~DecompilerContextMenu();
    int start_pos;
    int end_pos;


signals:
    void copy();

public slots:
    void setOffset(RVA offset);
    void setOffsetsInLine(QList<RVA> offsetList);
    void setCanCopy(bool enabled);

private slots:
    void aboutToShowSlot();

    void actionCopyTriggered();

    void actionAddBreakpointTriggered();
    void actionAdvancedBreakpointTriggered();

    void actionContinueUntilTriggered();
    void actionSetPCTriggered();

private:
    void setShortcutContextInActions(QMenu *menu);
    void getBreakpoints();
    RVA offset;
    QList<RVA> offsetsInLine;
    QList<RVA> availableBreakpoints;
    MainWindow *mainWindow;

    QAction actionCopy;
    QAction *copySeparator;

    QMenu *breakpointMenu;
    QAction actionAddBreakpoint;
    QAction actionAdvancedBreakpoint;

    QMenu *debugMenu;
    QAction actionContinueUntil;
    QAction actionSetPC;

    // Set actions
    void setActionCopy();

    void setActionAddBreakpoint();
    void setActionAdvancedBreakpoint();

    void setActionContinueUntil();
    void setActionSetPC();

    // Add Menus
    void addBreakpointMenu();
    void addDebugMenu();
    // I left out the following part from RAnnotatedCode. Probably, we will be returning/passing annotations
    // from/to the function getThingUsedHere() and updateTargetMenuActions(). This block of comment will get removed in
    // future PRs.
    //
    // struct ThingUsedHere {
    //     QString name;
    //     RVA offset;
    //     enum class Type {
    //         Var,
    //         Function,
    //         Flag,
    //         Address
    //     };
    //     Type type;
    // };
    // QVector<ThingUsedHere> getThingUsedHere(RVA offset);

    // void updateTargetMenuActions(const QVector<ThingUsedHere> &targets);
};

#endif // DECOMPILERCONTEXTMENU_H
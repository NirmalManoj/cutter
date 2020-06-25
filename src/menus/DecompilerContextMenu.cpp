#include "DecompilerContextMenu.h"
#include "dialogs/preferences/PreferencesDialog.h"
#include "MainWindow.h"
#include "dialogs/BreakpointsDialog.h"

#include <QtCore>
#include <QShortcut>
#include <QJsonArray>
#include <QClipboard>
#include <QApplication>
#include <QPushButton>

DecompilerContextMenu::DecompilerContextMenu(QWidget *parent, MainWindow *mainWindow)
    :   QMenu(parent),
        offset(0),
        mainWindow(mainWindow),
        actionCopy(tr("Copy"), this),
        actionAddBreakpoint(tr("Add/remove breakpoint"), this),
        actionAdvancedBreakpoint(tr("Advanced breakpoint"), this),
        actionContinueUntil(tr("Continue until line"), this),
        actionSetPC(tr("Set PC"), this)
{
    setActionCopy();
    addSeparator();

    addBreakpointMenu();
    addDebugMenu();

    setShortcutContextInActions(this);

    connect(this, &DecompilerContextMenu::aboutToShow,
            this, &DecompilerContextMenu::aboutToShowSlot);
}

DecompilerContextMenu::~DecompilerContextMenu()
{
}

void DecompilerContextMenu::setOffset(RVA offset)
{
    this->offset = offset;

    // this->actionSetFunctionVarTypes.setVisible(true);
}

void DecompilerContextMenu::setOffsetsInLine(QList<RVA> offsetList){
    this->offsetsInLine = offsetList;
}

void DecompilerContextMenu::setCanCopy(bool enabled)
{
    actionCopy.setVisible(enabled);
}

void DecompilerContextMenu::setShortcutContextInActions(QMenu *menu)
{
    for (QAction *action : menu->actions()) {
        if (action->isSeparator()) {
            //Do nothing
        } else if (action->menu()) {
            setShortcutContextInActions(action->menu());
        } else {
            action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
        }
    }
}

void DecompilerContextMenu::getBreakpoints(){
    this->availableBreakpoints.clear();
    for(auto curOffset: this->offsetsInLine){
        if(Core()->breakpointIndexAt(curOffset) > -1){
            this->availableBreakpoints.push_back(curOffset);
        }
    }
}

void DecompilerContextMenu::aboutToShowSlot()
{
    // Only show debug options if we are currently debugging
    debugMenu->menuAction()->setVisible(Core()->currentlyDebugging);

    getBreakpoints();
    // bool hasBreakpoint = Core()->breakpointIndexAt(offset) > -1;
    bool hasBreakpoint = !this->availableBreakpoints.isEmpty();
    int numberOfBreakpoints = this->availableBreakpoints.size();
    if(numberOfBreakpoints == 0){
        actionAddBreakpoint.setText(tr("Add breakpoint %1").arg(this->start_pos));
    }else if(numberOfBreakpoints == 1){
        actionAddBreakpoint.setText(tr("Remove breakpoint %1").arg(this->start_pos));
    }else{
        actionAddBreakpoint.setText(tr("Remove all breakpoints"));
    }
    actionAdvancedBreakpoint.setText(hasBreakpoint ?
                                     tr("Edit breakpoint %1").arg(this->end_pos) : tr("Advanced breakpoint %1").arg(this->end_pos));
    QString progCounterName = Core()->getRegisterName("PC").toUpper();
    actionSetPC.setText(tr("Set %1 here").arg(progCounterName));
}

// Set up actions

void DecompilerContextMenu::setActionCopy()
{
    connect(&actionCopy, &QAction::triggered, this, &DecompilerContextMenu::actionCopyTriggered);
    addAction(&actionCopy);
    actionCopy.setShortcut(QKeySequence::Copy);
}

void DecompilerContextMenu::setActionAddBreakpoint()
{
    connect(&actionAddBreakpoint, &QAction::triggered, this,
            &DecompilerContextMenu::actionAddBreakpointTriggered);
    actionAddBreakpoint.setShortcuts({Qt::Key_F2, Qt::CTRL + Qt::Key_B});
}

void DecompilerContextMenu::setActionAdvancedBreakpoint()
{
    connect(&actionAdvancedBreakpoint, &QAction::triggered, this,
            &DecompilerContextMenu::actionAdvancedBreakpointTriggered);
    actionAdvancedBreakpoint.setShortcut({Qt::CTRL + Qt::Key_F2});
}

void DecompilerContextMenu::setActionContinueUntil()
{
    connect(&actionContinueUntil, &QAction::triggered, this,
            &DecompilerContextMenu::actionContinueUntilTriggered);
}

void DecompilerContextMenu::setActionSetPC()
{
    connect(&actionSetPC, &QAction::triggered, this, &DecompilerContextMenu::actionSetPCTriggered);
}

// Set up action responses

void DecompilerContextMenu::actionCopyTriggered()
{
    emit copy();
}

void DecompilerContextMenu::actionAddBreakpointTriggered()
{
    Core()->toggleBreakpoint(offset);
}

void DecompilerContextMenu::actionAdvancedBreakpointTriggered()
{
    int index = Core()->breakpointIndexAt(offset);
    if (index >= 0) {
        BreakpointsDialog::editBreakpoint(Core()->getBreakpointAt(offset), this);
    } else {
        BreakpointsDialog::createNewBreakpoint(offset, this);
    }
}

void DecompilerContextMenu::actionContinueUntilTriggered()
{
    Core()->continueUntilDebug(RAddressString(offset));
}

void DecompilerContextMenu::actionSetPCTriggered()
{
    QString progCounterName = Core()->getRegisterName("PC");
    Core()->setRegister(progCounterName, RAddressString(offset).toUpper());
}

// Set up menus

void DecompilerContextMenu::addBreakpointMenu()
{
    breakpointMenu = addMenu(tr("Breakpoint"));

    setActionAddBreakpoint();
    breakpointMenu->addAction(&actionAddBreakpoint);
    setActionAdvancedBreakpoint();
    breakpointMenu->addAction(&actionAdvancedBreakpoint);
}

void DecompilerContextMenu::addDebugMenu()
{
    debugMenu = addMenu(tr("Debug"));

    setActionContinueUntil();
    debugMenu->addAction(&actionContinueUntil);
    setActionSetPC();
    debugMenu->addAction(&actionSetPC);
}

/****************************************************************************
** Form implementation generated from reading ui file 'mainform.ui'
**
** Created: 二  7月 20 20:27:21 2010
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "mainform.h"

#include <qvariant.h>
#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include "mainform.ui.h"

/* 
 *  Constructs a mainForm as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 */
mainForm::mainForm( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    if ( !name )
	setName( "mainForm" );
    setMinimumSize( QSize( 21, 22 ) );
    setMaximumSize( QSize( 640, 480 ) );
    QFont f( font() );
    f.setPointSize( 14 );
    setFont( f ); 
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );

    tabWidget = new QTabWidget( centralWidget(), "tabWidget" );
    tabWidget->setGeometry( QRect( 10, 0, 620, 460 ) );
    tabWidget->setMinimumSize( QSize( 620, 460 ) );
    tabWidget->setMaximumSize( QSize( 620, 460 ) );
    QFont tabWidget_font(  tabWidget->font() );
    tabWidget->setFont( tabWidget_font ); 

    tab = new QWidget( tabWidget, "tab" );

    groupBox5 = new QGroupBox( tab, "groupBox5" );
    groupBox5->setGeometry( QRect( 160, 130, 163, 43 ) );
    groupBox5->setColumnLayout(0, Qt::Vertical );
    groupBox5->layout()->setSpacing( 6 );
    groupBox5->layout()->setMargin( 11 );
    groupBox5Layout = new QGridLayout( groupBox5->layout() );
    groupBox5Layout->setAlignment( Qt::AlignTop );

    textLabel = new QLabel( groupBox5, "textLabel" );
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 253, 253, 253) );
    cg.setColor( QColorGroup::Dark, QColor( 206, 206, 206) );
    cg.setColor( QColorGroup::Mid, QColor( 191, 191, 191) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 68, 100, 172) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Light, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::Midlight, QColor( 253, 253, 253) );
    cg.setColor( QColorGroup::Dark, QColor( 143, 143, 143) );
    cg.setColor( QColorGroup::Mid, QColor( 191, 191, 191) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 94, 122, 183) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Light, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::Midlight, QColor( 253, 253, 253) );
    cg.setColor( QColorGroup::Dark, QColor( 143, 143, 143) );
    cg.setColor( QColorGroup::Mid, QColor( 191, 191, 191) );
    cg.setColor( QColorGroup::Text, QColor( 191, 191, 191) );
    cg.setColor( QColorGroup::BrightText, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setDisabled( cg );
    textLabel->setPalette( pal );
    QFont textLabel_font(  textLabel->font() );
    textLabel->setFont( textLabel_font ); 
    textLabel->setScaledContents( FALSE );
    textLabel->setAlignment( int( QLabel::AlignCenter ) );

    groupBox5Layout->addWidget( textLabel, 0, 0 );

    groupBox7 = new QGroupBox( tab, "groupBox7" );
    groupBox7->setGeometry( QRect( 340, 130, 68, 43 ) );
    groupBox7->setColumnLayout(0, Qt::Vertical );
    groupBox7->layout()->setSpacing( 6 );
    groupBox7->layout()->setMargin( 11 );
    groupBox7Layout = new QGridLayout( groupBox7->layout() );
    groupBox7Layout->setAlignment( Qt::AlignTop );

    doorstatusLabel = new QLabel( groupBox7, "doorstatusLabel" );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 253, 253, 253) );
    cg.setColor( QColorGroup::Dark, QColor( 206, 206, 206) );
    cg.setColor( QColorGroup::Mid, QColor( 191, 191, 191) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 68, 100, 172) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Light, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::Midlight, QColor( 253, 253, 253) );
    cg.setColor( QColorGroup::Dark, QColor( 143, 143, 143) );
    cg.setColor( QColorGroup::Mid, QColor( 191, 191, 191) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 94, 122, 183) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Light, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::Midlight, QColor( 253, 253, 253) );
    cg.setColor( QColorGroup::Dark, QColor( 143, 143, 143) );
    cg.setColor( QColorGroup::Mid, QColor( 191, 191, 191) );
    cg.setColor( QColorGroup::Text, QColor( 191, 191, 191) );
    cg.setColor( QColorGroup::BrightText, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setDisabled( cg );
    doorstatusLabel->setPalette( pal );
    QFont doorstatusLabel_font(  doorstatusLabel->font() );
    doorstatusLabel->setFont( doorstatusLabel_font ); 
    doorstatusLabel->setAlignment( int( QLabel::AlignCenter ) );

    groupBox7Layout->addWidget( doorstatusLabel, 0, 0 );

    groupBox4 = new QGroupBox( tab, "groupBox4" );
    groupBox4->setGeometry( QRect( -10, 380, 630, 50 ) );

    alertLabel = new QLabel( groupBox4, "alertLabel" );
    alertLabel->setGeometry( QRect( 10, 10, 610, 30 ) );
    QFont alertLabel_font(  alertLabel->font() );
    alertLabel_font.setPointSize( 12 );
    alertLabel->setFont( alertLabel_font ); 

    unlockButton = new QPushButton( tab, "unlockButton" );
    unlockButton->setGeometry( QRect( 280, 200, 80, 40 ) );
    QFont unlockButton_font(  unlockButton->font() );
    unlockButton->setFont( unlockButton_font ); 

    refreshButton = new QPushButton( tab, "refreshButton" );
    refreshButton->setGeometry( QRect( 186, 200, 90, 40 ) );
    QFont refreshButton_font(  refreshButton->font() );
    refreshButton->setFont( refreshButton_font ); 
    tabWidget->insertTab( tab, "" );

    tab_2 = new QWidget( tabWidget, "tab_2" );

    buttonGroup1 = new QButtonGroup( tab_2, "buttonGroup1" );
    buttonGroup1->setGeometry( QRect( 140, 250, 150, 170 ) );

    backspaceButton1 = new QPushButton( buttonGroup1, "backspaceButton1" );
    backspaceButton1->setGeometry( QRect( 60, 130, 80, 29 ) );

    pushButton2 = new QPushButton( buttonGroup1, "pushButton2" );
    pushButton2->setGeometry( QRect( 60, 10, 30, 30 ) );
    pushButton2->setMinimumSize( QSize( 30, 30 ) );
    pushButton2->setMaximumSize( QSize( 30, 30 ) );

    pushButton3 = new QPushButton( buttonGroup1, "pushButton3" );
    pushButton3->setGeometry( QRect( 110, 10, 30, 30 ) );
    pushButton3->setMinimumSize( QSize( 30, 30 ) );
    pushButton3->setMaximumSize( QSize( 30, 30 ) );

    pushButton6 = new QPushButton( buttonGroup1, "pushButton6" );
    pushButton6->setGeometry( QRect( 110, 50, 30, 30 ) );
    pushButton6->setMinimumSize( QSize( 30, 30 ) );
    pushButton6->setMaximumSize( QSize( 30, 30 ) );

    pushButton7 = new QPushButton( buttonGroup1, "pushButton7" );
    pushButton7->setGeometry( QRect( 10, 90, 30, 30 ) );
    pushButton7->setMinimumSize( QSize( 30, 30 ) );
    pushButton7->setMaximumSize( QSize( 30, 30 ) );

    pushButton8 = new QPushButton( buttonGroup1, "pushButton8" );
    pushButton8->setGeometry( QRect( 60, 90, 30, 30 ) );
    pushButton8->setMinimumSize( QSize( 30, 30 ) );
    pushButton8->setMaximumSize( QSize( 30, 30 ) );

    pushButton9 = new QPushButton( buttonGroup1, "pushButton9" );
    pushButton9->setGeometry( QRect( 110, 90, 30, 30 ) );
    pushButton9->setMinimumSize( QSize( 30, 30 ) );
    pushButton9->setMaximumSize( QSize( 30, 30 ) );

    pushButton0 = new QPushButton( buttonGroup1, "pushButton0" );
    pushButton0->setGeometry( QRect( 10, 130, 30, 30 ) );
    pushButton0->setMinimumSize( QSize( 30, 30 ) );
    pushButton0->setMaximumSize( QSize( 30, 30 ) );

    pushButton5 = new QPushButton( buttonGroup1, "pushButton5" );
    pushButton5->setGeometry( QRect( 60, 50, 30, 30 ) );
    pushButton5->setMinimumSize( QSize( 30, 30 ) );
    pushButton5->setMaximumSize( QSize( 30, 30 ) );

    pushButton1 = new QPushButton( buttonGroup1, "pushButton1" );
    pushButton1->setGeometry( QRect( 10, 10, 30, 30 ) );
    pushButton1->setMinimumSize( QSize( 30, 30 ) );
    pushButton1->setMaximumSize( QSize( 30, 30 ) );

    pushButton4 = new QPushButton( buttonGroup1, "pushButton4" );
    pushButton4->setGeometry( QRect( 10, 50, 30, 30 ) );
    pushButton4->setMinimumSize( QSize( 30, 30 ) );
    pushButton4->setMaximumSize( QSize( 30, 30 ) );

    buttonGroup2 = new QButtonGroup( tab_2, "buttonGroup2" );
    buttonGroup2->setGeometry( QRect( 300, 270, 240, 110 ) );

    newuserButton = new QPushButton( buttonGroup2, "newuserButton" );
    newuserButton->setGeometry( QRect( 11, 11, 107, 40 ) );

    modifypwdButton = new QPushButton( buttonGroup2, "modifypwdButton" );
    modifypwdButton->setEnabled( FALSE );
    modifypwdButton->setGeometry( QRect( 130, 10, 107, 40 ) );

    deleteuserButton = new QPushButton( buttonGroup2, "deleteuserButton" );
    deleteuserButton->setEnabled( FALSE );
    deleteuserButton->setGeometry( QRect( 10, 60, 107, 40 ) );

    saveButton = new QPushButton( buttonGroup2, "saveButton" );
    saveButton->setEnabled( FALSE );
    saveButton->setGeometry( QRect( 130, 60, 107, 40 ) );

    groupBox6 = new QGroupBox( tab_2, "groupBox6" );
    groupBox6->setGeometry( QRect( 30, 10, 540, 240 ) );

    pwdButton = new QPushButton( groupBox6, "pwdButton" );
    pwdButton->setGeometry( QRect( 220, 10, 140, 35 ) );
    pwdButton->setFlat( TRUE );

    fingermapButton = new QPushButton( groupBox6, "fingermapButton" );
    fingermapButton->setGeometry( QRect( 360, 10, 140, 35 ) );
    fingermapButton->setFlat( TRUE );

    previewButton = new QPushButton( groupBox6, "previewButton" );
    previewButton->setGeometry( QRect( 100, 200, 83, 30 ) );

    nextviewButton = new QPushButton( groupBox6, "nextviewButton" );
    nextviewButton->setGeometry( QRect( 190, 200, 83, 30 ) );

    labelButton3 = new QPushButton( groupBox6, "labelButton3" );
    labelButton3->setGeometry( QRect( 60, 107, 30, 20 ) );
    labelButton3->setFlat( TRUE );

    labelButton4 = new QPushButton( groupBox6, "labelButton4" );
    labelButton4->setGeometry( QRect( 60, 136, 30, 20 ) );
    labelButton4->setFlat( TRUE );

    labelButton5 = new QPushButton( groupBox6, "labelButton5" );
    labelButton5->setGeometry( QRect( 60, 160, 30, 20 ) );
    labelButton5->setFlat( TRUE );

    usernameButton = new QPushButton( groupBox6, "usernameButton" );
    usernameButton->setGeometry( QRect( 100, 10, 120, 35 ) );
    usernameButton->setFlat( TRUE );

    lineEdit12 = new QLineEdit( groupBox6, "lineEdit12" );
    lineEdit12->setGeometry( QRect( 220, 44, 140, 30 ) );
    lineEdit12->setMaxLength( 6 );
    lineEdit12->setReadOnly( TRUE );

    lineEdit13 = new QLineEdit( groupBox6, "lineEdit13" );
    lineEdit13->setGeometry( QRect( 359, 44, 140, 30 ) );
    lineEdit13->setMaxLength( 3 );
    lineEdit13->setFrame( TRUE );
    lineEdit13->setReadOnly( TRUE );

    lineEdit21 = new QLineEdit( groupBox6, "lineEdit21" );
    lineEdit21->setGeometry( QRect( 100, 74, 120, 28 ) );
    lineEdit21->setMaxLength( 4 );
    lineEdit21->setReadOnly( TRUE );

    lineEdit22 = new QLineEdit( groupBox6, "lineEdit22" );
    lineEdit22->setGeometry( QRect( 220, 74, 140, 28 ) );
    lineEdit22->setMaxLength( 6 );
    lineEdit22->setReadOnly( TRUE );

    lineEdit31 = new QLineEdit( groupBox6, "lineEdit31" );
    lineEdit31->setGeometry( QRect( 100, 102, 120, 28 ) );
    lineEdit31->setMaxLength( 4 );
    lineEdit31->setReadOnly( TRUE );

    lineEdit41 = new QLineEdit( groupBox6, "lineEdit41" );
    lineEdit41->setGeometry( QRect( 100, 130, 120, 28 ) );
    lineEdit41->setMaxLength( 4 );
    lineEdit41->setReadOnly( TRUE );

    lineEdit51 = new QLineEdit( groupBox6, "lineEdit51" );
    lineEdit51->setGeometry( QRect( 100, 158, 120, 28 ) );
    lineEdit51->setMaxLength( 4 );
    lineEdit51->setReadOnly( TRUE );

    lineEdit32 = new QLineEdit( groupBox6, "lineEdit32" );
    lineEdit32->setGeometry( QRect( 220, 102, 140, 28 ) );
    lineEdit32->setMaxLength( 6 );
    lineEdit32->setReadOnly( TRUE );

    lineEdit42 = new QLineEdit( groupBox6, "lineEdit42" );
    lineEdit42->setGeometry( QRect( 220, 130, 140, 28 ) );
    lineEdit42->setMaxLength( 6 );
    lineEdit42->setReadOnly( TRUE );

    lineEdit52 = new QLineEdit( groupBox6, "lineEdit52" );
    lineEdit52->setGeometry( QRect( 220, 158, 140, 28 ) );
    lineEdit52->setMaxLength( 6 );
    lineEdit52->setReadOnly( TRUE );

    lineEdit33 = new QLineEdit( groupBox6, "lineEdit33" );
    lineEdit33->setGeometry( QRect( 359, 102, 140, 28 ) );
    lineEdit33->setMaxLength( 3 );
    lineEdit33->setReadOnly( TRUE );

    lineEdit23 = new QLineEdit( groupBox6, "lineEdit23" );
    lineEdit23->setGeometry( QRect( 359, 74, 140, 28 ) );
    lineEdit23->setMaxLength( 3 );
    lineEdit23->setReadOnly( TRUE );

    lineEdit43 = new QLineEdit( groupBox6, "lineEdit43" );
    lineEdit43->setGeometry( QRect( 359, 130, 140, 28 ) );
    lineEdit43->setMaxLength( 3 );
    lineEdit43->setReadOnly( TRUE );

    lineEdit53 = new QLineEdit( groupBox6, "lineEdit53" );
    lineEdit53->setGeometry( QRect( 359, 158, 140, 28 ) );
    lineEdit53->setMaxLength( 3 );
    lineEdit53->setEdited( FALSE );
    lineEdit53->setReadOnly( TRUE );

    lineEdit11 = new QLineEdit( groupBox6, "lineEdit11" );
    lineEdit11->setGeometry( QRect( 100, 44, 120, 30 ) );
    lineEdit11->setMaxLength( 4 );
    lineEdit11->setReadOnly( TRUE );

    labelButton2 = new QPushButton( groupBox6, "labelButton2" );
    labelButton2->setGeometry( QRect( 60, 80, 30, 20 ) );
    labelButton2->setFlat( TRUE );

    labelButton1 = new QPushButton( groupBox6, "labelButton1" );
    labelButton1->setGeometry( QRect( 60, 50, 30, 20 ) );
    labelButton1->setFlat( TRUE );

    textLabel7 = new QLabel( groupBox6, "textLabel7" );
    textLabel7->setGeometry( QRect( 280, 190, 250, 43 ) );
    textLabel7->setAlignment( int( QLabel::AlignCenter ) );
    tabWidget->insertTab( tab_2, "" );

    tab_3 = new QWidget( tabWidget, "tab_3" );

    buttonGroup3 = new QButtonGroup( tab_3, "buttonGroup3" );
    buttonGroup3->setGeometry( QRect( 470, 40, 105, 139 ) );
    buttonGroup3->setColumnLayout(0, Qt::Vertical );
    buttonGroup3->layout()->setSpacing( 6 );
    buttonGroup3->layout()->setMargin( 11 );
    buttonGroup3Layout = new QGridLayout( buttonGroup3->layout() );
    buttonGroup3Layout->setAlignment( Qt::AlignTop );

    callButton = new QPushButton( buttonGroup3, "callButton" );

    buttonGroup3Layout->addWidget( callButton, 0, 0 );

    holdButton = new QPushButton( buttonGroup3, "holdButton" );

    buttonGroup3Layout->addWidget( holdButton, 1, 0 );

    messageButton = new QPushButton( buttonGroup3, "messageButton" );

    buttonGroup3Layout->addWidget( messageButton, 2, 0 );

    buttonGroup4 = new QButtonGroup( tab_3, "buttonGroup4" );
    buttonGroup4->setGeometry( QRect( 30, 200, 550, 170 ) );

    bachspaceButton2 = new QPushButton( buttonGroup4, "bachspaceButton2" );
    bachspaceButton2->setGeometry( QRect( 370, 90, 30, 30 ) );
    bachspaceButton2->setMinimumSize( QSize( 30, 30 ) );
    bachspaceButton2->setMaximumSize( QSize( 30, 30 ) );

    shiftButton = new QPushButton( buttonGroup4, "shiftButton" );
    shiftButton->setGeometry( QRect( 10, 130, 30, 30 ) );
    shiftButton->setMinimumSize( QSize( 30, 30 ) );
    shiftButton->setMaximumSize( QSize( 30, 30 ) );
    shiftButton->setToggleButton( TRUE );

    pushButton_enter = new QPushButton( buttonGroup4, "pushButton_enter" );
    pushButton_enter->setGeometry( QRect( 370, 50, 40, 30 ) );
    pushButton_enter->setMinimumSize( QSize( 40, 30 ) );
    pushButton_enter->setMaximumSize( QSize( 40, 30 ) );

    poundButton = new QPushButton( buttonGroup4, "poundButton" );
    poundButton->setGeometry( QRect( 510, 130, 30, 30 ) );
    poundButton->setMinimumSize( QSize( 30, 30 ) );
    poundButton->setMaximumSize( QSize( 30, 30 ) );

    pushButton_7 = new QPushButton( buttonGroup4, "pushButton_7" );
    pushButton_7->setGeometry( QRect( 430, 90, 30, 30 ) );
    pushButton_7->setMinimumSize( QSize( 30, 30 ) );
    pushButton_7->setMaximumSize( QSize( 30, 30 ) );

    pushButton_8 = new QPushButton( buttonGroup4, "pushButton_8" );
    pushButton_8->setGeometry( QRect( 470, 90, 30, 30 ) );
    pushButton_8->setMinimumSize( QSize( 30, 30 ) );
    pushButton_8->setMaximumSize( QSize( 30, 30 ) );

    pushButton_9 = new QPushButton( buttonGroup4, "pushButton_9" );
    pushButton_9->setGeometry( QRect( 510, 90, 30, 30 ) );
    pushButton_9->setMinimumSize( QSize( 30, 30 ) );
    pushButton_9->setMaximumSize( QSize( 30, 30 ) );

    pushButton_5 = new QPushButton( buttonGroup4, "pushButton_5" );
    pushButton_5->setGeometry( QRect( 470, 50, 30, 30 ) );
    pushButton_5->setMinimumSize( QSize( 30, 30 ) );
    pushButton_5->setMaximumSize( QSize( 30, 30 ) );

    pushButton_6 = new QPushButton( buttonGroup4, "pushButton_6" );
    pushButton_6->setGeometry( QRect( 510, 50, 30, 30 ) );
    pushButton_6->setMinimumSize( QSize( 30, 30 ) );
    pushButton_6->setMaximumSize( QSize( 30, 30 ) );

    pushButton_3 = new QPushButton( buttonGroup4, "pushButton_3" );
    pushButton_3->setGeometry( QRect( 510, 10, 30, 30 ) );
    pushButton_3->setMinimumSize( QSize( 30, 30 ) );
    pushButton_3->setMaximumSize( QSize( 30, 30 ) );

    pushButton_2 = new QPushButton( buttonGroup4, "pushButton_2" );
    pushButton_2->setGeometry( QRect( 470, 10, 30, 30 ) );
    pushButton_2->setMinimumSize( QSize( 30, 30 ) );
    pushButton_2->setMaximumSize( QSize( 30, 30 ) );

    pushButton_4 = new QPushButton( buttonGroup4, "pushButton_4" );
    pushButton_4->setGeometry( QRect( 430, 50, 30, 30 ) );
    pushButton_4->setMinimumSize( QSize( 30, 30 ) );
    pushButton_4->setMaximumSize( QSize( 30, 30 ) );

    pushButton_p = new QPushButton( buttonGroup4, "pushButton_p" );
    pushButton_p->setGeometry( QRect( 370, 10, 30, 30 ) );
    pushButton_p->setMinimumSize( QSize( 30, 30 ) );
    pushButton_p->setMaximumSize( QSize( 30, 30 ) );

    pushButton_o = new QPushButton( buttonGroup4, "pushButton_o" );
    pushButton_o->setGeometry( QRect( 330, 10, 30, 30 ) );
    pushButton_o->setMinimumSize( QSize( 30, 30 ) );
    pushButton_o->setMaximumSize( QSize( 30, 30 ) );

    pushButton_i = new QPushButton( buttonGroup4, "pushButton_i" );
    pushButton_i->setGeometry( QRect( 290, 10, 30, 30 ) );
    pushButton_i->setMinimumSize( QSize( 30, 30 ) );
    pushButton_i->setMaximumSize( QSize( 30, 30 ) );

    pushButton_u = new QPushButton( buttonGroup4, "pushButton_u" );
    pushButton_u->setGeometry( QRect( 250, 10, 30, 30 ) );
    pushButton_u->setMinimumSize( QSize( 30, 30 ) );
    pushButton_u->setMaximumSize( QSize( 30, 30 ) );

    pushButton_y = new QPushButton( buttonGroup4, "pushButton_y" );
    pushButton_y->setGeometry( QRect( 210, 10, 30, 30 ) );
    pushButton_y->setMinimumSize( QSize( 30, 30 ) );
    pushButton_y->setMaximumSize( QSize( 30, 30 ) );

    pushButton_t = new QPushButton( buttonGroup4, "pushButton_t" );
    pushButton_t->setGeometry( QRect( 170, 10, 30, 30 ) );
    pushButton_t->setMinimumSize( QSize( 30, 30 ) );
    pushButton_t->setMaximumSize( QSize( 30, 30 ) );

    pushButton_r = new QPushButton( buttonGroup4, "pushButton_r" );
    pushButton_r->setGeometry( QRect( 130, 10, 30, 30 ) );
    pushButton_r->setMinimumSize( QSize( 30, 30 ) );
    pushButton_r->setMaximumSize( QSize( 30, 30 ) );

    pushButton_e = new QPushButton( buttonGroup4, "pushButton_e" );
    pushButton_e->setGeometry( QRect( 90, 10, 30, 30 ) );
    pushButton_e->setMinimumSize( QSize( 30, 30 ) );
    pushButton_e->setMaximumSize( QSize( 30, 30 ) );

    pushButton_w = new QPushButton( buttonGroup4, "pushButton_w" );
    pushButton_w->setGeometry( QRect( 50, 10, 30, 30 ) );
    pushButton_w->setMinimumSize( QSize( 30, 30 ) );
    pushButton_w->setMaximumSize( QSize( 30, 30 ) );

    pushButton_q = new QPushButton( buttonGroup4, "pushButton_q" );
    pushButton_q->setGeometry( QRect( 10, 10, 30, 30 ) );
    pushButton_q->setMinimumSize( QSize( 30, 30 ) );
    pushButton_q->setMaximumSize( QSize( 30, 30 ) );

    pushButton_s = new QPushButton( buttonGroup4, "pushButton_s" );
    pushButton_s->setGeometry( QRect( 50, 50, 30, 30 ) );
    pushButton_s->setMinimumSize( QSize( 30, 30 ) );
    pushButton_s->setMaximumSize( QSize( 30, 30 ) );

    pushButton_d = new QPushButton( buttonGroup4, "pushButton_d" );
    pushButton_d->setGeometry( QRect( 90, 50, 30, 30 ) );
    pushButton_d->setMinimumSize( QSize( 30, 30 ) );
    pushButton_d->setMaximumSize( QSize( 30, 30 ) );

    pushButton_f = new QPushButton( buttonGroup4, "pushButton_f" );
    pushButton_f->setGeometry( QRect( 130, 50, 30, 30 ) );
    pushButton_f->setMinimumSize( QSize( 30, 30 ) );
    pushButton_f->setMaximumSize( QSize( 30, 30 ) );

    pushButton_g = new QPushButton( buttonGroup4, "pushButton_g" );
    pushButton_g->setGeometry( QRect( 170, 50, 30, 30 ) );
    pushButton_g->setMinimumSize( QSize( 30, 30 ) );
    pushButton_g->setMaximumSize( QSize( 30, 30 ) );

    pushButton_h = new QPushButton( buttonGroup4, "pushButton_h" );
    pushButton_h->setGeometry( QRect( 210, 50, 30, 30 ) );
    pushButton_h->setMinimumSize( QSize( 30, 30 ) );
    pushButton_h->setMaximumSize( QSize( 30, 30 ) );

    pushButton_j = new QPushButton( buttonGroup4, "pushButton_j" );
    pushButton_j->setGeometry( QRect( 250, 50, 30, 30 ) );
    pushButton_j->setMinimumSize( QSize( 30, 30 ) );
    pushButton_j->setMaximumSize( QSize( 30, 30 ) );

    pushButton_k = new QPushButton( buttonGroup4, "pushButton_k" );
    pushButton_k->setGeometry( QRect( 290, 50, 30, 30 ) );
    pushButton_k->setMinimumSize( QSize( 30, 30 ) );
    pushButton_k->setMaximumSize( QSize( 30, 30 ) );

    pushButton_l = new QPushButton( buttonGroup4, "pushButton_l" );
    pushButton_l->setGeometry( QRect( 330, 50, 30, 30 ) );
    pushButton_l->setMinimumSize( QSize( 30, 30 ) );
    pushButton_l->setMaximumSize( QSize( 30, 30 ) );

    fullstopButton = new QPushButton( buttonGroup4, "fullstopButton" );
    fullstopButton->setGeometry( QRect( 330, 90, 30, 30 ) );
    fullstopButton->setMinimumSize( QSize( 30, 30 ) );
    fullstopButton->setMaximumSize( QSize( 30, 30 ) );

    commaButton = new QPushButton( buttonGroup4, "commaButton" );
    commaButton->setGeometry( QRect( 290, 90, 30, 30 ) );
    commaButton->setMinimumSize( QSize( 30, 30 ) );
    commaButton->setMaximumSize( QSize( 30, 30 ) );

    spaceButton = new QPushButton( buttonGroup4, "spaceButton" );
    spaceButton->setGeometry( QRect( 250, 130, 71, 30 ) );
    spaceButton->setMinimumSize( QSize( 71, 30 ) );
    spaceButton->setMaximumSize( QSize( 70, 30 ) );

    capslockButton = new QPushButton( buttonGroup4, "capslockButton" );
    capslockButton->setGeometry( QRect( 330, 130, 80, 30 ) );
    capslockButton->setMinimumSize( QSize( 80, 30 ) );
    capslockButton->setMaximumSize( QSize( 80, 30 ) );
    capslockButton->setToggleButton( TRUE );

    starButton = new QPushButton( buttonGroup4, "starButton" );
    starButton->setGeometry( QRect( 430, 130, 30, 30 ) );
    starButton->setMinimumSize( QSize( 30, 30 ) );
    starButton->setMaximumSize( QSize( 30, 30 ) );

    pushButton_0 = new QPushButton( buttonGroup4, "pushButton_0" );
    pushButton_0->setGeometry( QRect( 470, 130, 30, 30 ) );
    pushButton_0->setMinimumSize( QSize( 30, 30 ) );
    pushButton_0->setMaximumSize( QSize( 30, 30 ) );

    plusButton = new QPushButton( buttonGroup4, "plusButton" );
    plusButton->setGeometry( QRect( 210, 130, 30, 30 ) );
    plusButton->setMinimumSize( QSize( 30, 30 ) );
    plusButton->setMaximumSize( QSize( 30, 30 ) );

    closeparenButton = new QPushButton( buttonGroup4, "closeparenButton" );
    closeparenButton->setGeometry( QRect( 170, 130, 30, 30 ) );
    closeparenButton->setMinimumSize( QSize( 30, 30 ) );
    closeparenButton->setMaximumSize( QSize( 30, 30 ) );

    openparenButton = new QPushButton( buttonGroup4, "openparenButton" );
    openparenButton->setGeometry( QRect( 130, 130, 30, 30 ) );
    openparenButton->setMinimumSize( QSize( 30, 30 ) );
    openparenButton->setMaximumSize( QSize( 30, 30 ) );

    questionButton = new QPushButton( buttonGroup4, "questionButton" );
    questionButton->setGeometry( QRect( 90, 130, 30, 30 ) );
    questionButton->setMinimumSize( QSize( 30, 30 ) );
    questionButton->setMaximumSize( QSize( 30, 30 ) );

    exclamationButton = new QPushButton( buttonGroup4, "exclamationButton" );
    exclamationButton->setGeometry( QRect( 50, 130, 30, 30 ) );
    exclamationButton->setMinimumSize( QSize( 30, 30 ) );
    exclamationButton->setMaximumSize( QSize( 30, 30 ) );

    pushButton_z = new QPushButton( buttonGroup4, "pushButton_z" );
    pushButton_z->setGeometry( QRect( 10, 90, 30, 30 ) );
    pushButton_z->setMinimumSize( QSize( 30, 30 ) );
    pushButton_z->setMaximumSize( QSize( 30, 30 ) );

    pushButton_x = new QPushButton( buttonGroup4, "pushButton_x" );
    pushButton_x->setGeometry( QRect( 50, 90, 30, 30 ) );
    pushButton_x->setMinimumSize( QSize( 30, 30 ) );
    pushButton_x->setMaximumSize( QSize( 30, 30 ) );

    pushButton_c = new QPushButton( buttonGroup4, "pushButton_c" );
    pushButton_c->setGeometry( QRect( 90, 90, 30, 30 ) );
    pushButton_c->setMinimumSize( QSize( 30, 30 ) );
    pushButton_c->setMaximumSize( QSize( 30, 30 ) );

    pushButton_v = new QPushButton( buttonGroup4, "pushButton_v" );
    pushButton_v->setGeometry( QRect( 130, 90, 30, 30 ) );
    pushButton_v->setMinimumSize( QSize( 30, 30 ) );
    pushButton_v->setMaximumSize( QSize( 30, 30 ) );

    pushButton_b = new QPushButton( buttonGroup4, "pushButton_b" );
    pushButton_b->setGeometry( QRect( 170, 90, 30, 30 ) );
    pushButton_b->setMinimumSize( QSize( 30, 30 ) );
    pushButton_b->setMaximumSize( QSize( 30, 30 ) );

    pushButton_n = new QPushButton( buttonGroup4, "pushButton_n" );
    pushButton_n->setGeometry( QRect( 210, 90, 30, 30 ) );
    pushButton_n->setMinimumSize( QSize( 30, 30 ) );
    pushButton_n->setMaximumSize( QSize( 30, 30 ) );

    pushButton_m = new QPushButton( buttonGroup4, "pushButton_m" );
    pushButton_m->setGeometry( QRect( 250, 90, 30, 30 ) );
    pushButton_m->setMinimumSize( QSize( 30, 30 ) );
    pushButton_m->setMaximumSize( QSize( 30, 30 ) );

    pushButton_a = new QPushButton( buttonGroup4, "pushButton_a" );
    pushButton_a->setGeometry( QRect( 10, 50, 30, 30 ) );
    pushButton_a->setMinimumSize( QSize( 30, 30 ) );
    pushButton_a->setMaximumSize( QSize( 30, 30 ) );

    pushButton_1 = new QPushButton( buttonGroup4, "pushButton_1" );
    pushButton_1->setGeometry( QRect( 430, 10, 30, 30 ) );
    pushButton_1->setMinimumSize( QSize( 30, 30 ) );
    pushButton_1->setMaximumSize( QSize( 30, 30 ) );

    textEdit = new QLineEdit( tab_3, "textEdit" );
    textEdit->setGeometry( QRect( 30, 120, 430, 40 ) );
    textEdit->setAlignment( int( QLineEdit::AlignLeft ) );

    numberEdit = new QLineEdit( tab_3, "numberEdit" );
    numberEdit->setGeometry( QRect( 30, 70, 430, 40 ) );
    numberEdit->setAlignment( int( QLineEdit::AlignRight ) );
    tabWidget->insertTab( tab_3, "" );

    // actions
    Action = new QAction( this, "Action" );


    // toolbars

    languageChange();
    resize( QSize(640, 480).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( previewButton, SIGNAL( clicked() ), this, SLOT( previewButton_clicked() ) );
    connect( nextviewButton, SIGNAL( clicked() ), this, SLOT( nextviewButton_clicked() ) );
    connect( newuserButton, SIGNAL( clicked() ), this, SLOT( newuserButton_clicked() ) );
    connect( deleteuserButton, SIGNAL( clicked() ), this, SLOT( deleteuserButton_clicked() ) );
    connect( modifypwdButton, SIGNAL( clicked() ), this, SLOT( modifypwdButton_clicked() ) );
    connect( backspaceButton1, SIGNAL( clicked() ), this, SLOT( backspaceButton1_clicked() ) );
    connect( callButton, SIGNAL( clicked() ), this, SLOT( callButton_clicked() ) );
    connect( holdButton, SIGNAL( clicked() ), this, SLOT( holdButton_clicked() ) );
    connect( messageButton, SIGNAL( clicked() ), this, SLOT( messageButton_clicked() ) );
    connect( pushButton_enter, SIGNAL( clicked() ), this, SLOT( pushButton_enter_clicked() ) );
    connect( shiftButton, SIGNAL( toggled(bool) ), this, SLOT( shiftButton_toggled(bool) ) );
    connect( capslockButton, SIGNAL( toggled(bool) ), this, SLOT( capslockButton_toggled(bool) ) );
    connect( spaceButton, SIGNAL( clicked() ), this, SLOT( spaceButton_clicked() ) );
    connect( bachspaceButton2, SIGNAL( clicked() ), this, SLOT( bachspaceButton2_clicked() ) );
    connect( pushButton0, SIGNAL( clicked() ), this, SLOT( numKey_clicked() ) );
    connect( pushButton1, SIGNAL( clicked() ), this, SLOT( numKey_clicked() ) );
    connect( pushButton2, SIGNAL( clicked() ), this, SLOT( numKey_clicked() ) );
    connect( pushButton3, SIGNAL( clicked() ), this, SLOT( numKey_clicked() ) );
    connect( pushButton4, SIGNAL( clicked() ), this, SLOT( numKey_clicked() ) );
    connect( pushButton6, SIGNAL( clicked() ), this, SLOT( numKey_clicked() ) );
    connect( pushButton7, SIGNAL( clicked() ), this, SLOT( numKey_clicked() ) );
    connect( pushButton9, SIGNAL( clicked() ), this, SLOT( numKey_clicked() ) );
    connect( pushButton_a, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_b, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_c, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_d, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_e, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_f, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_g, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_h, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_i, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_j, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_k, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_l, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_m, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_n, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_o, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_p, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_q, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_r, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_s, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_t, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_u, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_v, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_w, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_x, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_y, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_z, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_0, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_1, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_2, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_3, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_4, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_5, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_6, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_7, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_8, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton_9, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( commaButton, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( fullstopButton, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( exclamationButton, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( questionButton, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( openparenButton, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( closeparenButton, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( plusButton, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( starButton, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( poundButton, SIGNAL( clicked() ), this, SLOT( charKey_clicked() ) );
    connect( pushButton5, SIGNAL( clicked() ), this, SLOT( numKey_clicked() ) );
    connect( pushButton8, SIGNAL( clicked() ), this, SLOT( numKey_clicked() ) );
    connect( usernameButton, SIGNAL( clicked() ), this, SLOT( usernameButton_clicked() ) );
    connect( pwdButton, SIGNAL( clicked() ), this, SLOT( pwdButton_clicked() ) );
    connect( fingermapButton, SIGNAL( clicked() ), this, SLOT( fingermapButton_clicked() ) );
    connect( labelButton1, SIGNAL( clicked() ), this, SLOT( labelButton1_clicked() ) );
    connect( labelButton2, SIGNAL( clicked() ), this, SLOT( labelButton2_clicked() ) );
    connect( labelButton3, SIGNAL( clicked() ), this, SLOT( labelButton3_clicked() ) );
    connect( labelButton4, SIGNAL( clicked() ), this, SLOT( labelButton4_clicked() ) );
    connect( labelButton5, SIGNAL( clicked() ), this, SLOT( labelButton5_clicked() ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( saveButton_clicked() ) );
    connect( refreshButton, SIGNAL( clicked() ), this, SLOT( refreshButton_clicked() ) );
    connect( unlockButton, SIGNAL( clicked() ), this, SLOT( unlockButton_clicked() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
mainForm::~mainForm()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void mainForm::languageChange()
{
    setCaption( tr( "SmartHome" ) );
    groupBox5->setTitle( QString::null );
    textLabel->setText( tr( "status of the door" ) );
    groupBox7->setTitle( QString::null );
    doorstatusLabel->setText( tr( "closed" ) );
    groupBox4->setTitle( QString::null );
    alertLabel->setText( tr( "System status:" ) );
    unlockButton->setText( tr( "unlock" ) );
    refreshButton->setText( tr( "refresh" ) );
    tabWidget->changeTab( tab, tr( "Door Status" ) );
    buttonGroup1->setTitle( QString::null );
    backspaceButton1->setText( tr( "backspace" ) );
    pushButton2->setText( tr( "2" ) );
    pushButton3->setText( tr( "3" ) );
    pushButton6->setText( tr( "6" ) );
    pushButton7->setText( tr( "7" ) );
    pushButton8->setText( tr( "8" ) );
    pushButton9->setText( tr( "9" ) );
    pushButton0->setText( tr( "0" ) );
    pushButton5->setText( tr( "5" ) );
    pushButton1->setText( tr( "1" ) );
    pushButton4->setText( tr( "4" ) );
    buttonGroup2->setTitle( QString::null );
    newuserButton->setText( tr( "new user" ) );
    modifypwdButton->setText( tr( "modify pwd" ) );
    deleteuserButton->setText( tr( "delete user" ) );
    saveButton->setText( tr( "Save" ) );
    groupBox6->setTitle( QString::null );
    pwdButton->setText( tr( "Password" ) );
    fingermapButton->setText( tr( "Fingermap NO." ) );
    previewButton->setText( tr( "preview" ) );
    nextviewButton->setText( tr( "nextview" ) );
    labelButton3->setText( tr( "3" ) );
    labelButton4->setText( tr( "4" ) );
    labelButton5->setText( tr( "5" ) );
    usernameButton->setText( tr( "User Name" ) );
    labelButton2->setText( tr( "2" ) );
    labelButton1->setText( tr( "1" ) );
    textLabel7->setText( QString::null );
    tabWidget->changeTab( tab_2, tr( "User Manage" ) );
    buttonGroup3->setTitle( QString::null );
    callButton->setText( tr( "Call" ) );
    holdButton->setText( tr( "Hold" ) );
    messageButton->setText( tr( "Message" ) );
    buttonGroup4->setTitle( QString::null );
    bachspaceButton2->setText( tr( "<--" ) );
    shiftButton->setText( tr( "Shift" ) );
    pushButton_enter->setText( tr( "Enter" ) );
    poundButton->setText( tr( "#" ) );
    pushButton_7->setText( tr( "7" ) );
    pushButton_8->setText( tr( "8" ) );
    pushButton_9->setText( tr( "9" ) );
    pushButton_5->setText( tr( "5" ) );
    pushButton_6->setText( tr( "6" ) );
    pushButton_3->setText( tr( "3" ) );
    pushButton_2->setText( tr( "2" ) );
    pushButton_4->setText( tr( "4" ) );
    pushButton_p->setText( tr( "p" ) );
    pushButton_o->setText( tr( "o" ) );
    pushButton_i->setText( tr( "i" ) );
    pushButton_u->setText( tr( "u" ) );
    pushButton_y->setText( tr( "y" ) );
    pushButton_t->setText( tr( "t" ) );
    pushButton_r->setText( tr( "r" ) );
    pushButton_e->setText( tr( "e" ) );
    pushButton_w->setText( tr( "w" ) );
    pushButton_q->setText( tr( "q" ) );
    pushButton_s->setText( tr( "s" ) );
    pushButton_d->setText( tr( "d" ) );
    pushButton_f->setText( tr( "f" ) );
    pushButton_g->setText( tr( "g" ) );
    pushButton_h->setText( tr( "h" ) );
    pushButton_j->setText( tr( "j" ) );
    pushButton_k->setText( tr( "k" ) );
    pushButton_l->setText( tr( "l" ) );
    fullstopButton->setText( tr( "." ) );
    commaButton->setText( tr( "," ) );
    spaceButton->setText( QString::null );
    capslockButton->setText( tr( "CapsLock" ) );
    starButton->setText( tr( "*" ) );
    pushButton_0->setText( tr( "0" ) );
    plusButton->setText( tr( "+" ) );
    closeparenButton->setText( tr( ")" ) );
    openparenButton->setText( tr( "(" ) );
    questionButton->setText( tr( "?" ) );
    exclamationButton->setText( tr( "!" ) );
    pushButton_z->setText( tr( "z" ) );
    pushButton_x->setText( tr( "x" ) );
    pushButton_c->setText( tr( "c" ) );
    pushButton_v->setText( tr( "v" ) );
    pushButton_b->setText( tr( "b" ) );
    pushButton_n->setText( tr( "n" ) );
    pushButton_m->setText( tr( "m" ) );
    pushButton_a->setText( tr( "a" ) );
    pushButton_1->setText( tr( "1" ) );
    tabWidget->changeTab( tab_3, tr( "GPRS Module" ) );
    Action->setText( tr( "Action" ) );
}


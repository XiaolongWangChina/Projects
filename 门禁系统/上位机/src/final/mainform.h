/****************************************************************************
** Form interface generated from reading ui file 'mainform.ui'
**
** Created: 二  7月 20 20:27:20 2010
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MAINFORM_H
#define MAINFORM_H

#include <qvariant.h>
#include <qmainwindow.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QButtonGroup;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTabWidget;
class QWidget;

class mainForm : public QMainWindow
{
    Q_OBJECT

public:
    mainForm( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~mainForm();

    QTabWidget* tabWidget;
    QWidget* tab;
    QGroupBox* groupBox5;
    QLabel* textLabel;
    QGroupBox* groupBox7;
    QLabel* doorstatusLabel;
    QGroupBox* groupBox4;
    QLabel* alertLabel;
    QPushButton* unlockButton;
    QPushButton* refreshButton;
    QWidget* tab_2;
    QButtonGroup* buttonGroup1;
    QPushButton* backspaceButton1;
    QPushButton* pushButton2;
    QPushButton* pushButton3;
    QPushButton* pushButton6;
    QPushButton* pushButton7;
    QPushButton* pushButton8;
    QPushButton* pushButton9;
    QPushButton* pushButton0;
    QPushButton* pushButton5;
    QPushButton* pushButton1;
    QPushButton* pushButton4;
    QButtonGroup* buttonGroup2;
    QPushButton* newuserButton;
    QPushButton* modifypwdButton;
    QPushButton* deleteuserButton;
    QPushButton* saveButton;
    QGroupBox* groupBox6;
    QPushButton* pwdButton;
    QPushButton* fingermapButton;
    QPushButton* previewButton;
    QPushButton* nextviewButton;
    QPushButton* labelButton3;
    QPushButton* labelButton4;
    QPushButton* labelButton5;
    QPushButton* usernameButton;
    QLineEdit* lineEdit12;
    QLineEdit* lineEdit13;
    QLineEdit* lineEdit21;
    QLineEdit* lineEdit22;
    QLineEdit* lineEdit31;
    QLineEdit* lineEdit41;
    QLineEdit* lineEdit51;
    QLineEdit* lineEdit32;
    QLineEdit* lineEdit42;
    QLineEdit* lineEdit52;
    QLineEdit* lineEdit33;
    QLineEdit* lineEdit23;
    QLineEdit* lineEdit43;
    QLineEdit* lineEdit53;
    QLineEdit* lineEdit11;
    QPushButton* labelButton2;
    QPushButton* labelButton1;
    QLabel* textLabel7;
    QWidget* tab_3;
    QButtonGroup* buttonGroup3;
    QPushButton* callButton;
    QPushButton* holdButton;
    QPushButton* messageButton;
    QButtonGroup* buttonGroup4;
    QPushButton* bachspaceButton2;
    QPushButton* shiftButton;
    QPushButton* pushButton_enter;
    QPushButton* poundButton;
    QPushButton* pushButton_7;
    QPushButton* pushButton_8;
    QPushButton* pushButton_9;
    QPushButton* pushButton_5;
    QPushButton* pushButton_6;
    QPushButton* pushButton_3;
    QPushButton* pushButton_2;
    QPushButton* pushButton_4;
    QPushButton* pushButton_p;
    QPushButton* pushButton_o;
    QPushButton* pushButton_i;
    QPushButton* pushButton_u;
    QPushButton* pushButton_y;
    QPushButton* pushButton_t;
    QPushButton* pushButton_r;
    QPushButton* pushButton_e;
    QPushButton* pushButton_w;
    QPushButton* pushButton_q;
    QPushButton* pushButton_s;
    QPushButton* pushButton_d;
    QPushButton* pushButton_f;
    QPushButton* pushButton_g;
    QPushButton* pushButton_h;
    QPushButton* pushButton_j;
    QPushButton* pushButton_k;
    QPushButton* pushButton_l;
    QPushButton* fullstopButton;
    QPushButton* commaButton;
    QPushButton* spaceButton;
    QPushButton* capslockButton;
    QPushButton* starButton;
    QPushButton* pushButton_0;
    QPushButton* plusButton;
    QPushButton* closeparenButton;
    QPushButton* openparenButton;
    QPushButton* questionButton;
    QPushButton* exclamationButton;
    QPushButton* pushButton_z;
    QPushButton* pushButton_x;
    QPushButton* pushButton_c;
    QPushButton* pushButton_v;
    QPushButton* pushButton_b;
    QPushButton* pushButton_n;
    QPushButton* pushButton_m;
    QPushButton* pushButton_a;
    QPushButton* pushButton_1;
    QLineEdit* textEdit;
    QLineEdit* numberEdit;
    QAction* Action;

public slots:
    void styleChar( QLineEdit * le1, QLineEdit * le2, QLineEdit * le3, int loadArrayIndex );
    virtual void loadUserToTable( int whichpage );
    virtual void refreshButton_clicked();
    virtual void unlockButton_clicked();
    virtual void previewButton_clicked();
    virtual void nextviewButton_clicked();
    virtual void newuserButton_clicked();
    virtual void deleteuserButton_clicked();
    virtual void modifypwdButton_clicked();
    virtual void backspaceButton1_clicked();
    virtual void callButton_clicked();
    virtual void holdButton_clicked();
    virtual void messageButton_clicked();
    virtual void pushButton_enter_clicked();
    virtual void shiftButton_toggled( bool on );
    virtual void capslockButton_toggled( bool on );
    virtual void spaceButton_clicked();
    virtual void bachspaceButton2_clicked();
    virtual void numKey_clicked();
    virtual void charKey_clicked();
    virtual void usernameButton_clicked();
    virtual void pwdButton_clicked();
    virtual void fingermapButton_clicked();
    virtual void labelButton1_clicked();
    virtual void labelButton2_clicked();
    virtual void labelButton3_clicked();
    virtual void labelButton4_clicked();
    virtual void labelButton5_clicked();
    virtual void saveButton_clicked();

protected:
    QGridLayout* groupBox5Layout;
    QGridLayout* groupBox7Layout;
    QGridLayout* buttonGroup3Layout;

protected slots:
    virtual void languageChange();
private:
    void init();
    void destroy();

};

#endif // MAINFORM_H

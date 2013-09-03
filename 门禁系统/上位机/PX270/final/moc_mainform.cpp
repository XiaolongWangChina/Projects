/****************************************************************************
** mainForm meta object code from reading C++ file 'mainform.h'
**
** Created: Tue Jul 20 20:27:27 2010
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "mainform.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *mainForm::className() const
{
    return "mainForm";
}

QMetaObject *mainForm::metaObj = 0;
static QMetaObjectCleanUp cleanUp_mainForm( "mainForm", &mainForm::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString mainForm::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "mainForm", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString mainForm::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "mainForm", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* mainForm::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "le1", &static_QUType_ptr, "QLineEdit", QUParameter::In },
	{ "le2", &static_QUType_ptr, "QLineEdit", QUParameter::In },
	{ "le3", &static_QUType_ptr, "QLineEdit", QUParameter::In },
	{ "loadArrayIndex", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"styleChar", 4, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "whichpage", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"loadUserToTable", 1, param_slot_1 };
    static const QUMethod slot_2 = {"refreshButton_clicked", 0, 0 };
    static const QUMethod slot_3 = {"unlockButton_clicked", 0, 0 };
    static const QUMethod slot_4 = {"previewButton_clicked", 0, 0 };
    static const QUMethod slot_5 = {"nextviewButton_clicked", 0, 0 };
    static const QUMethod slot_6 = {"newuserButton_clicked", 0, 0 };
    static const QUMethod slot_7 = {"deleteuserButton_clicked", 0, 0 };
    static const QUMethod slot_8 = {"modifypwdButton_clicked", 0, 0 };
    static const QUMethod slot_9 = {"backspaceButton1_clicked", 0, 0 };
    static const QUMethod slot_10 = {"callButton_clicked", 0, 0 };
    static const QUMethod slot_11 = {"holdButton_clicked", 0, 0 };
    static const QUMethod slot_12 = {"messageButton_clicked", 0, 0 };
    static const QUMethod slot_13 = {"pushButton_enter_clicked", 0, 0 };
    static const QUParameter param_slot_14[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"shiftButton_toggled", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"capslockButton_toggled", 1, param_slot_15 };
    static const QUMethod slot_16 = {"spaceButton_clicked", 0, 0 };
    static const QUMethod slot_17 = {"bachspaceButton2_clicked", 0, 0 };
    static const QUMethod slot_18 = {"numKey_clicked", 0, 0 };
    static const QUMethod slot_19 = {"charKey_clicked", 0, 0 };
    static const QUMethod slot_20 = {"usernameButton_clicked", 0, 0 };
    static const QUMethod slot_21 = {"pwdButton_clicked", 0, 0 };
    static const QUMethod slot_22 = {"fingermapButton_clicked", 0, 0 };
    static const QUMethod slot_23 = {"labelButton1_clicked", 0, 0 };
    static const QUMethod slot_24 = {"labelButton2_clicked", 0, 0 };
    static const QUMethod slot_25 = {"labelButton3_clicked", 0, 0 };
    static const QUMethod slot_26 = {"labelButton4_clicked", 0, 0 };
    static const QUMethod slot_27 = {"labelButton5_clicked", 0, 0 };
    static const QUMethod slot_28 = {"saveButton_clicked", 0, 0 };
    static const QUMethod slot_29 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "styleChar(QLineEdit*,QLineEdit*,QLineEdit*,int)", &slot_0, QMetaData::Public },
	{ "loadUserToTable(int)", &slot_1, QMetaData::Public },
	{ "refreshButton_clicked()", &slot_2, QMetaData::Public },
	{ "unlockButton_clicked()", &slot_3, QMetaData::Public },
	{ "previewButton_clicked()", &slot_4, QMetaData::Public },
	{ "nextviewButton_clicked()", &slot_5, QMetaData::Public },
	{ "newuserButton_clicked()", &slot_6, QMetaData::Public },
	{ "deleteuserButton_clicked()", &slot_7, QMetaData::Public },
	{ "modifypwdButton_clicked()", &slot_8, QMetaData::Public },
	{ "backspaceButton1_clicked()", &slot_9, QMetaData::Public },
	{ "callButton_clicked()", &slot_10, QMetaData::Public },
	{ "holdButton_clicked()", &slot_11, QMetaData::Public },
	{ "messageButton_clicked()", &slot_12, QMetaData::Public },
	{ "pushButton_enter_clicked()", &slot_13, QMetaData::Public },
	{ "shiftButton_toggled(bool)", &slot_14, QMetaData::Public },
	{ "capslockButton_toggled(bool)", &slot_15, QMetaData::Public },
	{ "spaceButton_clicked()", &slot_16, QMetaData::Public },
	{ "bachspaceButton2_clicked()", &slot_17, QMetaData::Public },
	{ "numKey_clicked()", &slot_18, QMetaData::Public },
	{ "charKey_clicked()", &slot_19, QMetaData::Public },
	{ "usernameButton_clicked()", &slot_20, QMetaData::Public },
	{ "pwdButton_clicked()", &slot_21, QMetaData::Public },
	{ "fingermapButton_clicked()", &slot_22, QMetaData::Public },
	{ "labelButton1_clicked()", &slot_23, QMetaData::Public },
	{ "labelButton2_clicked()", &slot_24, QMetaData::Public },
	{ "labelButton3_clicked()", &slot_25, QMetaData::Public },
	{ "labelButton4_clicked()", &slot_26, QMetaData::Public },
	{ "labelButton5_clicked()", &slot_27, QMetaData::Public },
	{ "saveButton_clicked()", &slot_28, QMetaData::Public },
	{ "languageChange()", &slot_29, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"mainForm", parentObject,
	slot_tbl, 30,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_mainForm.setMetaObject( metaObj );
    return metaObj;
}

void* mainForm::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "mainForm" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool mainForm::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: styleChar((QLineEdit*)static_QUType_ptr.get(_o+1),(QLineEdit*)static_QUType_ptr.get(_o+2),(QLineEdit*)static_QUType_ptr.get(_o+3),(int)static_QUType_int.get(_o+4)); break;
    case 1: loadUserToTable((int)static_QUType_int.get(_o+1)); break;
    case 2: refreshButton_clicked(); break;
    case 3: unlockButton_clicked(); break;
    case 4: previewButton_clicked(); break;
    case 5: nextviewButton_clicked(); break;
    case 6: newuserButton_clicked(); break;
    case 7: deleteuserButton_clicked(); break;
    case 8: modifypwdButton_clicked(); break;
    case 9: backspaceButton1_clicked(); break;
    case 10: callButton_clicked(); break;
    case 11: holdButton_clicked(); break;
    case 12: messageButton_clicked(); break;
    case 13: pushButton_enter_clicked(); break;
    case 14: shiftButton_toggled((bool)static_QUType_bool.get(_o+1)); break;
    case 15: capslockButton_toggled((bool)static_QUType_bool.get(_o+1)); break;
    case 16: spaceButton_clicked(); break;
    case 17: bachspaceButton2_clicked(); break;
    case 18: numKey_clicked(); break;
    case 19: charKey_clicked(); break;
    case 20: usernameButton_clicked(); break;
    case 21: pwdButton_clicked(); break;
    case 22: fingermapButton_clicked(); break;
    case 23: labelButton1_clicked(); break;
    case 24: labelButton2_clicked(); break;
    case 25: labelButton3_clicked(); break;
    case 26: labelButton4_clicked(); break;
    case 27: labelButton5_clicked(); break;
    case 28: saveButton_clicked(); break;
    case 29: languageChange(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool mainForm::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool mainForm::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool mainForm::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

#include <qapplication.h>
#include "mainform.h"
int main(int argc, char ** argv)
{
    QApplication a(argc, argv);
    mainForm form;
    form.setCaption("Form");
    a.setMainWidget(&form);
    form.show();
    return a.exec();
}

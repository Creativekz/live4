#include <iostream>
#include <view.h>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);   //  управляет главным потоком и настройками GUI
    Window w;   //  создание объекта класса главного окна
    w.show();   //  отображение созданного окна
    qDebug() << "TEST";
    return app.exec();  //  начало обработки событий
}

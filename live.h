//  Файл с классом обеспечивающим работу алгоритма самой игры

#include <algorithm>
#include <fstream>
#include <QtDebug>
//  перечисление статусов клетки
enum E_RSTATE	{
    R_DEAD,     //  мертва
    R_ALIVE,    //  жива
    R_BORN,     //  возродилась (была мертва)
    R_DIED      //  умерла (была жива)
};

class LiveModel
{
public:
    LiveModel(int, int);
    LiveModel(const char*);
    ~LiveModel();

    int Save(const char*);
    void Step();
    void SetRow(int, int, bool);
    bool GetRow(int, int);
    void GetSize(int&, int&);
    bool IsGameOver();

private:
    int Calculate(int, int);
    E_RSTATE Rules(int, E_RSTATE);

    int sizeX = 0, sizeY = 0;   // размеры поля
    E_RSTATE** Field;   //  указатель на динамический массив со статусами клеток поля
};

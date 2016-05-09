#include "live.h"

//  Конструктор класса игры
LiveModel::LiveModel(int x, int y)
{
    qDebug() << "Live model";
    // создает новую игру на основе размеров поля (введенных в окне пользователем)
    sizeX = x, sizeY = y;

    //  выделение памяти под массив хранящий статусы клеток поля
    Field = new E_RSTATE*[sizeY];
    for (int i = 0; i < sizeY; i++)
    {
        Field[i] = new E_RSTATE[sizeX];
        for (int j = 0; j < sizeX; j++)
            Field[i][j] = R_DEAD;   //  задание начального статуса клетке (= мертвая)
    }
}

//  Альтернативный конструктор класса игры
LiveModel::LiveModel(const char* filename)
{
    //  грузит данные об игре из файла
    std::ifstream in(filename);
    if (!in.is_open())
        return;
    in >> sizeX >> sizeY;

    Field = new E_RSTATE*[sizeY];
    for (int i = 0; i < sizeY; i++)
    {
        Field[i] = new E_RSTATE[sizeX];
        for (int j = 0; j < sizeX; j++) {
            int val;
            in >> val;
            Field[i][j] = (E_RSTATE)val;
            if (in.fail())	{
                Field[i][j] = R_DEAD;
            }
        }
    }
    in.close();
}

//  деструктор класса
LiveModel::~LiveModel()
{
    // очистка динамической памяти, выделенной в конструкторе
    for (int i = 0; i < sizeY; i++)
        delete[] Field[i];
    delete[] Field;
}

//  сохранение данных игры в файл
int LiveModel::Save(const char* filename)
{
    std::ofstream out(filename);
    if (!out.is_open())	{
        return false;
    }
    out << sizeX << " " << sizeY << " " << '\n';
    for (int i = 0; i < sizeY; i++)
        for (int j = 0; j < sizeX; j++)
            out << Field[i][j] << '\n';
    out.close();
    return true;
}

//  функция алгоритма самой игры: высчитывает количество клеток вокруг заданой
int LiveModel::Calculate(int x, int y)
{
    int p = 0;
    for (int i = y - 1; i <= y + 1; i++)
    {
        for (int j = x - 1; j <= x + 1; j++)
        {
            if (i < 0 || j < 0 || i >= sizeY || j >= sizeX || (i == y && j == x))
                continue;

            if (Field[i][j] == R_ALIVE || Field[i][j] == R_DIED)  p++;
        }
    }
    return p;
}

//  функция алгоритма самой игры: на основании количества живых соседей вокруг клетки определяет ее новый статус
E_RSTATE LiveModel::Rules(int p, E_RSTATE state)
{
    if (state == R_DEAD && p > 2)					return R_BORN;
    else if (state == R_ALIVE && (p < 2 || p > 3))	return R_DIED;
    else 											return state;
}

//  функция алгоритма самой игры: обновляет статусы клеток вызывая сопутствующие алгоритмы Rules и Calculate
void LiveModel::Step()
{
    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            Field[i][j] = Rules(Calculate(j, i), Field[i][j]);
        }
    }
    //  дабы новые значения не повлияли на результат выполнения алгоритма для соседних клеток, имеются промежуточные статусы
    //  хранящие как бы и старое значение и новое: R_BORN - возродилась (была мертва) и R_DIED - умерла (была жива)
    // соответсвенно после обработки всего поля, заменяем временные статусы на настоящие
    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            if (Field[i][j] == R_BORN)		Field[i][j] = R_ALIVE;
            else if (Field[i][j] == R_DIED)	Field[i][j] = R_DEAD;
        }
    }
}

//  функция алгоритма самой игры: проверяет условия окончания игры
bool LiveModel::IsGameOver()
{
    int live = 0;
    for (int i = 0; i < sizeY; i++)
        for (int j = 0; j < sizeX; j++)
            if(Field[i][j] != R_DEAD)   live++;
    return (live == 0 || live == sizeX * sizeY) ? true : false;
}

//  устанавливает статус клетки
void LiveModel::SetRow(int x, int y, bool state)
{
    Field[y][x] = (state) ? R_ALIVE : R_DEAD;
}

//  получает статус клетки
bool LiveModel::GetRow(int x, int y)
{
    return (bool)Field[y][x];
}

//  получает размер поля
void LiveModel::GetSize(int& x, int& y)
{
    x = sizeX;
    y = sizeY;
}

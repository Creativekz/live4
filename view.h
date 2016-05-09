//  Файл с классами обеспечивающими работу графического интерфейса

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QSpinBox>
#include <QTime>
#include <QSlider>
#include <QMouseEvent>

#include <memory>

#include <live.h>

//  максимальные размеры поля
#define MaxSizeX 100
#define MaxSizeY 100

//  Класс, объектами которых является каждая отдельная клетка поля
class Sector : public QWidget
{
    Q_OBJECT    //  обязательно для любого объекта, поддерживающего сигналы, слоты

public:
    Sector(QWidget *parent, int x, int y, int size, QColor color);

signals:
    void SelectedCell(int, int);

protected:
    virtual void mousePressEvent(QMouseEvent *me);

private:
    int x, y;
};

//  Класс всего поля, содержит в себе массив из объектов класса Sector
class Field : public QWidget
{
public:
    Field(QWidget* parent, int ASizeX, int ASizeY);
    void SetSectorColor(QColor color, int x, int y);
    int SizeX, SizeY;
private:
    std::map<int, Sector*> arr;
};

//  Класс панели кнопок управления игрой
class Console : public QWidget
{
    Q_OBJECT    //  обязательно для любого объекта, поддерживающего сигналы, слоты

    QFileDialog* fd_load;
    QPushButton* Pause;

public:
    Console(QWidget* parent);

public slots:
    void FileChosenLoad(const QString&);
    void ChangeText(const QString&);

signals:
    void LoadSelect(const QString& str);
    void SaveSelect(const QString& str);
};

//  Класс диалога создания новой игры
class NewWorldDialog : public QDialog
{
    Q_OBJECT    //  обязательно для любого объекта, поддерживающего сигналы, слоты

public:
    QSpinBox* HBox;
    QSpinBox* VBox;
    QSpinBox* PredBox;
    QSpinBox* VictBox;

    NewWorldDialog(QWidget* parent);

public slots:
    void Confirmed(bool);

signals:
    void ConfirmedNewWorld();
};

//  Класс главного окна игры
class Window : public QWidget
{
    Q_OBJECT

    Field* field = 0;
    LiveModel* live = 0;

    bool pause = false;

    QFileDialog* fd_save;
    NewWorldDialog* dialog;
    QVBoxLayout* vlayout;
    int period = 250;

public:
    Window();
    void SetSectorColor(QColor color, int x, int y);
    void Action();
    void UpdateField();

public slots:
    void SelectedCell(int, int);
    void CreateNewWorld();
    void SetNewWorld();
    void Load(const QString&);
    void SaveFile();
    void Save(const QString&);
    void Pause();
    void SetPeriod(int);

signals:
    void step();
    void Paused(QString);
    void Started();

private:
    QMenu *fileMenu;
    QAction *newGameAction, *openAction, *saveGameAction, *exitAction;
};

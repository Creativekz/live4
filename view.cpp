#include "view.h"

//  конструктор класса отображения конкретной клетки
Sector::Sector(QWidget* parent, int x, int y, int size, QColor color) : QWidget(parent)
{
    QSize s;
    s.setWidth(size);
    s.setHeight(size);
    setFixedSize(s);                //  установка размеров клетки
    setPalette(QPalette(color));    //  установка цвета клетки
    setAutoFillBackground(true);
    this->x = x;
    this->y = y;
}

//  вызывается при нажатии мышкой на клетку
void Sector::mousePressEvent(QMouseEvent *me)
{
    //  проверяем что нажали именно левой кнопкой мышки
    if(me->buttons() && Qt::LeftButton) {
        emit SelectedCell(x, y);
    }
}

//  конструктор класса отображения поля
Field::Field(QWidget *parent, int SizeX, int SizeY) : QWidget(parent)
{
    this->SizeX = SizeX; this->SizeY = SizeY;
    QGridLayout *grid = new QGridLayout;
    int SectorSizeX = 350 / SizeX;
    int SectorSizeY = 350 / SizeY;
    int SectorSize = (SectorSizeX > SectorSizeY) ? SectorSizeY : SectorSizeX;
    for (int y = 0; y < SizeY; y++)
    {
        for (int x = 0; x < SizeX; x++)
        {
            arr[y * SizeX + x] = new Sector(this, x, y, SectorSize, QColor(255, 255, 255));
            connect(arr[y * SizeX + x], SIGNAL(SelectedCell(int, int)), parent, SLOT(SelectedCell(int, int)));
            grid->addWidget(arr[y * SizeX + x], y, x);
        }
    }
    grid->setSpacing(2);
    setLayout(grid);
    setPalette(QPalette(QColor(100, 100, 100)));
    setAutoFillBackground(true);
    setFixedSize(sizeHint());
}

//  устанавливает цвет конкретной клетке по координатам
void Field::SetSectorColor(QColor color, int x, int y)
{
    arr[y * SizeX + x]->setPalette(QPalette(color));
}

//  функция вызываемая при выборе файла в окне для загрузки игры
void Console::FileChosenLoad(const QString& str)
{
    fd_load->close();   //  закрываем диалоговое окно выбора файла
    emit LoadSelect(str);   //  вызываем функцию загружающую игру из файла
}

//  изменяет текст объекта Pause (кнопки)
void Console::ChangeText(const QString& str)
{
    Pause->setText(str);
}

//  конструктор раздела с кнопками управления игрой
Console::Console(QWidget *parent) : QWidget(parent)
{
    QPushButton* NewWorld = new QPushButton("Новая игра", this);
    NewWorld->setFixedSize(100, 30);

    connect(NewWorld, SIGNAL(clicked(bool)), parent, SLOT(SetNewWorld()));

    QPushButton* Load = new QPushButton("Загрузка", this);
    Load->setFixedSize(100, 30);

    fd_load = new QFileDialog(this, "Загрузка");
    fd_load->setFileMode(QFileDialog::ExistingFile);

    connect(Load, SIGNAL(clicked()), fd_load, SLOT(show()));
    connect(fd_load, SIGNAL(fileSelected(const QString&)), this, SLOT(FileChosenLoad(const QString&)));

    QPushButton* Save = new QPushButton("Сохранение", this);
    Save->setFixedSize(100, 30);

    connect(Save, SIGNAL(clicked()), parent, SLOT(SaveFile()));

    Pause = new QPushButton("Начать", this);
    Pause->setFixedSize(100, 30);

    connect(Pause, SIGNAL(clicked(bool)), parent, SLOT(Pause()));
    connect(parent, SIGNAL(Paused(const QString&)), this, SLOT(ChangeText(const QString&)));

    QSlider* slider = new QSlider(this);
    slider->setRange(0, 500);
    slider->setValue(250);
    slider->setOrientation(Qt::Horizontal);
    connect(slider, SIGNAL(valueChanged(int)), parent, SLOT(SetPeriod(int)));

    QLabel* label = new QLabel("<p align = center>Скорость игры</p>", this);

    QVBoxLayout* vl = new QVBoxLayout(this);
    vl->addWidget(NewWorld);
    vl->addWidget(Load);
    vl->addWidget(Save);
    vl->addWidget(Pause);
    vl->addWidget(label);
    vl->addWidget(slider);
    setLayout(vl);
}

//  функция вызываемая при нажатии "ОК" в диалоге создания новой игры
void NewWorldDialog::Confirmed(bool)
{
    emit ConfirmedNewWorld();   //  вызываем функцию создания игры
    close();    // закрываем окно
}

// конструктор окна создания новой игры
NewWorldDialog::NewWorldDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Новая игра");
    setFixedSize(200, 150);

    QLabel* HSize = new QLabel("Размер по горизонтали");
    HBox = new QSpinBox(this);
    HBox->setRange(1, MaxSizeX);
    HBox->setValue(20);
    HBox->setFixedWidth(50);
    QHBoxLayout* HLayout = new QHBoxLayout();
    HLayout->addWidget(HSize);
    HLayout->addWidget(HBox);

    QLabel* VSize = new QLabel("Размер по вертикали");
    VBox = new QSpinBox(this);
    VBox->setRange(1, MaxSizeY);
    VBox->setValue(20);
    VBox->setFixedWidth(50);
    QHBoxLayout* VLayout = new QHBoxLayout();
    VLayout->addWidget(VSize);
    VLayout->addWidget(VBox);

    QPushButton* confirm = new QPushButton("Создать", this);
    confirm->setFixedSize(100, 30);

    connect(confirm, SIGNAL(clicked(bool)), this, SLOT(Confirmed(bool)));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(HLayout);
    layout->addLayout(VLayout);
    layout->addWidget(confirm);
    setLayout(layout);
}

//  функция окна которая вызывает функцию поля которая устанавливает цвет клетки
void Window::SetSectorColor(QColor color, int x, int y)
{
    field->SetSectorColor(color, x, y);
}

//  конструктор главного окна игры
Window::Window() : QWidget(0)
{
    Console* console = new Console(this);   //  создает объект кнопок управления игрой

    connect(console, SIGNAL(LoadSelect(const QString&)), this, SLOT(Load(const QString&))); //  соединяем сигнал с объекта кнопок со слотом главного окна
    dialog = new NewWorldDialog(this);  //  создаем объект класса с окном создания новой игры
    connect(dialog, SIGNAL(ConfirmedNewWorld()), this, SLOT(CreateNewWorld())); // соединяем сигнал с объекта диалога создания игры со слотом главного окна
    fd_save = new QFileDialog(this, "Сохранение");  //  создание файлового диалога для сохранения игры в файл
    fd_save->setFileMode(QFileDialog::AnyFile);
    connect(fd_save, SIGNAL(fileSelected(const QString&)), this, SLOT(Save(const QString&))); // соединяем сигнал с объекта диалога сохранения игры со слотом главного окна
    setMinimumSize(640, 480);
}

//  изменение скорости игры
void Window::SetPeriod(int p)
{
    period = 500-p;
}

//  обновление главного окна игры
void Window::Action()
{
    do
    {
        if (!this->isActiveWindow())
            break;
        if(pause)   break;

        live->Step();   //  выполнение очередного хода игры
        UpdateField();  //  обвноление поля

        //  пауза между обновлением (чтобы замедлять/ускорять игру)
        QTime time;
        time.start();
        for (; time.elapsed() < period;)
            qApp->processEvents();
        qApp->processEvents();
    }
    while(live->IsGameOver() == false || pause);    //  проверка условий окончания игры
    if(pause == false)  {
        Pause();
        QMessageBox::information(0, "Game Over", "Игра закончена!");
    }
}

//  обвноление цвета клеток
void Window::UpdateField()
{
    for (int x = 0; x < field->SizeX; x++)
        for (int y = 0; y < field->SizeY; y++)
            SetSectorColor(live->GetRow(x, y) ? Qt::yellow : Qt::gray, x, y);
}

//  функция нажатия на клетку
void Window::SelectedCell(int x, int y)
{
    if(pause)   {
        //  если игра не напаузе, то клетка при нажатии изменит свой статус на противоположенный
        //  мертвая оживает, живая умирает
        live->SetRow(x, y, !live->GetRow(x, y));
        UpdateField();  //  поле обновляется после этого целиком
    }
}

//  функция показа окна для сохранения игры в файл
void Window::SaveFile()
{
    if(live)    //  если игра начата
    {
        fd_save->show();    //  показ окна
    }
    else
    {
        // иначе ошибка
        QMessageBox::information(0, "Ошибка", "Не создано поле для сохранения");
    }
}

//  функция отображения окна создания новой игры
void Window::SetNewWorld()
{
    dialog->show();
}

//  функция создания новой игры
void Window::CreateNewWorld()
{
    if(live)    {
        // если игра уже идет, то удаляем ее
        delete live;
    }

    //  создаем новый объект класса игры
    live = new LiveModel(dialog->HBox->value(), dialog->VBox->value());
    if (field)
        field->close();
    //  создаем новое поле
    field = new Field(this, dialog->HBox->value(), dialog->VBox->value());
    field->move(150, 10);
    field->show();  // показываем поле
    UpdateField();  // обвноляем его
    pause = true;   //  ставим на паузу изначально
}

//  функция загрузки игры из файла
void Window::Load(const QString& filename)
{
    if(live)    {
        // если игра уже идет, то удаляем ее
        delete live;
    }
    //  создаем новый объект класса игры
    live = new LiveModel(filename.toStdString().c_str());
    if (field)
        field->close();
    int x, y;
    live->GetSize(x, y);
    if(x == 0 || y == 0)    {
        QMessageBox::information(0, "Информация", "Поле не было загружено!");
    }
    field = new Field(this, x, y);
    field->move(150, 10);
    field->show();  // показываем поле
    UpdateField();  // обвноляем его
    pause = true;   //  ставим на паузу изначально
    QMessageBox::information(0, "Информация", "Поле успешно загружено!");
}

//  функция сохранения игры в файл
void Window::Save(const QString& filename)
{
       //   вызываем функцию сохранения и если все успещно выводим инфу, либо ошибку
    if(live->Save(filename.toStdString().c_str()))  {
        QMessageBox::information(0, "Информация", "Поле сохранено!");
    }
    else
    {
        QMessageBox::information(0, "Информация", "Поле не сохранено!");
    }
}

//  функция остановки игры
void Window::Pause()
{
    if(live)
    {   //  если игра создана
        if(pause)
        {   //  и стоит пауза
            pause = false;  //  то паузу снимаем
            emit Paused("Пауза");
            Action();   //  продолжаем игру
        }
        else
        {   //  если пауза не стоит, то ставим игру на паузу
            pause = true;
            emit Paused("Начать");
        }
    }
}

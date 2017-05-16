#include "mainwindow.h"
#include "ataxxsave.h"
#include <cmath>
#include <ctime>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QString>
#include <QMessageBox>
#include <QDialog>

const int chesskindnum = 2, boardkindnum = 5;

void AnimWait(int tt)
{
    QEventLoop eventloop;
    QTimer::singleShot(tt, &eventloop, SLOT(quit()));
    eventloop.exec();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QWidget *center = new QWidget(this);
    setCentralWidget(center);
    board = new Board(this);
    board->setMinimumSize(460, 460);
    board->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //board->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    QuitButton = new QPushButton("退出游戏");
    QObject::connect(QuitButton, SIGNAL(clicked()), this, SLOT(close()));
    NewGameButton = new QPushButton("开始新游戏");
    QObject::connect(NewGameButton, SIGNAL(clicked()), board, SLOT(NewGame()));
    SaveGameButton = new QPushButton("存储游戏");
    QObject::connect(SaveGameButton, SIGNAL(clicked()), board, SLOT(SaveGame()));
    SaveGameButton->setEnabled(false);
    QObject::connect(board, SIGNAL(GameEnd()), this, SLOT(DisableSave()));
    QObject::connect(board, SIGNAL(GameStart()), this, SLOT(EnableSave()));
    LoadGameButton = new QPushButton("读取游戏");
    QObject::connect(LoadGameButton, SIGNAL(clicked()), board, SLOT(LoadGame()));
    RetractButton = new QPushButton("悔棋");
    QObject::connect(RetractButton, SIGNAL(clicked()), board, SLOT(Retract()));
    RetractButton->setEnabled(false);
    QObject::connect(board, SIGNAL(checkzero()), this, SLOT(DisableRetract()));
    QObject::connect(board, SIGNAL(checknotzero()), this, SLOT(EnableRetract()));
    RuleButton = new QPushButton("查看规则");
    QObject::connect(RuleButton, SIGNAL(clicked()), board, SLOT(ShowRule()));
    BoardButton = new QPushButton("棋盘样式");
    QObject::connect(BoardButton, SIGNAL(clicked()), board, SLOT(ChangeBoard()));
    ChessButton = new QPushButton("棋子样式");
    QObject::connect(ChessButton, SIGNAL(clicked()), board, SLOT(ChangeChess()));

    StatusLab = new QLabel("未开始游戏");
    StatusLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    WhiteLab = new QLabel;
    WhiteLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    BlackLab = new QLabel;
    BlackLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    CountLab = new QLabel;
    CountLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    QObject::connect(board, SIGNAL(ChangeBar()), this, SLOT(UpdateBar()));
    QObject::connect(board, SIGNAL(BlackWin()), this, SLOT(BlackWinBar()));
    QObject::connect(board, SIGNAL(WhiteWin()), this, SLOT(WhiteWinBar()));
    QObject::connect(board, SIGNAL(Deuce()), this, SLOT(DeuceBar()));

    PlayerLab = new QLabel("\n");
    PlayerLab->setAlignment(Qt::AlignTop);

    ModeLab = new QLabel;
    ModeLab->setAlignment(Qt::AlignTop);

    QVBoxLayout *Menu = new QVBoxLayout;
    Menu->addWidget(NewGameButton);
    Menu->addWidget(RetractButton);
    Menu->addWidget(SaveGameButton);
    Menu->addWidget(LoadGameButton);
    Menu->addWidget(QuitButton);
    Menu->addWidget(RuleButton);
    QHBoxLayout *Bar = new QHBoxLayout;
    Bar->addWidget(StatusLab);
    Bar->addWidget(BlackLab);
    Bar->addWidget(WhiteLab);
    Bar->addWidget(CountLab);
    QHBoxLayout *Cgbt = new QHBoxLayout;
    Cgbt->addStretch();
    Cgbt->addWidget(BoardButton);
    Cgbt->addStretch();
    Cgbt->addWidget(ChessButton);
    Cgbt->addStretch();
    QVBoxLayout *PLab = new QVBoxLayout;
    PLab->addWidget(ModeLab);
    PLab->addWidget(PlayerLab);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(board, 0, 0);
    layout->addLayout(Menu, 0, 1);
    layout->addLayout(Bar, 2, 0);
    layout->addLayout(Cgbt, 1, 0);
    layout->addLayout(PLab, 1, 1, 2, 1);
    //layout->setColumnMinimumWidth(0, 480);
    //layout->setRowMinimumHeight(0, 480);
    center->setLayout(layout);
}

MainWindow::~MainWindow()
{
    //delete ui;
}

SaveDialog::SaveDialog(QWidget *parent) :
    QDialog(parent)
{
    setFixedSize(360, 400);
    setWindowTitle("存储游戏");
    Savelab = new QLabel("存档名称：");
    LEdit = new QLineEdit(this);
    QRegExp reg("[a-zA-Z0-9-_]{25}");
    QRegExpValidator *validator = new QRegExpValidator(reg, this);
    LEdit->setValidator(validator);
    SaveList = new QListWidget(this);

    QObject::connect(SaveList, SIGNAL(currentTextChanged(const QString &)), LEdit, SLOT(setText(const QString &)));
    QObject::connect(SaveList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(DisplayInfo()));

    CancelButton = new QPushButton("Cancel");
    OKButton = new QPushButton("Save");

    Timelab = new QLabel;
    Player1lab = new QLabel;
    Player2lab = new QLabel;
    Modelab = new QLabel;

    QObject::connect(CancelButton, SIGNAL(clicked()), this, SLOT(CancelSave()));
    QObject::connect(OKButton, SIGNAL(clicked()), this, SLOT(SaveFile()));
    QObject::connect(this, SIGNAL(SaveSignal(QString)), parent, SLOT(Output(QString)));

    QGridLayout *Form = new QGridLayout;
    Form->addWidget(Savelab, 0, 0);
    Form->addWidget(LEdit, 0, 1);
    Form->addWidget(SaveList, 1, 0, 1, 2);

    QGridLayout *Info = new QGridLayout;
    Info->addWidget(Timelab, 0, 0);
    Info->addWidget(Modelab, 1, 0);
    Info->addWidget(Player1lab, 0, 1);
    Info->addWidget(Player2lab, 1, 1);

    QHBoxLayout *Btn = new QHBoxLayout;
    Btn->addStretch();
    Btn->addWidget(CancelButton);
    Btn->addWidget(OKButton);
    Btn->addStretch();

    Form->addLayout(Info, 2, 0, 1, 2);
    Form->addLayout(Btn, 3, 0, 1, 2);

    this->setLayout(Form);
}

SaveDialog::~SaveDialog()
{

}

void SaveDialog::CancelSave()
{
    close();
    delete this;
}

void SaveDialog::SaveFile()
{
    QString Path;
    Path = QCoreApplication::applicationDirPath()+"/Saves/"+LEdit->text()+".dat";
    QFileInfo fi(Path);
    if (fi.exists()) {
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("覆盖存储内容"),
                                       QString(tr("此存档已存在，确认覆盖？")),
                                       QMessageBox::No | QMessageBox::Yes);
        if (button == QMessageBox::No) return;
    }
    emit SaveSignal(Path);
    close();
}

void SaveDialog::DisplayInfo()
{
    QString Path = QCoreApplication::applicationDirPath()+"/Saves/"+LEdit->text()+".dat";
    QFileInfo fi(Path);
    if (fi.exists()) {
        QFile file(Path);
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_7);
        QString x;
        int md;
        in >> x >> md;
        Timelab->setText(QString("存储时间:"+x.section('_', 0, 0).trimmed()+" "+x.section('_', 1, 1).trimmed()));
        QString mdlab = "游戏模式:";
        if (md == 0) mdlab += " 玩家对战";
        else if (md == 1) mdlab += " 人机对战(智障AI)";
        else if (md == 2) mdlab += " 人机对战(简单AI)";
        else if (md == 3) mdlab += " 人机对战(普通AI)";
        else if (md == 4) mdlab += " 人机对战(困难AI)";
        Modelab->setText(mdlab);
        Player1lab->setText(QString("黑方: "+x.section('_', 3, 3)));
        Player2lab->setText(QString("白方: "+x.section('_', 4, 4)));
    }
    else {
        Timelab->setText("");
        Modelab->setText("");
        Player1lab->setText("");
        Player2lab->setText("");
    }
}

LoadDialog::LoadDialog(QWidget *parent) :
    QDialog(parent)
{
    setFixedSize(360, 400);
    setWindowTitle("读取游戏");
    Loadlab = new QLabel("读取存档：");
    LEdit = new QLineEdit(this);
    LEdit->setReadOnly(true);
    SaveList = new QListWidget(this);

    QObject::connect(SaveList, SIGNAL(currentTextChanged(const QString &)), LEdit, SLOT(setText(const QString &)));
    QObject::connect(SaveList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(DisplayInfo()));

    CancelButton = new QPushButton("Cancel");
    OKButton = new QPushButton("Load");

    Timelab = new QLabel;
    Player1lab = new QLabel;
    Player2lab = new QLabel;
    Modelab = new QLabel;

    QObject::connect(CancelButton, SIGNAL(clicked()), this, SLOT(CancelLoad()));
    QObject::connect(OKButton, SIGNAL(clicked()), this, SLOT(LoadFile()));
    QObject::connect(this, SIGNAL(LoadSignal(QString)), parent, SLOT(Input(QString)));

    QGridLayout *Form = new QGridLayout;
    Form->addWidget(Loadlab, 0, 0);
    Form->addWidget(LEdit, 0, 1);
    Form->addWidget(SaveList, 1, 0, 1, 2);

    QGridLayout *Info = new QGridLayout;
    Info->addWidget(Timelab, 0, 0);
    Info->addWidget(Modelab, 1, 0);
    Info->addWidget(Player1lab, 0, 1);
    Info->addWidget(Player2lab, 1, 1);

    QHBoxLayout *Btn = new QHBoxLayout;
    Btn->addStretch();
    Btn->addWidget(CancelButton);
    Btn->addWidget(OKButton);
    Btn->addStretch();

    Form->addLayout(Info, 2, 0, 1, 2);
    Form->addLayout(Btn, 3, 0, 1, 2);

    this->setLayout(Form);
}

LoadDialog::~LoadDialog()
{

}

void LoadDialog::CancelLoad()
{
    close();
    delete this;
}

void LoadDialog::LoadFile()
{
    QString Path;
    Path = QCoreApplication::applicationDirPath()+"/Saves/"+LEdit->text()+".dat";
    QFileInfo fi(Path);
    if (fi.exists()) {
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("读取存档"),
                                       QString(tr("当前未存储进度将丢失，确认继续？")),
                                       QMessageBox::No | QMessageBox::Yes);
        if (button == QMessageBox::No) return;
        emit LoadSignal(Path);
        close();
    }
    else {
        QMessageBox::information(this, "读取失败", "未选择存档！", QMessageBox::Ok);
    }
}

void LoadDialog::DisplayInfo()
{
    QString Path = QCoreApplication::applicationDirPath()+"/Saves/"+LEdit->text()+".dat";
    QFileInfo fi(Path);
    if (fi.exists()) {
        QFile file(Path);
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_7);
        QString x;
        int md;
        in >> x >> md;
        Timelab->setText(QString("存储时间:"+x.section('_', 0, 0).trimmed()+" "+x.section('_', 1, 1).trimmed()));
        QString mdlab = "游戏模式:";
        if (md == 0) mdlab += " 玩家对战";
        else if (md == 1) mdlab += " 人机对战(智障AI)";
        else if (md == 2) mdlab += " 人机对战(简单AI)";
        else if (md == 3) mdlab += " 人机对战(普通AI)";
        else if (md == 4) mdlab += " 人机对战(困难AI)";
        Modelab->setText(mdlab);
        Player1lab->setText(QString("黑方: "+x.section('_', 3, 3)));
        Player2lab->setText(QString("白方: "+x.section('_', 4, 4)));
    }
    else {
        Timelab->setText("");
        Modelab->setText("");
        Player1lab->setText("");
        Player2lab->setText("");
    }
}

NewGameDialog::NewGameDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("新游戏");
    NewGamelab = new QLabel("请选择游戏模式与难度：");
    box = new QComboBox;
    box->addItem("玩家对战");
    box->addItem("人机对战(智障AI)");
    box->addItem("人机对战(简单AI)");
    box->addItem("人机对战(普通AI)");
    box->addItem("人机对战(困难AI)");
    CancelButton = new QPushButton("Cancel");
    OKButton = new QPushButton("Start");

    QObject::connect(CancelButton, SIGNAL(clicked()), this, SLOT(CancelNewGame()));
    QObject::connect(OKButton, SIGNAL(clicked()), this, SLOT(StartNewGame()));
    QObject::connect(this, SIGNAL(GameSignal(int, QString, QString)), parent, SLOT(SetGame(int, QString, QString)));
    QObject::connect(this, SIGNAL(AISignal(int, int)), parent, SLOT(SetAI(int, int)));

    QHBoxLayout *toplayout = new QHBoxLayout;
    toplayout->addWidget(NewGamelab);
    toplayout->addWidget(box);

    QHBoxLayout *Btn = new QHBoxLayout;
    Btn->addWidget(CancelButton);
    Btn->addWidget(OKButton);

    QVBoxLayout *MainLayout = new QVBoxLayout;
    MainLayout->addLayout(toplayout);
    MainLayout->addLayout(Btn);
    setLayout(MainLayout);
}

NewGameDialog::~NewGameDialog()
{

}

void NewGameDialog::CancelNewGame()
{
    close();
    delete this;
}

void NewGameDialog::StartNewGame()
{
    int id = box->currentIndex();
    if (id == 0)
    {
        MvMDialog *dialog = new MvMDialog(this);
        dialog->show();
    }
    else
    {
        MvCDialog *dialog = new MvCDialog(this);
        dialog->cmode = id;
        dialog->p1->setText("Player1");
        if (id == 1) dialog->p2->setText("RetardAI");
        else if (id == 2) dialog->p2->setText("SimpleAI");
        else if (id == 3) dialog->p2->setText("NormalAI");
        else if (id == 4) dialog->p2->setText("HardAI");
        dialog->p2->setReadOnly(true);
        dialog->show();
    }
}

void NewGameDialog::SetGameSignal(int md, QString name1, QString name2)
{
    emit GameSignal(md, name1, name2);
    close();
}

void NewGameDialog::SetAISignal(int md, int color)
{
    emit AISignal(md, color);
    close();
}

MvMDialog::MvMDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("新玩家对局");
    lab1 = new QLabel("黑方玩家：");
    lab2 = new QLabel("白方玩家：");
    p1 = new QLineEdit;
    p2 = new QLineEdit;
    p1->setText("Player1");
    p2->setText("Player2");
    CancelButton = new QPushButton("Cancel");
    OKButton = new QPushButton("Start");

    QObject::connect(CancelButton, SIGNAL(clicked()), this, SLOT(CancelMvM()));
    QObject::connect(OKButton, SIGNAL(clicked()), this, SLOT(StartMvM()));
    QObject::connect(this, SIGNAL(MvMSignal(int, QString, QString)), parent, SLOT(SetGameSignal(int, QString, QString)));

    QHBoxLayout *h1 = new QHBoxLayout;
    h1->addWidget(lab1);
    h1->addWidget(p1);
    QHBoxLayout *h2 = new QHBoxLayout;
    h2->addWidget(lab2);
    h2->addWidget(p2);
    QHBoxLayout *Btn = new QHBoxLayout;
    Btn->addWidget(CancelButton);
    Btn->addWidget(OKButton);
    QVBoxLayout *Mainlayout = new QVBoxLayout;
    Mainlayout->addLayout(h1);
    Mainlayout->addLayout(h2);
    Mainlayout->addLayout(Btn);
    setLayout(Mainlayout);
}

MvMDialog::~MvMDialog()
{

}

void MvMDialog::CancelMvM()
{
    close();
    delete this;
}

void MvMDialog::StartMvM()
{
    emit MvMSignal(0, p1->text(), p2->text());
    close();
    delete this;
}

MvCDialog::MvCDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("新人机对局");
    lab1 = new QLabel("黑方玩家：");
    lab2 = new QLabel("白方玩家：");
    Colorlab = new QLabel("请选择你的棋子颜色：");
    p1 = new QLineEdit;
    p2 = new QLineEdit;
    CancelButton = new QPushButton("Cancel");
    OKButton = new QPushButton("Start");
    BlackButton = new QPushButton("黑棋");
    BlackButton->setStyleSheet("color: white; background-color:black");
    WhiteButton = new QPushButton("白棋");
    WhiteButton->setStyleSheet("color: black; background-color:white");

    QObject::connect(BlackButton, SIGNAL(clicked()), this, SLOT(ChooseBlack()));
    QObject::connect(WhiteButton, SIGNAL(clicked()), this, SLOT(ChooseWhite()));
    QObject::connect(CancelButton, SIGNAL(clicked()), this, SLOT(CancelMvC()));
    QObject::connect(OKButton, SIGNAL(clicked()), this, SLOT(StartMvC()));
    QObject::connect(this, SIGNAL(MvCSignal(int, QString, QString)), parent, SLOT(SetGameSignal(int, QString, QString)));
    QObject::connect(this, SIGNAL(AIInfo(int, int)), parent, SLOT(SetAISignal(int, int)));

    QHBoxLayout *Choosecolor = new QHBoxLayout;
    Choosecolor->addWidget(Colorlab);
    Choosecolor->addWidget(BlackButton);
    Choosecolor->addWidget(WhiteButton);
    QHBoxLayout *h1 = new QHBoxLayout;
    h1->addWidget(lab1);
    h1->addWidget(p1);
    QHBoxLayout *h2 = new QHBoxLayout;
    h2->addWidget(lab2);
    h2->addWidget(p2);
    QHBoxLayout *Btn = new QHBoxLayout;
    Btn->addWidget(CancelButton);
    Btn->addWidget(OKButton);
    QVBoxLayout *Mainlayout = new QVBoxLayout;
    Mainlayout->addLayout(Choosecolor);
    Mainlayout->addLayout(h1);
    Mainlayout->addLayout(h2);
    Mainlayout->addLayout(Btn);
    setLayout(Mainlayout);
}

MvCDialog::~MvCDialog()
{

}

void MvCDialog::ChooseBlack()
{
    QString temp;
    if (p1->isReadOnly())
    {
        temp = p1->text();
        p1->setText(p2->text());
        p2->setText(temp);
        p2->setReadOnly(true);
        p1->setReadOnly(false);
    }
}

void MvCDialog::ChooseWhite()
{
    QString temp;
    if (p2->isReadOnly())
    {
        temp = p1->text();
        p1->setText(p2->text());
        p2->setText(temp);
        p2->setReadOnly(false);
        p1->setReadOnly(true);
    }
}

void MvCDialog::CancelMvC()
{
    close();
    delete this;
}

void MvCDialog::StartMvC()
{
    emit MvCSignal(cmode, p1->text(), p2->text());
    if (p1->isReadOnly()) emit AIInfo(cmode, 1);
    else emit AIInfo(cmode, -1);
    close();
    delete this;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("退出游戏"),
                                   QString(tr("确认退出游戏？未存储的棋局将丢失.")),
                                   QMessageBox::No | QMessageBox::Yes);
    if (button == QMessageBox::No) {
          event->ignore();
    }
    else if (button == QMessageBox::Yes) {
          event->accept();
    }
}

Board::Board(QWidget *parent) :
    QWidget(parent)
{
    resize(480, 480);
    memset(gridInfo, 0, sizeof(gridInfo));
    memset(indicator, 0, sizeof(indicator));
    mode = -1;
    winner = 0;
    count = 0;
    chesskind = 1;
    boardkind = 3;
    boardpng = QPixmap(":/board/Board03");
    blackpng = QPixmap(":/board/black1");
    whitepng = QPixmap(":/board/white1");
}

Board::~Board()
{

}

void Board::CopyBoard()
{
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            gridInfo[count+1][i][j] = gridInfo[count][i][j];
}

void Board::SetGame(int md, QString name1, QString name2)
{
    mode = md;
    gridInfo[0][0][0] = gridInfo[0][6][6] = 1;
    gridInfo[0][0][6] = gridInfo[0][6][0] = -1;
    currcolor = 1;
    chosenX = chosenY = -1;
    count = 0;
    winner = 0;
    p1.Name = name1;
    p2.Name = name2;
    p1.pcolor = 1; p2.pcolor = -1;
    if (mode == 0) p1.type = p2.type = 0;
    emit GameStart();
    update();
}

void Board::SetAI(int md, int color)
{
    if (color == 1) {
        p1.type = md;
        p2.type = 0;
    }
    else {
        p2.type = md;
        p1.type = 0;
    }
    currAI.AIcolor = color;
    memset(currAI.gInfo, 0, sizeof(currAI.gInfo));
    update();
    AIMove();
}

void Board::NewGame()
{
    if (mode >= 0) {
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("开始新游戏"),
                                       QString(tr("确认开始新游戏？当前未存储的棋局将丢失.")),
                                       QMessageBox::No | QMessageBox::Yes);
        if (button == QMessageBox::No) return;
        else mode = -1;
    }
    NewGameDialog *select = new NewGameDialog(this);
    select->show();
}

void Board::Input(QString Path)
{
    QFile file(Path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "读取失败", "读取失败！", QMessageBox::Ok);
        return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_7);
    QString GameInfo;
    in >> GameInfo >> mode >> count >> currcolor >> p1.type >> p2.type;
    p1.Name = GameInfo.section('_', 3, 3).trimmed();
    p2.Name = GameInfo.section('_', 4, 4).trimmed();
    if (p1.type) SetAI(mode, 1);
    else if (p2.type) SetAI(mode, -1);
    for (int k = 0; k <= count; k++)
        for (int i = 0; i < 7; i++)
            for (int j = 0; j < 7; j++)
                in >> gridInfo[k][i][j];
    emit GameStart();
    update();
}

void Board::Output(QString Path)
{
    QString GameInfo;
    QDateTime time = QDateTime::currentDateTime();
    GameInfo = time.toString("yyyy-MM-dd_hh:mm_");
    if (mode == 0) GameInfo += "MvM_";
    else if (mode > 0) GameInfo += "MvC_";
    GameInfo += p1.Name + "_" + p2.Name;
    QFile file(Path);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, "存储失败", "存储失败！", QMessageBox::Ok);
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_7);
    out << GameInfo << mode << count << currcolor << p1.type << p2.type;
    for (int k = 0; k <= count; k++)
        for (int i = 0; i < 7; i++)
            for (int j = 0; j < 7; j++)
                out << gridInfo[k][i][j];
    QDir currdir;
    QMessageBox::information(this, "存储成功", "存储成功！", QMessageBox::Ok);
}

void Board::SaveGame()
{
    QStringList FileList = GetFileList(QCoreApplication::applicationDirPath()+"/Saves/");
    int FileNum = GetFileNum(QCoreApplication::applicationDirPath()+"/Saves/");
    QDir *dir = new QDir;
    if (!dir->exists(QCoreApplication::applicationDirPath()+"/Saves/")) dir->mkdir(QCoreApplication::applicationDirPath()+"/Saves/");
    delete dir;
    SaveDialog *newsave = new SaveDialog(this);
    for (int i = 0; i < FileNum; i++) {
        new QListWidgetItem(FileList[i], newsave->SaveList);
    }
    if (FileNum == -1) FileNum = 0;
    FileNum++;
    QString filename = "Ataxxsave_";
    filename += QString::number(FileNum);
    newsave->LEdit->setText(filename);
    newsave->show();
}

void Board::LoadGame()
{
    QStringList FileList = GetFileList(QCoreApplication::applicationDirPath()+"/Saves/");
    int FileNum = GetFileNum(QCoreApplication::applicationDirPath()+"/Saves/");
    if (FileNum <= 0) {
        QMessageBox::information(this, "读取失败", "无存档", QMessageBox::Ok);
        return;
    }
    LoadDialog *newsave = new LoadDialog(this);
    for (int i = 0; i < FileNum; i++) {
        new QListWidgetItem(FileList[i], newsave->SaveList);
    }
    newsave->show();
}

void Board::ShowRule()
{
    QDialog *ruleview = new QDialog();
    QLabel *rule = new QLabel(ruleview);
    rule->setOpenExternalLinks(true);
    rule->setWordWrap(true);
    rule->setAlignment(Qt::AlignTop);
    ruleview->setFixedSize(600, 470);
    rule->setText(tr("<div><p>同化棋是一种二人棋盘游戏，黑白双方交替在7x7的棋盘上移动棋子。</p><p>玩家每次可以移动的位置是有限的，具体步骤解释如下：</p>")+
                  tr("<ul><li>选取现在在棋盘上己方的一枚棋子，再选择一个落子位置。<strong>要求落子处为空，既没有对方也没有己方的棋子。</strong></li>")+
                  tr("<li>选择的落子位置可以是以下二者之一：")+
                  tr("<ul>")+
                  tr("<li><strong>落子位置在以选取的棋子为中心的3x3的范围内。此时选取的棋子会复制自身到落子位置。一共有8个位置可以选择。</strong></li>")+
                  tr("<li><strong>落子位置在以选取的棋子为中心的3x3的范围外、5x5的范围内，此时选取的棋子会移动自身到落子位置，一共有16个位置可以选择。</strong></li>")+
                  tr("</ul>")+
                  tr("</li>")+
                  tr("<li>总的来说，移动一格是“复制粘贴”，移动两格是“剪切粘贴”。</li>")+
                  tr("<li>每次落子后，新位置的落子会将落子处紧邻的8个位置的棋子（如果有）都变成自己的棋子。</li>")+
                  tr("<li>若没有上述这种位置则游戏结束，将所有无子的地方算作有子可下一方的棋子，然后数出双方棋子数，棋多者胜。</li>")+
                  tr("<li>任意时刻对方棋子数为0时，己方胜利。</li>")+
                  tr("<li>游戏回合数到400时，游戏会直接结束，数出双方棋子数，棋多者胜。</li>")+
                  tr("<li>游戏开始时，黑白棋各占2个角，黑棋先行。")+
                  tr("</ul>")+
                  tr("<a href = \"https://wiki.botzone.org/index.php?title=Ataxx\" align = \"center\">查看更多</a>")+
                  tr("</div>"));
    QPushButton *OKButton = new QPushButton("OK", ruleview);
    QObject::connect(OKButton, SIGNAL(clicked()), ruleview, SLOT(close()));
    QHBoxLayout *Btn = new QHBoxLayout;
    Btn->addStretch();
    Btn->addWidget(OKButton);
    Btn->addStretch();
    QVBoxLayout *RuleLayout = new QVBoxLayout;
    RuleLayout->addWidget(rule);
    RuleLayout->addLayout(Btn);
    ruleview->setLayout(RuleLayout);
    ruleview->show();
}

void Board::ChangeBoard()
{
    boardkind++;
    if (boardkind == boardkindnum) boardkind = 0;
    if (boardkind == 1) boardpng.load(":/board/Board01");
    if (boardkind == 2) boardpng.load(":/board/Board02");
    if (boardkind == 3) boardpng.load(":/board/Board03");
    if (boardkind == 4) boardpng.load(":/board/Board04");
    update();
}

void Board::ChangeChess()
{
    chesskind++;
    if (chesskind == chesskindnum) chesskind = 0;
    if (chesskind == 0) {whitepng.load(":/board/white0"); blackpng.load(":/board/black0");}
    if (chesskind == 1) {whitepng.load(":/board/white1"); blackpng.load(":/board/black1");}
    update();
}

bool Board::Moveable(int color)
{
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            if (gridInfo[count][i][j] == color)
            {
                for (int ki = 0; ki < 7; ki++)
                    for (int kj = 0; kj < 7; kj++)
                        if (CheckMove(i, j, ki, kj))
                        {
                            return true;
                        }
            }
    return false;
}

int Board::GetNum(int color)
{
    int s = 0;
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            if (gridInfo[count][i][j] == color)
                s++;
    return s;
}

void Board::CheckWin()
{
    if (!Moveable(1) || !Moveable(-1))
    {
        if (Moveable(1) && GetNum(1) > 0 && GetNum(-1) > 0)
        {
            CopyBoard();
            count++;
            for (int i = 0; i < 7; i++)
                for (int j = 0; j < 7; j++)
                    if (!gridInfo[count][i][j])
                        gridInfo[count][i][j] = 1;
            if (GetNum(1) > GetNum(-1))
            {
                QString text;
                text.sprintf("白棋无法继续行棋，当前局面黑棋可占数目为%d，白棋数目为%d，黑棋胜！", GetNum(1), GetNum(-1));
                QMessageBox::information(this, "Black Win", text, QMessageBox::Ok);
                mode = -1;
                winner = 1;
            }
            else if (GetNum(1) < GetNum(-1))
            {
                QString text;
                text.sprintf("白棋无法继续行棋，当前局面黑棋可占数目为%d，白棋数目为%d，白棋胜！", GetNum(1), GetNum(-1));
                QMessageBox::information(this, "White Win", text, QMessageBox::Ok);
                mode = -1;
                winner = -1;
            }
            count--;
        }
        else if (Moveable(-1) && GetNum(1) > 0 && GetNum(-1) > 0)
        {
            CopyBoard();
            count++;
            for (int i = 0; i < 7; i++)
                for (int j = 0; j < 7; j++)
                    if (!gridInfo[count][i][j])
                        gridInfo[count][i][j] = -1;
            if (GetNum(1) > GetNum(-1))
            {
                QString text;
                text.sprintf("黑棋无法继续行棋，当前局面黑棋数目为%d，白棋可占数目为%d，黑棋胜！", GetNum(1), GetNum(-1));
                QMessageBox::information(this, "Black Win", text, QMessageBox::Ok);
                mode = -1;
                winner = 1;
            }
            else if (GetNum(1) < GetNum(-1))
            {
                QString text;
                text.sprintf("黑棋无法继续行棋，当前局面黑棋数目为%d，白棋可占数目为%d，白棋胜！", GetNum(1), GetNum(-1));
                QMessageBox::information(this, "White Win", text, QMessageBox::Ok);
                mode = -1;
                winner = -1;
            }
            count--;
        }
        else
        {
            if (GetNum(1) > GetNum(-1))
            {
                QString text;
                text.sprintf("游戏结束，黑棋数目为%d，白棋数目为%d，黑棋胜！", GetNum(1), GetNum(-1));
                QMessageBox::information(this, "Black Win", text, QMessageBox::Ok);
                mode = -1;
                winner = 1;
            }
            else if (GetNum(1) < GetNum(-1))
            {
                QString text;
                text.sprintf("游戏结束，黑棋数目为%d，白棋数目为%d，白棋胜！", GetNum(1), GetNum(-1));
                QMessageBox::information(this, "White Win", text, QMessageBox::Ok);
                mode = -1;
                winner = -1;
            }
        }
    }
    else if (count >= 400)
    {
        if (GetNum(1) > GetNum(-1))
        {
            QString text;
            text.sprintf("本局游戏已达到400步，当前局面黑棋数目为%d，白棋数目为%d，黑棋胜！", GetNum(1), GetNum(-1));
            QMessageBox::information(this, "Black Win", text, QMessageBox::Ok);
            mode = -1;
            winner = 1;
        }
        else if (GetNum(1) < GetNum(-1))
        {
            QString text;
            text.sprintf("本局游戏已达到400步，当前局面黑棋数目为%d，白棋数目为%d，白棋胜！", GetNum(1), GetNum(-1));
            QMessageBox::information(this, "White Win", text, QMessageBox::Ok);
            mode = -1;
            winner = -1;
        }
        else
        {
            QString text;
            text.sprintf("本局游戏已达到400步，当前局面黑棋数目为%d，白棋数目为%d，平局！", GetNum(1), GetNum(-1));
            QMessageBox::information(this, "Deuce", text, QMessageBox::Ok);
            mode = -1;
            winner = 2;
        }
    }
}

int Board::CheckMove(int x0, int y0, int x1, int y1)
{
    if (x1 < 0 || y1 < 0 || x1 > 6 || y1 > 6) return 0;
    if (x1 == x0 && y1 == y0) return 0;
    if (abs(x1-x0) > 2 || abs(y1-y0) > 2) return 0;
    if (gridInfo[count][x1][y1] != 0) return 0;
    if (abs(x1-x0) <= 1 && abs(y1-y0) <= 1) return 1;
    return 2;
}

void Board::UpMove(int x, int y, int color)
{
    if (x > 0) {
        if (gridInfo[count][x-1][y] == -color) gridInfo[count][x-1][y] = color;
        if (y > 0 && gridInfo[count][x-1][y-1] == -color) gridInfo[count][x-1][y-1] = color;
        if (y < 6 && gridInfo[count][x-1][y+1] == -color) gridInfo[count][x-1][y+1] = color;
    }
    if (y > 0 && gridInfo[count][x][y-1] == -color) gridInfo[count][x][y-1] = color;
    if (y < 6 && gridInfo[count][x][y+1] == -color) gridInfo[count][x][y+1] = color;
    if (x < 6) {
        if (gridInfo[count][x+1][y] == -color) gridInfo[count][x+1][y] = color;
        if (y > 0 && gridInfo[count][x+1][y-1] == -color) gridInfo[count][x+1][y-1] = color;
        if (y < 6 && gridInfo[count][x+1][y+1] == -color) gridInfo[count][x+1][y+1] = color;
    }
}

void Board::MoveAnimation(int sx, int sy, int ex, int ey, int color)
{
    int ltx = (w/2)-200, lty = (h/2)-200;
    QLabel *png = new QLabel(this);
    if (color == 1) png->setPixmap(blackpng);
    else png->setPixmap(whitepng);
    png->show();
    QPropertyAnimation *moveanim = new QPropertyAnimation(png, "pos");
    moveanim->setDuration(150);
    moveanim->setStartValue(QPoint(sx*60+ltx+10, sy*60+lty+10));
    moveanim->setEndValue(QPoint(ex*60+ltx+10, ey*60+lty+10));
    moveanim->start();
    AnimWait(180);
    delete png; delete moveanim;
}

void Board::ChangeAnimation(int x, int y, int color)
{
    int ltx = (w/2)-200, lty = (h/2)-200;
    int len = 150;
    QPixmap mp;
    if (color == 1) mp = blackpng;
    else mp = whitepng;
    QLabel *png1 = new QLabel(this); png1->setPixmap(mp); png1->move(x*60+ltx+10, y*60+lty+10);
    QLabel *png2 = new QLabel(this); png2->setPixmap(mp); png2->move(x*60+ltx+10, y*60+lty+10);
    QLabel *png3 = new QLabel(this); png3->setPixmap(mp); png3->move(x*60+ltx+10, y*60+lty+10);
    QLabel *png4 = new QLabel(this); png4->setPixmap(mp); png4->move(x*60+ltx+10, y*60+lty+10);
    QLabel *png5 = new QLabel(this); png5->setPixmap(mp); png5->move(x*60+ltx+10, y*60+lty+10);
    QLabel *png6 = new QLabel(this); png6->setPixmap(mp); png6->move(x*60+ltx+10, y*60+lty+10);
    QLabel *png7 = new QLabel(this); png7->setPixmap(mp); png7->move(x*60+ltx+10, y*60+lty+10);
    QLabel *png8 = new QLabel(this); png8->setPixmap(mp); png8->move(x*60+ltx+10, y*60+lty+10);
    QParallelAnimationGroup group;
    QPropertyAnimation *anim1 = new QPropertyAnimation(png1, "pos"); anim1->setDuration(len);
    anim1->setStartValue(QPoint(x*60+ltx+10, y*60+lty+10)); anim1->setEndValue(QPoint((x-1)*60+ltx+10, y*60+lty+10));
    QPropertyAnimation *anim2 = new QPropertyAnimation(png2, "pos"); anim2->setDuration(len);
    anim2->setStartValue(QPoint(x*60+ltx+10, y*60+lty+10)); anim2->setEndValue(QPoint((x-1)*60+ltx+10, (y-1)*60+lty+10));
    QPropertyAnimation *anim3 = new QPropertyAnimation(png3, "pos"); anim3->setDuration(len);
    anim3->setStartValue(QPoint(x*60+ltx+10, y*60+lty+10)); anim3->setEndValue(QPoint((x-1)*60+ltx+10, (y+1)*60+lty+10));
    QPropertyAnimation *anim4 = new QPropertyAnimation(png4, "pos"); anim4->setDuration(len);
    anim4->setStartValue(QPoint(x*60+ltx+10, y*60+lty+10)); anim4->setEndValue(QPoint(x*60+ltx+10, (y-1)*60+lty+10));
    QPropertyAnimation *anim5 = new QPropertyAnimation(png5, "pos"); anim5->setDuration(len);
    anim5->setStartValue(QPoint(x*60+ltx+10, y*60+lty+10)); anim5->setEndValue(QPoint(x*60+ltx+10, (y+1)*60+lty+10));
    QPropertyAnimation *anim6 = new QPropertyAnimation(png6, "pos"); anim6->setDuration(len);
    anim6->setStartValue(QPoint(x*60+ltx+10, y*60+lty+10)); anim6->setEndValue(QPoint((x+1)*60+ltx+10, y*60+lty+10));
    QPropertyAnimation *anim7 = new QPropertyAnimation(png7, "pos"); anim7->setDuration(len);
    anim7->setStartValue(QPoint(x*60+ltx+10, y*60+lty+10)); anim7->setEndValue(QPoint((x+1)*60+ltx+10, (y-1)*60+lty+10));
    QPropertyAnimation *anim8 = new QPropertyAnimation(png8, "pos"); anim8->setDuration(len);
    anim8->setStartValue(QPoint(x*60+ltx+10, y*60+lty+10)); anim8->setEndValue(QPoint((x+1)*60+ltx+10, (y+1)*60+lty+10));
    group.addAnimation(anim1); group.addAnimation(anim2); group.addAnimation(anim3); group.addAnimation(anim4);
    group.addAnimation(anim5); group.addAnimation(anim6); group.addAnimation(anim7); group.addAnimation(anim8);
    if (x > 0) {
        if (gridInfo[count][x-1][y] == -color) png1->show();
        if (y > 0 && gridInfo[count][x-1][y-1] == -color) png2->show();
        if (y < 6 && gridInfo[count][x-1][y+1] == -color) png3->show();
    }
    if (y > 0 && gridInfo[count][x][y-1] == -color) png4->show();
    if (y < 6 && gridInfo[count][x][y+1] == -color) png5->show();
    if (x < 6) {
        if (gridInfo[count][x+1][y] == -color) png6->show();
        if (y > 0 && gridInfo[count][x+1][y-1] == -color) png7->show();
        if (y < 6 && gridInfo[count][x+1][y+1] == -color) png8->show();
    }
    group.start();
    AnimWait(len+50);
    delete png1; delete png2; delete png3; delete png4; delete png5; delete png6; delete png7; delete png8;
    delete anim1; delete anim2; delete anim3; delete anim4; delete anim5; delete anim6; delete anim7; delete anim8;
}

void Board::Retract()
{
    if (count == 0) return;
    if (mode == 0)
    {
        count -= 1;
        currcolor = -currcolor;
    }
    else if (mode > 0) count -= 2;
    memset(indicator, 0, sizeof(indicator));
    chosenX = chosenY = -1;
    update();
    if (count == 0 || (mode > 0 && count <= 1)) emit checkzero();
}

void MainWindow::DisableRetract()
{
    RetractButton->setEnabled(false);
}

void MainWindow::EnableRetract()
{
    RetractButton->setEnabled(true);
}

void MainWindow::DisableSave()
{
    SaveGameButton->setEnabled(false);
}

void MainWindow::EnableSave()
{
    SaveGameButton->setEnabled(true);
}

void MainWindow::UpdateBar()
{
    if (board->currcolor == 1)
        StatusLab->setText("黑方行棋");
    else if (board->currcolor == -1)
        StatusLab->setText("白方行棋");
    BlackLab->setText("黑棋数目:"+QString::number(board->GetNum(1)));
    WhiteLab->setText("白棋数目:"+QString::number(board->GetNum(-1)));
    CountLab->setText("步数:"+QString::number(board->count));
    PlayerLab->setText("黑棋："+board->p1.Name+'\n'+"白棋："+board->p2.Name);
    QString mdl = "游戏模式：\n";
    if (board->mode == 0) mdl += "玩家对战";
    else if (board->mode == 1) mdl += "人机对战(智障AI)";
    else if (board->mode == 2) mdl += "人机对战(简单AI)";
    else if (board->mode == 3) mdl += "人机对战(普通AI)";
    else if (board->mode == 4) mdl += "人机对战(困难AI)";
    ModeLab->setText(mdl);
}

void MainWindow::BlackWinBar()
{
    StatusLab->setText("黑棋获胜！");
    BlackLab->setText("黑棋数目:"+QString::number(board->GetNum(1)));
    WhiteLab->setText("白棋数目:"+QString::number(board->GetNum(-1)));
    CountLab->setText("步数:"+QString::number(board->count));
}

void MainWindow::WhiteWinBar()
{
    StatusLab->setText("白棋获胜！");
    BlackLab->setText("黑棋数目:"+QString::number(board->GetNum(1)));
    WhiteLab->setText("白棋数目:"+QString::number(board->GetNum(-1)));
    CountLab->setText("步数:"+QString::number(board->count));
}

void MainWindow::DeuceBar()
{
    StatusLab->setText("平局！");
    BlackLab->setText("黑棋数目:"+QString::number(board->GetNum(1)));
    WhiteLab->setText("白棋数目:"+QString::number(board->GetNum(-1)));
    CountLab->setText("步数:"+QString::number(board->count));
}

void Board::AIMove()
{
    if (mode <= 0) return;
    if ((currcolor == 1 && p1.type == 0) || (currcolor == -1 && p2.type == 0)) return;
    int sx, sy, ex, ey;
    if (mode > 0 && currcolor == 1 && p1.type)
        currAI.MakeDecision(sx, sy, ex, ey, currcolor, gridInfo[count], p1.type);
    else if (mode > 0 && currcolor == -1 && p2.type)
        currAI.MakeDecision(sx, sy, ex, ey, currcolor, gridInfo[count], p2.type);
    else return;
    CopyBoard();
    count++;
    if (CheckMove(sx, sy, ex, ey) == 2) gridInfo[count][sx][sy] = 0;
    update();
    MoveAnimation(sx, sy, ex, ey, currcolor);
    gridInfo[count][ex][ey] = currcolor;
    update();
    ChangeAnimation(ex, ey, currcolor);
    UpMove(ex, ey, currcolor);
    currcolor = -currcolor;
    update();
    CheckWin();
}

void Board::resizeEvent(QResizeEvent *)
{
    w = this->width(), h = this->height();
}

void Board::paintEvent(QPaintEvent *)
{
    if ((mode == 0 && count) || (mode > 0 && count > 1)) emit checknotzero();
    if (count == 0 || mode == -1) emit checkzero();
    if (mode == -1) emit GameEnd();
    if (mode >= 0) emit ChangeBar();
    if (winner == 1) emit BlackWin();
    if (winner == -1) emit WhiteWin();
    if (winner == 2) emit Deuce();
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    int ltx = (w/2)-200, lty = (h/2)-200;
    if (boardkind) p.drawPixmap(ltx, lty, boardpng);

    if (!boardkind || boardkind == 3)
    {
        for (int i = 0; i < 8; i++)
        {
            p.drawLine(ltx, lty + i * 60, ltx+420, lty + i * 60);
            p.drawLine(ltx + i * 60, lty, ltx + i * 60, lty+420);
        }
    }

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (gridInfo[count][i][j] == 1 && (chosenX != i || chosenY != j))
            {
                if (!chesskind)
                {
                    brush.setColor(Qt::black);
                    p.setPen(Qt::black);
                    p.setBrush(brush);
                    p.drawEllipse(QPoint((i + 1) * 60 + (ltx-30), (j + 1) * 60 + (lty-30)), 20, 20);
                }
                else p.drawPixmap(i*60+ltx+10, j*60+ltx+10, blackpng);
            }
            else if (gridInfo[count][i][j] == -1 && (chosenX != i || chosenY != j))
            {
                if (!chesskind)
                {
                    brush.setColor(Qt::white);
                    p.setPen(Qt::black);
                    p.setBrush(brush);
                    p.drawEllipse(QPoint((i + 1) * 60 + (ltx-30), (j + 1) * 60 + (lty-30)), 20, 20);
                }
                else p.drawPixmap(i*60+ltx+10, j*60+ltx+10, whitepng);
            }
            else if (gridInfo[count][i][j] == 1 && chosenX == i && chosenY == j)
            {
                brush.setColor(Qt::black);
                p.setPen(QPen(Qt::red, 3));
                p.setBrush(brush);
                p.drawEllipse(QPoint((i + 1) * 60 + (ltx-30), (j + 1) * 60 + (lty-30)), 20, 20);
                if (chesskind) p.drawPixmap(i*60+ltx+10, j*60+ltx+10, blackpng);
            }
            else if (gridInfo[count][i][j] == -1 && chosenX == i && chosenY == j)
            {
                brush.setColor(Qt::white);
                p.setPen(QPen(Qt::red, 3));
                p.setBrush(brush);
                p.drawEllipse(QPoint((i + 1) * 60 + (ltx-30), (j + 1) * 60 + (lty-30)), 20, 20);
                if (chesskind) p.drawPixmap(i*60+ltx+10, j*60+ltx+10, whitepng);
            }
            if (indicator[i][j])
            {
                if (indicator[i][j] == 1)
                {
                    p.setPen(QPen(QColor(230, 0, 16)));
                    p.setBrush(QColor(99, 176, 210));
                    p.drawRect(i*60+ltx, j*60+lty, 60, 60);
                }
                else if (indicator[i][j] == 2)
                {
                    p.setPen(QPen(QColor(230, 0, 16)));
                    p.setBrush(QColor(255, 216, 116));
                    p.drawRect(i*60+ltx, j*60+lty, 60, 60);
                }
            }
        }
    }
}

void Board::mouseReleaseEvent(QMouseEvent *e)
{
    int x, y;
    int ltx = (w/2)-200, lty = (h/2)-200;
    if (mode == 0 || (mode > 0 && currcolor == 1 && p1.type == 0) || (mode > 0 && currcolor == -1 && p2.type == 0))
    {
        if(e->x() >= ltx && e->x() < ltx+420 && e->y() >= lty && e->y() < lty+420)
        {
            x = (e->x() - ltx) / 60;
            y = (e->y() - lty) / 60;
            if (gridInfo[count][x][y] == currcolor && chosenX < 0 && chosenY < 0)
            {
                //gridInfo[count][x][y] *= 2;
                chosenX = x;
                chosenY = y;
                for (int i = 0; i < 7; i++)
                    for (int j = 0; j < 7; j++)
                        indicator[i][j] = CheckMove(x, y, i, j);
            }
            else if (!gridInfo[count][x][y] && chosenX >= 0 && chosenY >= 0 && CheckMove(chosenX, chosenY, x, y))
            {
                CopyBoard();
                count++;
                if (CheckMove(chosenX, chosenY, x, y) == 2) gridInfo[count][chosenX][chosenY] = 0;
                memset(indicator, 0, sizeof(indicator));
                currcolor = -currcolor;
                update();
                MoveAnimation(chosenX, chosenY, x, y, -currcolor);
                gridInfo[count][x][y] = -currcolor;
                chosenX = chosenY = -1;
                update();
                ChangeAnimation(x, y, -currcolor);
                UpMove(x, y, -currcolor);
            }
            else if (chosenX >= 0 && chosenY >= 0)
            {
                //gridInfo[count][chosenX][chosenY] /= 2;
                chosenX = chosenY = -1;
                memset(indicator, 0, sizeof(indicator));
            }
        }
        else
        {
            //gridInfo[count][chosenX][chosenY] /= 2;
            chosenX = chosenY = -1;
            memset(indicator, 0, sizeof(indicator));
        }
        update();
        CheckWin();
        AIMove();
    }
    else
        e->ignore();
}

QStringList GetFileList(QString Path)
{
    QStringList fl, ft;
    QDir dir(Path);
    if (!dir.exists()) return fl;
    ft << "*.dat";
    dir.setFilter(QDir::Files);
    dir.setNameFilters(ft);
    dir.setSorting(QDir::Time);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); i++)
        fl << list.at(i).fileName().section('.', 0, 0).trimmed();
    return fl;
}

int GetFileNum(QString Path)
{
    QDir dir(Path);
    if (!dir.exists()) return -1;
    QStringList filter;
    filter << "*.dat";
    dir.setFilter(QDir::Files);
    dir.setNameFilters(filter);
    QFileInfoList list = dir.entryInfoList();
    return list.size();
}

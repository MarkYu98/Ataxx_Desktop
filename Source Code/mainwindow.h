#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QString>
#include <QComboBox>
#include <QDialog>
#include "AI.h"

/*namespace Ui {
class MainWindow;
}*/

class QPushButton;
class QLabel;
class Board;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    QPushButton *QuitButton, *NewGameButton, *SaveGameButton, *LoadGameButton, *RetractButton, *RuleButton;
    QPushButton *BoardButton, *ChessButton;
    QLabel *StatusLab, *BlackLab, *WhiteLab, *CountLab;
    QLabel *PlayerLab, *ModeLab;
    Board *board;
    void closeEvent(QCloseEvent *);

private slots:
    void EnableRetract();
    void DisableRetract();
    void EnableSave();
    void DisableSave();
    void UpdateBar();
    void BlackWinBar();
    void WhiteWinBar();
    void DeuceBar();

/*private:
    Ui::MainWindow *ui;
*/
};

class Player
{
public:
    QString Name;
    int type, pcolor;
};

class Board : public QWidget
{
    Q_OBJECT

public:
    explicit Board(QWidget *parent = 0);
    ~Board();
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    int GetNum(int color);
    int count;
    int currcolor;
    Player p1, p2;
    AI currAI;
    QPixmap boardpng, blackpng, whitepng;
    int chesskind, boardkind;
    int mode;

private:
    int gridInfo[410][7][7];
    int indicator[7][7];
    int CheckMove(int x0, int y0, int x1, int y1);
    int chosenX, chosenY;
    int winner;
    int w, h;
    void UpMove(int x, int y, int color);
    void CopyBoard();
    void CheckWin();
    void AIMove();
    bool Moveable(int color);
    void MoveAnimation(int sx, int sy, int ex, int ey, int color);
    void ChangeAnimation(int x, int y, int color);

signals:
    void checkzero();
    void checknotzero();
    void ChangeBar();
    void BlackWin();
    void WhiteWin();
    void Deuce();
    void GameEnd();
    void GameStart();

public slots:
    void SaveGame();
    void LoadGame();
    void NewGame();
    void Retract();
    void ShowRule();
    void Output(QString Path);
    void Input(QString Path);
    void SetGame(int md, QString name1, QString name2);
    void SetAI(int md, int color);
    void ChangeBoard();
    void ChangeChess();
};

class NewGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGameDialog(QWidget *parent = 0);
    ~NewGameDialog();
    QLabel *NewGamelab;
    QPushButton *CancelButton, *OKButton;
    QLabel *Timelab, *Player1lab, *Player2lab, *Modelab;
    QComboBox *box;
signals:
    void GameSignal(int md, QString name1, QString name2);
    void AISignal(int md, int color);

public slots:
    void CancelNewGame();
    void StartNewGame();
    void SetGameSignal(int md, QString name1, QString name2);
    void SetAISignal(int md, int color);
};

class MvMDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MvMDialog(QWidget *parent = 0);
    ~MvMDialog();
    QLabel *lab1, *lab2;
    QLineEdit *p1, *p2;
    QPushButton *CancelButton, *OKButton;

signals:
    void MvMSignal(int md, QString name1, QString name2);

public slots:
    void CancelMvM();
    void StartMvM();
};

class MvCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MvCDialog(QWidget *parent = 0);
    ~MvCDialog();
    QLabel *lab1, *lab2, *Colorlab;
    QLineEdit *p1, *p2;
    QPushButton *BlackButton, *WhiteButton, *CancelButton, *OKButton;
    int cmode;

signals:
    void MvCSignal(int md, QString name1, QString name2);
    void AIInfo(int md, int color);

public slots:
    void CancelMvC();
    void StartMvC();
    void ChooseBlack();
    void ChooseWhite();
};

#endif // MAINWINDOW_H

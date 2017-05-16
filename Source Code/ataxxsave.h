#ifndef ATAXXSAVE_H
#define ATAXXSAVE_H
#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include <QLineEdit>

QStringList GetFileList(QString Path);
int GetFileNum(QString Path);

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDialog(QWidget *parent = 0);
    ~SaveDialog();
    QListWidget *SaveList;
    QLabel *Savelab;
    QLineEdit *LEdit;
    QPushButton *CancelButton, *OKButton;
    QLabel *Timelab, *Player1lab, *Player2lab, *Modelab;

signals:
    void SaveSignal(QString Path);

public slots:
    void CancelSave();
    void SaveFile();
    void DisplayInfo();
};

class LoadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadDialog(QWidget *parent = 0);
    ~LoadDialog();
    QListWidget *SaveList;
    QLabel *Loadlab;
    QLineEdit *LEdit;
    QPushButton *CancelButton, *OKButton;
    QLabel *Timelab, *Player1lab, *Player2lab, *Modelab;

signals:
    void LoadSignal(QString Path);

public slots:
    void CancelLoad();
    void LoadFile();
    void DisplayInfo();
};


#endif // SAVE_H

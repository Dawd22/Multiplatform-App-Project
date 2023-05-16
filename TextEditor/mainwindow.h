#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QAction>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_actionClose_triggered();

    void on_actionSave_triggered();

    void changeFontSize(int index);

    void changeTextColor(int index);

    void changeAlignment(int index);

    void changeFont(int index);

    void toggleBold();

    void toggleItalic();

    void toggleUnderline();

private:
    Ui::MainWindow *ui;

    QTextEdit *textEdit;

    QComboBox *fontSizeComboBox;

    QVBoxLayout *layout;

    QComboBox *textColorComboBox;

    QComboBox *alignmentComboBox;

    QComboBox *fontComboBox;

    QComboBox *boldComboBox;

    QComboBox *italicComboBox;

    QComboBox *underlineComboBox;

};


#endif // MAINWINDOW_H
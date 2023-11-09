#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <clientfunctions.h>
#include <QApplication>
#include <QProcess>
#include <taskform.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    // Обновление данных
    bool update_data();

signals:
    // Сигнал открытия окна
    void openWindow();

private slots:
    // Обработчики кнопок задач
    void on_task1_clicked();
    void on_task2_clicked();
    void on_task3_clicked();

    // Обработчик выхода из аккаунта
    void on_account_exit_triggered();

    // Открытие окна с задачей
    void openTask(QVariantMap task);

    // Обработчик нажатия на ссылку со статистикой
    void on_StatLabel_linkActivated(const QString &link);

private:
    Ui::MainWindow *ui;
    TaskForm *tWindow;
};

#endif // MAINWINDOW_H

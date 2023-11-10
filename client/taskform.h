#ifndef TASKFORM_H
#define TASKFORM_H

#include <QWidget>
#include <clientfunctions.h>

namespace Ui {
class TaskForm;
}

class TaskForm : public QWidget
{
    Q_OBJECT

public:
    explicit TaskForm(QWidget *parent = nullptr);
    ~TaskForm();

    // Инициализация задачи
    void initTask(QString taskText, int taskId);

private slots:
    // Обработчик кнопки "Попробовать"
    void on_TryButton_clicked();

    // Обработчик кнопки "Сдаться"
    void on_LoseButton_clicked();

signals:
    // Сигнал открытия главного окна
    void openWindow();

private:
    Ui::TaskForm *ui;
    int taskId;
};

#endif // TASKFORM_H

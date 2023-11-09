#include "taskform.h"
#include "ui_taskform.h"

TaskForm::TaskForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskForm)
{
    // Инициализация формы задачи
    ui->setupUi(this);
    ui->taskText->setWordWrap(true);
}

void TaskForm::initTask(QString taskText, int taskId) {
    // Инициализация текста задачи и идентификатора задачи
    this->taskId = taskId;
    this->ui->taskText->setText(taskText);
}

TaskForm::~TaskForm()
{
    // Освобождение ресурсов при закрытии формы задачи
    delete ui;
}

void TaskForm::on_TryButton_clicked()
{
    // Обработка кнопки "Попробовать"
    QString userAnswer = this->ui->AnswerLineEdit->text();
    bool status = checkTask(this->taskId, userAnswer);

    // Отображение статуса ответа пользователя
    if (status)
        this->ui->statusLabel->setText("Правильный ответ");
    else
        this->ui->statusLabel->setText("Неправильный ответ");
}

void TaskForm::on_LoseButton_clicked()
{
    // Обработка кнопки "Закрыть"
    this->close();
}

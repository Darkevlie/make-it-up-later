#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Инициализация главного окна
    ui->setupUi(this);

    // Обновление информации в главном окне
    this->update_data();

    // Инициализация окна с задачей
    tWindow = new TaskForm();
    connect(tWindow, &TaskForm::openWindow, this, &TaskForm::show);
}

bool MainWindow::update_data() {
    // Обновление информации в главном окне
    ui->StatusLabel->setText(getStat());
}

MainWindow::~MainWindow()
{
    // Освобождение ресурсов
    delete ui;
}

void MainWindow::openTask(QVariantMap task) {
    // Открытие окна с задачей
    int taskId = task["id"].toInt();
    QString taskText = task["text"].toString();
    tWindow->initTask(taskText, taskId);
    tWindow->show();
}

void MainWindow::on_task1_clicked()
{
    // Получение и открытие задачи 1
    QVariantMap task = getTask(1);
    openTask(task);
}

void MainWindow::on_task2_clicked()
{
    // Получение и открытие задачи 2
    QVariantMap task = getTask(2);
    openTask(task);
}

void MainWindow::on_task3_clicked()
{
    // Получение и открытие задачи 3
    QVariantMap task = getTask(3);
    openTask(task);
}

void MainWindow::on_account_exit_triggered() {
    // Выход из аккаунта и перезапуск приложения
    reset_settings();
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void MainWindow::on_StatLabel_linkActivated(const QString &link)
{
    // Обработка события клика по ссылке в статусе
}

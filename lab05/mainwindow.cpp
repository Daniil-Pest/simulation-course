#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Начальные значения для метода Фибоначчи
unsigned int F_prev = 17; // Зерно
unsigned int F_curr = 5; // Зерно

void MainWindow::on_pushButton_clicked()
{
    // Генерация следующего числа Фибоначчи с модулем
    const unsigned int m = 2147483647; // 2^31 - 1; большое простое число
    unsigned int F_next = (F_prev + F_curr) % m;

    if (F_next % 2 == 0)
    {
        ui->label_2->setText("Да");
    }
    else
    {
        ui->label_2->setText("Нет");
    }

    F_prev = F_curr;
    F_curr = F_next;
}

unsigned long long seed = 1; // Начальное значение МКГ (зерно)

void MainWindow::on_pushButton_2_clicked()
{
    if (ui->textEdit->toPlainText()!="")
    {
        // Параметры МКГ
        const unsigned long long a = 16807; // Классический множитель для МКГ
        const unsigned long long m = 2147483647; // 2^31 - 1; большое простое число

        // Генерация следующего числа с помощью МКГ
        seed = (a * seed) % m;

        std::vector<std::string> answers = { "Безусловно, да", "Да", "Вероятно, да", "Возможно", "Лучше не рассказывать", "Спроси позже", "Вряд ли", "Нет", "Определённо нет"};
        ui->label->setText("Шар предсказаний говорит: «" + QString::fromStdString(answers[seed % answers.size()]) + "».");
    }
    else
    {
        QMessageBox::critical(nullptr,"Ошибка","Сначала введите вопрос.");
    }
}

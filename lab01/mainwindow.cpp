#include "mainwindow.h"

#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Создание «диаграммы»
    chart = new QChart();
    chart->setTitle("Моделирование полёта тела с учётом сопротивления воздуха");
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::NoAnimation);

    // Настройка осей
    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("Расстояние (м)");

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Высота (м)");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    // Создание виджета для отображения графика
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Создание элементов управления
    spinBoxV0 = new QDoubleSpinBox;
    spinBoxV0->setRange(0.1, 1000.0);
    spinBoxV0->setValue(10.0);
    spinBoxV0->setSingleStep(10.0);

    spinBoxAngle = new QDoubleSpinBox;
    spinBoxAngle->setRange(0.0, 90.0);
    spinBoxAngle->setValue(45.0);
    spinBoxAngle->setSingleStep(5.0);

    spinBoxH0 = new QDoubleSpinBox;
    spinBoxH0->setRange(0.0, 1000.0);
    spinBoxH0->setValue(5.0);
    spinBoxH0->setSingleStep(10.0);

    spinBoxRadius = new QDoubleSpinBox;
    spinBoxRadius->setRange(0.01, 1.0);
    spinBoxRadius->setValue(0.1);
    spinBoxRadius->setSingleStep(0.01);

    spinBoxMass = new QDoubleSpinBox;
    spinBoxMass->setRange(0.001, 100.0);
    spinBoxMass->setValue(1.0);
    spinBoxMass->setSingleStep(0.1);
    spinBoxMass->setDecimals(3);

    spinBoxDelta = new QDoubleSpinBox;
    spinBoxDelta->setRange(0.0001, 1.0);
    spinBoxDelta->setValue(0.0001);
    spinBoxDelta->setSingleStep(0.0001);
    spinBoxDelta->setDecimals(4);

    addTrajectoryButton = new QPushButton("Смоделировать полёт");

    // Метка для вывода результатов
    labelFinal = new QLabel("");
    labelFinal->setStyleSheet("font-weight: bold; color: red;");

    // Создание layout для формы
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Начальная скорость (м/с)"));
    layout->addWidget(spinBoxV0);
    layout->addWidget(new QLabel("Угол (°)"));
    layout->addWidget(spinBoxAngle);
    layout->addWidget(new QLabel("Начальная высота (м)"));
    layout->addWidget(spinBoxH0);
    layout->addWidget(new QLabel("Радиус тела (м)"));
    layout->addWidget(spinBoxRadius);
    layout->addWidget(new QLabel("Масса тела (кг)"));
    layout->addWidget(spinBoxMass);
    layout->addWidget(new QLabel("Шаг моделирования (с)"));
    layout->addWidget(spinBoxDelta);
    layout->addWidget(addTrajectoryButton);
    layout->addWidget(chartView);
    layout->addWidget(labelFinal);

    // Создание центрального виджета и установка layout
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Подключение сигнала кнопки к слоту добавления траектории
    connect(addTrajectoryButton, &QPushButton::clicked, this, &MainWindow::addTrajectory);
}

void MainWindow::addTrajectory()
{
    double v0 = spinBoxV0->value();         // Начальная скорость (м/с)
    double angle = spinBoxAngle->value();   // Угол (°)
    double y0 = spinBoxH0->value();         // Начальная высота (м)
    double radius = spinBoxRadius->value(); // Радиус тела (м)
    double mass = spinBoxMass->value();     // Масса тела (кг)
    double dt = spinBoxDelta->value();      // Шаг моделирования (с)

    // Преобразование в радианы
    double angleRad = angle * M_PI / 180.0;

    // Начальные компоненты скорости
    double vx = v0 * cos(angleRad);
    double vy = v0 * sin(angleRad);

    // Константы для сопротивления воздуха
    const double rho = 1.225;             // Плотность воздуха (кг/м³)
    const double Cd = 0.47;               // Коэффициент лобового сопротивления для сферы
    double area = M_PI * radius * radius; // Площадь поперечного сечения

    const double g = 9.81; // Ускорение свободного падения (м/с²)

    // Создание новой серии данных для траектории
    QSplineSeries *series = new QSplineSeries;
    series->setName(QString("v0=%1, angle=%2°, r=%3, m=%4").arg(v0).arg(angle).arg(radius).arg(mass));

    // Начальные координаты
    double x = 0.0;
    double y = y0;

    double y_max = 0;
    double v = 0;
    double t = 0;

    // Добавление начальной точки
    series->append(x, y);

    while (y > 0.0)
    {
        v = sqrt(vx * vx + vy * vy);

        // Сопротивление воздуха
        double F_drag = 0.5 * rho * Cd * area * v * v;
        double a_drag_x = (F_drag / mass) * (-vx / v);
        double a_drag_y = (F_drag / mass) * (-vy / v);

        // Обновление компонент скорости с учётом сопротивления и гравитации
        vx += a_drag_x * dt;
        vy += (a_drag_y - g) * dt;

        // Обновление координат
        x += vx * dt;
        y += vy * dt;
        if (y > y_max)
            y_max = y;

        series->append(x, y);

        t += dt;
    }

    // Добавление серии на график
    chart->addSeries(series);
    series->attachAxis(chart->axes(Qt::Horizontal).first());
    series->attachAxis(chart->axes(Qt::Vertical).first());

    // Сохранение серии в список
    seriesList.append(series);

    // Автоматическое обновление диапазона осей
    double minX = 0, maxX = 0, minY = 0, maxY = 0;
    bool firstPoint = true; // В первый раз применяется всегда

    for (QSplineSeries *s : seriesList)
    {
        for (const QPointF &point : s->points())
        {
            if (firstPoint || point.x() < minX) minX = point.x();
            if (firstPoint || point.x() > maxX) maxX = point.x();
            if (firstPoint || point.y() < minY) minY = point.y();
            if (firstPoint || point.y() > maxY) maxY = point.y();
            firstPoint = false;
        }
    }

    // Устанавка диапазона осей с небольшим запасом
    double margin = 0.1;
    chart->axes(Qt::Horizontal).first()->setRange(minX - margin * (maxX - minX), maxX + margin * (maxX - minX));
    chart->axes(Qt::Vertical).first()->setRange(minY - margin * (maxY - minY), maxY + margin * (maxY - minY));

    // Обновление метки (вывод результатов)
    labelFinal->setText(QString("Дальность полёта — %1 м; максимальная высота — %2 м; скорость в конечной точке: %3 м/с.").arg(x, 0, 'f', 3).arg(y_max, 0, 'f', 3).arg(v, 0, 'f', 3));
}

MainWindow::~MainWindow()
{
    // Очистка списка из серий
    for (QSplineSeries *series : seriesList)
    {
        delete series;
    }
}

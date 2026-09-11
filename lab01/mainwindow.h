#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QChartView>
#include <QChart>
#include <QValueAxis>
#include <QSplineSeries>
#include <QVector>

    class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addTrajectory();

private:
    QChart *chart;
    QChartView *chartView;
    QDoubleSpinBox *spinBoxV0;          // Начальная скорость (м/с)
    QDoubleSpinBox *spinBoxAngle;       // Угол (°)
    QDoubleSpinBox *spinBoxH0;          // Начальная высота (м)
    QDoubleSpinBox *spinBoxRadius;      // Радиус тела (м)
    QDoubleSpinBox *spinBoxMass;        // Масса тела (кг)
    QDoubleSpinBox *spinBoxDelta;       // Шаг моделирования (с)
    QPushButton *addTrajectoryButton;
    QVector<QSplineSeries*> seriesList; // Список траекторий
    QLabel *labelFinal;                 // Метка для вывода результатов
};

#endif // MAINWINDOW_H

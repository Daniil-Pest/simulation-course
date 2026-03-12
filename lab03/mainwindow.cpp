#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // для sleep()

#include <QTableWidget>
#include <QApplication>
#include <QVector>

using namespace std;

const int N = 30;
const int M = 15;

const int EMPTY = 0;
const int TREE = 1;
const int FIRE = 2;
const int ASH = 3;

int tree_count = 1;

double humidity = 0.5; // уровень влажности (0 — сухо, 1 — влажно)
int wind_direction = 0; // направление ветра: 0 — нет ветра, 1 — вправо, 2 — влево, 3 — вверх, 4 — вниз

vector<vector<int>> grid(N, vector<int>(M));

void initialize()
{
    srand(time(0));

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            grid[i][j] = rand() % 2 == 0 ? EMPTY : TREE;
            if (grid[i][j] == TREE)
                tree_count++;
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        int x = rand() % N;
        int y = rand() % M;
        grid[x][y] = FIRE;
    }
}

void upgrade()
{
    srand(time(0));

    vector<vector<int>> new_grid = grid;

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            if (grid[i][j] == TREE)
            {
                bool fire_around = false;
                for (int di = -1; di <= 1; ++di)
                {
                    for (int dj = -1; dj <= 1; ++dj)
                    {
                        if (di == 0 && dj == 0) continue;
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni >= 0 && ni < N && nj >= 0 && nj < M && grid[ni][nj] == FIRE)
                        {
                            fire_around = true;
                            //break;
                        }
                    }
                }

                // вероятность возгорания от соседей (с учётом влажности)
                double prob = 0.5 * (1 - humidity);

                // ветер
                if (wind_direction == 1 && j < M - 1 && grid[i][j+1] == FIRE)
                {
                    prob *= 1.5;
                } else if (wind_direction == 2 && j > 0 && grid[i][j-1] == FIRE)
                {
                    prob *= 1.5;
                } else if (wind_direction == 3 && i > 0 && grid[i-1][j] == FIRE)
                {
                    prob *= 1.5;
                } else if (wind_direction == 4 && i < N - 1 && grid[i+1][j] == FIRE)
                {
                    prob *= 1.5;
                }

                if (fire_around && (rand() % 100) / 100.0 < prob)
                {
                    new_grid[i][j] = FIRE;
                } else if ((rand() % 100) / 100.0 < 0.01)
                {
                    new_grid[i][j] = FIRE;
                }
            }
            else if (grid[i][j] == FIRE)
            {
                if ((rand() % 100) / 100.0 < 0.1)
                {
                    new_grid[i][j] = ASH;
                }
            }
            else if (grid[i][j] == ASH)
            {
                if ((rand() % 100) / 100.0 < 0.5)
                {
                    new_grid[i][j] = EMPTY;
                    tree_count--;
                }
            }
        }
    }

    grid = new_grid;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initialize();

    ui->tableWidget->setRowCount(N);
    ui->tableWidget->setColumnCount(M);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    while(tree_count > 0)
    {
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < M; ++j)
            {
                QTableWidgetItem *item = new QTableWidgetItem(QString::number(grid[i][j]));
                switch(grid[i][j])
                {
                    case EMPTY: item->setBackground(QColor(0, 0, 0)); break;
                    case TREE: item->setBackground(QColor(0, 255, 0)); break;
                    case FIRE: item->setBackground(QColor(255, 0, 0)); break;
                    case ASH: item->setBackground(QColor(255, 0, 255)); break;
                }
                ui->tableWidget->setItem(i,j,item);
                QApplication::processEvents();
            }
        }

        sleep(1);

        upgrade();
    }
}

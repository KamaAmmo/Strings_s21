#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

#ifdef __cplusplus
extern "C" {
#endif
#include "../calc_functions/s21_smart_calc.h"
#ifdef __cplusplus
}
#endif

#define graph_x 980
#define graph_y 390
#define graph_size 600
#define pushButton_index 11
#define PRECISION 2

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 protected:
  void paintEvent(QPaintEvent *event);

 private slots:
  void on_pushButton_clicked();

  void pushBoxKeys();

  void on_textEdit_textChanged();

  void pushButtonKeys();

  void on_action_triggered();

 private:
  void free_coord_table();

  Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H

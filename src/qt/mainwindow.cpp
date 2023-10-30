#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)

    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  MainWindow::on_action_triggered();
  free_coord_table();
  setlocale(LC_NUMERIC, "C");
  QList<QSpinBox *> allSBox = findChildren<QSpinBox *>();
  for (int i = 0; i < allSBox.size(); i++) {
    QSpinBox *cur = allSBox.at(i);
    QObject::connect(cur, SIGNAL(valueChanged(int   )), this, SLOT(pushBoxKeys()));
  }

  QList<QPushButton *> allPButtons = findChildren<QPushButton *>();
  for (int i = 0; i < allPButtons.size(); i++) {
    QPushButton *cur = allPButtons.at(i);
    while (cur == ui->pushButton) {
      allPButtons.removeAt(i);
      if (i == allPButtons.size()) break;
      cur = allPButtons.at(i);
    }
    if (i == allPButtons.size()) break;
    QObject::connect(cur, SIGNAL(clicked()), this, SLOT(pushButtonKeys()));
  }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::free_coord_table() {
  for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
    ui->tableWidget->setItem(i, 0, new QTableWidgetItem(0));
    ui->tableWidget->setItem(i, 1, new QTableWidgetItem(0));
  }
}

void MainWindow::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);
  if (ui->groupBox->isVisible()) {
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
    painter.drawLine(graph_x, graph_y - graph_size / 2, graph_x,
                     graph_y + graph_size / 2);
    painter.drawLine(graph_x, graph_y - graph_size / 2,
                     graph_x - graph_size / 20,
                     graph_y - graph_size / 2 + graph_size / 20);
    painter.drawLine(graph_x, graph_y - graph_size / 2,
                     graph_x + graph_size / 20,
                     graph_y - graph_size / 2 + graph_size / 20);
    painter.drawLine(graph_x - graph_size / 2, graph_y,
                     graph_x + graph_size / 2, graph_y);
    painter.drawLine(graph_x + graph_size / 2 - graph_size / 20,
                     graph_y - graph_size / 20, graph_x + graph_size / 2,
                     graph_y);
    painter.drawLine(graph_x + graph_size / 2 - graph_size / 20,
                     graph_y + graph_size / 20, graph_x + graph_size / 2,
                     graph_y);
    double size_x = 1. * ui->spinBox->value() / (graph_size / 2);
    double size_y = 1. * ui->spinBox_2->value() / (graph_size / 2);
    painter.setPen(QPen(Qt::red, 2, Qt::DotLine, Qt::FlatCap));
    for (int i = 0; i < STEPS * PRECISION; i++) {
      double x1 =
          ui->tableWidget->item(i, 0)->text().toDouble() / size_x + graph_x;
      double y1 =
          -ui->tableWidget->item(i, 1)->text().toDouble() / size_y + graph_y;
      double x2 =
          ui->tableWidget->item(i + 1, 0)->text().toDouble() / size_x + graph_x;
      double y2 = -ui->tableWidget->item(i + 1, 1)->text().toDouble() / size_y +
                  graph_y;
      if (y1 == y1 && y2 == y2 && (y2 / y1) > 0)
        painter.drawLine(x1, y1, x2, y2);
    }
  }
}

void MainWindow::on_pushButton_clicked() {
  free_coord_table();
  if (ui->textEdit->toPlainText().size() != 0) {
    if (ui->textEdit->toPlainText().size() < STR_SIZE) {
      QByteArray ba = ui->textEdit->toPlainText().toUtf8();
      char *input = ba.data();
      bool is_correct_input = s21_isCorrectInput(input);
      if (is_correct_input) {
        if (strchr(input, 'x')) {
          ui->tableWidget->setRowCount(STEPS * PRECISION + 1);
          double point_x = -ui->spinBox->value();
          double *x_ptr = &point_x;
          for (int i = 0; i <= STEPS * PRECISION; i++) {
            double point_y = s21_smart_calc(input, x_ptr);
            if (!std::isnan(point_y)) {
              ui->tableWidget->setItem(
                  i, 0, new QTableWidgetItem(QString::number(point_x)));
              ui->tableWidget->setItem(
                  i, 1, new QTableWidgetItem(QString::number(point_y)));
              point_x += 1. * ui->spinBox->value() / (STEPS * PRECISION / 2);
            }
          }
          repaint();
        } else{
            ui->textEdit->setPlainText(QString::number(s21_smart_calc(input, nullptr)));
        }
      } else
        ui->textEdit->setText(ui->textEdit->toPlainText() +
                              "\nError: Incorrect data");
    } else
      ui->textEdit->setText(ui->textEdit->toPlainText() +
                            "\nError: string is too big (> 255 chars)");
  } else
    ui->textEdit->setText("\nError: Empty string");
}

void MainWindow::pushBoxKeys() {
  if (fabs(ui->tableWidget->item(0, 0)->text().toDouble()) >= EPS)
    on_pushButton_clicked();
}

void MainWindow::on_textEdit_textChanged() {
  free_coord_table();
  repaint();
}

void MainWindow::pushButtonKeys() {
  switch (
      ((char *)sender()->objectName().toLocal8Bit().data())[pushButton_index]) {
    case 'q':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "+");
      break;
    case 'w':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "-");
      break;
    case 'e':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "*");
      break;
    case 'r':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "/");
      break;
    case 't':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "^");
      break;
    case 'y':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "mod");
      break;
    case 'u':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "(");
      break;
    case 'i':
      ui->textEdit->setText(ui->textEdit->toPlainText() + ")");
      break;
    case 'a':
      ui->textEdit->setText(ui->textEdit->toPlainText() + ".");
      break;
    case 's':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "acos(");
      break;
    case 'f':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "sin(");
      break;
    case 'g':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "cos(");
      break;
    case 'h':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "tan(");
      break;
    case 'j':
      ui->textEdit->setText("");
      break;
    case 'z':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "asin(");
      break;
    case 'c':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "atan(");
      break;
    case 'v':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "sqrt(");
      break;
    case 'b':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "ln(");
      break;
    case 'n':
      ui->textEdit->setText(ui->textEdit->toPlainText() + "log(");
      break;
    case 'm':
      ui->textEdit->setText(ui->textEdit->toPlainText().remove(
          ui->textEdit->toPlainText().size() - 1, 1));
      break;
    case 'p':
      MainWindow::on_action_triggered();
      break;
    default:
      ui->textEdit->setText(ui->textEdit->toPlainText() +
                            sender()->objectName()[11]);
  }
}

void MainWindow::on_action_triggered() { ui->groupBox->setVisible(true); }

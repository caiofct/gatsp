#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "graphicsScene.h"
#include "genetico.h"
#include "individuo.h"
#include "changePreferences.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
      void closeEvent(QCloseEvent *event);

private slots:
    void sobre();
    void ajuda();
    void carregarImagemFundo();
    void criarCidade(QPointF &pos);
    void removerCidade(int &chave);
    void habilitarIniciar();
    void executar();
    void abortar();
    void limparCanvas();
    void avancarGeracao();
    void mudarPreferencias();
    void changeTime(int time);

private:
    Ui::MainWindow *ui;
    ChangePreferences *preferences;

    GraphicsScene *gscene;
    Genetico *genetico;
    void lockInterface();
    void writeSettings();
    void readSettings();

    bool abort;
    bool running;
    int generation; //geração atual
    QTimer *timer;
};

#endif // MAINWINDOW_H

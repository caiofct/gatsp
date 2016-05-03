#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_configDialog.h"
#include "cidade.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    ui->setupUi(this);
    ui->iniciarButton->setDisabled(true);
    ui->pararButton->setDisabled(true);

    this->generation = 0;

    connect(ui->sobreAction, SIGNAL(triggered()), this, SLOT(sobre()));
    connect(ui->helpAction, SIGNAL(triggered()), this, SLOT(ajuda()));
    connect(ui->backgroundImageAction, SIGNAL(triggered()), this, SLOT(carregarImagemFundo()));

    //configurando a cena
    gscene = new GraphicsScene();
    gscene->setCidadeWidth(10.0);
    gscene->setCidadeHeigth(10.0);

    //limpar canvas
    connect(ui->limparTelaAction, SIGNAL(triggered()), this, SLOT(limparCanvas()));

    //preferências
    connect(ui->preferenciasAction, SIGNAL(triggered()), this, SLOT(mudarPreferencias()));

    //validação dos inputs
    QRegExp regExp("[0-9]{1,}");

    ui->qtdIndividuosLine->setValidator(new QRegExpValidator(regExp, this));
    ui->qtdGeracoesLine->setValidator(new QRegExpValidator(regExp, this));

    connect(ui->qtdIndividuosLine, SIGNAL(textChanged(QString)), this, SLOT(habilitarIniciar()));
    connect(ui->qtdGeracoesLine, SIGNAL(textChanged(QString)), this, SLOT(habilitarIniciar()));

    connect(gscene, SIGNAL(cidadeCriada(QPointF &)), this, SLOT(criarCidade(QPointF &)));
    connect(gscene, SIGNAL(cidadeRemovida(int &)), this, SLOT(removerCidade(int &)));
    connect(ui->iniciarButton, SIGNAL(clicked()), this, SLOT(executar()));
    connect(ui->pararButton, SIGNAL(clicked()), this, SLOT(abortar()));

    //atribuindo cena ao canvas
    gscene->setSceneRect(QRectF(0.0,0.0, (float)ui->canvas->width(), (float)ui->canvas->height()));
    ui->canvas->setScene(gscene);
    ui->canvas->show();

    //criando instância do algoritmo genético
    genetico = new Genetico();

    Cidade::setLastId(0);

    abort = false;
    running = false;

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(avancarGeracao()));

    readSettings(); //carregando configurações
}

MainWindow::~MainWindow()
{
    abort = true;

    delete ui;
}

/**
 * Ao fechar o programa
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
       writeSettings(); //gravando configurações

       event->accept();
}

/**
 * Mudar preferências
 */
void MainWindow::mudarPreferencias()
{
    preferences = new ChangePreferences(this->timer->interval() / 1000, this);
    connect(preferences, SIGNAL(changeTime(int)), this, SLOT(changeTime(int)));

    preferences->exec();
}

/**
 * Alterar valor do timer, toda vez que o valor do campo de preferências mudar
 */
void MainWindow::changeTime(int time)
{
    this->timer->setInterval(time * 1000);
}

/**
 * Sobre o programa
 */
void MainWindow::sobre()
{
    QMessageBox::about(this, tr("Sobre GATSP"),
            tr("<h2>GATSP 0.2</h2>"
               "<p>Copyright &copy; 2009 Adyson Maia e Caio Fellipe."
               "<p>Genetic Algorithm for Traveling Salesman Problem(GATSP) "
               "tenta achar uma boa solu&ccedil;&atilde;o para o problema do Caxeiro Viajante "
               "usando algoritmos gen&eacute;ticos."
              ));
}

/**
 * Ajuda no programa
 */
void MainWindow::ajuda()
{
    QMessageBox::about(this, tr("Ajuda GATSP"),
            tr("<p>Adicione cidades com o click do botão esquerdo do mouse</p>"
               "<p>Remova cidades com o click do botão direito do mouse sobre a cidade</p>"
               "<p>Insira a quantidade de indivíduos e de gerações para iniciar o algoritmo</p>"
              ));
}

/**
 * Carregar imagem de fundo
 */
void MainWindow::carregarImagemFundo()
{
   QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Carregar Imagem"), QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first(),
                                   tr("Imagens (*.png *.jpg *.jpeg *.gif"));
   if(!fileName.isEmpty())
   {
        QPixmap img;
        img.load(fileName);
        img.scaled(qRound(gscene->sceneRect().width()), qRound(gscene->sceneRect().height()));

        QGraphicsPixmapItem *pixmap = gscene->addPixmap(img);
        pixmap->setZValue(0.0);
        pixmap->setData(1, 2);
   }
}

/**
 * Limpar canvas
 */
void MainWindow::limparCanvas()
{
    //limpando as cidades que estavam nos genes
    genetico->clearGenes();

    //limpando o canvas
    gscene->clearCanvas();

    //limpando o histórico
    ui->historyList->clear();

    //testando se o botão iniciar deve ser habilitado
    this->habilitarIniciar();

}

/**
 * Método chamado quando uma nova cidade é criada na tela
 */
void MainWindow::criarCidade(QPointF &pos)
{
    Cidade *cidade = new Cidade((float)pos.x(), (float)pos.y()); //criando cidade
    Cidade::setLastId(Cidade::getLastId() + 1); //incrementando último id das cidades

    genetico->addGene(cidade); //adicionando cidade ao algoritmo

    this->habilitarIniciar();
}

/**
 * Método chamado quando cidade é removida da interface
 */
void MainWindow::removerCidade(int &chave)
{
    Cidade *cidade = new Cidade(chave);
    genetico->removeGene(cidade);

    this->habilitarIniciar();
}

/**
 * Habilita ou não o botão iniciar
 */
void MainWindow::habilitarIniciar()
{
    ui->iniciarButton->setEnabled(genetico->getGenes().size() > 2 && ui->qtdIndividuosLine->text().toInt() > 0 && ui->qtdGeracoesLine->text().toInt() > 0);
}

/**
 * Método chamado quando o botão parar for clicado
 */
void MainWindow::abortar()
{
    abort = true;
}

/**
 * Bloqueia certas funções da interface quando o algoritmo está rodando
 */
void MainWindow::lockInterface()
{
    gscene->setRunning(running);

    ui->limparTelaAction->setEnabled(!running);
    ui->backgroundImageAction->setEnabled(!running);
    ui->preferenciasAction->setEnabled(!running);
    ui->iniciarButton->setEnabled(!running);
    ui->qtdGeracoesLine->setEnabled(!running);
    ui->qtdIndividuosLine->setEnabled(!running);
    ui->taxaMutacaoLine->setEnabled(!running);

    ui->pararButton->setEnabled(running);
}

/**
 * Método chamado quando o botão iniciar for clicado
 */
void MainWindow::executar()
{
    abort = false;
    running = true;
    lockInterface();
    this->ui->historyList->clear();
    this->generation = 0;

    //recebendo valores dos campos de entrada
    int qtdGeracoes = ui->qtdGeracoesLine->text().toInt();
    int qtdIndividuos = ui->qtdIndividuosLine->text().toInt();
    float taxaMutacao = (float)ui->taxaMutacaoLine->value();

    //configurando os parâmetros do algoritmo genético
    genetico->setGeracao(qtdGeracoes);
    genetico->setPopulacaoInicial(qtdIndividuos);
    genetico->setTaxaMutacao(taxaMutacao);

    genetico->reiniciar();

    this->timer->start();
}

/**
 * Avança para a próxima geração baseado no intervalo de tempo escolhido
 */
void MainWindow::avancarGeracao()
{
    qApp->processEvents();
    QScrollBar *bar = this->ui->historyList->verticalScrollBar();
    bar->setValue(bar->maximum());

    if(abort)
    {
        this->timer->stop();
        abort = false;
        running = false;
        lockInterface();

        return;
    }

    if(this->generation < genetico->getGeracaoMaxima())
    {
        genetico->proximaGeracao(); //obtendo próxima geração

        //desenhando o caminho entre as cidades
        Individuo *individuo = genetico->melhorIndividuo();
        QList<Cidade *> solucao = individuo->getCromossomo();

        this->ui->historyList->addItem(tr("Geracao: %1 \n  Custo: %2").arg(this->generation + 1).arg(individuo->custo()));

        gscene->drawPath(individuo);

        this->generation++;
    }
    else
    {
        this->timer->stop();
        abort = false;
        running = false;
        lockInterface();
    }
}

/**
 * Salvando configurações
 */
void MainWindow::writeSettings()
{
    QSettings settings("Gatsp Settings", "Gatsp");
    settings.setValue("geometry", geometry());
    settings.setValue("timer", this->timer->interval());
}

/**
 * Carregando configurações
 */
void MainWindow::readSettings()
{
    QSettings settings("Gatsp Settings", "Gatsp");

    QRect rect = settings.value("geometry", QRect(200, 200, 777, 529)).toRect();
    move(rect.topLeft());
    resize(rect.size());

    //restaurando timer
    this->timer->setInterval(settings.value("timer", 1000).toInt());
}


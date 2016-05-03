#include "graphicsScene.h"
#include "cidade.h"
#include <QtDebug>

GraphicsScene::GraphicsScene(QWidget *parent)
   : QGraphicsScene(parent)
{
    lastPos = QPointF(-10.0,-10.0);

    running = false;
}

void GraphicsScene::setCidadeWidth(qreal w)
{
    this->cidadeWidth = w;
}

qreal GraphicsScene::getCidadeWidth()
{
    return this->cidadeWidth;
}

void GraphicsScene::setCidadeHeigth(qreal h)
{
    this->cidadeHeigth = h;
}

qreal GraphicsScene::getCidadeHeigth()
{
    return this->cidadeHeigth;
}

/**
 * Evento chamado quando o mouse é clicado
 */
void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  if(!running)
  {
    QPointF pos = mouseEvent->scenePos(); //posição do mouse na hora do clique
    //qDebug() << "x: " << pos.x() << " y: " << pos.y();
    bool colidiu = false;
    QList<QGraphicsItem *> cidades = this->items(); //lista de cidades na tela

    if(mouseEvent->button() == Qt::LeftButton) //adicionar uma cidade
    {
        QRectF bboxCidade(pos.x(),pos.y(), cidadeWidth, cidadeHeigth); //bouding box da elipse da cidade

        //gradiente de cores de cada cidade
        QRadialGradient gradient(10, 10, 10, 10, 10);
        gradient.setColorAt(0, QColor::fromRgbF(0.5, 0.8, 0.7, 1));
        gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
        gradient.setSpread(QGradient::ReflectSpread);
        QBrush brush(gradient);

        QGraphicsEllipseItem *cidade = new QGraphicsEllipseItem(bboxCidade);
        cidade->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        cidade->setBrush(brush);
        cidade->setZValue(1.0);

        for(int i = 0; i < cidades.size(); i++) //percorrendo a lista de cidades
        {
            if(cidades.at(i)->collidesWithItem(cidade) && cidades.at(i)->data(1).toBool())
            {
                colidiu = true;
                break;
            }
        }

        if(!colidiu) //se não houve colisão
        {
           cidade->setData(0, Cidade::getLastId() + 1);
           cidade->setData(1, 1);
           cidade->setData(4, pos);
           this->addItem(cidade);

           if(lastPos.x() != -10.0 && lastPos.y() != -10.0)
           {
               QPoint cids(Cidade::getLastId(),Cidade::getLastId() + 1);
               //this->drawEdge(lastPos, pos, cids);
           }

           lastPos = pos;
           emit cidadeCriada(pos); //sinal emitido quando uma cidade é criada
        }
    }
    else if(mouseEvent->button() == Qt::RightButton) //remover uma cidade
    {
        for(int i = 0; i < cidades.size(); i++) //percorrendo a lista de cidades
        {
            if(cidades.at(i)->contains(pos) && cidades.at(i)->data(1).toInt() == 1) //o mouse foi clicado em cima de uma cidade
            {
                int chave = cidades.at(i)->data(0).toInt();
                this->removeItem(cidades.at(i));
                //this->removeEdge(chave);

                if(items().size() == 0) lastPos = QPointF(-10,-10);
                emit cidadeRemovida(chave); //sinal emitido quando uma cidade removida
            }
        }
    }
  }
}

void GraphicsScene::removeEdge(int cidadeId)
{
    QList<QGraphicsItem *> itens = this->items();
    for(int i = 0; i < itens.size(); i++)
    {
        if(itens.at(i)->data(1).toInt() == 3) //é uma aresta
        {
            QPoint cids = itens.at(i)->data(2).toPoint();
            if(cids.x() == cidadeId || cids.y() == cidadeId)
            {
                this->removeItem(itens.at(i));
            }
        }
    }
}

void GraphicsScene::drawEdge(QPointF &lastPos, QPointF &pos, QPoint &ids)
{
    QGraphicsLineItem *edge = new QGraphicsLineItem(lastPos.x() + (cidadeWidth / 2.0),lastPos.y() + (cidadeHeigth / 2.0), pos.x() + (cidadeWidth / 2.0),pos.y() + (cidadeHeigth / 2.0));
    edge->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    edge->setData(1, 3);
    edge->setData(2, ids);
    this->addItem(edge);
}

//retorna a posição do centro de uma cidade que possui um determinado id
QPointF GraphicsScene::getCityPosition(int &id)
{
    QList<QGraphicsItem *> itens = this->items();
    for(int i = 0; i < itens.size(); i++)
    {
        if(itens.at(i)->data(1).toInt() == 1) //é uma cidade
        {
            if(itens.at(i)->data(0).toInt() == id) //achou id
            {
                return itens.at(i)->data(4).toPointF();
            }
        }
    }

    return QPointF(0.0,0.0);
}

/**
 * Limpa as arestas da tela
 */
void GraphicsScene::clearPath()
{
    QList<QGraphicsItem *> itens = this->items();
    for(int i = 0; i < itens.size(); i++)
    {
        if(itens.at(i)->data(1).toInt() == 3) //é uma aresta
        {
            this->removeItem(itens.at(i));
        }
    }
}

/**
 * Limpa as cidades e arestas da tela
 */
void GraphicsScene::clearCanvas()
{
    QList<QGraphicsItem *> itens = this->items();
    for(int i = 0; i < itens.size(); i++)
    {
        if(itens.at(i)->data(1).toInt() == 3 || itens.at(i)->data(1).toInt() == 1) //é uma aresta ou uma cidade
        {
            this->removeItem(itens.at(i));
        }
    }
}

void GraphicsScene::drawPath(const Individuo *individuo)
{
    int i,id1,id2;
    QPointF pos1,pos2;
    QPoint ids;

    this->clearPath();
    QList<Cidade *> cromossomo = individuo->getCromossomo();

    //percorrendo a lista de cidades e desenhando arestas entre elas
    for(i = 0; i < cromossomo.size()-1; i++)
    {
        id1 = cromossomo.at(i)->getId();
        id2 = cromossomo.at(i+1)->getId();
        pos1 = this->getCityPosition(id1);
        pos2 = this->getCityPosition(id2);

        ids = QPoint(id1, id2);
        this->drawEdge(pos1, pos2, ids);
    }

    id1 = cromossomo.at(i)->getId();
    id2 = cromossomo.at(0)->getId();
    pos1 = this->getCityPosition(id1);
    pos2 = this->getCityPosition(id2);
    ids = QPoint(id1, id2);

    this->drawEdge(pos1, pos2, ids);
}


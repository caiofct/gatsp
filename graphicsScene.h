#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QtWidgets>
#include <individuo.h>

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicsScene(QWidget *parent = 0);

    void setCidadeWidth(qreal w);
    qreal getCidadeWidth();

    void setCidadeHeigth(qreal h);
    qreal getCidadeHeigth();

    void drawPath(const Individuo *individuo);
    void clearPath();
    QPointF getCityPosition(int &id);

    inline void setRunning(const bool running) {this->running = running;}

signals:
    void cidadeCriada(QPointF &pos);
    void cidadeRemovida(int &chave);

protected:
    void mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent );

public slots:
    void clearCanvas();

private:
    void drawEdge(QPointF &lastPos, QPointF &pos, QPoint &ids);
    void removeEdge(int cidadeId);

    bool running;

    qreal cidadeWidth;
    qreal cidadeHeigth;

    QPointF lastPos;

};

#endif // GRAPHICSSCENE_H

#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include "cidade.h"
#include <QList>
#include <ctime>
#include <cstdlib>
#include <QtDebug>

class Individuo
{
public:
    Individuo();
    void mutar(float);
    QList<Individuo*> cruzar(const Individuo& );
    float custo();

    inline QList<Cidade*> getCromossomo() const {return this->cromossomo;}
    inline void setCromossomo(QList<Cidade*> cromo) {this->cromossomo = cromo;}
    inline void setFitness(float value) {this->fitness = value;}
    inline float getFitness() const {return this->fitness;}
    inline void setAcumulatedFitness(float value) {this->acumulatedFitness = value;}
    inline float getAcumulatedFitness() {return this->acumulatedFitness;}
    inline void addCidade(Cidade *cidade) {this->cromossomo.append(cidade);}
    inline bool operator<(const Individuo& ind) const {return  ind.getFitness() < this->getFitness();}
private:
    /**
      * @return QPair<int, int> define um ponto de corte do cromossomo, o primeiro elemento do par e a posicao incial e o segundo o tamanho do corte
      */
    QPair<int, int> corteCromossomo();

private:
    QList<Cidade*> cromossomo;

    float fitness;
    float acumulatedFitness;
};

#endif // INDIVIDUO_H

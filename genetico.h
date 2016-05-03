#ifndef GENETICO_H
#define GENETICO_H

#include "individuo.h"

class Genetico
{
public:
    Genetico();
    /**
     * reiniciar o algoritmo
     */
    void reiniciar();
    /**
      * gera a proxima geracao
      */
    void proximaGeracao();
    /**
      * retorna o melhor individuo em uma geracao
      */
    Individuo* melhorIndividuo();

    inline void  addGene(Cidade* cid) {this->genes.append(cid);}
    void removeGene(Cidade *cid);
    inline void  clearGenes() {this->genes.clear();}
    inline void  setGeracao(int ger) {this->geracaoMaxima = ger;}
    inline void  setTaxaMutacao(float taxa) {this->taxaMutacao = taxa;}
    inline void  setPopulacaoInicial(int qtd) {this->populacaoInicial = qtd;}
    inline int   getGeracaoAtual() const {return this->geracao;}
    inline int   getPopulacaoInicial() const {return this->populacaoInicial;}
    inline float getTaxaMutacao() const {return this->taxaMutacao;}
    inline int   getGeracaoMaxima() {return this->geracaoMaxima;}

    inline QList<Cidade*> getGenes() const {return this->genes;}
    inline int getQtdPopulacaoPar() const {return this->populacaoInicial + (this->populacaoInicial % 2);}
    inline QList<Individuo*> getPopulacao() const {return this->populacao;}
private:
     /**
      * gera a proxima geracao
      */
    void gerarPopulacaoInicial();
    void normalizarFitness(float minf, float maxf);
    QList<Individuo*> selecao();
    Individuo* selecionarIndividuo();
private:
    /**
      * populacao atual
      */
    QList<Individuo*> populacao;
    /**
      * todos os possiveis genes
      */
    QList<Cidade*> genes;
    /**
      * geracao atual
      */
    int geracao;
    /**
      * quantidade de geracoes que serao geradas
      */
    int geracaoMaxima;
    /**
      * quantidade de indivious na primeira geracao
      */
    int populacaoInicial;
    /**
     * taxa de mutacao
     */
    float taxaMutacao;
    //último número randômico obtido na seleção de um indivíduo
    float lastRandom;
};

#endif // GENETICO_H

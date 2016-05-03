#include "genetico.h"
#include <cfloat>
#include <cstdlib>
#include <cmath>
#include <QtDebug>

Genetico::Genetico()
{
    this->setGeracao(0);
    this->lastRandom = 1;
}

void Genetico::reiniciar()
{
    this->geracao = 0;
    populacao.clear();
    //qDebug() << "Iniciar";
    this->gerarPopulacaoInicial();
}

void Genetico::removeGene(Cidade *cid)
{
    for(int i = 0; i < genes.size(); i++)
    {
        if(genes.at(i)->getId() == cid->getId())
            this->genes.removeAt(i);
    }
    
    //this->reiniciar();
}

//Compara se um inidivíduo tem Fitness menor que outro indivíduo
bool individuoLessThan(const Individuo *ind1, const Individuo *ind2) {
    return (ind1->getFitness() < ind2->getFitness());
}


/**
 * Seleciona um Indivíduo a ser cruzado, usando o modelo da roleta para sortear os indivíduos
 * uma vez que os com o menor custo terão maior chance de serem selecionados
 */
Individuo* Genetico::selecionarIndividuo()
{
    float random;
    float custoTotal = 0.0;
    float fitV,minf,maxf;

    for(int i = 0; i < populacao.size(); i++)
    {
        custoTotal += populacao.at(i)->custo();
        //qDebug() << "\nIndividuo: " << i << " Custo: " << populacao.at(i)->custo();
    }

    minf = custoTotal;
    maxf = 0.0;

    //qDebug() << "\nGeracao" << this->getGeracaoAtual() << "Custo Total: " << custoTotal << "\n";

    //normalizando os custos e calculando a função de fitness
    for(int i = 0; i < populacao.size(); i++)
    {
        fitV = 1 - (populacao.at(i)->custo()/custoTotal);
        //setando valores max e min para o fitness
        /*if(fitV < minf) minf = fitV;
        if(fitV > maxf) maxf = fitV;*/

        populacao.at(i)->setFitness(fitV);
    }

    //normalização
    //this->normalizarFitness(minf,maxf);
    //qDebug() << "\nMin: " << minf << "Max: " << maxf << "\n";

    //ordenando a população em ordem crescente de fitness
    qSort(populacao.begin(), populacao.end(), individuoLessThan);

    //calculando o fitness acumulado para cada indivíduo
    populacao.at(populacao.size() - 1)->setAcumulatedFitness(populacao.at(populacao.size() - 1)->getFitness());
    for(int i = populacao.size() - 2; i >= 0; i--)
    {
        populacao.at(i)->setAcumulatedFitness(populacao.at(i)->getFitness() + populacao.at(i+1)->getAcumulatedFitness());
        //qDebug() << "Fit: " << populacao.at(i)->getFitness() << "AcF: "<< populacao.at(i)->getAcumulatedFitness();
    }

    //número entre 0 e Max( AcumulatedFitness )
    random = ((float)rand() / ((float)(RAND_MAX)+(float)(1)));
    random = populacao.at(0)->getAcumulatedFitness() * random;

    this->lastRandom = random;

    //qDebug() << "Rand: " << random;

    //selecionando 1º indivíduo que a Fitness acumulada ultrapassar random
    for(int i = populacao.size() - 1; i >= 0; i--)
    {
        if(populacao.at(i)->getAcumulatedFitness() > random)
        {
            //debug ini
            //qDebug() << populacao.at(i)->getFitness() << "\n";
            //debug fim

            return populacao.at(i);
        }
    }

    //caso a seleção não retorne nenhum, retorna um indivíduo aleatoriamente
    return populacao.at(rand() % populacao.size());
}

void Genetico::normalizarFitness(float minf, float maxf)
{
    for(int i = 0; i < populacao.size(); i++)
    {
        populacao.at(i)->setFitness((populacao.at(i)->getFitness() -  minf) / (maxf - minf));
    }
}

/**
 * Selecionando a quantidade par indivíduos mais próxima da definida pelo usuário
 */
QList<Individuo*> Genetico::selecao()
{
    QList<Individuo *> selecionados;

    for(int i = 0; i < this->getQtdPopulacaoPar(); i++)
    {
        selecionados << selecionarIndividuo();
    }

    return selecionados;
}

/**
 * Gera população inicial aleatoriamente
 */
void Genetico::gerarPopulacaoInicial()
{
    int random1, random2;

    for(int i = 0; i < getQtdPopulacaoPar(); i++)
    {
        Individuo *individuo =  new Individuo();
        for(int j = 0; j < genes.size(); j++)
        {
            srand(i * j * time(NULL));
            random1 = rand() % genes.size(); //escolhe uma posição para trocar os genes
            random2 = rand() % genes.size(); //escolhe outra posição para trocar os genes

            genes.swap(random1, random2); //troca dos genes
        }
        individuo->setCromossomo(genes);

        populacao.append(individuo); //inserindo indivíduo na população
    }
}

/**
 * Gerando próxima geração
 */
void Genetico::proximaGeracao()
{
    //fase de selecao
    QList<Individuo *> selecionados = this->selecao();

    //elitismo, preservando o melhor indivíduo da geração passada
    Individuo *melhorAntes = new Individuo();
    melhorAntes->setCromossomo(this->melhorIndividuo()->getCromossomo());

    //fase de cruzamento
    for(int i = 0; i < selecionados.size() - 1; i += 2)
    {
        QList<Individuo *> filhos = selecionados.at(i)->cruzar(*selecionados[i+1]);
        populacao[i] = filhos.at(0);
        populacao[i+1] = filhos.at(1);
    }

    //fase de mutação
    for(int i = 0; i < populacao.size(); i++)
    {
        populacao.at(i)->mutar(this->getTaxaMutacao());
    }

    populacao.append(melhorAntes);
    this->geracao++;
}

/**
 * Seleciona melhor indivíduo até agora
 */
Individuo * Genetico::melhorIndividuo()
{
    float melhorCusto = FLT_MAX;
    int indice = -1;

    for(int i = 0; i < populacao.size(); i++)
    {
        if(populacao.at(i)->custo() < melhorCusto)
        {
            melhorCusto = populacao.at(i)->custo();

            indice = i;
        }
    }

    if(indice != -1)
      return populacao.at(indice);

    return new Individuo();
}

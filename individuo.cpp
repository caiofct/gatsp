#include "individuo.h"

Individuo::Individuo()
{

}

float Individuo::custo()
{
    float custo = 0.0;
    Cidade *cid0 = NULL, *cid1 = NULL, *cid2 = NULL;
    float distancia = 0.0;
    int size = this->cromossomo.size();

    if (size <= 2) {
        return custo;
    }
    cid0 = cid1 = this->cromossomo.at(0);

    for (int i=1; i < this->cromossomo.size(); i++) {
        distancia = 0.0;
        cid2 = this->cromossomo.at(i);
        distancia = cid1->distancia(*cid2);

        custo += distancia;
        cid1 = cid2;
    }
    distancia = 0.0;
    distancia = cid2->distancia(*cid0);
    custo += distancia;

    return custo;
}

QPair<int, int> Individuo::corteCromossomo()
{
    int size = this->cromossomo.size();
    int inicio = 0;
    int tamanho = 0;
    QPair<int,int> retorno(0,0);

    if (size <= 2)
    {
        return retorno;
    }

    inicio = rand() % size;
    tamanho = (rand() % (size -2)) + 1;

    retorno.first  = inicio;
    retorno.second = tamanho;

    //qDebug() << tamanho;

    return retorno;
}

QList<Individuo*> Individuo::cruzar(const Individuo& indiv)
{
    int size1, size2;
    QPair<int,int> pontoCorte;
    Individuo *filho1, *filho2;
    QList<Cidade*> cromo1, cromo2;
    QList<Cidade*> cromoOri1, cromoOri2;

    pontoCorte = this->corteCromossomo();
    int inicio  = pontoCorte.first;
    int tamanho = pontoCorte.second;
    int fim     = inicio + tamanho -1;

    cromo1    = this->getCromossomo();
    cromo2    = indiv.getCromossomo();
    cromoOri1 = this->getCromossomo();
    cromoOri2 = indiv.getCromossomo();

    size1 = cromoOri1.size();
    size2 = cromoOri2.size();

    // cria cromossomo filho 1
    int pos1 = (fim)%size1;
    int pos2 = (fim)%size2;

    for (int i=1; i <= size1 - tamanho; i++) {
        pos1 = (1+pos1)%size1;
        Cidade *gene2;
        while(true) {
            pos2 = (1+pos2)%size2;
            gene2 = cromoOri2.at(pos2);

            bool jaExiste = false;
            for (int l=0; l < tamanho; l++) {
                int pos3 = (l+inicio)%size1;
                Cidade *gene1 = cromo1.at( pos3 );
                if (*gene1 == *gene2) {
                    jaExiste = true;
                }
            }
            if (!jaExiste) {
                break;
            }
        }

        cromo1[pos1] = gene2;
    }
//
//    // cria cromossomo filho 2
    pos1 = (fim)%size1;
    pos2 = (fim)%size2;

    for (int i=1; i <= size2 - tamanho; i++) {
        pos2 = (1+pos2)%size2;
        Cidade *gene1;
        while(true) {
            pos1 = (1+pos1)%size1;
            gene1 = cromoOri2.at(pos1);

            bool jaExiste = false;
            for (int l=0; l < tamanho; l++) {
                int pos3 = (l+inicio)%size1;
                Cidade *gene2 = cromo2.at( pos3 );
                if (*gene1 == *gene2) {
                    jaExiste = true;
                }
            }
            if (!jaExiste) {
                break;
            }
        }

        cromo2[pos2] = gene1;
    }

    filho1 = new Individuo();
    filho2 = new Individuo();

    filho1->setCromossomo(cromo1);
    filho2->setCromossomo(cromo2);

    QList<Individuo*> retorno;
    retorno << filho1 << filho2;
    return retorno;
}

void Individuo::mutar(float taxaMutacao)
{
    int taxa = 0;
    taxa = (int)ceil(taxaMutacao);

    int valor = 0;
    valor = rand()%101;

    if (valor < taxa) {
        int size = this->cromossomo.size();
        int qtd  = (int)valor*size/100;

        if (qtd == 0 ) qtd++;

        for (int i=0; i < qtd; i++) {
            int pos1 = rand()%size;
            int pos2 = rand()%size;

            Cidade *gene1, *gene2;
            gene1 = this->cromossomo.at(pos1);
            gene2 = this->cromossomo.at(pos2);

            this->cromossomo[pos1] = gene2;
            this->cromossomo[pos2] = gene1;
        }
    }
}

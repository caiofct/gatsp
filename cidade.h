#ifndef CIDADE_H
#define CIDADE_H

#include <QPair>
#include <cmath>

static int lastid;

class Cidade
{
public:
    Cidade(float x, float y);
    inline Cidade(int chave) { this->id = chave; }
    inline const int getId() const {return this->id;}
    inline QPair<float, float> getCoordenadas() const {return this->coordenada;}
    inline bool operator==(const Cidade& cid) {return cid.getId() == this->id;}

    float distancia(const Cidade&) const;
    inline static void setLastId(int value) {lastid = value;}
    inline static int getLastId() {return lastid;}

private:
    QPair<float, float> coordenada;
    int id;
};

#endif // CIDADE_H

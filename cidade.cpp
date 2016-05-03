#include "cidade.h"

Cidade::Cidade(float x, float y)
{
    this->coordenada.first = x;
    this->coordenada.second = y;
    this->id = Cidade::getLastId() + 1;
}

float Cidade::distancia(const Cidade& cid) const {
    float x1, x2, y1, y2;
    float dist;
    x1 = this->coordenada.first;
    y1 = this->coordenada.second;

    x2 = cid.getCoordenadas().first;
    y2 = cid.getCoordenadas().second;

    dist = sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
    return dist;
}

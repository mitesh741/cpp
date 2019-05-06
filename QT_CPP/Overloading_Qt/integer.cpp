#include "integer.h"

Integer::Integer(int value)
{
    this->nr = value;
}
Integer::operator int()
{

    return this->nr ;
}
int Integer::operator +=(Integer o )
{
    this->nr = o.getNr() + this->nr;
    return this->nr;
}

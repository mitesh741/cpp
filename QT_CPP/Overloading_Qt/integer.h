#ifndef INTEGER_H
#define INTEGER_H


class Integer
{
    int nr;

public:
    //    Integer() {}
    Integer(int value);
    operator int();
    int operator+=(Integer);
    int getNr()
    {
        return nr;
    }
};

#endif // INTEGER_H

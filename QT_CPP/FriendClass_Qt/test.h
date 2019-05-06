#ifndef TEST_H
#define TEST_H


class B;
class A
{
public :
    void setSecretValue(B &, int);
};

class B
{
    friend class A;
    int secretValue;
public:
    int getSecretValue()
    {
        return secretValue ;
    }

};
#endif // TEST_H

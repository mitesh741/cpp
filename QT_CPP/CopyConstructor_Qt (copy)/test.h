#ifndef TEST_H
#define TEST_H


class Test
{
public:
    int x;
    int y;
    int *p;
    Test(int x, int y,int );
    Test(const Test &);

    ~Test();
};

#endif // TEST_H

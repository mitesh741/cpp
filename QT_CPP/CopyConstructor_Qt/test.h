#ifndef TEST_H
#define TEST_H


class Test
{
public:
    static Test *getInstance();

    int x;
    int y;
    int *p;
    Test(const Test &);
    Test(int x, int y,int );

    ~Test();

private :
    static Test * instance;
};

#endif // TEST_H

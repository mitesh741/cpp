#ifndef POSITION_H
#define POSITION_H
class Position
{
        int x,y;
        public :
        Position(int ,int);
        ~Position();
        void getPosition() const;
        void setPosition(int,int) ;
       friend void setX(Position &,int);
};
#endif // POSITION_H

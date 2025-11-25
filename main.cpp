#include <iostream>
#include "Log.h"

class Player
{   
public:
    int x, y;
    int speed;

    void Move(int xa,int ya)
    {
        x += xa * speed;
        y += ya * speed;
    }
};


int main(int, char**)
{
    Player player;
    player.x = 10;
    player.y = 10;
    player.speed = 2;

    player.Move(2,0);

    std::cout << "Player x: " << player.x << " y : " << player.y << std::endl;
    return 0;
}
                                                                                                                    
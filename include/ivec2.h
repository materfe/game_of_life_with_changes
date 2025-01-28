class Vec2i
{
public:
    int x,y;
    Vec2i(int x_, int y_) : x(x_), y(y_){};

    void operator+(Vec2i other) const
    {
        x + other.x;
        y + other.y;
    }
};
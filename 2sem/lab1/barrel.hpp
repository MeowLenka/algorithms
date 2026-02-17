#ifndef BARREL_HPP
#define BARREL_HPP

class Barrel
{
private:
    double volume;
    // double v_alch, v_water;
    double concentration;

public:
    Barrel(double volume, double concentration);

    int get_volume();
    double get_concentration();

    void set_all(int volume, double concentration);
    void transfuse(Barrel &other, int n_liter); // перелить из other
};

#endif

#include <iostream>
#include "barrel.hpp"

Barrel::Barrel(double volume, double concentration)
{
    this->volume = volume;
    this->concentration = concentration;
}

int Barrel::get_volume()
{
    return volume;
}

double Barrel::get_concentration()
{
    return concentration;
}

void Barrel::set_all(int volume, double concentration)
{
    this->volume = volume;
    this->concentration = concentration;
}

void Barrel::transfuse(Barrel &other, int n_liter) // перелить из other
{
    int v_other = other.get_volume();
    double c_other = other.get_concentration();

    concentration = (concentration * volume + (c_other * n_liter)) / (volume+n_liter);
    volume += n_liter;
    other.set_all(v_other-n_liter, c_other);
}

#include "perso.hpp"

perso::perso()
    :vie(100),miam(100),pos(0,0)
{}

perso::perso(std::pair<int,int> spawn)
    :vie(100),miam(100),pos(spawn)
{}

perso::perso(int type)
    :type(type)
{}

perso::~perso(){}

std::pair<int,int>& perso::deplacer()
{
  return pos;
}

std::pair<int,int> perso::deplacer() const
{
  return pos;
}

int perso::getsetmiam() const {return miam;}
int& perso::getsetmiam(){return miam;}
int perso::getsetvie() const {return vie;}
int& perso::getsetvie() {return vie;}
std::pair<int,int> perso::getsetpos() const {return pos;}
std::pair<int,int>& perso::getsetpos() {return pos;}
int& perso::getsetscore(){return score;}
int perso::getsetscore() const{return score;}
int perso::getsettype() const{return type;}
int& perso::getsettype(){return type;}

void perso::mangerp()
{
    if (miam+20 > 100)
    {
        miam=100;
    }else
    {
        miam+=20;
    }
}

void perso::mangerm()
{
    if (miam-1 < 0)
    {
        miam=0;
    }else
    {
        miam-=1;
    }
}

void perso::viep()
{
    if (vie+10 > 100)
    {
        vie=100;
    }else
    {
        vie+=10;
    }
}

void perso::viem()
{
    if (vie-30 < 0)
    {
        vie=0;
    }else
    {
        vie-=30;
    }
}

aventurier::aventurier(int type)
{
    this->getsettype() = 2;
}

void aventurier::mangerp()
{
    //std::cout<<"aventurier mangerp"<<std::endl;
    if (miam+20 > 100)
    {
        miam=100;
    }else
    {
        miam+=20;
    }
}

void aventurier::mangerm()
{
    //std::cout<<"aventurier mangerm"<<std::endl;
    if (miam-2 < 0)
    {
        miam=0;
    }else
    {
        miam-=2;
    }
}

void aventurier::viep()
{
    //std::cout<<"aventurier viep"<<std::endl;
    if (vie+10 > 100)
    {
        vie=100;
    }else
    {
        vie+=10;
    }
}

void aventurier::viem()
{
    //std::cout<<"aventurier viem"<<std::endl;
    if (vie-30 < 0)
    {
        vie=0;
    }else
    {
        vie-=30;
    }
}

guerrier::guerrier(int type)
{
    this->getsettype() = 1;
}

void guerrier::mangerp()
{
    //std::cout<<"guerrier mangerp"<<std::endl;
    if (miam+20 > 100)
    {
        miam=100;
    }else
    {
        miam+=20;
    }
}

void guerrier::mangerm()
{
    //std::cout<<"guerrier mangerm"<<std::endl;
    if (miam-4 < 0)
    {
        miam=0;
    }else
    {
        miam-=4;
    }
}

void guerrier::viep()
{
    //std::cout<<"guerrier viep"<<std::endl;
    if (vie+10 > 100)
    {
        vie=100;
    }else
    {
        vie+=10;
    }
}

void guerrier::viem()
{
    //std::cout<<"guerrier viem"<<std::endl;
    if (vie-20 < 0)
    {
        vie=0;
    }else
    {
        vie-=20;
    }
}

sorcier::sorcier(int type)
{
    this->getsettype() = 3;
}

void sorcier::mangerp()
{
    //std::cout<<"sorcier mangerp"<<std::endl;
    if (miam+20 > 100)
    {
        miam=100;
    }else
    {
        miam+=20;
    }
}

void sorcier::mangerm()
{
    //std::cout<<"sorcier mangerm"<<std::endl;
    if (miam-4 < 0)
    {
        miam=0;
    }else
    {
        miam-=4;
    }
}

void sorcier::viep()
{
    //std::cout<<"sorcier viep"<<std::endl;
    if (vie+20 > 100)
    {
        vie=100;
    }else
    {
        vie+=20;
    }
}

void sorcier::viem()
{
    //std::cout<<"sorcier viem"<<std::endl;
    if (vie-30 < 0)
    {
        vie=0;
    }else
    {
        vie-=30;
    }
}



#include "noeud.hpp"

noeud::noeud()
    :acces(true),item(0),liste_voisin()
{}

noeud::noeud(bool p_acces,int p_item)
   :acces(p_acces),item(p_item)
{}

std::ostream& operator<<(std::ostream& s, noeud const & n)
{
    s<< "item : "<< n.getsetitem() << " ; acces : " << n.acces;
    return s;
}

void noeud::set_voisin(int pos_x,int pos_y,int grid_x,int grid_y)
{
    if (pos_x == 0)
    {
        if (pos_y == 0)
        {
            std::pair<int,int> coord (pos_x,pos_y+1);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x+1,pos_y);
            liste_voisin.push_back(coord);
        }else if (pos_y == grid_y-1)
        {
            std::pair<int,int> coord (pos_x,pos_y-1);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x+1,pos_y);
            liste_voisin.push_back(coord);
        }else
        {
            std::pair<int,int> coord (pos_x,pos_y-1);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x,pos_y+1);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x+1,pos_y);
            liste_voisin.push_back(coord);
        }
    }else if (pos_x == grid_x-1)
    {
        if (pos_y == 0)
        {
            std::pair<int,int> coord (pos_x,pos_y+1);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x-1,pos_y);
            liste_voisin.push_back(coord);
        }else if (pos_y == grid_y-1)
        {
            std::pair<int,int> coord (pos_x,pos_y-1);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x-1,pos_y);
            liste_voisin.push_back(coord);
        }else
        {
            std::pair<int,int> coord (pos_x,pos_y-1);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x,pos_y+1);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x-1,pos_y);
            liste_voisin.push_back(coord);
        }
    }else
    {
        if (pos_y == 0)
        {
            std::pair<int,int> coord (pos_x+1,pos_y);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x-1,pos_y);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x,pos_y+1);
            liste_voisin.push_back(coord);
        }else if (pos_y == grid_y-1)
        {
            std::pair<int,int> coord (pos_x+1,pos_y);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x-1,pos_y);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x,pos_y-1);
            liste_voisin.push_back(coord);
        }else
        {
            std::pair<int,int> coord (pos_x+1,pos_y);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x-1,pos_y);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x,pos_y-1);
            liste_voisin.push_back(coord);
            coord = std::make_pair(pos_x,pos_y+1);
            liste_voisin.push_back(coord);
        }
    }
}

void noeud::afficher_voisin()
{
    for (auto val : liste_voisin)
    {
        std::cout<<"("<<val.first<<","<<val.second<<")     ";
    }
    std::cout<<std::endl;
}

int noeud::getsetitem() const{return item;}
int& noeud::getsetitem(){return item;}



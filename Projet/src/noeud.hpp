#pragma once

#ifndef NOEUD_HPP
#define NOEUD_HPP

#include <iostream>
#include <map>
#include <utility>
#include <vector>

struct noeud
{
    // Attribut pour savoir si c'est un obstacle ou non
    //  true : case libre
    //  false : case avec obstacle
    bool acces;
    // Item contenu sur la case
    //  0 : aucun item présent
    //  1 : debut
    //  2 : arrivee
    //  3 : parcouru par un algo de parcours
    //  4 : nourriture
    //  5 : monstre
    //  6 : vie
    //  7 : changement de map


    std::vector<std::pair<int,int>> liste_voisin;
    int distance;
    std::pair<int,int> predecesseur;

    noeud();

    noeud(bool p_acces,int p_item);

    void set_voisin(int pos_x,int pos_y,int grid_x,int grid_y);

    void afficher_voisin();

    int getsetitem() const;
    int& getsetitem();

private:
    int item;

};

std::ostream& operator<<(std::ostream& s, noeud const & n);

#endif // NOEUD_HPP

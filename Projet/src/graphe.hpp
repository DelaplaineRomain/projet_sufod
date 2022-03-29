#pragma once


#ifndef GRAPHE_HPP
#define GRAPHE_HPP

#include <iostream>
#include <map>
#include <utility>
#include <vector>

#include "noeud.hpp"
#include "perso.hpp"

struct graphe
{
    perso *Yugo;
    int dim_x;
    int dim_y;

    graphe();
    graphe(int p_dim_x,int p_dim_y,std::map<std::pair<int,int>,noeud> p_map);

    void afficher();

    int get_nb_mur();

    void generate_wall();
    void generate_perso(int type);

    int checknoeud();
    void reset_parcours();
    void reset_debut();
    void reset_fin();
    void reset_map();

    std::map<std::pair<int,int> , noeud> getsetliste_case() const;
    std::map<std::pair<int,int> , noeud>& getsetliste_case();

private:
    std::map<std::pair<int,int> , noeud> liste_case;

};

#endif // GRAPHE_HPP

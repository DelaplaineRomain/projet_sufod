#include <QApplication>
#include "window.hpp"

#include <stdlib.h>

int main(int argc,char *argv[])
{
    //#### Creation de notre graphe ####//

    //Ses dimensions
    std::map<std::pair<int,int> , noeud> liste_case;
    int dim_x = 20;
    int dim_y = 12;
    //Sa liste de case
    for (int i = 0; i < dim_x; i++)
    {
        for (int j = 0; j < dim_y; j++)
        {
            noeud ma_case = noeud(true,0);
            ma_case.set_voisin(i,j,dim_x,dim_y);
            //std::cout<<"case : ("<<i<<","<<j<<") :";
            //ma_case.afficher_voisin();
            std::pair <int,int> coord (i,j);
            liste_case[coord] = ma_case;
        }
    }

    //creation du graphe
    graphe mon_graphe = graphe(dim_x,dim_y,liste_case);

//    //Son perso
//    perso *Yugo = new perso();
//    mon_graphe.Yugo = Yugo;

    // mon_graphe.afficher(); //affichage graphique

    //#### Gestion graphique ####//
    QApplication app(argc,argv);
    window win(mon_graphe);
    win.show();
    return app.exec();


    return 0;
}

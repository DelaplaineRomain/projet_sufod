#include "render_area.hpp"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <thread>
#include <chrono>

#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QImageReader>


#ifdef _WIN32
#include <windows.h>
#define SYST "win"
#else
#include <unistd.h>
#define SYST "lin"
#endif

//#### Fonction libre ####//
int boucle(noeud &parcours, std::list<std::pair<int,int>> &queue, std::map<std::pair<int,int>,bool> &visite,graphe &g,std::pair<int,int> &arrivee,int & ret,bool check,bool refill)
{
    for (auto coord_voisin: parcours.liste_voisin)
    {
        //Si les coordonées du voisin correspondent à l'arrivée on sort de la boucle
        if (check && arrivee == coord_voisin)
        {
            return 0;
        }

        //Si on tombe sur un mur, on n'ajoute pas ses coordonnées dans queue mais on dit qu'on l'a vérifié
        if (g.getsetliste_case()[coord_voisin].acces== false ) //mur
        {
            visite[coord_voisin]=true;
        }

        //Sinon on dit qu'on a vérifié le voisin et on l'ajoute à la queue pour refaire l'algo
        if (visite[coord_voisin]==false)
        {
            g.getsetliste_case()[coord_voisin].getsetitem() = 3;
            visite[coord_voisin]=true;
            queue.push_back(coord_voisin);
            ret++;
            if (refill)
            {
                g.getsetliste_case()[coord_voisin].acces = false;
            }
        }
    }
}

std::pair<std::pair<int,int>,noeud> find_min (graphe g,graphe P)
{
    int dist = 10000;
    std::pair<std::pair<int,int>,noeud> ret;
    for (auto val : g.getsetliste_case())
    {
        if (P.getsetliste_case().find(val.first) == P.getsetliste_case().end())
        {
            if (std::get<1>(val).distance < dist)
            {
                dist = val.second.distance;
                ret = val;
            }
        }
    }
    return ret;
}

//#### Fonction interne à render area ####//

// Constructeur
render_area::render_area(graphe& mon_graphe,Ui::MainWindow *ui,QWidget *parent)
    :QWidget(parent),pixmap(new QPixmap),grid_state(false),mon_graphe(mon_graphe),ui(ui),
      x_old(0),y_old(0)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

// Destructeur
render_area::~render_area()
{
    if(pixmap!=nullptr)
    {
        delete pixmap;
        pixmap=nullptr;
    }
}

// Fonction d'affichage des éléments graphiques dans l'espace de dessin
void render_area::paintEvent(QPaintEvent*)
{
    //A painter class able to draw in 2D
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //The drawing pen with its properties
    QPen pen;
    pen.setWidth(1.0);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    //The brush class is usefull to fill the interior of the shapes
    QBrush brush = painter.brush();
    brush.setStyle(Qt::SolidPattern); //fill the interior
    painter.setBrush(brush);


    if (mon_graphe.Yugo->fin)
    {
        painter.eraseRect(0,0,mon_graphe.dim_x,mon_graphe.dim_y);
        QString string = tr("Game Over");
        painter.setFont(QFont("Times", 80, QFont::Bold));
        painter.drawText(50,200,string);
    }
    //if grid_state is true, then we draw the grid
    else if(grid_state)
    {
        int dim_x = mon_graphe.dim_x;
        int dim_y = mon_graphe.dim_y;

        for (int i = 0; i < dim_x; i++)
        {
            for (int j = 0; j < dim_y; j++)
            {

                int coin_sup_x = i*((this->width())/dim_x);
                int coin_sup_y = j*((this->height())/dim_y);
                int coin_inf_x = coin_sup_x + (this->width())/dim_x;
                int coin_inf_y = coin_sup_y + (this->height())/dim_y;
                // std::cout<<mon_graphe.getsetliste_case()[std::make_pair(i,j)].acces<<std::endl;


                if (mon_graphe.getsetliste_case()[std::make_pair(i,j)].acces && mon_graphe.getsetliste_case()[std::make_pair(i,j)].getsetitem() == 4) //food
                {
                    QString chemin = QString("%1/../src/img/food.png").arg(QCoreApplication::applicationDirPath());
                    QImageReader reader;
                    reader.setFileName(chemin);
                    QImage image_food = reader.read();
                    if (image_food.isNull())
                    {
                        QColor color(119, 200, 42);
                        brush.setColor(color);
                        painter.setBrush(brush);
                        painter.drawRect(coin_sup_x,coin_sup_y,coin_inf_x,coin_inf_y);
                    }else
                    {
                        brush.setColor(Qt::white);
                        painter.setBrush(brush);
                        painter.drawRect(coin_sup_x,coin_sup_y,coin_inf_x,coin_inf_y);
                        painter.drawImage(coin_sup_x+(coin_inf_x-coin_sup_x)*1/10, coin_sup_y+(coin_inf_y-coin_sup_y)*1/10, image_food.scaled((coin_inf_x-coin_sup_x)*8/10,(coin_inf_y-coin_sup_y)*8/10));
                    }
                }else if (mon_graphe.getsetliste_case()[std::make_pair(i,j)].acces && mon_graphe.getsetliste_case()[std::make_pair(i,j)].getsetitem() == 5 ) //monstre
                {
                    QString chemin = QString("%1/../src/img/monstre.png").arg(QCoreApplication::applicationDirPath());
                    QImageReader reader;
                    reader.setFileName(chemin);
                    QImage image_monstre = reader.read();
                    if (image_monstre.isNull())
                    {
                        QColor color(255, 158, 27);
                        brush.setColor(color);
                        painter.setBrush(brush);
                        painter.drawRect(coin_sup_x,coin_sup_y,coin_inf_x,coin_inf_y);
                    }else
                    {
                        brush.setColor(Qt::white);
                        painter.setBrush(brush);
                        painter.drawRect(coin_sup_x,coin_sup_y,coin_inf_x,coin_inf_y);
                        painter.drawImage(coin_sup_x+(coin_inf_x-coin_sup_x)*2/10, coin_sup_y+2, image_monstre.scaled((coin_inf_x-coin_sup_x)*6/10,(coin_inf_y-coin_sup_y)*9/10));
                    }
                }else if (mon_graphe.getsetliste_case()[std::make_pair(i,j)].acces && mon_graphe.getsetliste_case()[std::make_pair(i,j)].getsetitem() == 6) //vie
                {
                    QString chemin = QString("%1/../src/img/heart.png").arg(QCoreApplication::applicationDirPath());
                    QImageReader reader;
                    reader.setFileName(chemin);
                    QImage image_vie = reader.read();
                    if (image_vie.isNull())
                    {
                        QColor color(255, 0, 0);
                        brush.setColor(color);
                        painter.setBrush(brush);
                        painter.drawRect(coin_sup_x,coin_sup_y,coin_inf_x,coin_inf_y);
                    }else
                    {
                        brush.setColor(Qt::white);
                        painter.setBrush(brush);
                        painter.drawRect(coin_sup_x,coin_sup_y,coin_inf_x,coin_inf_y);
                        painter.drawImage(coin_sup_x+(coin_inf_x-coin_sup_x)*5/20, coin_sup_y+(coin_inf_y-coin_sup_y)*3/20, image_vie.scaled((coin_inf_x-coin_sup_x)*5/10,(coin_inf_y-coin_sup_y)*7/10));
                    }
                }else if (mon_graphe.getsetliste_case()[std::make_pair(i,j)].acces && mon_graphe.getsetliste_case()[std::make_pair(i,j)].getsetitem() == 7) //tp
                {
                    QColor color(165, 10, 196);
                    brush.setColor(color);
                    painter.setBrush(brush);
                    painter.drawRect(coin_sup_x,coin_sup_y,coin_inf_x,coin_inf_y);
                }else if (mon_graphe.getsetliste_case()[std::make_pair(i,j)].acces == false)
                {
                    QColor color(100, 100, 100);
                    brush.setColor(color);
                    painter.setBrush(brush);
                    painter.drawRect(coin_sup_x,coin_sup_y,coin_inf_x,coin_inf_y);
                }else
                {
                    brush.setColor(Qt::white);
                    painter.setBrush(brush);
                    painter.drawRect(coin_sup_x,coin_sup_y,coin_inf_x,coin_inf_y);
                }

                if (mon_graphe.Yugo->getsetpos().first == i && mon_graphe.Yugo->getsetpos().second == j)
                {
                    QString chemin = QString("%1/../src/img/adventurer.png").arg(QCoreApplication::applicationDirPath());
                    if (mon_graphe.Yugo->getsettype() == 1) chemin = QString("%1/../src/img/warrior.png").arg(QCoreApplication::applicationDirPath());
                    else if (mon_graphe.Yugo->getsettype() == 3) chemin = QString("%1/../src/img/sorcerer.png").arg(QCoreApplication::applicationDirPath());
                    QImageReader reader;
                    reader.setFileName(chemin);
                    QImage image = reader.read();
                    if (image.isNull())
                    {
                        painter.drawRect(coin_sup_x,coin_sup_y,coin_inf_x,coin_inf_y);
                        QColor color(72, 122, 255);
                        brush.setColor(color);
                        painter.setBrush(brush);
                        painter.drawEllipse(coin_sup_x,coin_sup_y,coin_inf_x-coin_sup_x,coin_inf_y-coin_sup_y);
                    }else
                    {
                        painter.drawImage(coin_sup_x+(coin_inf_x-coin_sup_x)*2/10, coin_sup_y+2, image.scaled((coin_inf_x-coin_sup_x)*6/10,(coin_inf_y-coin_sup_y)*9/10));

                    }

                }
            }
        }
    }


}

// Fonction de parcours en largeur | renvoie le nombre de case parcouru
int render_area::parcourslargeur(graphe & g, int x_dep,int y_dep, int x_ar, int y_ar,bool check = true,bool refill = false)
{
    g.reset_parcours();
    int ret = 0;
    //Creation coorndonées de départ et d'arrivée.
    std::pair<int,int> debut(x_dep,y_dep);
    std::pair<int,int> arrivee(x_ar,y_ar);
    int stop = 1;
    // On crée une grille bool et on initialise
    // à 0 pour dire qu'on a visité aucune case
    std::map<std::pair<int,int>,bool> visite;
    for (int i=0; i<g.dim_x;i++)
    {
        for (int j=0; j<g.dim_y;j++)
        {
        std::pair<int,int> coord(i,j);
        visite[coord]=false;
        }
    }

    //On crée une queue de coordonnées à vérifier
    std::list<std::pair<int,int>> queue;

    //Notre point de départ est vérifié on le met dans la queue
    visite[debut]=true;
    queue.push_back(debut);
    //On parcourt la queue tant qu'elle n'est pas vide.
    while(stop!=0 && !queue.empty())
    {
        //On affiche l'élement qu'on va vérifier de la queue
        debut=queue.front();
        //std::cout<<debut.first<<debut.second<<std::endl;
        queue.pop_front();
        noeud parcours = g.getsetliste_case()[debut];
        stop = boucle(parcours,queue,visite,g,arrivee,ret,check,refill);
        //repaint();
    }
    if (check) std::cout<<"ARRIVEE TROUVEE"<<std::endl;
    return ret;
}

// Fonction de parcours, Dijkstra | renvoie 1 si le jeu est finis 0 sinon
int render_area::dijkstra(graphe & g, int x_dep,int y_dep, int x_ar, int y_ar)
{
    // On reset le parcours précédent
    g.reset_parcours();

    // Initialisation des variables
    std::map<std::pair<int,int>,noeud> P_liste;
    graphe P = graphe(g.dim_x,g.dim_y,P_liste);
    std::pair<int,int> coord_debut (x_dep,y_dep);
    std::pair<int,int> coord_fin (x_ar,y_ar);

    // On assigne les distances a tout les noeuds de g
    // La case de debut possede une distance nulle et les autres une distance infini (1000 ici)
    for (auto val : g.getsetliste_case())
    {
        std::pair<int,int> coord = val.first;
        g.getsetliste_case()[coord].distance = 10000;
        if (coord.first == coord_debut.first && coord.second == coord_debut.second){g.getsetliste_case()[coord].distance = 0;}
    }

    // variable pour eviter de boucler infiniment
    // signifie que le chemin n'existe pas
    int compteur = 0;
    // On applique l'algorithme de recherche
    while(P.getsetliste_case().size() < g.getsetliste_case().size()-g.get_nb_mur() && compteur <= g.getsetliste_case().size()-g.get_nb_mur())
    {
        compteur++;
        std::pair<std::pair<int,int>,noeud> a = find_min(g,P);
        P.getsetliste_case()[a.first] = std::get<1>(a);
        auto liste = g.getsetliste_case()[a.first].liste_voisin;
        for (auto b : liste)
        {
            if (P.getsetliste_case().find(b) == P.getsetliste_case().end())
            {
                if (g.getsetliste_case()[b].acces)
                {
                    if (g.getsetliste_case()[b].distance > std::get<1>(a).distance + 1)
                    {
                        g.getsetliste_case()[b].distance = std::get<1>(a).distance + 1;
                        g.getsetliste_case()[b].predecesseur = a.first;
                    }
                }
            }
        }
    }

    // Si on a trouver un chemin alors on le trace
    if (compteur <= g.getsetliste_case().size()-g.get_nb_mur())
    {
        // On en deduit le chemin le plus court entre debut et fin
        std::vector<std::pair<int,int>> A;
        std::pair<int,int> coord_s = coord_fin;
        while (coord_s.first != coord_debut.first || coord_s.second != coord_debut.second)
        {
            A.push_back(std::make_pair(std::get<0>(coord_s),std::get<1>(coord_s)));
            coord_s = g.getsetliste_case()[coord_s].predecesseur;
        }
        A.push_back(std::make_pair(std::get<0>(coord_debut),std::get<1>(coord_debut)));

        // inverser A pour aller du debut a la fin
        std::reverse(A.begin(),A.end());

        // implementation graphique
        for (auto val : A)
        {
            if (g.getsetliste_case()[val].getsetitem() != 1 && g.getsetliste_case()[val].getsetitem() != 2)
            {
                if (g.getsetliste_case()[g.getsetliste_case()[val].predecesseur].getsetitem() == 3)
                {
                    g.getsetliste_case()[g.getsetliste_case()[val].predecesseur].getsetitem() = 0;
                }

                // On update les stats de Yugo
                g.Yugo->deplacer() = val;
                int cgt_map = g.checknoeud();
                ui->miam->setValue(g.Yugo->getsetmiam());
                ui->vie->setValue(g.Yugo->getsetvie());
                ui->score->display(g.Yugo->getsetscore());

                // On check si on a perdu
                check_fin();
                if (g.Yugo->fin)
                {
                    repaint();
                    return 1;
                }

                g.getsetliste_case()[val].getsetitem() = 3;

                if (cgt_map == 1)
                {
                    game_start(g.Yugo->getsetpos().first,g.Yugo->getsetpos().second,true,0);
                    return 2;
                }
            }
            // pause de 200 ms pour creer une animation
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            repaint();
        }
    }else
    {
        std::cout<<"Aucun chemin trouve"<<std::endl;
    }
    return 0;
}

// Fonction pour générer les murs
void render_area::generate_wall()
{

    // #### initialisation de mur random

    for (int i = 0; i < mon_graphe.dim_x; i++)
    {
        for (int j = 0; j < mon_graphe.dim_y; j++)
        {
            int chance = rand() % 100 + 1;
            if (chance < 25)
            {
                mon_graphe.getsetliste_case()[std::make_pair(i,j)].acces = false;
            }
        }
    }

    // #### On rebouche les zones isolee

    std::map<std::pair<int,int>,int> zone;

    graphe g_copie = mon_graphe;
    while(!g_copie.getsetliste_case().empty() && g_copie.getsetliste_case().size() != g_copie.get_nb_mur())
    {
        //std::cout<<"modif acces"<<std::endl;
        // definir la premiere case non mur
        auto it = g_copie.getsetliste_case().begin();
        auto coord_debut = it->first;
        bool check = false;
        while (!check)
        {
            if (!g_copie.getsetliste_case()[coord_debut].acces)
            {
                it++;
                coord_debut = it->first;
            }else
            {
                check = true;
            }
        }

        // algo de parcours à partir du noeud de depart
        int nb_case_zone = 1+parcourslargeur(g_copie,std::get<0>(coord_debut),std::get<1>(coord_debut),0,0,false);
        zone[coord_debut] = nb_case_zone;

        // on enleve toute les cases parcouru lors de l'algo de parcours
        auto liste_temp = g_copie.getsetliste_case();
        for (auto val : liste_temp)
        {
            auto coord = val.first;
            if (g_copie.getsetliste_case()[coord].getsetitem() == 3)
            {
                g_copie.getsetliste_case().erase(coord);
            }
        }
        g_copie.getsetliste_case().erase(coord_debut);

        // mise a jour des voisins dans la liste de case restante
        for (auto val : g_copie.getsetliste_case())
        {
            auto coord = val.first;
            for (int i = 0; i < g_copie.getsetliste_case()[coord].liste_voisin.size(); i++ )
            {
                if (g_copie.getsetliste_case().find(g_copie.getsetliste_case()[coord].liste_voisin.at(i)) == g_copie.getsetliste_case().end())
                {
                    g_copie.getsetliste_case()[coord].liste_voisin.erase(g_copie.getsetliste_case()[coord].liste_voisin.begin()+i);
                }
            }
        }
    }

    // determination de la plus grande zone
    int max = 0;
    for (auto it=zone.begin(); it != zone.end(); ++it)
    {
        auto coord = it->first;
        if (zone[coord] > max )
        {
            max = zone[coord];
        }
    }

    // remplissage des zones isolee
    for (auto it=zone.begin(); it != zone.end(); ++it)
    {
        auto coord = it->first;
        if (zone[coord] != max)
        {
            // permet de passer a false tte les cases de la zones
            parcourslargeur(mon_graphe,std::get<0>(coord),std::get<1>(coord),0,0,false,true);
            mon_graphe.getsetliste_case()[coord].acces = false; // on le ft manuellement pour le premier element
        }
    }
    // repaint();
}

// Fonction pour générer les items
void render_area::generate_item()
{
    // #### initialisation de item random

    for (int i = 0; i < mon_graphe.dim_x; i++)
    {
        for (int j = 0; j < mon_graphe.dim_y; j++)
        {
            int chance_miam = rand() % 100 + 1;
            int chance_vie = rand() % 100 + 1;
            int chance_monstre = rand() % 100 + 1;
            if (chance_miam < 13 && mon_graphe.getsetliste_case()[std::make_pair(i,j)].acces)
            {
                mon_graphe.getsetliste_case()[std::make_pair(i,j)].getsetitem() = 4;
            }
            if (chance_vie < 5 && mon_graphe.getsetliste_case()[std::make_pair(i,j)].acces)
            {
                mon_graphe.getsetliste_case()[std::make_pair(i,j)].getsetitem() = 6;
            }
            if (chance_monstre < 5 && mon_graphe.getsetliste_case()[std::make_pair(i,j)].acces)
            {
                mon_graphe.getsetliste_case()[std::make_pair(i,j)].getsetitem() = 5;
            }
        }
    }
    //repaint();
}

// Fonction pour générer les items sur la map
void render_area::generate_tp()
{
    int coord_x = 0;
    int coord_y = floor((mon_graphe.dim_y-1)/2);
    std::pair<int,int> position(coord_x,coord_y);

    // config de la case de fin dans la map
    bool check = false;
    while (!check)
    {
        if (mon_graphe.getsetliste_case()[position].acces)
        {
            mon_graphe.getsetliste_case()[position].getsetitem() = 7;
            check = true;
        }else
        {
            if (std::get<0>(position) < mon_graphe.dim_x-1){std::get<0>(position) += 1;}
            else if (std::get<1>(position) < mon_graphe.dim_y-1){std::get<1>(position) += 1;}
            else
            {
                std::get<0>(position) = 0;
                std::get<1>(position) = 0;
            }
        }
    }

    coord_x = floor((mon_graphe.dim_x-1)/2);
    coord_y = 0;
    std::get<0>(position) = coord_x;
    std::get<1>(position) = coord_y;

    // config de la case de fin dans la map
    check = false;
    while (!check)
    {
        if (mon_graphe.getsetliste_case()[position].acces)
        {
            mon_graphe.getsetliste_case()[position].getsetitem() = 7;
            check = true;
        }else
        {
            if (std::get<0>(position) < mon_graphe.dim_x-1){std::get<0>(position) += 1;}
            else if (std::get<1>(position) < mon_graphe.dim_y-1){std::get<1>(position) += 1;}
            else
            {
                std::get<0>(position) = 0;
                std::get<1>(position) = 0;
            }
        }
    }

    coord_x = mon_graphe.dim_x-1;
    coord_y = floor((mon_graphe.dim_y-1)/2);
    std::get<0>(position) = coord_x;
    std::get<1>(position) = coord_y;

    // config de la case de fin dans la map
    check = false;
    while (!check)
    {
        if (mon_graphe.getsetliste_case()[position].acces)
        {
            mon_graphe.getsetliste_case()[position].getsetitem() = 7;
            check = true;
        }else
        {
            if (std::get<0>(position) < mon_graphe.dim_x-1){std::get<0>(position) += 1;}
            else if (std::get<1>(position) < mon_graphe.dim_y-1){std::get<1>(position) += 1;}
            else
            {
                std::get<0>(position) = 0;
                std::get<1>(position) = 0;
            }
        }
    }

    coord_x = floor((mon_graphe.dim_x-1)/2);
    coord_y = mon_graphe.dim_y-1;
    std::get<0>(position) = coord_x;
    std::get<1>(position) = coord_y;

    // config de la case de fin dans la map
    check = false;
    while (!check)
    {
        if (mon_graphe.getsetliste_case()[position].acces)
        {
            mon_graphe.getsetliste_case()[position].getsetitem() = 7;
            check = true;
        }else
        {
            if (std::get<0>(position) < mon_graphe.dim_x-1){std::get<0>(position) += 1;}
            else if (std::get<1>(position) < mon_graphe.dim_y-1){std::get<1>(position) += 1;}
            else
            {
                std::get<0>(position) = 0;
                std::get<1>(position) = 0;
            }
        }
    }
}

// Fonction pour definir aléatoirement ou non dans la map les coordonées de debut
std::pair<int,int> render_area::define_debut(int pos_x = 0,int pos_y = 0,bool accept_pos = false)
{
    // reset du debut dans le graphe
    mon_graphe.reset_debut();

    // init des coords du debut et de la fin dans la map

    if (!accept_pos)
    {
        std::pair<int,int> debut (rand() % (mon_graphe.dim_x-1),rand() % (mon_graphe.dim_y-1));

        // config de la case de debut dans la map
        bool check = false;
        while (!check)
        {
            if (mon_graphe.getsetliste_case()[debut].acces)
            {
                mon_graphe.getsetliste_case()[debut].getsetitem() = 1;
                check = true;
            }else
            {
                if (std::get<0>(debut) < mon_graphe.dim_x-1){std::get<0>(debut) += 1;}
                else if (std::get<1>(debut) < mon_graphe.dim_y-1){std::get<1>(debut) += 1;}
                else
                {
                    std::get<0>(debut) = 0;
                    std::get<1>(debut) = 0;
                }
            }
        }
        return debut;
    }else
    {
        std::pair<int,int> debut (pos_x,pos_y);

        // config de la case de debut dans la map
        bool check = false;
        while (!check)
        {
            if (mon_graphe.getsetliste_case()[debut].acces)
            {
                mon_graphe.getsetliste_case()[debut].getsetitem() = 1;
                check = true;
            }else
            {
                if (std::get<0>(debut) < mon_graphe.dim_x-1){std::get<0>(debut) += 1;}
                else if (std::get<1>(debut) < mon_graphe.dim_y-1){std::get<1>(debut) += 1;}
                else
                {
                    std::get<0>(debut) = 0;
                    std::get<1>(debut) = 0;
                }
            }
        }
        return debut;
    }

    //repaint();

}

// Fonction pour definir aléatoirement ou non dans la map les coordonées de fin
std::pair<int,int> render_area::define_fin(int pos_x = 0,int pos_y = 0,bool accept_pos = false)
{
    // reset de la fin dans le graphe
    mon_graphe.reset_fin();

    // init des coords du debut et de la fin dans la map
    int fin_x;
    int fin_y;
    if (accept_pos)
    {
        fin_x = rand() % (mon_graphe.dim_x-1);
        fin_y = rand() % (mon_graphe.dim_y-1);

    }else
    {
        fin_x = pos_x;
        fin_y = pos_y;
    }
    std::pair<int,int> fin (fin_x,fin_y);

    // config de la case de fin dans la map
    bool check = false;
    while (!check)
    {
        if (mon_graphe.getsetliste_case()[fin].acces && mon_graphe.getsetliste_case()[fin].getsetitem() != 1)
        {
            mon_graphe.getsetliste_case()[fin].getsetitem() = 2;
            check = true;
        }else
        {
            if (std::get<0>(fin) < mon_graphe.dim_x-1){std::get<0>(fin) += 1;}
            else if (std::get<1>(fin) < mon_graphe.dim_y-1){std::get<1>(fin) += 1;}
            else
            {
                std::get<0>(fin) = 0;
                std::get<1>(fin) = 0;
            }
        }
    }
    //repaint();
    return fin;
}

// Fonction pour changer l'etat de parcours (permet de savoir si on dessine un parcours ou non)
void render_area::change_parcours_state()
{
    parcours_state=!parcours_state;
    std::pair<int,int> debut;
    std::pair<int,int> fin;
    for (auto val : mon_graphe.getsetliste_case())
    {
        auto coord = std::get<0>(val);
        if (mon_graphe.getsetliste_case()[coord].getsetitem() == 1)
        {
            debut = coord;
        }else if (mon_graphe.getsetliste_case()[coord].getsetitem() == 2)
        {
            fin = coord;
        }
    }

    // Appel des algo de parcours
    dijkstra(mon_graphe,debut.first,debut.second,fin.first,fin.second);
}

// Fonction pour changer l'etat de grid_state (permet de savoir si on dessine on non la grille)
void render_area::change_grid_state()
{
    grid_state=!grid_state;
    //repaint();
}

// Fonction pour gérer le clic souris
void render_area::mousePressEvent(QMouseEvent *event)
{
    //when a click occurs, we store the current mouse position
    x_old=event->x();
    y_old=event->y();

    if (!mon_graphe.Yugo->fin)
    {
        int col_ind = x_old / (this->width()/mon_graphe.dim_x);
        int ligne_ind = y_old / (this->height()/mon_graphe.dim_y);

        std::pair<int,int> coord_fin (col_ind,ligne_ind);

        if (mon_graphe.getsetliste_case()[coord_fin].acces)
        {
            // On realise le parcours de dijkstra entre la position de Yugo et là on l'on clic
            int ret = dijkstra(mon_graphe,std::get<0>(mon_graphe.Yugo->getsetpos()),std::get<1>(mon_graphe.Yugo->getsetpos()),coord_fin.first,coord_fin.second);

            // on redefinit la position de Yugo si on a pas changer de map
            if (ret != 2)
            {
                mon_graphe.Yugo->deplacer() = std::make_pair(col_ind,ligne_ind);
            }

            if (ret ==1)
            {
                std::cout<<"Game end"<<std::endl;
            }
        }
    }
    //repaint();
}

// Fonction de lancement du jeu
void render_area::game_start(int pos_x = 0, int pos_y = 0,bool restart = false,int type = 2)
{
    if (!restart)
    {

        std::cout<<"Game start"<<std::endl;
        change_grid_state();
        ui->miam->setValue(100);
        ui->vie->setValue(100);
        mon_graphe.generate_perso(type);
    }
    generate_wall();
    generate_item();
    generate_tp();
    define_debut(pos_x,pos_y,restart);

    // recuperation des coord du debut et de fin
    std::pair<int,int> debut;
    for (auto val : mon_graphe.getsetliste_case())
    {
        auto coord = std::get<0>(val);
        if (mon_graphe.getsetliste_case()[coord].getsetitem() == 1)
        {
            debut = coord;
        }
    }

    mon_graphe.Yugo->deplacer() = debut;

    repaint();
}

// Fonction pour check si on a perdu
void render_area::check_fin()
{
    if (mon_graphe.Yugo->getsetmiam() <= 0 || mon_graphe.Yugo->getsetvie() <= 0)
    {
        mon_graphe.Yugo->fin = true;
        change_grid_state();
    }
}

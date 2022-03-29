#pragma once

#ifndef RENDER_AREA_HPP
#define RENDER_AREA_HPP

#include <QWidget>
#include "ui_mainwindow.h"
#include "graphe.hpp"

//Forward declaration of QPixmap
class QPixmap;

/** Declaration of render_area class */
class render_area : public QWidget
{
    Q_OBJECT
    graphe& mon_graphe;
    Ui::MainWindow *ui;

public:
    render_area(graphe& mon_graphe,Ui::MainWindow *ui,QWidget *parent = nullptr);
    ~render_area();

    /** Inverse the state of the draw_grid variable
        Draw or not the grid */
    void change_grid_state();

    /** Inverse the state of the parcours variable
        Draw the grid with the parcours*/
    void change_parcours_state();

    /** Draw the wall on the grid*/
    void generate_wall();

    /** Draw the item on the grid*/
    void generate_item();

    void generate_tp();

    std::pair<int,int> define_debut(int pos_x,int pos_y,bool accept_pos);
    std::pair<int,int> define_fin(int pos_x,int pos_y,bool accept_pos);

    void game_start(int pos_x, int pos_y,bool restart,int type);
    void check_fin();

protected:
	/** The actual painting function */
     void paintEvent(QPaintEvent *event);
     /** Function called when the mouse is pressed */
     void mousePressEvent(QMouseEvent *event);

     /** Function called when we want to make parcours */
     int parcourslargeur(graphe & g, int x_dep,int y_dep, int x_ar, int y_ar, bool chack, bool refill);
     /** Function called when we want to make dijkstra parcours */
     int dijkstra(graphe & g, int x_dep,int y_dep, int x_ar, int y_ar);



private:

	/** A QPixmap is an image */
    QPixmap *pixmap;
    /** A boolean indicating the grid should be drawn or not */
    bool grid_state;
    /** A boolean indicating the parcours should be drawn or not */
    bool parcours_state;
    /** Storage for the previous position of the mouse */
    int x_old,y_old;



};

#endif

#include "window.hpp"
#include "ui_mainwindow.h"

#include <iostream>


window::window(graphe& mon_graphe,QWidget *parent)
    :QMainWindow(parent),ui(new Ui::MainWindow),render(new render_area(mon_graphe,ui)),mon_graphe(mon_graphe)
{
    std::cout<<"Generate the Window"<<std::endl;

    //setup the graphical interface to the current widget
    ui->setupUi(this);

    //Attach the render_area window to the widget
    ui->layout_scene->addWidget(render);

    //connect signals
    connect(ui->quit,SIGNAL(clicked()),this,SLOT(action_quit()));
    connect(ui->Lancer,SIGNAL(clicked()),this,SLOT(action_lancer()));

    //configure default option for ui
    ui->miam->setVisible(0);
    ui->vie->setVisible(0);
    ui->label->setVisible(0);
    ui->label_2->setVisible(0);

    QAction *guerrier = new QAction();
    QAction *aventurier = new QAction();
    QAction *sorcier = new QAction();
    aventurier->setText("Adventurer");
    guerrier->setText("Warrior");
    sorcier->setText("Sorcerer");


    QMenu *menu = new QMenu();
    menu->addAction(guerrier);
    menu->addAction(aventurier);
    menu->addAction(sorcier);

    ui->character->setMenu(menu);

    connect(guerrier, SIGNAL(triggered()), this, SLOT(action_guerrier()));
    connect(aventurier, SIGNAL(triggered()), this, SLOT(action_aventurier()));
    connect(sorcier, SIGNAL(triggered()), this, SLOT(action_sorcier()));
}


window::~window()
{
    std::cout<<"Destructor of Window is called"<<std::endl;
}

void window::action_quit()
{
    close();
}

void window::action_lancer()
{
    render->game_start(0,0,false,2);
    ui->label_character->setText("You are an Adventurer");
}

void window::action_guerrier()
{
    //std::cout<<"action guerrier"<<std::endl;
    render->game_start(0,0,false,1);
    ui->label_character->setText("You are a Warrior");

    ui->Lancer->hide();
    ui->character->hide();
    ui->label->show();
    ui->label_2->show();
    ui->vie->show();
    ui->miam->show();
}

void window::action_aventurier()
{
    //std::cout<<"action aventurier"<<std::endl;
    render->game_start(0,0,false,2);
    ui->label_character->setText("You are an Adventurer");

    ui->Lancer->hide();
    ui->character->hide();
    ui->label->show();
    ui->label_2->show();
    ui->vie->show();
    ui->miam->show();
}

void window::action_sorcier()
{
    //std::cout<<"action sorcerer"<<std::endl;
    render->game_start(0,0,false,3);
    ui->label_character->setText("You are a Sorcerer");

    ui->Lancer->hide();
    ui->character->hide();
    ui->label->show();
    ui->label_2->show();
    ui->vie->show();
    ui->miam->show();
}


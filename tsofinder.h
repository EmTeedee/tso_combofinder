/*
 *  This file is part of TSO Combofinder.
 *
 *  Copyright 2010-2014 by malkolm <tobid@gmx.ch>
 *  Copyright 2016 by EmTeedee <emteedee@web.de>
 *
 *  TSO Combofinder is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  TSO Combofinder is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with TSO Combofinder.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TSOFINDER_H
#define TSOFINDER_H

#include <QMainWindow>
#include "QMessageBox"
#include "QColor"
#include "QPainter"
#include <QColormap>
#include <QPoint>
#include <QSettings>
#include <QElapsedTimer>
#include <stdlib.h>
#include <stdio.h>
#include <QColorDialog>
#include <QFile>
#include <QInputDialog>

namespace Ui {
    class TSOFinder;
}

class TSOFinder : public QMainWindow
{
    Q_OBJECT

public:
    // DEFINDE STRUCTURES FOR OFTEN USED DATA_STORAGE_OBJECTS
    struct rgb_value {
        int min;
        int max;
    };

    struct pixel_pos {
        int x_pos;
        int y_pos;
    };

    struct pixel_data {
        rgb_value r;
        rgb_value g;
        rgb_value b;
        pixel_pos pos;
        bool negating;

        pixel_data():negating(false){}
    };

    struct player_area_data {
        bool found;
        pixel_pos min_pos;
        pixel_pos max_pos;

        player_area_data():found(false){}
    };

    struct item_shape {
        int pixelcount;
        pixel_data* pixel;
        int needed; //NEEDED DISCOVERYS FOR SUCCESS
    };

    struct item {
        const char* caption;
        int shapecount;
        item_shape* shape;
        QString item_type;
        QColor color;
        QColor alt_color;
        Qt::PenStyle penstyle;

        item():item_type("standard"){}
    };

    struct event {
        QString caption;
        int id;
        int itemcount;
    };

    struct configuration {
        int vid;

        QPoint position_small;
        QSize size;
        bool ontop;

        QColor items_standard_color;
        QColor items_event_color;
        int waittime;
        bool show_searchtimer;
        bool useitems_basic;
        bool useitems_adventure;
        int use_event;
        QVector<QString> bg_caption;
        int use_bg;

        bool jump_position;
        bool stay_big;
    };

    // minumum number of pixels between found collectibles
    const int SEPARATION_MIN = 5;

    // standard acceptable r/g/b/ value difference
    const int STANDARD_EPSILON = 5;

    // VERSION PARAMETERS
    const QString CURRENT_VERSION = "2016-06-17";
    const int CONFIG_VERSION = 160614;

    //PIXELFINDER_RELATED OBJECTS/VARIABLES
    item* items;
    int itemcount;
    event* events;
    int eventcount;
    int*** pixels;
    pixel_pos player_area_size;
    QVector<QVector<int> > discovery;

    //BUILDING_SITE_RELATED OBJECTS/VARIABLES
    bool bg_exist;
    int bg_count;
    QImage BGImage,divImage;
    QPainter paint_dev2;
    QRgb bg_imgrgb,divrgb;
    int count_ok,count_ok_min,count_brigth,count_brigth_min,count_double;

    //COMMON USED OBJECTS/VARIABLES

    QMessageBox msgbox;
    QElapsedTimer timer;
    QSettings* configfile;
    configuration config;
    FILE* results;

    QPixmap originalPixmap;
    QImage originalImage;
    QPainter paint_dev;
    QPen pen_dev;
    QRgb imgrgb;

    int totaldiscoverys;
    int d_width,d_heigth;

    bool debug;
    bool current_ontop_windowsetting;
    bool current_small_window;
    bool options_visible;

    explicit TSOFinder(int argc,char* argv[],QWidget *parent = 0);
    ~TSOFinder();

    //DECLARATION OF IMPLEMENTED FUNCTIONS
    int get_method();
    int get_bgcount();
    void set_gui(bool small, bool is_debug);
    void setactive(bool active);
    void activate_buttons(bool activate);
    void log_discoverys();
    void validate_discoverys(player_area_data player_area);
    void draw_valids();
    void draw_discovery(QPoint coords, QColor color, Qt::PenStyle style);
    void init_items();
    void init_events();
    void load_settings();
    void load_background();
    void check_version();
    void init_ui_options();
    void save_settings();
    void set_windowparameters(int value);
    void set_optionwindow(bool visible);
    bool is_green(int r, int g, int b);
    bool is_brigth(int g);

    pixel_data getPixel(int r, int g, int b, int x_pos, int y_pos);
    pixel_data getPixel(int r, int g, int b, int epsilon, int x_pos, int y_pos);
    pixel_data getPixel(int r, int epsilon_r, int g, int epsilon_g, int b, int epsilon_b, int x_pos, int y_pos);

protected:
    void resizeEvent(QResizeEvent* event);


private slots:
    void on_takeButton_clicked();

    void on_resizeButton_clicked();

    void on_debugloadButton_clicked();

    void on_findButton_clicked();

    void on_debugsaveButton_clicked();

    void on_methodtab_currentChanged(int index);

    void on_savebgButton_clicked();

    void on_showbgButton_clicked();

    void on_ontopcheckbox_stateChanged(int is_checked);

    void on_optionButton_clicked();

    void on_waittime_slider_sliderMoved(int position);

    void on_waittime_lineEdit_textEdited(const QString &arg1);

    void on_searchtimercheckBox_stateChanged(int arg1);

    void on_itembasiccheckBox_stateChanged(int arg1);

    void on_itemadventurecheckBox_stateChanged(int arg1);

//    void on_itemhalloween13checkBox_stateChanged(int arg1);

//    void on_itemchristmas13checkBox_stateChanged(int arg1);

    void on_jumpcheckBox_stateChanged(int arg1);

    void on_staybigcheckBox_stateChanged(int arg1);

//    void on_itemeastern14checkBox_stateChanged(int arg1);

    void on_colorstandardpushButton_clicked();

    void on_coloreventpushButton_clicked();

    void on_itemeventcomboBox_activated(int index);

    void on_bgcomboBox_activated(int index);

    void on_bg_editpushButton_clicked();

    void on_bgdeletepushButton_clicked();

private:
    Ui::TSOFinder *ui;
};

#endif // TSOFINDER_H

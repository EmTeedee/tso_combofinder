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

#include "tsofinder.h"
#include "ui_tsofinder.h"

//THIS FILE CONTAINS THE CODE FOR INTERACTING WITH THE SETTINGS-MENU

void TSOFinder::set_optionwindow(bool visible)
{
    if(visible) {
        ui->screenlabel->hide();
        ui->optionwidget->show();
        options_visible = true;
    } else {
        ui->screenlabel->show();
        ui->optionwidget->hide();
        options_visible = false;
    }
}

void TSOFinder::on_optionButton_clicked()
{
    if(!options_visible) {
        set_optionwindow(true);
    } else {
        set_optionwindow(false);
    }
}

void TSOFinder::on_waittime_slider_sliderMoved(int position)
{
    ui->waittime_lineEdit->setText(QString("%1").arg(position));
    config.waittime = position;
}

void TSOFinder::on_waittime_lineEdit_textEdited(const QString &arg1)
{
    if(arg1.toInt() != 0) {
        if(arg1.toInt()>2000) {
            ui->waittime_slider->setSliderPosition(2000);
            ui->waittime_lineEdit->setText(QString("%1").arg(2000));
            config.waittime = 2000;
        }
        ui->waittime_slider->setSliderPosition(arg1.toInt());
        config.waittime = arg1.toInt();
    }
}

void TSOFinder::on_searchtimercheckBox_stateChanged(int is_checked)
{
    if(is_checked) {
        config.show_searchtimer = true;
        ui->fixlabel2->show();
        ui->timelabel->show();
    }
    else {
        config.show_searchtimer = false;
        ui->fixlabel2->hide();
        ui->timelabel->hide();
    }
}

/*void TSOFinder::on_coloralternativecheckBox_stateChanged(int is_checked)
{
    if(is_checked) config.use_alternate_scheme = true;
    else config.use_alternate_scheme = false;
}*/


void TSOFinder::on_colorstandardpushButton_clicked()
{
    config.items_standard_color = QColorDialog::getColor(config.items_standard_color,this);
    ui->colorstandardpushButton->setStyleSheet(QString("border-radius: 10px; background-color: rgb(%1, %2, %3);").arg(config.items_standard_color.red()).arg(config.items_standard_color.green()).arg(config.items_standard_color.blue()));
}

void TSOFinder::on_coloreventpushButton_clicked()
{
    config.items_event_color = QColorDialog::getColor(config.items_event_color,this);
    ui->coloreventpushButton->setStyleSheet(QString("border-radius: 10px; background-color: rgb(%1, %2, %3);").arg(config.items_event_color.red()).arg(config.items_event_color.green()).arg(config.items_event_color.blue()));
}


void TSOFinder::on_itembasiccheckBox_stateChanged(int is_checked)
{
    if(is_checked) config.useitems_basic = true;
    else config.useitems_basic = false;
    init_items();
}

void TSOFinder::on_itemadventurecheckBox_stateChanged(int is_checked)
{
    if(is_checked) config.useitems_adventure = true;
    else config.useitems_adventure = false;
    init_items();
}

void TSOFinder::on_itemeventcomboBox_activated(int index)
{
    config.use_event = index;
    init_items();
}


void TSOFinder::on_jumpcheckBox_stateChanged(int is_checked)
{
    if(is_checked) config.jump_position = true;
    else config.jump_position = false;
}

void TSOFinder::on_staybigcheckBox_stateChanged(int is_checked)
{
    if(is_checked) {
        config.stay_big = true;
        ui->waittimegroupbox->setDisabled(true);
    }
    else {
        config.stay_big = false;
        ui->waittimegroupbox->setEnabled(true);
    }
}


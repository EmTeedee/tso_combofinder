#include "tsofinder.h"
#include "ui_tsofinder.h"
#include "QDesktopWidget"

TSOFinder::TSOFinder(int argc,char* argv[],QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TSOFinder)
{
    // VERSION PARAMETERS
    currentversion = "2016-06-14";
    cvid = 160614;

    //SET FLAGS FOR NON-DEFAULT WINDOW (NO MINIMIZE & MAXIMIZE BUTTON) AND TITLE
    Qt::WindowFlags flags = Qt::CustomizeWindowHint|Qt::WindowTitleHint|Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    QColorDialog::setCustomColor(0,QColor(255,0,0).rgb());
    QColorDialog::setCustomColor(1,QColor(220,170,40).rgb());
    current_ontop_windowsetting = false;
    current_small_window = true;
    options_visible = false;
    ui->setupUi(this);
    this->setWindowTitle(QString("TSO Combo-Finder build "+currentversion));

    //BEGIN DEBUG
    debug=false;
    if(argc>1) if(strcmp(argv[1],"-d")==0) debug=true;
    //END DEBUG

    //INIT VARS
    d_width = 0;
    d_heigth = 0;
    avatar_width = 125;
    avatar_heigth = 280;
    bg_exist = false;
    bg_count = get_bgcount(); //CHECK CURRENT DIRECTORY FOR VALID BACKGROUND FILES
    load_settings(); //LOAD USER-DEFINED VARIABLES
    check_version(); //CHECK FOR OUT OF DATE VERSION
    init_events(); //LOOK AT events.cpp FOR LIST OF EVENTS
    init_ui_options(); //SET OPTIONS IN THE SETTINGS TAB
    init_items(); //LOOK AT items.cpp FOR LIST OF ITEMS

    //START RESIZED SMALL
    this->setMinimumSize(100,55);
    set_gui(true,debug);
}

TSOFinder::~TSOFinder()
{
    //DECONSTRUCTUR - SAVE SETTING AND KILL CLASS
    save_settings();
    delete ui;
}


void TSOFinder::on_takeButton_clicked()
{
    //TAKE SCREENSHOT AND ALLOCATE PROPPER PIXELARRAY
    if(current_small_window) config.position_small = this->pos();
    if(!config.jump_position) config.position_small = this->pos();
    set_optionwindow(false);
    if(!config.stay_big) {
        set_gui(true,debug);
        timer.start();
        while(timer.elapsed()< config.waittime);                  // WAIT 600ms FOR PROPER RESIZING ON SLOWER MACHINES
    }
    originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    if(originalPixmap.width() != d_width || originalPixmap.height() != d_heigth) { //ONLY REALLOCATE IF SCREENSIZE HAS CHANGED (MULTIMONITOR) -> DELETE MISSING!
        d_width = originalPixmap.width();
        d_heigth = originalPixmap.height();
        pixels = new int**[originalPixmap.width()];
        for(i=0;i<originalPixmap.width();i++) {
            pixels[i] = new int*[originalPixmap.height()];
            for(k=0;k<originalPixmap.height();k++) {
                pixels[i][k] = new int[3];
                for(j=0;j<3;j++) {
                    pixels[i][k][j] = 0;
                }
            }
        }
    }
    //RESIZE TO FULL AND SHOW SCREENSHOT
    ui->screenlabel->setPixmap(originalPixmap.scaled(ui->screenlabel->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    set_gui(false,debug);
}

void TSOFinder::resizeEvent(QResizeEvent *event)
{
    if(this && !current_small_window)
    {
        ui->screenlabel->setPixmap(originalPixmap.scaled(ui->screenlabel->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        config.size = this->size();
    }
}

void TSOFinder::on_resizeButton_clicked()
{
    //RESIZE TO SMALL
    if(!config.jump_position) config.position_small = this->pos();
    set_gui(true,debug);
}

void TSOFinder::on_debugsaveButton_clicked()
{
    //DEBUGMODE: SAVE SCREENSHOT
    originalPixmap.save("screen.png",0,100);
}

int TSOFinder::get_bgcount()
{
    int count = 0;
    for(i=0;i<10;i++) {
        if(QFile::exists(QString("background%1.png").arg(i))) {
            if(i!=count) {
                QFile::rename(QString("background%1.png").arg(i),QString("background%1.png").arg(count));
            }
            count++;
        }
    }

    return count;
}

void TSOFinder::draw_discovery(QPoint coords, QColor color, Qt::PenStyle style)
{
    //ON DISCOVERY PAINT CIRCLE OF GIVEN COLOR AND STYLE
    //if(!(coords.x() <= avatar_width && coords.y() <= avatar_heigth)) { //Do not find something on your Avatar :)
        pen_dev.setColor(color);
        pen_dev.setStyle(style);
        paint_dev.setPen(pen_dev);
        paint_dev.drawEllipse(coords,20,20);
    //}
}

bool TSOFinder::is_green(int red, int green, int blue)
{
    //CHECK IF RGB IS GREENISH (natural greens)
    if(blue<50 && green>red && green>blue && green>10 && green<140) return true;
    else return false;
}

bool TSOFinder::is_brigth(int green)
{
    //CHECK IF RGB IS A BRIGHTER GREEN
    if(green>60) return true;
    else return false;
}

void TSOFinder::log_discoverys() // WRITE ALL DISCOVERYS IN LOGFILE
{
    fprintf(results,"Found %i items in screenshot\n",discovery.size());
    for(int i_dis = 0; i_dis<discovery.size();i_dis++) {
        fprintf(results,"%d: %s (Shape %i) found at %i %i\n",i_dis,items[discovery[i_dis][2]].caption,discovery[i_dis][3],discovery[i_dis][0],discovery[i_dis][1]);
    }
}

void TSOFinder::validate_discoverys() // CHECK FOR SEPARATION OF DISCOVERYS AND DISCARD BUNCHED FALSE POSITIVES e.g. IN AVATAR
{
    for(int i_dis = 0; i_dis<discovery.size();i_dis++) {
        //if(discovery[i_dis][4] != 0) {
            for(int j_dis = 0; j_dis<discovery.size();j_dis++) {
                if((i_dis != j_dis) && ((abs(discovery[i_dis][0]-discovery[j_dis][0])+abs(discovery[i_dis][1]-discovery[j_dis][1])<5))) {
                    discovery[i_dis][4] = 0;
                    discovery[j_dis][4] = 0;
                    fprintf(results,"Discarded discoverys %d and %d because of too low separation\n",i_dis,j_dis);
                }
            }
        //}
    }
}

void TSOFinder::draw_valids() // DRAW ALL VALID DISCOVERYS
{
    for(int i_dis=0; i_dis<discovery.size();i_dis++) {
        if(discovery[i_dis][4] > 0) {
            totaldiscoverys++;
            if(items[discovery[i_dis][2]].item_type.compare(QString("standard"))==0 ) draw_discovery(QPoint(discovery[i_dis][0],discovery[i_dis][1]),config.items_standard_color,items[discovery[i_dis][2]].penstyle);
            else draw_discovery(QPoint(discovery[i_dis][0],discovery[i_dis][1]),config.items_event_color,items[discovery[i_dis][2]].penstyle);
        }
    }
}

void TSOFinder::on_findButton_clicked()
{
    set_optionwindow(false);
    setactive(false);
                                    //C O M M O N
    originalImage = originalPixmap.toImage();
    results=fopen("results.dat","w");
    totaldiscoverys = 0;
    discovery.clear();
    timer.start();  //START TIMER

    //INIT PAINT-DEVICE
    paint_dev.begin(&originalImage);
    pen_dev.setWidth(5);
    paint_dev.setPen(pen_dev);

    if(get_method()==0) {           //M E T H O D  1:  P I X E L  F O O T P R I N T S

        //RESET PIXELARRAY
        for(x=0;x<originalPixmap.width();x++) {
            for(y=0;y<originalPixmap.height();y++) {
                for(j=0;j<3;j++) {
                    pixels[x][y][j] = 0;
                }
            }
        }

        //SCAN SCREENSHOT AND FILL PIXELARRAY
        //ACTUALLY ITS FASTER TO WRITE ALL PIXELS INTO AN INT-ARRAY FIRST THEN DIRECTLY ACCESS THE QImage-CLASS PIXELVALUE IN FURTHER CALCULATIONS. MIGHT BE COMPILER DEPENDEND.
        for(x=0;x<originalPixmap.width();x++) {
            for(y=0;y<originalPixmap.height();y++) {
                imgrgb = originalImage.pixel(x,y);
                r = qRed(imgrgb);
                g = qGreen(imgrgb);
                b = qBlue(imgrgb);
                pixels[x][y][0] = r;
                pixels[x][y][1] = g;
                pixels[x][y][2] = b;
            }
        }

        //MAIN SEARCH ALGORITHM
        for(x=borderbox;x<originalPixmap.width()-borderbox;x++) {       //SCAN IMAGE (X)
            for(y=borderbox;y<originalPixmap.height()-borderbox;y++) {  //SCAN IMAGE (Y)
                for(i=0;i<itemcount;i++) {                          //SCAN FOR ALL DEFINED ITEMS (items.cpp)
                    item_found = false;
                    for(j=0;j<items[i].shapecount;j++) {            //SCAN FOR ALL DEFINED SHAPES
                        if(!item_found) {
                            shape_disc = 0;
                            shape_negating = 0;
                            for(k=0;k<items[i].shape[j].pixelcount;k++) {                //SCAN ALL DEFINED PIXELS OF ITEMS
                                if(items[i].shape[j].pixelcount - items[i].shape[j].needed + shape_disc >= k) { //SPEED UP IF DISCOVERY IMPOSSIBLE
                                    if( pixels[x+items[i].shape[j].pixel[k].pos.x_pos][y+items[i].shape[j].pixel[k].pos.y_pos][0]>items[i].shape[j].pixel[k].r.min &&
                                        pixels[x+items[i].shape[j].pixel[k].pos.x_pos][y+items[i].shape[j].pixel[k].pos.y_pos][0]<items[i].shape[j].pixel[k].r.max &&
                                        pixels[x+items[i].shape[j].pixel[k].pos.x_pos][y+items[i].shape[j].pixel[k].pos.y_pos][1]>items[i].shape[j].pixel[k].g.min &&
                                        pixels[x+items[i].shape[j].pixel[k].pos.x_pos][y+items[i].shape[j].pixel[k].pos.y_pos][1]<items[i].shape[j].pixel[k].g.max &&
                                        pixels[x+items[i].shape[j].pixel[k].pos.x_pos][y+items[i].shape[j].pixel[k].pos.y_pos][2]>items[i].shape[j].pixel[k].b.min &&
                                        pixels[x+items[i].shape[j].pixel[k].pos.x_pos][y+items[i].shape[j].pixel[k].pos.y_pos][2]<items[i].shape[j].pixel[k].b.max) {
                                        if(!items[i].shape[j].pixel[k].negating) shape_disc++;
                                        else shape_negating++;
                                    }
                                }
                            }
                            if(shape_disc >= items[i].shape[j].needed && shape_negating==0) {             //IF ITEM IS DISCOVERED MARK IT ON IMAGE AND WRITE LOGFILE. REWORK: WRITE DISCOVERYS INTO LIST AND DRAW AFTER THE FULL SCAN. FALSE POSITIVES IN AVATAR CAN THUS BE FILTERED OUT.
                                item_found = true;
                                //fprintf(results,"Possible %s (Shape %i) found at %i %i\n",items[i].caption,j,x,y);
                                discovery.append(QVector<int>() << x << y << i << j << 1); // 0:X 1:Y 2:Item 3:Shape 4:Valid
                                //totaldiscoverys++;
                                //if(items[i].item_type.compare(QString("standard"))==0) { draw_discovery(QPoint(x,y),config.items_standard_color,items[i].penstyle); }
                                //else {draw_discovery(QPoint(x,y),config.items_event_color,items[i].penstyle);};
                            }
                        }
                    }
                }
            }
        }
        log_discoverys();
        validate_discoverys();
        draw_valids();

    } else if(get_method()==1) {                // M E T H O D  2:  B U I L D I N G S I T E S
        //CHECK FOR CORRECT BACKGROUND IMAGE SIZE
        if(bg_exist) {
            if(BGImage.width()!=originalImage.width() || BGImage.height() != originalImage.height()) {
                msgbox.setText(QString("Background file has wrong resolution!"));
                msgbox.exec();
                bg_exist = false;
            }
        } else {
            msgbox.setText(QString("No valid background file"));
            msgbox.exec();
        }
        if(bg_exist) {

            //INIT VARIABLES
            divImage = originalPixmap.toImage(); //CREATE DIVIMAGE FROM CURRENT PIXMAP

            int fieldwidth = 10;    //SIZE OF SEARCHAREA FOR BUILDING SITES
            count_ok_min=70;        //MIN# OF GREEN PIXELS IN FIELDWIDTH*FIELDWIDTH AREA
            count_brigth_min=10;    //MIN# OF BRIGHT GREEN PIXELS IN AREA

            //FILL DIVIMAGE WITH VALUES OF DIFFERENCE
            for(x=0;x<divImage.width();x++) {
                for(y=0;y<divImage.height();y++) {
                    imgrgb = originalImage.pixel(x,y);
                    bg_imgrgb = BGImage.pixel(x,y);
                    r = abs(qRed(bg_imgrgb)-qRed(imgrgb));
                    g = abs(qGreen(bg_imgrgb)-qGreen(imgrgb));
                    b = abs(qBlue(bg_imgrgb)-qBlue(imgrgb));

                    if(b>250) divrgb=qRgb(0,0,0);   //IF PIXEL IS BLUE GET RID OF IT (NEEDED FOR FURTHER DISCRIMINATION)
                    else divrgb=qRgb(r,g,b);

                    divImage.setPixel(x,y,divrgb);
                }
            }

            //INIT ADDITIONAL PAINT DEVICE
            paint_dev2.begin(&divImage);
            paint_dev2.setPen(pen_dev);

            //MAIN SEARCH ALGORITHM
            for(x=0;x<divImage.width()-2*fieldwidth;x++) {
                for(y=0;y<divImage.height()-2*fieldwidth;y++) {
                    count_ok=0;
                    count_brigth=0;
                    count_double=0;
                    divrgb=divImage.pixel(x,y);
                    if(is_green(qRed(divrgb),qGreen(divrgb),qBlue(divrgb))) {   //IF PIXEL IS GREEN SCAN FIELDWIDTH*FIELDWIDTH FOR BUILDING SITE
                        for(i=0;i<fieldwidth;i++){
                            for(k=0;k<fieldwidth;k++){
                                divrgb=divImage.pixel(x+i,y+k);
                                if(is_green(qRed(divrgb),qGreen(divrgb),qBlue(divrgb))) {   //COUNT GREEN PIXELS
                                    count_ok++;
                                    if(is_brigth(qGreen(divrgb))) count_brigth++;           //COUNT BRIGHT GREEN PIXELS
                                }
                                if(qBlue(divrgb)==255) count_double++;                      //CHECK IF ALLREADY FOUND SOMETHING HERE
                            }
                        }
                    }
                    //IF AREA LOOKS LIKE BUILDING SITE MARK ON BOTH IMAGES
                    if(count_ok>count_ok_min && count_brigth>count_brigth_min && count_double == 0) {
                        paint_dev2.fillRect(x,y,2*fieldwidth,2*fieldwidth,QColor(0,0,255));
                        draw_discovery(QPoint(x+(fieldwidth/2),y+(fieldwidth/2)),config.items_standard_color,Qt::SolidLine);
                        fprintf(results,"Found something near %i %i\n",x+(fieldwidth/2),y+(fieldwidth/2));
                    }
                }
            }

            paint_dev2.end();

            if(debug) {
                //SAVE IMAGES IF IN DEBUG MODE
                divImage.save("div.png",0,100);
            }
        }
    }

    //SHOW RESULTING IMAGE AND COUNT
    paint_dev.end();
    ui->screenlabel->setPixmap(QPixmap::fromImage(originalImage).scaled(ui->screenlabel->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->countlabel->setText(QString("%1").arg(totaldiscoverys));
    ui->timelabel->setText(QString("%1").arg(timer.elapsed()));   //SHOW CALCULATION TIME IF IN DEBUG MODE. HELPFULL FOR CODE-OPTIMIZATION AND COMPILER COMPARISON.
    fclose(results);
    setactive(true);

}

void TSOFinder::on_debugloadButton_clicked()
{
    //DEBUGMODE: LOAD SCREEN
    originalPixmap.load("screen.png");
    ui->screenlabel->setPixmap(originalPixmap.scaled(ui->screenlabel->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

void TSOFinder::on_savebgButton_clicked()
{
    //SAVE & LOAD TAKEN IMAGE AS BACKGROUND IMAGE
    bool ok = false;
    QString default_text;
    if(config.bg_caption.size() > config.use_bg) default_text = config.bg_caption[config.use_bg];
    else default_text = "Main Island S5";
    QString caption = QInputDialog::getText(this,QString("Save in Slot %1").arg(config.use_bg + 1),"Enter caption:",QLineEdit::Normal,default_text,&ok);

    if(ok) {
        if(config.use_bg == bg_count) {
            config.bg_caption.append(caption);
            bg_count++;
            ui->bgcomboBox->insertItem(config.use_bg,QString("%1: %2").arg(config.use_bg+1).arg(config.bg_caption[config.use_bg]));
            ui->bgcomboBox->setCurrentIndex(config.use_bg);
        } else if(config.use_bg < bg_count) {
            if(QMessageBox::question(this,"Confirm overwrite",QString("Do you really want to replace the bachground in Slot %1?").arg(config.use_bg+1),QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel) == QMessageBox::Ok) {
                config.bg_caption[config.use_bg] = caption;
                ui->bgcomboBox->setItemText(config.use_bg,QString("%1: %2").arg(config.use_bg+1).arg(caption));
            }
        }
        originalPixmap.save(QString("background%1.png").arg(config.use_bg),0,100);
        load_background();
    }
}

void TSOFinder::on_showbgButton_clicked()
{
    //SHOW BACKGROUND IF EXISTING
    if(bg_exist) {
        ui->screenlabel->setPixmap(QPixmap::fromImage(BGImage).scaled(ui->screenlabel->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    } else {
        msgbox.setText(QString("No valid background file."));
        msgbox.exec();
    }
}

void TSOFinder::on_bg_editpushButton_clicked()
{
    if(config.use_bg >= bg_count) return;
    bool ok = false;
    QString caption = QInputDialog::getText(this,QString("Edit Slot %1").arg(config.use_bg + 1),"Enter new caption:",QLineEdit::Normal,config.bg_caption[config.use_bg],&ok);
    if(ok) {
        config.bg_caption[config.use_bg] = caption;
        ui->bgcomboBox->setItemText(config.use_bg,QString("%1: %2").arg(config.use_bg+1).arg(caption));
    }
}

void TSOFinder::on_bgdeletepushButton_clicked()
{
    if(config.use_bg >= bg_count) return;
    int ok = QMessageBox::question(this,"Delete Background",QString("Do you really want to delete the background file in Slot %1?").arg(config.use_bg+1),
                                        QMessageBox::Ok | QMessageBox::Cancel,
                                        QMessageBox::Cancel);
    if(ok == QMessageBox::Ok) {
        QFile::remove(QString("background%1.png").arg(config.use_bg));
        ui->bgcomboBox->removeItem(config.use_bg);
        bg_count--;
        for(int i_del = config.use_bg;i_del<bg_count;i_del++) {
            ui->bgcomboBox->setItemText(i_del,QString("%1: %2").arg(i_del).arg(config.bg_caption[i_del+1]));
            config.bg_caption[i_del] = config.bg_caption[i_del+1];
            QFile::rename(QString("background%1.png").arg(i_del+1),QString("background%1.png").arg(i_del));
        }
        load_background();
    }
}
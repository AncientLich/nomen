/*************************************************************************************************
 *  NomenGifSave library
 *  Copyright (C) Nobun, 2012
 *  License: LGPL 2.1
 * 
 *     This library is a component of Nomen project (http://nomeneditor.sourceforge.net)
 *     but can be freely used under the terms of LGPL 2.1 license
 * 
 * --------------------
 *
 *  Extra Credits to:
 *     Qt-gif plugin made by ecloud (http://gitorious.org/qt-gif-plugin)
 * 
 *     The "heart" function used in NomenGifSave for writing GIF file ( write(QIODevice * device) )
 *     derives from the code written in the "write" function of Qt-gif plugin made by ecloud
 *    
 *     The ecloud's code was a little modified here. The most important ones are:
 *            - Added support for transparency (including Graphic Control Extension blocks for images)
 *            - Added support for animated gif (also including "NETSCAPE" block)
 *            - Writing image(s) is a bit more flexible (every frame can have different dimensions. Logic screen is 
 *                               calculated separately, write calculate the effective pos for every frame 
 *                               in logic screen)
 *            - Global Palette is always setted as 1st image palette. This becouse global palette is not used in 
 *                        giflib (this becouse giflib always write local palette, so global palette will not be used)
 *                        for the same reason global background color is fixed to index0 (like ecloud code):
 *                            background color index is somewhat ignored
 * 
 *************************************************************************************************/

#ifndef NGIF_KEEP_TRANS
#define NGIF_KEEP_TRANS -1

#define NGIF_KEEP_TRANS_OR_0 -2
#define NGIF_KEEP_TRANS_OR_255 -3
#define NGIF_NO_TRANS -5

#include <QImage>
#include <QString>
#include <QByteArray>


struct NOMENGIF_DATA {
  QImage image;
  int x, y;
  unsigned short time;
  int transcol; // -1 = no_trans_col
};


class NomenGifSave {
    QList <NOMENGIF_DATA> data;
    int x0, y0, x1, y1;
    int default_col;
    QString comment;
  public:	
    NomenGifSave() { x0 = 5000; y0 = 5000; x1 = -5000; y1 = -5000; default_col = NGIF_KEEP_TRANS; comment="Created with NomenGifSave library"; }
    void addImage(QImage &src, int x, int y, unsigned short time, int transparent_color);
    void addImage(QImage &src, int x, int y, unsigned short time);
    void addImage(QImage &src, int x, int y);
    void addImage(QImage &src, unsigned short time);
    void addImage(QImage &src);
    bool write(QIODevice * device);
    bool write(QString &filename);
    bool write(const char * filename);
    bool write(QByteArray * array);
    void setDefaultTrans(int color_index);
    void setComment(QString &str);
    void setComment(const char * str);
    void clear();
};


#endif


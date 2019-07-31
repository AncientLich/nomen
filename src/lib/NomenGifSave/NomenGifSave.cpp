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

#include <QDebug>
#include <QVariant>
#include <gif_lib.h>
#include <string.h>		// memset
#include <QPainter>
#include <QFile>
#include <QBuffer>
#include "NomenGifSave.h"

extern int _GifError;

int _nomenGifSave_doOutput(GifFileType* gif, const GifByteType * data, int i)
{
	QIODevice* out = (QIODevice*)gif->UserData;
//	qDebug("given %d bytes to write; device is writeable? %d", i, out->isWritable());
	return out->write((const char*)data, i);
}



bool NomenGifSave::write(QIODevice * device) {
    if(data.count() > 0) {
	  //define global map
	  ColorMapObject global_map; //global map is somewhat unused in giflib logic. So global map will be the map of first image
	  QVector<QRgb> colorTable = data[0].image.colorTable();
      global_map.ColorCount = 256; //during "write" step, NomenGifSave will handle only 256-color images (eventually every conversion would be done during addImage step)
      global_map.BitsPerPixel = 8;
      GifColorType * colorValues = (GifColorType *)malloc (256 * sizeof(GifColorType));
      global_map.Colors = colorValues;
      for(int c=0; c < 256; ++c) 
      {
        colorValues[c].Red = qRed(colorTable[c]);
        colorValues[c].Green = qGreen(colorTable[c]);
        colorValues[c].Blue = qBlue(colorTable[c]);
      }
	  
	  //set Gif to Gif89a
	  EGifSetGifVersion("89a");
	  
	  //open Gif buffer
	  GifFileType * gif = EGifOpen(device, _nomenGifSave_doOutput);
      
      //screen description - now set background color to 1 for debugging reasons
	  if (EGifPutScreenDesc(gif, (x1-x0+1), (y1-y0+1), 256, 0, &global_map) == GIF_ERROR) {
		  qCritical("EGifPutScreenDesc returned error %d", GifLastError());
		  return false; 
	  }
		 	  
	  //put comment
	  if (!comment.isEmpty())
		   EGifPutComment(gif, comment.toUtf8().constData());
	    
	  //put NETSCAPE extension in case of animated GIF
	  if(data.count() >=2) {
	    unsigned char nsle[12] = "NETSCAPE2.0";
        unsigned char block[3];
        block[0] = 1; block[1] = 0xff; block[2] = 0xff;
        EGifPutExtensionFirst(gif, 0xFF, 11, nsle);
        EGifPutExtensionLast(gif, 0, 3, block);
      }
      
      //writing images
      for(int a=0; a<data.count(); a++) {
	    
	    //Graphic Control Extension (gce)
	    unsigned char gce[4] = {8,0,0,0};
	      //gce transcol
	    if(data[a].transcol >=0) {
		  gce[0] = 9; gce[3] = (unsigned char) data[a].transcol;
	    }
	      //gce time
	    {
		  unsigned short * val = (unsigned short *) &gce[1];
		  * val = data[a].time;    
	    }
	      //write gce
	    EGifPutExtension(gif, 0xf9, 4, gce);
	    
	    //Image Description (desc)
	    {
		   //color map
		   ColorMapObject cmap;
		   colorTable = data[a].image.colorTable(); //"colorTable" already defined when declaring global color map
           cmap.ColorCount = 256; //during "write" step, NomenGifSave will handle only 256-color images (eventually every conversion would be done during addImage step)
           cmap.BitsPerPixel = 8;
           GifColorType * colorValues2 = (GifColorType *)malloc (256 * sizeof(GifColorType));
           cmap.Colors = colorValues2;
           for(int c=0; c < 256; ++c) 
           {
               colorValues2[c].Red = qRed(colorTable[c]);
               colorValues2[c].Green = qGreen(colorTable[c]);
               colorValues2[c].Blue = qBlue(colorTable[c]);
           }
		   
           //xt, yt
           int xt = data[a].x - x0;
		   int yt = data[a].y - y0;
		   
		   EGifPutImageDesc(gif, xt, yt, data[a].image.width(), data[a].image.height(), 0, &cmap);
		   free(colorValues2);
	    }
	    
	    //write image
	    int lc = data[a].image.height();
	    int llen = data[a].image.width();
	    for (int l = 0; l < lc; ++l)
	    {
		  unsigned char * lline = data[a].image.scanLine(l);
		  EGifPutLine(gif, (GifPixelType*)lline, llen);
	    } 
	    
      } //end foreach image (for "a")  
	    
	  //end	    
	  free(colorValues);
	  EGifCloseFile(gif);
	  return true;
	}
	
   else return false;
}



void NomenGifSave::addImage(QImage &src, int x, int y, unsigned short time, int transparent_color) {
  //image conversion (if non-256 colors or palette with less than 256 colors)
  QImage img = src;
  if(img.format()!=QImage::Format_Indexed8) { //case1: non-256 colors
	 img = img.convertToFormat(QImage::Format_Indexed8);
	 /* if non-256 colors transparency setting will be ignored
	    this becouse convertToFormat will apply itself a color palette so it is a no-sense to define a trasparent color
	    in this way there will be a transparent color if only present into original image.
	    else no transparent color at all
	    BUT, if transparent_color is completely excluded (with NGIF_NO_TRANS) then transparent_color remain as NO_TRANS
	 */
	 if(transparent_color != NGIF_NO_TRANS) transparent_color = NGIF_KEEP_TRANS;
  }
  if(img.colorCount() < 256) { //case2: color table with less than 256-color. Can be happen also after case 1 so it is not an "else"
    int a = img.colorCount();
    img.setColorCount(256);
    for(int k = a-1; k < 256; k++) {
	  QRgb empty_color = qRgb(0,255,0);
	  img.setColor(k, empty_color);
    }
  }
  
  NOMENGIF_DATA tmpdata;
  
  //calculating trans color value for tmpdata
  if(transparent_color < 0 && transparent_color > NGIF_NO_TRANS) { 
	//flexible value: KEEP_TRANS and similar. Firstly check for actual transparent color (if any) than if not found set to -1, 0, or 255 depending of KEEP_TRANS setting
    //checks if there is an actual trans color into img. If it is so, img_has_trans will be true and val = transcol
    bool img_has_trans = false;
    int val = 0;
    for(val = 0; val < 256; val++) {
	  if( qAlpha(img.color(val)) == 0 ) {
	    img_has_trans = true; break;	  
      }
    }
    //now assign transcolor
    if(img_has_trans) tmpdata.transcol = val;
    else { // !img_has_trans
	         if(transparent_color == NGIF_KEEP_TRANS) tmpdata.transcol = -1;
	    else if(transparent_color == NGIF_KEEP_TRANS_OR_0) tmpdata.transcol = 0;
	    else if(transparent_color == NGIF_KEEP_TRANS_OR_255) tmpdata.transcol = 255;
	    else tmpdata.transcol = -1;
    } 
  }
  else if(transparent_color == NGIF_NO_TRANS) tmpdata.transcol = -1;
  else { //transparent_color >=0 (assigned a fixed value)
    tmpdata.transcol = transparent_color;
    if(tmpdata.transcol > 255) tmpdata.transcol = -1;
  }
  
  //assigning image to tmpdata
  tmpdata.image = img;
  
  //assigning time,x,y to tmpdata. 
  tmpdata.time = time;
  tmpdata.x = x;
  tmpdata.y = y;
  
  //Check if logic screen must be expanded (x0, y0, x1, y1)
  if(x < x0) x0 = x;
  if(y < y0) y0 = y;
  {
    int xt = x - 1 + img.width();
    int yt = y - 1 + img.height();
    
    if(xt > x1) x1 = xt;
    if(yt > y1) y1 = yt;
  }
  
  //finally add the image to data
  this->data.append(tmpdata);
}



bool NomenGifSave::write(QString &filename) {
  bool value;
  QFile infile(filename);
  if(!infile.open(QIODevice::WriteOnly)) return false;
  value = this->write(&infile);
  infile.close();
  return value;
}



bool NomenGifSave::write(const char * filename) {
  QString str(filename);
  return this->write(str);	
}



bool NomenGifSave::write(QByteArray * array) {
  QBuffer buffer(array);
  buffer.open(QIODevice::WriteOnly);
  bool value = this->write(&buffer);
  buffer.close();
  return value;
}



void NomenGifSave::addImage(QImage &src, int x, int y, unsigned short time) {
  this->addImage(src, x, y, time, default_col);	
}
    


void NomenGifSave::addImage(QImage &src, int x, int y) {
  this->addImage(src, x, y, 1, default_col);
}



void NomenGifSave::addImage(QImage &src, unsigned short time) {
  this->addImage(src, 0, 0, time, default_col);	
}



void NomenGifSave::addImage(QImage &src) {
  this->addImage(src, 0, 0, 1, default_col);	
}



void NomenGifSave::setDefaultTrans(int color_index) {
  default_col = color_index;	
}



void NomenGifSave::setComment(QString & str) {
  comment = str;	
}



void NomenGifSave::setComment(const char * str) {
  comment = QString(str);	
}



void NomenGifSave::clear() {
  data.clear();
  x0 = 5000; y0 = 5000; x1 = -5000; y1 = -5000;	
}



/******************************************************************************
 * MODULE     : qt_chooser_widget.cpp
 * DESCRIPTION: File chooser widget, QT version
 * COPYRIGHT  : (C) 2008  Massimiliano Gubinelli
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "qt_chooser_widget.hpp"
#include "qt_utilities.hpp"
#include "widget.hpp"
#include "message.hpp"
#include "analyze.hpp"
#include "converter.hpp"
#include "QTMFileDialog.hpp"

#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QByteArray>

/*!
 * @param _cmd  Scheme closure to execute after the dialog is closed.
 * @param _type What kind of dialog to show. Can be one of "image", "directory",
 *              or any of the supported file formats: "texmacs", "tmml",
                "postscript", etc. See perform_dialog()
 */
qt_chooser_widget_rep::qt_chooser_widget_rep (command _cmd, string _type, bool _save)
 : qt_widget_rep (file_chooser), cmd (_cmd), type (_type), save (_save), 
   position (coord2 (0, 0)), size (coord2 (100, 100)), file ("")
{
  if (DEBUG_QT)
    cout << "qt_chooser_widget_rep::qt_chooser_widget_rep type=\""
         << type << "\" save=\"" << save << "\"" << LF;
}

qt_chooser_widget_rep::~qt_chooser_widget_rep() {}


void
qt_chooser_widget_rep::send (slot s, blackbox val) {

  switch (s) {
    case SLOT_VISIBILITY:
    {   
      check_type<bool> (val, s);
      bool flag = open_box<bool> (val);
      (void) flag;
      NOT_IMPLEMENTED
    }
      break;
    case SLOT_SIZE:
      check_type<coord2>(val, s);
      size = open_box<coord2> (val);
      break;
    case SLOT_POSITION:
      check_type<coord2>(val, s);
      position = open_box<coord2> (val);
      break;
    case SLOT_KEYBOARD_FOCUS:
      check_type<bool>(val, s);
      perform_dialog ();
      break;              
    case SLOT_STRING_INPUT:
      check_type<string>(val, s);
      if (DEBUG_QT) cout << "string input: " << open_box<string> (val) << LF;
      NOT_IMPLEMENTED
      break;
    case SLOT_INPUT_TYPE:
      check_type<string>(val, s);
      type = open_box<string> (val);
      break;
#if 0
    case SLOT_INPUT_PROPOSAL:
        //send_string (THIS, "default", val);
      NOT_IMPLEMENTED
      break;
#endif
    case SLOT_FILE:
        //send_string (THIS, "file", val);
      check_type<string>(val, s);
      if (DEBUG_QT) cout << "file: " << open_box<string> (val) << LF;
      file = open_box<string> (val);
      break;
    case SLOT_DIRECTORY:
      check_type<string>(val, s);
      directory = open_box<string> (val);
      directory = as_string (url_pwd () * url_system (directory));
      break;
      
    default:
      qt_widget_rep::send (s, val);
  }
  if (DEBUG_QT)
    cout << "qt_chooser_widget_rep: sent " << slot_name (s) 
         << "\t\tto widget\t"      << type_as_string() << LF;
}

blackbox
qt_chooser_widget_rep::query (slot s, int type_id) {
  if (DEBUG_QT)
    cout << "qt_chooser_widget_rep::query " << slot_name(s) << LF;
  switch (s) {
    case SLOT_POSITION:
    {
      check_type_id<coord2> (type_id, s);
      return close_box<coord2> (position);
    }
    case SLOT_SIZE:
    {
      check_type_id<coord2> (type_id, s);
      return close_box<coord2> (size);
    }
    case SLOT_STRING_INPUT:
    {
      check_type_id<string> (type_id, s);
      if (DEBUG_QT) cout << "String: " << file << LF;
      return close_box<string> (file);
    }
    default:
      return qt_widget_rep::query (s, type_id);
  }
}

widget
qt_chooser_widget_rep::read (slot s, blackbox index) {
  if (DEBUG_QT)
    cout << "qt_chooser_widget_rep::read " << slot_name(s) << LF;
  switch (s) {
    case SLOT_WINDOW:
      check_type_void (index, s);
      return this;
    case SLOT_FORM_FIELD:
      check_type<int> (index, s);
      return this;
    case SLOT_FILE:
      check_type_void (index, s);
      return this;
    case SLOT_DIRECTORY:
      check_type_void (index, s);
      return this;
    default:
      return qt_widget_rep::read(s,index);
  }
}

widget
qt_chooser_widget_rep::plain_window_widget (string s, command q)
{
  win_title = s;
  quit = q;       // (NOT USED)
  return this;
}

#if (defined(Q_WS_MAC) || defined(Q_WS_WIN))

/*!
 * Actually displays the dialog with all the options set.
 * @todo The file filters for the dialog should not be hardcoded!
 * @todo Retrieve image sizes when opening images.
 */ 
void
qt_chooser_widget_rep::perform_dialog () {
  QString _file;
  QStringList _files;
  QString _filter, _suffix, _caption, _directory;
  _caption = to_qstring (win_title);
  c_string tmp (directory * "/" * file);
  _directory = QString::fromLocal8Bit (tmp);
#if (QT_VERSION >= 0x040400)
  if (type == "directory") {
  } else if (type == "texmacs") {
    _filter= "TeXmacs file (*.tm *.ts *.tp)";
    _suffix= "tm";
  } else if (type == "image") {
    _filter= "Image file (*.gif *.jpg *.jpeg *.pdf *.png *.pnm *.ps *.eps *.ppm *.svg *.tif *.fig *.xpm)";
  } else if (type == "bibtex") {
    _filter= "BibTeX file (*.bib)";
    _suffix= "bib";
  } else if (type == "html") {
    _filter= "Html file (*.html *.xhtml *.htm)";
    _suffix= "html";
  } else if (type == "latex") {
    _filter= "LaTeX file (*.tex *.ltx *.sty *.cls)";
    _suffix= "tex";
  } else if (type == "stm") {
    _filter= "Scheme file (*.stm *.scm)";
    _suffix= "stm";
  } else if (type == "verbatim") {
    _filter= "Verbatim file (*.txt)";
    _suffix= "txt";
  } else if (type == "tmml") {
    _filter= "XML file (*.tmml)";
    _suffix= "tmml";  
  } else if (type == "pdf") {
    _filter= "Pdf file (*.pdf)";
    _suffix= "pdf";
  } else if (type == "postscript") {
    _filter= "PostScript file (*.ps *.eps)";
    _suffix= "ps";  
  }
#endif
  
  if (type == "directory")
    _file = QFileDialog::getExistingDirectory (0, _caption, _directory);
  else if (save)
    _file = QFileDialog::getSaveFileName(0, _caption, _directory, _filter);
  else
    _file = QFileDialog::getOpenFileName (0, _caption, _directory, _filter);
  
  if (_file.isEmpty()) {
    file= "#f";
  } else {
    url u = url_system (scm_unquote (from_qstring (_file)));
      // FIXME: charset detection in to_qstring() (if that hack is still there)
      // fails sometimes, so we bypass it force the proper (?) conversions here.
    QByteArray arr= utf8_to_qstring (cork_to_utf8 (as_string (u))).toLocal8Bit ();
    const char* cstr= arr.constData ();
    string localname = string ((char*) cstr);
    if (type == "image") {
      /*
       QPixmap _pic(_file);
       string params;
       // TEMPORARY HACK: wouldn't it be nicer to scale the image to, say, 
       // 90% of the current column width?
       params << "\"" << from_qstring(QString("%1px").arg(_pic.width())) << "\" ";
       params << "\"" << from_qstring(QString("%1px").arg(_pic.height())) << "\" ";
       params << "\"" << "" << "\" ";  // xps ??
       params << "\"" << "" << "\"";   // yps ??
       file = "(list (system->url " * scm_quote (as_string (u)) * ") " * params * ")";
       */
      file = "(list (system->url " * scm_quote (localname) * ") \"\" \"\" \"\" \"\")";
    } else {
      file = "(system->url " * scm_quote (localname) * ")";
    }
  }
  cmd ();
  if (!is_nil(quit)) quit ();
}
#else
/**
 * A file chooser dialog with image preview for platforms other than Mac/Win
 * FIXME: this is incomplete.
 */
void
qt_chooser_widget_rep::perform_dialog () {
    // int result;
  
  QTMFileDialog *dialog;
  QTMImageDialog *imgdialog= 0; // to avoid a dynamic_cast
  
  if (type  == "image")
    dialog= imgdialog= new QTMImageDialog (NULL, to_qstring (win_title),
                                           to_qstring(directory * "/" * file));
  else
    dialog= new QTMFileDialog (NULL, to_qstring (win_title), 
                               to_qstring (directory * "/" * file));
  
#if (defined(Q_WS_MAC) && (QT_VERSION >= 0x040500))
  dialog->setOptions(QFileDialog::DontUseNativeDialog);
#endif
  
  QPoint pos = to_qpoint(position);
    //cout << "Size :" << size.x1 << "," << size.x2 << LF;
  if (DEBUG_QT) {
    cout << "Position :" << pos.x() << "," << pos.y() << LF;
    cout << "Dir: " << directory * "/" * file << LF;
  }
  
  dialog->updateGeometry();
  QSize sz = dialog->sizeHint();
  QRect r; r.setSize(sz);
  r.moveCenter(pos);
  dialog->setGeometry(r);
  
  dialog->setViewMode (QFileDialog::Detail);
  if (type == "directory") {
    dialog->setFileMode(QFileDialog::Directory);
  } else if (type == "image") {
    dialog->setFileMode(QFileDialog::ExistingFile);
  } else {
    dialog->setFileMode(QFileDialog::AnyFile);
  }
  
#if (QT_VERSION >= 0x040400)
  if (type == "directory") {  
  } else if (type == "texmacs") {
    dialog->setNameFilter ("TeXmacs file (*.tm *.ts *.tp)");
    dialog->setDefaultSuffix ("tm");
  } else if (type == "image") {
    dialog->setNameFilter ("Image file (*.gif *.jpg *.jpeg *.pdf *.png *.pnm *.ps *.eps *.ppm *.svg *.tif *.tiff *.fig *.xpm)");
  } else if (type == "bibtex") {
    dialog->setNameFilter ("BibTeX file (*.bib)");
    dialog->setDefaultSuffix ("bib");
  } else if (type == "html") {
    dialog->setNameFilter ("Html file (*.htm *.html *.xhtml)");
    dialog->setDefaultSuffix ("html");
  } else if (type == "latex") {
    dialog->setNameFilter ("LaTeX file (*.tex *.ltx *.sty *.cls)");
    dialog->setDefaultSuffix ("tex");
  } else if (type == "stm") {
    dialog->setNameFilter ("Scheme file (*.stm *.scm)");
    dialog->setDefaultSuffix ("stm");
  } else if (type == "verbatim") {
    dialog->setNameFilter ("Verbatim file (*.txt)");
    dialog->setDefaultSuffix ("txt");
  } else if (type == "tmml") {
    dialog->setNameFilter ("XML file (*.tmml)");
    dialog->setDefaultSuffix ("tmml");  
  } else if (type == "pdf") {
    dialog->setNameFilter ("Pdf file (*.pdf)");
    dialog->setDefaultSuffix ("pdf");
  } else if (type == "postscript") {
    dialog->setNameFilter ("PostScript file (*.ps *.eps)");
    dialog->setDefaultSuffix ("ps");  
  }
#endif
  
  dialog->setLabelText(QFileDialog::Accept, "Ok");
  
  QStringList fileNames;
  if (dialog->exec ()) {
    fileNames = dialog->selectedFiles();
    if (fileNames.count() > 0) {
      file = from_qstring (fileNames[0]);
      url u = url_system (scm_unquote (file));
      QByteArray arr= to_qstring (as_string (u)).toLocal8Bit ();
      const char* cstr= arr.constData ();
      string localname = string ((char*) cstr);
      if (type == "image")
        file = "(list (system->url " *
        scm_quote (as_string (u)) *
        ") " * imgdialog->getParamsAsString () * ")";
      else
        file = "(system->url " * scm_quote (localname) * ")";
    }
  } else {
    file = "#f";
  }
  
  delete dialog;
  
  cmd ();
  if (!is_nil(quit)) quit ();
}
#endif

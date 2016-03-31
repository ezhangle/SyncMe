#include "../../Headers/Dropbox/SM_dropbox_fileinfo.h"

SM_Dropbox_FileInfo::SM_Dropbox_FileInfo(QObject *parent) :
    SM_Dropbox_Json(parent)
{
  _init();
}

SM_Dropbox_FileInfo::SM_Dropbox_FileInfo(QString jsonStr, QObject *parent) :
    SM_Dropbox_Json(jsonStr, parent)
{
    _init();
    dataFromJson();
}

SM_Dropbox_FileInfo::SM_Dropbox_FileInfo(const SM_Dropbox_FileInfo &other) :
    SM_Dropbox_Json(0)
{
    _init();
    copyFrom(other);
}

SM_Dropbox_FileInfo::~SM_Dropbox_FileInfo()
{
  if(_content != NULL)
    delete _content;
}

void SM_Dropbox_FileInfo::copyFrom(const SM_Dropbox_FileInfo &other)
{
    parseString(other.strContent());
    dataFromJson();
    setParent(other.parent());
    return;
}

SM_Dropbox_FileInfo &SM_Dropbox_FileInfo::operator=(const SM_Dropbox_FileInfo &other)
{
    copyFrom(other);
    return *this;
}

void SM_Dropbox_FileInfo::dataFromJson()
{
    if(!isValid())
        return;

    _size         = getString("size");
    _revision     = getUInt("revision");
    _thumbExists  = getBool("thumb_exists");
    _bytes        = getUInt("bytes");
    _icon         = getString("icon");
    _root         = getString("root");
    _path         = getString("path");
    _isDir        = getBool("is_dir");
    _mimeType     = getString("mime_type");
    _isDeleted    = getBool("is_deleted");
    _revisionHash = getString("rev");
    _modified     = getTimestamp("modified");
    _clientModified = getTimestamp("client_mtime");

    // create content list
    if(_isDir)
    {
#ifdef SM_QTDROPBOX_DEBUG
      qDebug() << "fileinfo: generating contents list";
#endif
      _content = new QList<SM_Dropbox_FileInfo>();
      QStringList contentsArray = getArray("contents");
      for(qint32 i = 0; i<contentsArray.size(); ++i)
      {
        SM_Dropbox_FileInfo contentInfo(contentsArray.at(i));
        if(!contentInfo.isValid())
          continue;

        _content->append(contentInfo);
      }
    }

    return;
}

void SM_Dropbox_FileInfo::_init()
{
    _size           = "";
    _revision       = 0;
    _thumbExists    = false;
    _bytes          = 0;
    _modified       = QDateTime::currentDateTime();
    _clientModified = QDateTime::currentDateTime();
    _icon           = "";
    _root           = "";
    _path           = "";
    _isDir          = false;
    _mimeType       = "";
    _isDeleted      = false;
    _revisionHash   = "";
    _content        = NULL;
    return;
}

QString SM_Dropbox_FileInfo::revisionHash()  const
{
    return _revisionHash;
}

bool SM_Dropbox_FileInfo::isDeleted()  const
{
    return _isDeleted;
}


QString SM_Dropbox_FileInfo::mimeType()  const
{
    return _mimeType;
}

bool SM_Dropbox_FileInfo::isDir()  const
{
    return _isDir;
}

QString SM_Dropbox_FileInfo::path()  const
{
    return _path;
}

QString SM_Dropbox_FileInfo::root()  const
{
    return _root;
}

QString SM_Dropbox_FileInfo::icon()  const
{
    return _icon;
}

QDateTime SM_Dropbox_FileInfo::clientModified()
{
    return _clientModified;
}

QDateTime SM_Dropbox_FileInfo::modified()
{
    return _modified;
}

quint64 SM_Dropbox_FileInfo::bytes()  const
{
    return _bytes;
}

bool SM_Dropbox_FileInfo::thumbExists()  const
{
    return _thumbExists;
}

quint64 SM_Dropbox_FileInfo::revision() const
{
    return _revision;
}

QString SM_Dropbox_FileInfo::size() const
{
    return _size;
}

QList<SM_Dropbox_FileInfo> SM_Dropbox_FileInfo::contents() const
{
   if(_content == NULL || !isDir())
   {
     QList<SM_Dropbox_FileInfo> l;
     l.clear();
     return l;
   }

   return *_content;
}

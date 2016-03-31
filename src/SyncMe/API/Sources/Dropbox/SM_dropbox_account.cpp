#include "SM_dropbox_account.h"

SM_Dropbox_Account::SM_Dropbox_Account(QObject *parent) :
    SM_Dropbox_Json(parent)
{
    _quotaShared = 0;
    _quota       = 0;
    _quotaNormal = 0;
    _uid         = 0;
}

SM_Dropbox_Account::SM_Dropbox_Account(QString jsonString, QObject *parent) :
    SM_Dropbox_Json(jsonString, parent)
{
    _init();
}

SM_Dropbox_Account::SM_Dropbox_Account(const SM_Dropbox_Account& other) :
    SM_Dropbox_Json()
{
    copyFrom(other);
}

void SM_Dropbox_Account::_init()
{
    if(!isValid())
    {
        valid = false;
        return;
    }

    if(!hasKey("referral_link") ||
       !hasKey("display_name")  ||
       !hasKey("uid") ||
       !hasKey("country") ||
       !hasKey("quota_info") ||
       !hasKey("email"))
    {
#ifdef SM_QTDROPBOX_DEBUG
        qDebug() << "json invalido 1" << endl;
#endif
        valid = false;
        return;
    }

    SM_Dropbox_Json* quota = getJson("quota_info");
    if(!quota->hasKey("shared") ||
       !quota->hasKey("quota") ||
       !quota->hasKey("normal"))
    {
#ifdef SM_QTDROPBOX_DEBUG
        qDebug() << "json invalido 2" << endl;
#endif
        valid = false;
        return;
    }

    _referralLink.setUrl(getString("referral_link"), QUrl::StrictMode);
    _displayName = getString("display_name");
    _uid         = getInt("uid");
    _country     = getString("country");
    _email       = getString("email");

    _quotaShared = quota->getUInt("shared", true);
    _quota       = quota->getUInt("quota", true);
    _quotaNormal = quota->getUInt("normal", true);

    valid = true;

#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "== datos de la cuenta ==" << endl;
    qDebug() << "reflink: " << _referralLink << endl;
    qDebug() << "nombre: " << _displayName << endl;
    qDebug() << "uid: " << _uid << endl;
    qDebug() << "pais: " << _country << endl;
    qDebug() << "email: " << _email << endl;
    qDebug() << "cuotaCompartida: " << _quotaShared << endl;
    qDebug() << "cuotaNormal: " << _quotaNormal << endl;
    qDebug() << "cuotaUsada: " << _quota << endl;
    qDebug() << "== fin datos de la cuenta ==" << endl;
#endif
    return;
}

QUrl SM_Dropbox_Account::referralLink()  const
{
    return _referralLink;
}

QString SM_Dropbox_Account::displayName()  const
{
    return _displayName;
}

qint64 SM_Dropbox_Account::uid()  const
{
    return _uid;
}

QString SM_Dropbox_Account::country()  const
{
    return _country;
}

QString SM_Dropbox_Account::email()  const
{
    return _email;
}

quint64 SM_Dropbox_Account::quotaShared()  const
{
    return _quotaShared;
}

quint64 SM_Dropbox_Account::quota()  const
{
    return _quota;
}

quint64 SM_Dropbox_Account::quotaNormal()  const
{
    return _quotaNormal;
}

SM_Dropbox_Account &SM_Dropbox_Account::operator =(SM_Dropbox_Account &a)
{
    copyFrom(a);
    return *this;
}

void SM_Dropbox_Account::copyFrom(const SM_Dropbox_Account &other)
{
    this->setParent(other.parent());
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "creando cuenta copiando datos de otra" << endl;
    qDebug() << "reflink usado: " << other.referralLink().toString() << endl;
#endif
    _referralLink = other.referralLink();
    _displayName  = other.displayName();
    _uid          = other.uid();
    _country      = other.country();
    _email        = other.email();
    _quotaShared  = other.quotaShared();
    _quota    = other.quota();
    _quotaNormal  = other.quotaNormal();
}

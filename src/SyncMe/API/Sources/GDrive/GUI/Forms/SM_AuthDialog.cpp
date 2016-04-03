#include "../../../../Headers/GDrive/GUI/Forms/SM_AuthDialog.h"
//#include "ui_authdialog.h"
#include "../../../../Headers/GDrive/Share/SM_Debug.h"
#include "../../../../Headers/GDrive/Share/SM_Defs.h"
#include "../../../../Headers/GDrive/Settings/SM_SettingsManager.h"
#include "ui_SM_AuthDialog.h"

SM_AuthDialog::SM_AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    init();
}

SM_AuthDialog::~SM_AuthDialog()
{  
    delete ui;
}

void SM_AuthDialog::init(void)
{
    ui->setupUi(this);

    auth.reset(new SM_Auth);

    ui->webView->setUrl(QUrl("https://accounts.google.com/o/oauth2/auth?response_type=code&client_id=" + SYNCME_APP_ID + "&redirect_uri=" + SYNCME_URI + "&scope=" + SYNCME_SCOPES));
    connect(ui->webView, SIGNAL(urlChanged(const QUrl&)), this, SLOT(slotUrlChanged(const QUrl&)));
    connect(auth.data(), SIGNAL(signalAuthResponse(const QString&, const QString&)), this, SLOT(slotAuthResponse(const QString&, const QString&)));
}

void SM_AuthDialog::slotUrlChanged(const QUrl &url)
{
    QString code(auth->getOAuth2Code(url));

    if(!code.isEmpty())
    {
       auth->getTokens(code, SYNCME_APP_ID, SYNCME_SECRET_ID, SYNCME_URI);
    }
}

void SM_AuthDialog::slotAuthResponse(const QString &accessToken, const QString &refreshToken)
{
    emit signalTokens(accessToken, refreshToken);
    QDialog::accept();
}

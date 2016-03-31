#include "../../Headers/Dropbox/SM_dropbox_deltaresponse.h"
#include "../../Headers/Dropbox/SM_dropbox_json.h"

SM_Dropbox_DeltaResponse::SM_Dropbox_DeltaResponse()
{
    _init();
}

SM_Dropbox_DeltaResponse::SM_Dropbox_DeltaResponse(QString response)
{
    _init();

    SM_Dropbox_Json js(response);

    this->_reset = js.getBool("reset");
    this->_cursor = js.getString("cursor");
    this->_has_more = js.getBool("has_more");

    QStringList entriesList = js.getArray("entries");

    for(QStringList::iterator i = entriesList.begin();
        i != entriesList.end();
        i++)
    {
        SM_Dropbox_Json s(*i);
        QStringList pair = s.getArray();

        QSharedPointer<SM_Dropbox_FileInfo> val(
                    new SM_Dropbox_FileInfo(
                            pair.value(1)
                        )
                    );
        this->_entries.insert(pair.value(0), val);
    }
}

const SM_Dropbox_DeltaEntryMap SM_Dropbox_DeltaResponse::getEntries() const
{
    return this->_entries;
}

bool SM_Dropbox_DeltaResponse::shouldReset() const
{
    return this->_reset;
}

QString SM_Dropbox_DeltaResponse::getNextCursor() const
{
    return this->_cursor;
}


bool SM_Dropbox_DeltaResponse::hasMore() const
{
    return this->_has_more;
}

void SM_Dropbox_DeltaResponse::_init()
{
    _reset = false;
    _cursor = "";
    _has_more = false;
}

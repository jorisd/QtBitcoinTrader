#include "translationabout.h"
#include "main.h"
#ifdef Q_OS_WIN
#include "qtwin.h"
#endif
#include "translationdialog.h"

TranslationAbout::TranslationAbout(QWidget *)
	: QDialog()
{
	ui.setupUi(this);
	setWindowFlags(Qt::WindowCloseButtonHint);
	setWindowModality(Qt::ApplicationModal);
	setAttribute(Qt::WA_DeleteOnClose,true);
	//setFixedSize(size());

#ifdef Q_OS_WIN
	if(QtWin::isCompositionEnabled())
		QtWin::extendFrameIntoClientArea(this);
#endif

#ifdef GENERATE_LANGUAGE_FILE
	julyTranslator->loadMapFromUi(this);
	julyTranslator->saveToFile("LanguageDefault.lng");
#endif

	julyTranslator->translateUi(this);
}

TranslationAbout::~TranslationAbout()
{

}

void TranslationAbout::createTranslation()
{
	accept();
	(new TranslationDialog)->show();
}
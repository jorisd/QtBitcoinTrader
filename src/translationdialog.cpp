#include "translationdialog.h"
#include "main.h"

TranslationDialog::TranslationDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose,true);
	//setFixedSize(size());

#ifdef GENERATE_LANGUAGE_FILE
	julyTranslator->loadMapFromUi(this);
	julyTranslator->saveToFile("LanguageDefault.lng");
#endif

	julyTranslator->translateUi(this);
}

TranslationDialog::~TranslationDialog()
{

}

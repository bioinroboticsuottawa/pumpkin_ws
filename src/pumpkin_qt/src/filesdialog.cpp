#include "include/pumpkin_qt/filesdialog.hpp"
#include "ui_filesdialog.h"

FilesDialog::FilesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FilesDialog)
{
	ui->setupUi(this);
}

FilesDialog::~FilesDialog()
{
	delete ui;
}

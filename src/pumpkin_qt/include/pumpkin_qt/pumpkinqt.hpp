#ifndef PUMPKINQT_H
#define PUMPKINQT_H

#include "ui_pumpkinqt.h"
#include "qnode.hpp"
#include "playbackactionclient.hpp"
#include "recordactionclient.hpp"
#include "loadconfig.hpp"
#include "sscmovecommand.hpp"

#include <QMainWindow>
#include <QStringListModel>
#include <vector>
#include <QList>
#include <QPair>
#include "pumpkin_messages/Files.h"
#include "pumpkin_messages/PlaybackAction.h"
#include "pumpkin_messages/RecordAction.h"

namespace pumpkin_qt {

class FolderItem {
private:
    FolderItem *_parent;
    QList<FolderItem *> _children;
    int _id;
    QString _folder;

    void appendChild(FolderItem *child) {_children.append(child); }
public:
    explicit FolderItem(const QString &name, int index, FolderItem *parent = 0);
    ~FolderItem();
    inline FolderItem *child(int row) {return _children.value(row);}
    inline int childCount() const {return _children.size();}
    inline int columnCount() const {return 2;}
    QVariant data(int column) const;
    int row() const;
    inline FolderItem * parent() {return _parent;}
};

typedef QList<QPair<int, QString> > FolderListType;


class FolderModel : public QAbstractItemModel {
    Q_OBJECT
private:
    FolderItem *_root;
public:
	explicit FolderModel(const FolderListType & folders, QObject *parent = nullptr);
    ~FolderModel();
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
};


class PumpkinQT : public QMainWindow
{
    Q_OBJECT
public:
    explicit PumpkinQT(int argc, char *argv[], QWidget *parent = 0);
    ~PumpkinQT();
    void resizeEvent(QResizeEvent *event);

Q_SIGNALS:
	void changePlaybackFilename(const QString & str);
	void changeRecordFilename(const QString & str);

public Q_SLOTS:
    void fillTable(const QString& base_path, const std::vector<pumpkin_messages::FileList>& file_list);
    void folderSelected(const QModelIndex &index);
    void fileSelected(const QModelIndex &index);
	void runPlayback();
	void runRecord();
	void changeFilename(const QString& filename);
	void actionFinished(int state);
	void showSSCMoveDialog();
	void showAboutDialog();

private:
	Ui::PumpkinQTDesign _ui;
	QNode _node;
	QString _base_path, _relative_path, _filename;
    QList<QStringListModel *> _model_list;
    FolderModel *_folder_model;
	PlaybackActionClient _playback;
	RecordActionClient _record;
	LoadConfig *_config_dialog;
	SSCMoveCommand *_move_dialog;
};

}
#endif // PUMPKINQT_H
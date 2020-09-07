//
// Created by Nox on 07-Sep-20.
//

#include "TrackItemDelegate.h"

#include <QPainter>
#include <QApplication>

#include "DatabaseManager.h"

using namespace ScrutchPlayer;

void TrackItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    QString albumArtist = index.model()->data(index.model()->index(index.row(), 0)).toString();
    QString artist = index.model()->data(index.model()->index(index.row(), 1)).toString();
    QString albumTitle = index.model()->data(index.model()->index(index.row(), 2)).toString();
    QString comment = index.model()->data(index.model()->index(index.row(), 3)).toString();
    QString genre = index.model()->data(index.model()->index(index.row(), 4)).toString();
    QString filePath = index.model()->data(index.model()->index(index.row(), 5)).toString();
    QString title = index.model()->data(index.model()->index(index.row(), 6)).toString();
    QString coverUuid = index.model()->data(index.model()->index(index.row(), 7)).toString();
    int bitrate = index.model()->data(index.model()->index(index.row(), 8)).toInt();
    int disc = index.model()->data(index.model()->index(index.row(), 9)).toInt();
    int totalDisc = index.model()->data(index.model()->index(index.row(), 10)).toInt();
    int trackNbr = index.model()->data(index.model()->index(index.row(), 11)).toInt();
    int totalTrackNbr = index.model()->data(index.model()->index(index.row(), 12)).toInt();
    int year = index.model()->data(index.model()->index(index.row(), 13)).toInt();
    int lengthInMS = index.model()->data(index.model()->index(index.row(), 14)).toInt();

    // draw correct background
    opt.text = "";
    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    QRect rect = opt.rect;
    QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
        cg = QPalette::Inactive;

    // set pen color
    if (opt.state & QStyle::State_Selected)
        painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
    else
        painter->setPen(opt.palette.color(cg, QPalette::Text));

    if (index.column() == 7)
    {
        // draw artwork
        QImage * coverImage = DatabaseManager::getInstance().getThumbnailManager().getThumbnailForUuid(coverUuid.toStdString());
        if (coverImage != nullptr)
        {
            QPixmap cover = QPixmap::fromImage(*DatabaseManager::getInstance().getThumbnailManager().getThumbnailForUuid(coverUuid.toStdString()));
            QSize size = cover.size().scaled(rect.size(), Qt::KeepAspectRatio);

            painter->drawPixmap(rect.x(), rect.y(), size.width(), size.height(), cover);
        }
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
//    // draw 2 lines of text
//    painter->drawText(QRect(rect.left()+160, rect.top(), rect.width(), rect.height()/2),
//                      opt.displayAlignment, title);
//    painter->drawText(QRect(rect.left()+160, rect.top()+rect.height()/2, rect.width(), rect.height()/2),
//                      opt.displayAlignment, artist);
}

QSize TrackItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QSize result = QStyledItemDelegate::sizeHint(option, index);
    result.setHeight(150);
    return result;
}

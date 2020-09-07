//
// Created by Nox on 07-Sep-20.
//

#pragma once

#include <QStyledItemDelegate>

namespace ScrutchPlayer
{
    /**
   * \class TrackItemDelegate
   * \brief
   */
    class TrackItemDelegate : public QStyledItemDelegate
    {
        // -- Attributes
    private:
        // --!Attributes

        // -- CTors & DTor
    public:
        TrackItemDelegate() = default;

        ~TrackItemDelegate() = default;

        // Let's 'delete' all those to avoid them being used inadvertently.
        TrackItemDelegate(const TrackItemDelegate &) = delete;

        TrackItemDelegate & operator=(const TrackItemDelegate &) = delete;

        TrackItemDelegate(TrackItemDelegate &&) = delete;

        TrackItemDelegate & operator=(TrackItemDelegate &&) = delete;
        // --!CTors & DTor

        // -- Methods
    protected:
        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
        QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const;


        // --!Methods
    };
}
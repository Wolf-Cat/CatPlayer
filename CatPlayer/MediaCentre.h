#ifndef MEDIACENTRE_H
#define MEDIACENTRE_H

#include <QObject>

class MediaCentre : public QObject
{
    Q_OBJECT
public:
    explicit MediaCentre(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MEDIACENTRE_H
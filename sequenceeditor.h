#ifndef SEQUENCEEDITOR_H
#define SEQUENCEEDITOR_H

#include <QWidget>
#include <QListWidget>
#include "sequence.h"

class SequenceEditor : public QWidget
{
    Q_OBJECT
public:
    explicit SequenceEditor(QWidget *parent = 0);
    
signals:
    void newSequenceMade(Sequence);
    void requestPoseList();

public slots:
    void copyPoseList(QListWidget*);
    void moveItemToSendList();
    void removeItemFromSendList();
    void testSequence();
    void outputAsCppSourcePart();

private:
    QListWidget* poseListFromEditor;
    QListWidget* poseListToSend;
};

#endif // SEQUENCEEDITOR_H

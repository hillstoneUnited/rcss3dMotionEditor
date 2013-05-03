#include "sequenceeditor.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <fstream>

SequenceEditor::SequenceEditor(QWidget *parent) :
    QWidget(parent)
{
    QLabel* label1 = new QLabel(tr("your pose list"));
    QPushButton* loadPoseListButton = new QPushButton(tr("load list from pose edtior"));
    poseListFromEditor = new QListWidget();
    QPushButton* addButton = new QPushButton(tr("add ->"));

    poseListToSend = new QListWidget();
    QPushButton* testButton = new QPushButton(tr("test"));
    QPushButton* outputButton = new QPushButton(tr("output as file"));
//        outputButton->setEnabled(false);
    QPushButton* removeButton = new QPushButton(tr("<- remove"));

    QVBoxLayout* left = new QVBoxLayout();
    QVBoxLayout* right = new QVBoxLayout();

    QHBoxLayout* entire = new QHBoxLayout();

    left->addWidget(label1);
    left->addWidget(loadPoseListButton);
    left->addWidget(poseListFromEditor);
    left->addWidget(addButton);

    right->addWidget(poseListToSend);
    right->addWidget(testButton);
    right->addWidget(outputButton);
    right->addWidget(removeButton);

    entire->addLayout(left);
    entire->addWidget(new QLabel(tr("    "))); // spacer is better
    entire->addLayout(right);

    this->setLayout(entire);

    connect(loadPoseListButton, SIGNAL(clicked()), this, SIGNAL(requestPoseList()));

    connect(addButton, SIGNAL(clicked()), this, SLOT(moveItemToSendList()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeItemFromSendList()));
    connect(testButton, SIGNAL(clicked()), this, SLOT(testSequence()));
    connect(outputButton, SIGNAL(clicked()), this, SLOT(outputAsCppSourcePart()));
}

Q_DECLARE_METATYPE(Pose)

void SequenceEditor::copyPoseList(QListWidget *list){
//    poseListFromEditor = list;
//    qDebug() << "seqedit::copyPoseList";

    poseListFromEditor->clear();
    poseListToSend->clear();

    for(int i=0; i<list->count(); i++){
        // item->clone() is ok?
        poseListFromEditor->addItem(list->item(i)->text());
        poseListFromEditor->item(i)->setData(Qt::UserRole, list->item(i)->data(Qt::UserRole));
    }
}

void SequenceEditor::moveItemToSendList(){
//    qDebug() << poseListFromEditor->currentRow();
    if(poseListFromEditor->currentRow() == -1){
        //not selected any item
        return;
    }
//    Pose temp = poseListFromEditor->currentItem()->data(Qt::UserRole).value<Pose>();
//    double tempD = temp.getTarget(3);
//    qDebug() << tempD;
    QListWidgetItem* added = new QListWidgetItem(poseListFromEditor->currentItem()->text());
    added->setData(Qt::UserRole, poseListFromEditor->currentItem()->data(Qt::UserRole));
    poseListToSend->addItem(added);

    // select next item
    if(poseListFromEditor->currentRow() != poseListFromEditor->count()){
        poseListFromEditor->setCurrentRow(poseListFromEditor->currentRow()+1);
    }

//    Pose temp = poseListToSend->item(poseListToSend->count()-1)->data(Qt::UserRole).value<Pose>();
//    qDebug() << temp.getTarget(1);
}

void SequenceEditor::removeItemFromSendList(){
    if(poseListToSend->currentRow() == -1){
        return;
    }
    poseListToSend->takeItem(poseListToSend->currentRow());
}

void SequenceEditor::testSequence(){
    Sequence tempSeq;

    for(int i=0; i<poseListToSend->count(); i++){
        tempSeq.addPose(poseListToSend->item(i)->data(Qt::UserRole).value<Pose>());
    }
    emit newSequenceMade(tempSeq);
}

void SequenceEditor::outputAsCppSourcePart(){
//    qDebug() << "output";
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), tr("cpp source part : cppp (*.cppp);;AllFiles(*.*)"));
    if(filename.isEmpty()){
        qDebug() << "SequenceEditor::outputAsCppSourcePart : filename is empty";
        return;
    }
    if(!filename.endsWith(".cppp")){
        qDebug() << "SequenceEditor::outputAsCppSourcePart : file type is not cppp. add \".cppp\"";
        filename = filename + ".cppp";
    }

    std::ofstream ofs;
    ofs.open(filename.toStdString().c_str());

    for(int i=0; i<poseListToSend->count(); i++){
        Pose p = poseListToSend->item(i)->data(Qt::UserRole).value<Pose>();
        ofs << "\tPose pose" << i << ";" << std::endl;
        ofs << "\tpose" << i << ".setTarget(hj1, " << p.getTarget(0) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(hj2, " << p.getTarget(1) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(laj1, " << p.getTarget(2) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(raj1, " << p.getTarget(3) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(laj2, " << p.getTarget(4) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(raj2, " << p.getTarget(5) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(laj3, " << p.getTarget(6) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(raj3, " << p.getTarget(7) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(laj4, " << p.getTarget(8) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(raj4, " << p.getTarget(9) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(llj1, " << p.getTarget(10) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(rlj1, " << p.getTarget(11) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(llj2, " << p.getTarget(12) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(rlj2, " << p.getTarget(13) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(llj3, " << p.getTarget(14) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(rlj3, " << p.getTarget(15) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(llj4, " << p.getTarget(16) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(rlj4, " << p.getTarget(17) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(llj5, " << p.getTarget(18) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(rlj5, " << p.getTarget(19) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(llj6, " << p.getTarget(20) << ");" << std::endl;
        ofs << "\tpose" << i << ".setTarget(rlj6, " << p.getTarget(21) << ");" << std::endl;
    }

    ofs << "\tSequence sequence;" << std::endl;
    for(int i=0; i<poseListToSend->count(); i++){
        ofs << "\tsequence.addPose(pose" << i << ");" << std::endl;
    }
    ofs << "\tsetNewSequence(sequence);" << std::endl;

    ofs.close();

}

#include "poseeditor.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

Q_DECLARE_METATYPE(Pose)

PoseEditor::PoseEditor(QWidget *parent) :
    QWidget(parent)
{
    ij[0] = new InputJoint(tr("hj1"), -120, 120, this);
    ij[1] = new InputJoint(tr("hj2"), -45, 45, this);
    ij[2] = new InputJoint(tr("laj1"), -120, 120, this);
    ij[3] = new InputJoint(tr("raj1"), -120, 120, this);
    ij[4] = new InputJoint(tr("laj2"), -1, 95, this);
    ij[5] = new InputJoint(tr("raj2"), -95, 1, this);
    ij[6] = new InputJoint(tr("laj3"), -120, 120, this);
    ij[7] = new InputJoint(tr("raj3"), -120, 120, this);
    ij[8] = new InputJoint(tr("laj4"), -90, 1, this);
    ij[9] = new InputJoint(tr("raj4"), -1, 90, this);
    ij[10] = new InputJoint(tr("llj1"), -90, 1, this);
    ij[11] = new InputJoint(tr("rlj1"), -90, 1, this);
    ij[12] = new InputJoint(tr("llj2"), -25, 45, this);
    ij[13] = new InputJoint(tr("rlj2"), -45, 25, this);
    ij[14] = new InputJoint(tr("llj3"), -25, 100, this);
    ij[15] = new InputJoint(tr("rlj3"), -25, 100, this);
    ij[16] = new InputJoint(tr("llj4"), -130, 1, this);
    ij[17] = new InputJoint(tr("rlj4"), -130, 1, this);
    ij[18] = new InputJoint(tr("llj5"), -45, 75, this);
    ij[19] = new InputJoint(tr("rlj5"), -45, 75, this);
    ij[20] = new InputJoint(tr("llj6"), -45, 25, this);
    ij[21]= new InputJoint(tr("rlj6"), -25, 45, this);

    posesList = new QListWidget();
    /*
    posesList->addItem(tr("pose0")); // using for(i ; i< length_of_posesList; i++) is better
    posesList->addItem(tr("pose1"));
    posesList->addItem(tr("pose2"));
    posesList->addItem(tr("pose3"));
    posesList->addItem(tr("pose4"));
    posesList->addItem(tr("pose5"));
    posesList->addItem(tr("pose6"));
    posesList->addItem(tr("pose7"));
    posesList->addItem(tr("pose8"));
    posesList->addItem(tr("pose9"));
    posesList->setCurrentRow(0);
    for(int i=0; i< 10; i++){
        posesList->item(i)->setFlags(posesList->item(i)->flags() | Qt::ItemIsEditable);
        posesList->item(i)->setData(Qt::UserRole, QVariant::fromValue(poseEditorList[i]));
    }
    */
    for(int i=0; i<10; i++){
        Pose tempP;
        addPoseItem("pose"+QString::number(i), tempP);
    }
    posesList->setCurrentRow(0);

    //    posesList->setEditTriggers(QAbstractItemView::DoubleClicked);
    QPushButton *saveProjectButton = new QPushButton(tr("save project"));
    QPushButton *loadProjectButton = new QPushButton(tr("load project"));

    fromComboBox = new QComboBox();
    toComboBox = new QComboBox();
    QPushButton* copyButton = new QPushButton(tr("copy"));

    for(int i=0; i < posesList->count(); i++){
        fromComboBox->addItem(posesList->item(i)->text());
        toComboBox->addItem(posesList->item(i)->text());
    }

    QHBoxLayout *copyerLayout = new QHBoxLayout();
    copyerLayout->addWidget(fromComboBox);
    copyerLayout->addWidget(new QLabel(tr("to")));
    copyerLayout->addWidget(toComboBox);
    copyerLayout->addWidget(copyButton);

    QVBoxLayout *utilityLayout = new QVBoxLayout();
    utilityLayout->addWidget(posesList);
    utilityLayout->addWidget(saveProjectButton);
    utilityLayout->addWidget(loadProjectButton);
    utilityLayout->addLayout(copyerLayout);

    QGridLayout *jointLayout = new QGridLayout();
    jointLayout->addWidget(ij[0], 0, 0, 1, 2);
    jointLayout->addWidget(ij[1], 1, 0, 1, 2);
    for(int i=1; i<11; i++){
        jointLayout->addWidget(ij[i*2], i+2, 0);
        jointLayout->addWidget(ij[i*2+1], i+2, 1);
    }

    QHBoxLayout *entireLayout = new QHBoxLayout();
    entireLayout->addLayout(utilityLayout);
    entireLayout->addLayout(jointLayout);

    this->setLayout(entireLayout);
    this->setMinimumSize(300, 350);
    posesList->setMaximumWidth(250);

    //signal slot connection
    // if value changed, modify shared information
    /*
        "check which joint is modified" is better,
        but "if some joint is modified, reread all data" is easier
    */

    for(int i=0; i<22; i++){
        connect(ij[i], SIGNAL(valueChanged(double)), this, SLOT(makeNewPose()));
    }

    connect(posesList, SIGNAL(currentRowChanged(int)), this, SLOT(changePoseListRow(int)));

    connect(saveProjectButton, SIGNAL(clicked()), this, SLOT(saveProject()));
    connect(loadProjectButton, SIGNAL(clicked()), this, SLOT(loadProject()));
    connect(copyButton, SIGNAL(clicked()), this, SLOT(copyPose()));
//    connect(posesList, SIGNAL(currentRowChanged(int)), this, SLOT(renameComboBox()));
    connect(posesList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(renameComboBox()));
    connect(posesList, SIGNAL(currentRowChanged(int)), fromComboBox, SLOT(setCurrentIndex(int)));
}

void PoseEditor::makeNewPose(){
    if(posesList->currentRow() == -1){
        return;
    }
    double inputedValue[22];
    for(int i=0; i<22; i++){
        inputedValue[i] = ij[i]->getValue();
    }
    Pose p(inputedValue);
//    poseEditorList[posesList->currentRow()] = p;
    posesList->currentItem()->setData(Qt::UserRole, QVariant::fromValue(p));
//    qDebug() << "newPoseMade @ makeNewPose";
    emit newPoseMade(p);
}

void PoseEditor::changePoseListRow(int i){
//    qDebug() << "changePoseListRow " << i;
    loadPose(i);
//    qDebug() << "newPoseMade @ changePoseListRow";
    Pose p;
    p = posesList->currentItem()->data(Qt::UserRole).value<Pose>();
    emit newPoseMade(p);
}

void PoseEditor::loadPose(Pose p){
    for(int i=0; i<22; i++){
        ij[i]->setValueNoSignal(p.getTarget()[i]);
    }
}

void PoseEditor::loadPose(int i){
    Pose p = posesList->item(i)->data(Qt::UserRole).value<Pose>();
    loadPose(p);
}

void PoseEditor::saveProject(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), tr("rcss3dPoseProject : ppj (*.ppj);;AllFiles(*.*)"));
    if(filename.isEmpty()){
        qDebug() << "PoseEditor : filename is empty";
        return;
    }
    if(!filename.endsWith(".ppj")){
        qDebug() << "PoseEditor:saveProject : file type isn't \"ppj\". add \".ppj\"";
        filename = filename + ".ppj";
    }

    std::ofstream ofs;
    ofs.open(filename.toStdString().c_str());

    for(int i=0; i<10; i++){
        ofs << "pose\t" << i << "\tname\t" << posesList->item(i)->text().toStdString() << std::endl;
//        ofs << "pose\t" << i << "\tgain\t" << poseEditorList[i].getGain() << std::endl;
        ofs << "pose\t" << i << "\tgain\t" << posesList->item(i)->data(Qt::UserRole).value<Pose>().getGain() << std::endl;
        for(int j=0; j<22; j++){
//            ofs << "pose\t" << i << "\tjoint\t" << j << "\t" << poseEditorList[i].getTarget(j) << std::endl;
            ofs << "pose\t" << i << "\tjoint\t" << j << "\t" << posesList->item(i)->data(Qt::UserRole).value<Pose>().getTarget(j) << std::endl;
        }
    }

    ofs.close();
}

void PoseEditor::loadProject(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Load File"), QDir::currentPath(), tr("rcss3dPoseProject : ppj (*.ppj);;AllFiles(*.*)"));
    if(filename.isEmpty()){
        qDebug() << "PoseEditor:loadProject : filename is empty";
        return;
    }
    if(!filename.endsWith(".ppj")){
        qDebug() << "PoseEditor:loadProject : file type isn't \"ppj\"";
        return;
    }

    std::ifstream ifs(filename.toStdString().c_str());
    std::string str;

    while(std::getline(ifs, str)){ // for all line in file
        std::vector<std::string> splitLine;
        boost::algorithm::split(splitLine, str, boost::is_any_of("\t"));
/*
        for(std::vector<std::string>::iterator it = splitLine.begin(); it != splitLine.end(); it++){
            std::cout << *it << ":";
        }
        std::cout << std::endl;
*/
        std::vector<std::string>::iterator it = splitLine.begin();
        if(*it == "pose"){
            it++;
            int i = boost::lexical_cast<int>(*it);
            it++;
            if(*it == "name"){
                it++;
                std::string nameStr = *it;
                QString nameQStr(nameStr.c_str());
                posesList->item(i)->setText(nameQStr);
            }else if(*it == "gain"){
                it++;
                double gain = boost::lexical_cast<double>(*it);
                Pose p = posesList->item(i)->data(Qt::UserRole).value<Pose>();
                p.setGain(gain);
                posesList->item(i)->setData(Qt::UserRole, QVariant::fromValue(p));
            }else if(*it == "joint"){
                it++;
                int jointNum = boost::lexical_cast<int>(*it);
                it++;
                double target = boost::lexical_cast<double>(*it);
                Pose p = posesList->item(i)->data(Qt::UserRole).value<Pose>();
                p.setTarget(jointNum, target);
                posesList->item(i)->setData(Qt::UserRole, QVariant::fromValue(p));
//                posesList->item(i)->data(Qt::UserRole).value<Pose>().setTarget(jointNum, target); // not works
            }
        }
    }
    changePoseListRow(posesList->currentRow());
}

void PoseEditor::copyPose(){
//    poseEditorList[toComboBox->currentIndex()] = poseEditorList[fromComboBox->currentIndex()];
    posesList->item(toComboBox->currentIndex())->setData(Qt::UserRole, posesList->item(fromComboBox->currentIndex())->data(Qt::UserRole));
    changePoseListRow(posesList->currentRow());
//    posesList->item(toComboBox->currentIndex())->setData(Qt::UserRole, QVariant::fromValue(poseEditorList[fromComboBox->currentIndex()]));
}

void PoseEditor::renameComboBox(){
//    qDebug() << "PoseEditor:renameComboBox()";
    fromComboBox->setItemText(posesList->currentRow(), posesList->item(posesList->currentRow())->text());
    toComboBox->setItemText(posesList->currentRow(), posesList->item(posesList->currentRow())->text());

    int fromCurrent = fromComboBox->currentIndex();
    int toCurrent = toComboBox->currentIndex();
    QList<QString> poseNames;

    for(int i=0; i<posesList->count(); i++){
        poseNames.append(posesList->item(i)->text());
    }

    fromComboBox->clear();
    toComboBox->clear();

    fromComboBox->addItems(poseNames);
    toComboBox->addItems(poseNames);

    if(fromCurrent < fromComboBox->count()){
        fromComboBox->setCurrentIndex(fromCurrent);
    }
    if(toCurrent < toComboBox->count()){
        toComboBox->setCurrentIndex(toCurrent);
    }
}

void PoseEditor::getPoseList(){
    emit poseList(posesList);
}

void PoseEditor::addPoseItem(QString txt, Pose p){
    QListWidgetItem* add = new QListWidgetItem();
    add->setText(txt);
    add->setFlags(add->flags() | Qt::ItemIsEditable);
    add->setData(Qt::UserRole, QVariant::fromValue(p));
    posesList->addItem(add);
}

void PoseEditor::removeSelectedItem(){
    posesList->removeItemWidget(posesList->currentItem());
    renameComboBox();
}


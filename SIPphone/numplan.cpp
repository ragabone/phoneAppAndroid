#include <QtCore>
#include "numplan.h"

numPlan::numPlan(char *file,int grid):mGrid(grid)
{
    strcpy(m_pFilename,file);
    getNumberPlanBuffer();
}

char *numPlan::getNumberPlanBuffer(){


    parseCSV();
    return 0;
}
QString numPlan::lookupNum(char *num){
    int i,j;
    int found = 0;
    QString numRet;

    for(i=0;i<16;i++){
        for(j=0;j<mGrid;j++){
            if(!strcmp(numBuffer[i][j],num)){
                numRet = QString("%1").arg(i, 2, 10, QChar('0')) + QString("%1").arg(j+1, 2, 10, QChar('0'));
                //numRet = QString("%1").arg(i, 2, 10, QChar('0')) + QString("%1").arg(j+1, 2, 10, QChar('0'));
                found = 1;
                break;
            }
        }
        if(found)
            break;
    }



    return numRet;
}

bool numPlan::parseCSV(){
    bool ret=false;
    char bufferLocal[2048];
    int result;



    QFile file(m_pFilename);
    if(file.exists()){
        if (!file.open(QIODevice::ReadOnly)) {

            return ret;
        }
        result = file.read(bufferLocal,2048);

        QString url(bufferLocal);
        QRegExp rx("(\\ |\\,|\\.|\\:|\\n)"); //RegEx for ' ' or ',' or '.' or ':' or '\t'

        QStringList pieces = url.split( rx );


        int k=0;
        for(int i=0;i<16;i++){
            for(int j=0;j<mGrid;j++){
                strcpy(numBuffer[i][j],pieces.at(k++).toLocal8Bit().constData());
            }

        }

        qDebug("Got String List");
        //resultChar=lookupNum(test);
        file.close();


    }
    return ret;

}

void numPlan::getNumberPlan(){
    getNumberPlanBuffer();
}

void numPlan::setBuffer(char *){



}

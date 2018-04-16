#include "functions.h"

QVector<QVector3D> toQVector3DArray(const QVector<float> & array)
{
    QVector<QVector3D> vectorArray;
    for(int i=0; i< array.size(); i++){
        if(i%3 == 2){
            float x=array.at(i-2);
            float y=array.at(i-1);
            float z=array.at(i);

            QVector3D vec(x,y,z);
            vectorArray.append(vec);
        }
    }

    return vectorArray;
}

QVector<float> toFloatArray(const QVector<QVector3D> & array)
{
    QVector<float> floatArray;

    for(int i=0; i<array.size(); i++){
        floatArray.append(array.at(i).x());
        floatArray.append(array.at(i).y());
        floatArray.append(array.at(i).z());
    }

    return floatArray;
}

QVector3D minLength(const QVector<QVector3D> &vectors)
{
    if(vectors.length() == 0){
        return QVector3D(0,0,0);
    }

    QVector3D min=vectors.at(0);
    for(int i=0; i< vectors.length(); i++){
        if(vectors.at(i).length() < min.length()){
            min=vectors.at(i);
        }
    }
    return min;
}



QVector3D maxLength(const QVector<QVector3D> &vectors)
{
    if(vectors.length() == 0){
        return QVector3D(0,0,0);
    }

    QVector3D max=vectors.at(0);
    for(int i=0; i< vectors.length(); i++){
        if(vectors.at(i).length() > max.length()){
            max=vectors.at(i);
        }
    }
    return max;
}

QVector<float> toLengthArray(const QVector<float> & array){
    QVector<float> lengthArray;
    for(int i = 0; i<array.size(); i++){
        if(i%3 == 2){
            QVector3D vec(array.at(i-2),array.at(i-1),array.at(i));
            lengthArray.append(vec.length());
        }
    }
    return lengthArray;
}

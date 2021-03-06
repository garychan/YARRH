#include "face.h"

//faces
Face::Face(HalfEdge *e1, HalfEdge *e2, HalfEdge *e3){
    this->edge1=e1;
    this->edge2=e2;
    this->edge3=e3;
    this->normal=computeNormal((QVector3D*)edge1->getStop(),(QVector3D*)edge2->getStop(),(QVector3D*)edge3->getStop());
    this->goodNormal=false;
    //find minimum and maximum z vertex
    //find min
    if(this->edge1->getStart()->z()<this->edge2->getStart()->z()){
        this->minZ=this->edge1->getStart();
    }
    else{
        this->minZ=this->edge2->getStart();
    }
    if(this->minZ->z()>this->edge3->getStart()->z()){
        this->minZ=this->edge3->getStart();
    }
    //find max
    if(this->edge1->getStart()->z()>this->edge2->getStart()->z()){
        this->maxZ=this->edge1->getStart();
    }
    else{
        this->maxZ=this->edge2->getStart();
    }
    if(this->maxZ->z()<this->edge3->getStart()->z()){
        this->maxZ=this->edge3->getStart();
    }
}

Face::Face(): edge1(NULL), edge2(NULL), edge3(NULL), normal(NULL){

}

void Face::flip(){
    HalfEdge* temp;
    this->edge1->removeFace(this);
    this->edge1 = this->edge1->getTwin();
    this->edge1->addFace(this);

    temp=this->edge2;
    this->edge2=this->edge3;
    this->edge3=temp;

    this->edge2->removeFace(this);
    this->edge2 = this->edge2->getTwin();
    this->edge2->addFace(this);

    this->edge3->removeFace(this);
    this->edge3 = this->edge3->getTwin();
    this->edge3->addFace(this);

    if(edge1->getStop()==edge2->getStart() && edge2->getStop()==edge3->getStart() && edge3->getStop()==edge1->getStart()){
        //qDebug() << "ok";
    }
    else{
        //qDebug() << "error";
    }
    *this->normal=*computeNormal(edge1->getStop(),edge2->getStop(),edge3->getStop());
}


QList<Face*> Face::getNeighbors(){
    QList<Face*> out;
    out.append(edge1->getTwin()->getFaces());
    out.append(edge2->getTwin()->getFaces());
    out.append(edge3->getTwin()->getFaces());
    return out;
}


QVector3D Face::getCenter(){
    return QVector3D(((edge1->getStop()->x()+edge2->getStop()->x()+edge3->getStop()->x())/(float)3),((edge1->getStop()->y()+edge2->getStop()->y()+edge3->getStop()->y())/(float)3),((edge1->getStop()->z()+edge2->getStop()->z()+edge3->getStop()->z())/(float)3));
}

// Compute p1,p2,p3 face normal into pOut
QVector3D* Face::computeNormal(QVector3D* p1, QVector3D* p2, QVector3D* p3)
{

    // Uses p2 as a new origin for p1,p3
    QVector3D a=*p3-*p2;
    QVector3D b=*p1-*p2;
    // Compute the cross product a X b to get the face normal
    QVector3D* pn= new QVector3D();
    *pn=QVector3D::crossProduct(a,b);
    pn->normalize();
    // Return a normalized vector
    return pn;
}


void Face::setNormalIsGood(bool value){
    this->goodNormal=value;
}

QString Face::getHash(){
    QString out;
    QList<size_t> edges;
    edges.append((size_t)this->edge1);
    edges.append((size_t)this->edge2);
    edges.append((size_t)this->edge3);
    //sort edges so all faces using these have same hash
    qSort(edges);
    out.append(QString::number(edges.at(0))+QString::number(edges.at(1))+QString::number(edges.at(2)));
    return out;
}


QList< HalfEdge* > Face::getEdgesCrossingPlane(double z, HalfEdge* remove){
    QList< HalfEdge* > out;
    if(this->edge1->crosesZPLane(z) && this->edge1!=remove){
        out.append(edge1);
    }
    if(this->edge2->crosesZPLane(z)&& this->edge2!=remove){
        out.append(edge2);
    }
    if(this->edge3->crosesZPLane(z)&& this->edge3!=remove){
        out.append(edge3);
    }
    return out;
}

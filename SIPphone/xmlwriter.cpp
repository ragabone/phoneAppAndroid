/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include "xmlwriter.h"

//! [0]
XmlWriter::XmlWriter(QString filename):mFilename(filename)
{
    m_pDoc = new QDomDocument("mydocument");
    QFile file(mFilename);
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!m_pDoc->setContent(&file)) {
        file.close();
        return;
    }
    file.close();
}
//! [0]

//! [1]
bool XmlWriter::findReplaceText(QString param,QString paramText)
{

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = m_pDoc->documentElement();

    QDomNodeList docNodeList = docElem.childNodes();
    qDebug() << "Document Node Count: " << docNodeList.count();

    int p=0;
    for(QDomNode n = docNodeList.at(0); !n.isNull(); p++,n = docNodeList.at(p))
    //for(QDomNode n = docElem.firstChild(); !n.isNull(); n = docElem.nextSibling())
    {
        qDebug()<< "Node Name: " << n.toElement().nodeName() << n.childNodes().count() ;
        if(!n.hasChildNodes()){
            QDomText t = n.toText();//
            if (!t.isNull())
                qDebug() << t.data();
        }
        else{
            QDomNodeList nodeList = n.childNodes();
            QDomElement el = nodeList.at(0).toElement();
            if(el.tagName() == "ParamName" ){
                if(el.text() == param){
                    int i=0;
                    qDebug()<< "text: " << el.text(); // prints content
                    for(i =1;i<nodeList.count();i++)
                    {
                        QDomElement el = nodeList.at(i).toElement();
                        if(el.tagName() == "ParamValue" ){
                            bool status;
                            QDomText newNodeText = m_pDoc->createTextNode(paramText);
                            el.replaceChild(newNodeText,el.childNodes().at(0));

                            // replace existing node with new node
                            qDebug()<< "new value: " << el.text(); // prints content
                            QFile outFile(mFilename);
                            status= outFile.open(QIODevice::WriteOnly);
                            QTextStream stream;
                            stream.setDevice(&outFile);
                            stream.setCodec("UTF-8");
                            m_pDoc->save(stream,4);
                            outFile.close();
                            break;
                        }
                        else
                            continue;
                        //qDebug()<< "tag: " << el.tagName(); // prints content


                    }
                    if(i == nodeList.count()){//Create New Node
                        bool status;
                        QDomElement node = m_pDoc->createElement("ParamValue");
                        n.appendChild(node);
                        // Now, add a text element to your node
                        node.appendChild( m_pDoc->createTextNode( paramText));
                        QFile outFile(mFilename);
                        status = outFile.open(QIODevice::WriteOnly);
                        QTextStream stream;
                        stream.setDevice(&outFile);
                        stream.setCodec("UTF-8");
                        m_pDoc->save(stream,4);
                        outFile.close();
                    }
                }
                else
                    continue;
            }
        }
    }

    return true;
}

QString XmlWriter::findParamText(QString param)
{
    QString retVal;
    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = m_pDoc->documentElement();

    QDomNodeList docNodeList = docElem.childNodes();
    qDebug() << "Document Node Count: " << docNodeList.count();

    int p=0;
    for(QDomNode n = docNodeList.at(0); !n.isNull(); p++,n = docNodeList.at(p))
    //for(QDomNode n = docElem.firstChild(); !n.isNull(); n = docElem.nextSibling())
    {
        qDebug()<< "Node Name: " << n.toElement().nodeName() << n.childNodes().count() ;
        if(!n.hasChildNodes()){
            QDomText t = n.toText();//
            if (!t.isNull())
                qDebug() << t.data();
        }
        else{
            QDomNodeList nodeList = n.childNodes();
            QDomElement el = nodeList.at(0).toElement();
            if(el.tagName() == "ParamName" ){
                if(el.text() == param){
                    int i=0;
                    qDebug()<< "text: " << el.text(); // prints content
                    for(i =1;i<nodeList.count();i++)
                    {
                        QDomElement el = nodeList.at(i).toElement();
                        if(el.tagName() == "ParamValue" ){
                            // replace existing node with new node
                            qDebug()<< "param value: " << el.text(); // prints content
                            retVal = el.text();
                            break;
                        }
                        else
                            continue;
                        //qDebug()<< "tag: " << el.tagName(); // prints content


                    }
                }
                else
                    continue;
            }
        }
    }

    return retVal;
}

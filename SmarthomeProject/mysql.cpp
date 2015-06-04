#include "mysql.h"
#include "arduPi.h"
#include "QNetworkInterface"

int resultSystemStatus[100][10];
int resultConfigStatus[100][20];
int resultDeviceType[100][10];

Mysql::Mysql()
{

}

int Mysql::convertQstring(QString qstring)
{
    bool convert;
    int returnValue;
    returnValue = qstring.toInt(&convert,10);
    return returnValue;
}
// get address MAC
QString Mysql::getAddressMac()
{
    QNetworkInterface *network = new QNetworkInterface();
    QString addressMAC;

    foreach(QNetworkInterface addressMac,network->allInterfaces())
    {
        addressMAC = addressMac.hardwareAddress();
    }
    //qDebug() << addressMAC;
    return addressMAC;
}



///////////////////////////////////////////////////////////////////////////
// Config Scene
//Read from config scene mysql
int Mysql::readConfigSceneMysql(void)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM `Config Scene` WHERE `Status`=0");
    query.exec();
    QSqlRecord record = query.record();
    //qDebug()<<query.next();
    int count;
    count =0;
    QString SceneNo[100],SceneName[100], SubnetID[100],RTUID[100];
    QString Channel1[100],Channel2[100] ,Channel3[100] ,Channel4[100];
    QString Channel5[100],Channel6[100] ,Channel7[100] ,Channel8[100];
    QString Status[100],DateTime[100];

    while(query.next())
    {
        count++;
        SceneNo[query.at()] =
                query.value(record.indexOf("Scene No")).toString();
        SceneName[query.at()] =
                query.value(record.indexOf("Scene Name")).toString();
        SubnetID[query.at()] =
                query.value(record.indexOf("Subnet ID")).toString();
        RTUID[query.at()] =
                query.value(record.indexOf("RTU ID")).toString();
        Channel1[query.at()] =
                query.value(record.indexOf("Channel 1")).toString();
        Channel2[query.at()] =
                query.value(record.indexOf("Channel 2")).toString();
        Channel3[query.at()] =
                query.value(record.indexOf("Channel 3")).toString();
        Channel4[query.at()] =
                query.value(record.indexOf("Channel 4")).toString();
        Channel5[query.at()] =
                query.value(record.indexOf("Channel 5")).toString();
        Channel6[query.at()] =
                query.value(record.indexOf("Channel 6")).toString();
        Channel7[query.at()] =
                query.value(record.indexOf("Channel 7")).toString();
        Channel8[query.at()] =
                query.value(record.indexOf("Channel 8")).toString();
        Status[query.at()] =
                query.value(record.indexOf("Status")).toString();
        DateTime[query.at()] =
                query.value(record.indexOf("Date Time")).toString();
        // qDebug()<<SceneName[query.at()]<<","<<SceneNo[query.at()];

    }

    int i;
    for(i=1;i<=count;i++)
    {
        resultConfigStatus[i][1]=convertQstring(SceneNo[i]);
        // result[i][2]=convertQstring(SceneName[i]);
        resultConfigStatus[i][3]=convertQstring(SubnetID[i]);
        resultConfigStatus[i][4]=convertQstring(RTUID[i]);
        resultConfigStatus[i][5]=convertQstring(Channel1[i]);
        resultConfigStatus[i][6]=convertQstring(Channel2[i]);
        resultConfigStatus[i][7]=convertQstring(Channel3[i]);
        resultConfigStatus[i][8]=convertQstring(Channel4[i]);
        resultConfigStatus[i][9]=convertQstring(Channel5[i]);
        resultConfigStatus[i][10]=convertQstring(Channel6[i]);
        resultConfigStatus[i][11]=convertQstring(Channel7[i]);
        resultConfigStatus[i][12]=convertQstring(Channel8[i]);
        resultConfigStatus[i][13]=convertQstring(Status[i]);

        //qDebug()<<resultConfigStatus[i][1];
    }

    return count;

}
//Read config scene value
int Mysql::readConfigSceneValue(int row, int column)
{
    //qDebug()<<resultConfigStatus[row][column];
    return resultConfigStatus[row][column];

}

//Write config scene to mysql
void Mysql::writeConfigSceneMysql(int SceneNo, QString SceneName, int SubnetID, int RTUID, int Channel1, int Channel2, int Channel3, int Channel4, int Channel5,
                                  int Channel6, int Channel7, int Channel8, int Status)
{
    QSqlQuery query;
    query.prepare("INSERT INTO `Config Scene`(`Scene No`, `Scene Name`, `Subnet ID`, `RTU ID`, `Channel 1`, `Channel 2`, `Channel 3`, `Channel 4`, `Channel 5`, `Channel 6`, `Channel 7`, `Channel 8`, `Status`,`Date Time`)"
                  "VALUES(:SceneNo, :SceneName, :SubnetID, :RTUID, :Channel1, :Channel2, :Channel3, :Channel4, :Channel5, :Channel6, :Channel7, :Channel8, :Status, NOW())");
    query.bindValue(":SceneNo",SceneNo);
    query.bindValue(":SceneName",SceneName);
    query.bindValue(":SubnetID",SubnetID);
    query.bindValue(":RTUID",RTUID);
    query.bindValue(":Channel1",Channel1);
    query.bindValue(":Channel2",Channel2);
    query.bindValue(":Channel3",Channel3);
    query.bindValue(":Channel4",Channel4);
    query.bindValue(":Channel5",Channel5);
    query.bindValue(":Channel6",Channel6);
    query.bindValue(":Channel7",Channel7);
    query.bindValue(":Channel8",Channel8);
    query.bindValue(":Status",Status);
    query.exec();

}



//////////////////////////////////////////////////////////
// System status
//Read data from mysql
int Mysql::readSystemStatusMysql(void)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM `System status` WHERE `Status`=0");
    query.exec();
    QSqlRecord record = query.record();
    // qDebug()<<query.next();
    int count;
    count =0;
    QString ID[100],SubnetID[100], RTUID[100],ChannelID[100];
    QString Area[100], DeviceType[100], Value[100], DateTime[100], Status[100];

    while(query.next())
    {
        count++;
        ID[query.at()] =
                query.value(record.indexOf("ID")).toString();
        SubnetID[query.at()] =
                query.value(record.indexOf("Subnet ID")).toString();
        RTUID[query.at()] =
                query.value(record.indexOf("RTU ID")).toString();
        ChannelID[query.at()] =
                query.value(record.indexOf("Channel ID")).toString();
        Area[query.at()] =
                query.value(record.indexOf("Area")).toString();
        DeviceType[query.at()] =
                query.value(record.indexOf("Device Type")).toString();
        Value[query.at()] =
                query.value(record.indexOf("Value")).toString();
        DateTime[query.at()] =
                query.value(record.indexOf("Date Time")).toString();
        Status[query.at()] =
                query.value(record.indexOf("Status")).toString();

    }

    int i;
    for(i=1;i<=count;i++)
    {
        resultSystemStatus[i][1]=convertQstring(ID[i]);
        resultSystemStatus[i][2]=convertQstring(SubnetID[i]);
        resultSystemStatus[i][3]=convertQstring(RTUID[i]);
        resultSystemStatus[i][4]=convertQstring(ChannelID[i]);
        //resultSystemStatus[i][5]=convertQstring(Area[i]);
        //resultSystemStatus[i][6]=convertQstring(DeviceType[i]);
        resultSystemStatus[i][7]=convertQstring(Value[i]);
        //result[i][8]=DateTime[i];
        resultSystemStatus[i][9]=convertQstring(Status[i]);
    }

    return count;
}
//Read system status value
int Mysql::readSystemStatusValue(int row, int column)
{
    return resultSystemStatus[row][column];

}
//Write data to systemstatus Mysql
void Mysql::writeSystemStatusMysql(int ID, int SubnetID, int RTUID, int ChannelID, QString Area, QString DeviceType,int Value,char Status)
{
    QSqlQuery query;
    query.prepare("INSERT INTO `System status`(`ID`, `Subnet ID`, `RTU ID`, `Channel ID`, `Area`, `Device Type`, `Value`,`Date Time`, `Status`)"
                  "VALUES(:ID, :SubnetID, :RTUID, :ChannelID, :Area, :DeviceType, :Value, NOW(), :Status)");
    query.bindValue(":ID",ID);
    query.bindValue(":SubnetID",SubnetID);
    query.bindValue(":RTUID",RTUID);
    query.bindValue(":ChannelID",ChannelID);
    query.bindValue(":Area",Area);
    query.bindValue(":Value",Value);
    query.bindValue(":DeviceType",DeviceType);
    query.bindValue(":Status",Status);
    query.exec();
}

//read device table
int Mysql::readDevice(void)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM `Devices Table` WHERE 1");
    query.exec();
    QSqlRecord record = query.record();
    // qDebug()<<query.next();
    int count;
    count =0;
    QString SubnetID[100], RTUID[100];
    QString DeviceType[100];

    while(query.next())
    {
        count++;
        SubnetID[query.at()] =
                query.value(record.indexOf("Subnet ID")).toString();
        RTUID[query.at()] =
                query.value(record.indexOf("RTU ID")).toString();
        DeviceType[query.at()] =
                query.value(record.indexOf("Device Type")).toString();
        //qDebug() << SubnetID[query.at()]<<" "<<RTUID[query.at()]<<" "<<DeviceType[query.at()]<< " "<< count;

    }

    int i;
    for(i=0;i<count;i++)
    {

        resultDeviceType[i][1]=convertQstring(SubnetID[i]);
        resultDeviceType[i][2]=convertQstring(RTUID[i]);
        resultDeviceType[i][3]=convertQstring(DeviceType[i]);

        //qDebug() << resultDeviceType[i][1]<<" "<<resultDeviceType[i][2]<<" "<<resultDeviceType[i][3]<< " "<< count;

    }

    return count;

}

int Mysql::readDeviceValue(int row, int colunm)
{
    return resultDeviceType[row][colunm];
}

//update config scene
void Mysql::updateConfigScene(char RTUID)
{
    QSqlQuery query;
    query.prepare("UPDATE `Config Scene` SET `Status`=1, `Date Time`=NOW() WHERE `RTU ID`= :RTUID");
    query.bindValue(":RTUID",RTUID);
    query.exec();
}

//update system status
void Mysql::updateSystemStatus(char RTUID, char ChannelID, char Value)
{
    QSqlQuery query;
    query.prepare("UPDATE `System status` SET `Value`=:Value,`Status`=1, `Date Time`=NOW() WHERE (`Channel ID`=:ChannelID)AND(`RTU ID`=:RTUID)");
    query.bindValue(":RTUID",RTUID);
    query.bindValue(":ChannelID",ChannelID);
    query.bindValue(":Value",Value);
    query.exec();
}

//create connect to mysql
void Mysql::MakeConnectMySQL(QString HostName, QString UserName, QString password, QString DatabaseName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(HostName);
    db.setUserName(UserName);
    db.setPassword(password);
    db.setDatabaseName(DatabaseName);

    if(!db.open())
    {
        qDebug() << db.lastError();
    }else
    {
        qDebug() << "Opened MYSQL Successfully";
    }

}

//Delete to mysql
void Mysql::DeleteToMysql(QString queryDelete)
{
    QSqlQuery query;
    query.prepare(queryDelete);
    query.exec();
}

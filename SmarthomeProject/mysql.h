#ifndef MYSQL_H
#define MYSQL_H

#include <QtSql>
#include <QString>
#include <arduPi.h>

class Mysql
{
private:
    int convertQstring(QString qstring);
public:
    Mysql();

    //Get address MAC
    QString getAddressMac(void);

    ////////////////////////////////////////////////
    // Config Scene
    //Read data from Config Scene
    int readConfigSceneMysql(void);
    //Read config Scene Value
    int readConfigSceneValue(int row, int column);
    //Write config scene to mysql
    void writeConfigSceneMysql(int SceneNo, QString SceneName, int SubnetID, int RTUID, int Channel1, int Channel2, int Channel3, int Channel4, int Channel5,
                               int Channel6, int Channel7, int Channel8, int Status);

    //////////////////////////////////////////////
    // System Status
    //Read system status value
    int readSystemStatusMysql(void);
    //Read system status value
    int readSystemStatusValue(int row, int column);
    //Write system status data to Mysql
    void writeSystemStatusMysql(int ID, int SubnetID, int RTUID, int ChannelID, QString Area, QString DeviceType,int Value,char Status);

    //Read device table
    int readDevice(void);

    int readDeviceValue(int row, int colunm);

    //update
    void updateConfigScene(char RTUID);

    void updateSystemStatus(char RTUID, char ChannelID, char Value);

    //Create connect to mysql
    void MakeConnectMySQL(QString HostName, QString UserName, QString password , QString DatabaseName);

    // Delete to mysql
    void DeleteToMysql(QString queryDelete);
};

#endif // MYSQL_H

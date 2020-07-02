#ifndef SETUP_H
#define SETUP_H

#include "cadc.h"
#include "cboxcar.h"
#include "cconfigfile.h"
#include "cgate.h"
#include "ctimer.h"
#include "citem.h"

struct UNITS_T
{
    int    unitNum;
    double unitSize;
    string singleName;
    string pluralName;
    string abbrevName;

    double unitLength;
    string lengthName;
};

enum SORTMODE_T
{
    //SORT_MGA
    SORT_SIMPLE,
    SORT_BATCH,
    SORT_EVEN_BATCH,
    SORT_MGA
};

enum BATCHMODE_T
{
    BATCH_WEIGHT,
    BATCH_QTY
};

enum DISP_T
{
    DISP_MAIN_DISP,
    DISP_GATE_EDIT,
    DISP_MENU_DISP,
    DISP_GATE_SETUP,
    DISP_GATE_RESET,
    DISP_GATE_ON_OFF
};

enum GATE_EDIT_T
{
    GATE_EDIT_LIM_LOWER,
    GATE_EDIT_LIM_UPPER,
    GATE_EDIT_BATCH,
    GATE_EDIT_EDIT_ALL
};

enum GATE_RESET_T
{
    GATE_RESET_CONF,
    GATE_RESET_ALL_CONF,
};

struct GATE_EVENT_T
{
    bool    enabled;

    int     gateNum;
    bool    turnOn;

    double  scheduledTime;
    double  objMass;
};

struct ITEM_HIST_T
{
    int     gateNum;
    double  mass;
};


struct BATCH_HIST_T
{
    int     gateNum;
    double  finalMass;
};
class Setup
{
public:
    Setup();
    ~Setup();
    void saveConfig(CConfigFile* cfg) const;
    bool loadConfig(const CConfigFile* cfg);
    int stepProgram();
    double switchUnits(bool change_units = true, int test_unit_num = 0);

    double round(double grams, double units, double *divisions = NULL) const;
    string roundString(double grams, double units, double *divisions = NULL) const;

    void updateGateLights();

    int loadCellDivisions;
    double loadCellMax;
    double oneDiv;
    bool settingsChanged;
    bool diagnostics;
    bool bufferEn;

    void getBeltZero();

    int totalGates();
    void setTotalGates(int gates);

    void setGateDir(bool LtoR);
    bool getGateDir();

    void setGateSides(int);
    int  getGateSides();

    void logCounts();

    void doReset();

    void printPreview(int yOffset = 0);
    void printStats();

    void sortObject(CItem* nextItem);

    int serialNum;
    int userPassword;

    double sortOpenGateDist;
    double sortDistToFirstGate;
    double sortDistBetweenGates;

    double minObjWeight;

    bool printEn;

    // printing options...
    bool printHeaders;
    bool printBatchNum;
    bool printGate;
    bool printItems;
    bool printBatchWeight;
    bool printTotalWeight;
    bool printGateRange;
    bool printUnits;
    bool printModelNum;
    bool printSerial;

    bool printerModelDataMax;
    //int  printerNum;

private:

    void scheduleObject(int gateNumber, CItem* nextItem);

    void updateOnBatchComplete(int gateNum);


    void outOfRangeItem();
    bool editGate(char key, bool editAll = false);
    void drawGate(int gateIdx, bool top = true);

    bool resetGate(char key);
    bool onOffGate(char key);

    void selectColor(int gateIdx, char* backColor, char* fontColor, GATE_STATE state = (GATE_STATE)(-1));

    void initGateSchedule();

    void printBatch(int gate);

    int  numberOfGates;
    bool gatesLeftToRight;
    bool gatesStartTop;
    int  gateSides;

    int* gateHist;

    int statsTotalItems;
    double statsTotalWeight;
    int statsTotalItemsInRange;
    int statsTotalBatches;

    int printedBatches;

    double statsLastWeight;
    int statsLastGate;
    double statsLastLength;
};

#endif // SETUP_H

const int DebugMode=1;
const float RefershCycle=1;

char *markup1;

#define _SVID_SOURCE

#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

/*==================================================*/

#define page_quantity 4

const int windowHeight=600;
const int windowWidth=900;
const int gridQuanV=100;
const int gridQuanH=100;

GtkWindow *window;
GtkNotebook *notebook;
GtkLabel *page_title[page_quantity];
char* page_title_char[page_quantity]={"首页","CPU","进程","Module",};
GtkWidget *page_table[page_quantity];

#include "creatwindow.h"

/*==================================================*/

GtkImage *logo;
GtkLabel* hostNameLabel;
GtkLabel* hostNameText;
GtkLabel* linuxSystemLabel;
GtkLabel* kernelVersionText;
GtkLabel* hardwareBriefLabel;
GtkLabel* cpuInfoText;
GtkLabel* memInfoText;
GtkLabel* authorInfo;
GtkHSeparator* hSep1;

#include "page1.h"

/*==================================================*/

GtkFrame *page2Frame[page_quantity];
char* page2FrameName[page_quantity]={"CPU Usage","Memory Usage"};


double cpu_rate;
double cput_old=0.0;
double cput_new;
double cput;
double idle_old=0.0;
double idle_new;
double idle_change;

double cpuUsage;
char cpuUsageText[512];
GtkProgress* cpuUsageProgressBar;

int memAll;
int memFree;
int swapAll;
int swapFree;
float memPercent;
float swapPercent;
GtkProgress* memProgressBar;
GtkProgress* swapProgressBar;
char memText[512];
char swapText[512];


#include "page2.h"

/*==================================================*/

int* processID=NULL;
int processNum=-1;
int R_process_num=0;
int S_process_num=0;
int D_process_num=0;
int Z_process_num=0;
int T_process_num=0;

struct PCB
{
    int PID;
    char* name;
    char* status;
    int pri;
    int UID;
    float memUseVmRSS;
    int cmdLine;
}* allProcessPCB=NULL;

GtkScrolledWindow *scrolled_window_p3;
GtkListStore *list_p3;
GtkTreeIter iter_p3;
GtkTreeView *view_p3;
GtkTreeViewColumn *pid_column,*name_column,*status_column,*pri_column,*uid_column,*mem_column,*cmd_column;
GtkCellRenderer *text_renderer_p3_1;
GtkCellRenderer *text_renderer_p3_2;
GtkCellRenderer *text_renderer_p3_3;
GtkCellRenderer *text_renderer_p3_4;
GtkCellRenderer *text_renderer_p3_5;
GtkCellRenderer *text_renderer_p3_6;
GtkCellRenderer *text_renderer_p3_7;

GtkFrame* page3Frame;
GtkLabel* page3Label;

char processOverallDataBuffer[512];

#include "page3.h"

/*==================================================*/

int moduleNum=0;

struct MODULE
{
    char name[512];
    int mem;
    char times[512];
}* allModule=NULL;

GtkScrolledWindow *scrolled_window_p4;
GtkListStore *list_p4;
GtkTreeIter iter_p4;
GtkTreeView *view_p4;
GtkTreeViewColumn *name_column_p4,*mem_column_p4,*times_column_p4;
GtkCellRenderer *text_renderer_p4_1;
GtkCellRenderer *text_renderer_p4_2;
GtkCellRenderer *text_renderer_p4_3;

GtkFrame* page4Frame;
GtkLabel* page4Label;

char moduleOverallDataBuffer[512];

#include "page4.h"

/*==================================================*/



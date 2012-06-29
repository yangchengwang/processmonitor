#include "generaldef.h"

void updateAllPages()
{
    updatePage2();
    updatePage3();
    updatePage4();
    //updatePage5();
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    creatWindow();
    page1Generate();
    page2Generate();
    page3Generate();
    page4Generate();
    //page5Generate();
    gtk_timeout_add((int)(RefershCycle*1000),updateAllPages,NULL);

    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();
    
    return 0;
}

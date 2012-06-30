void page1UpdateInfo(void)
{
}

int getInfo(char* file,char* buf,char* start,char* end)
{
    FILE *fp1=NULL;
    int i;
    char buf2[512];
    char* tempCharP=NULL;
    char* tempCharP2=NULL;
    char* tempCharP3=NULL;
    char* tempCharP4=NULL;

    for (i=0;i<512;++i)
    {
        buf[i]=0;
    }
    
    if ((start==NULL)&&(end==NULL))
    {
        fp1=fopen(file,"r");
        fgets(buf,512,fp1);
        fclose(fp1);
    }
    if (file&&start&&end)
    {
        fp1=fopen(file,"r");
        fgets(buf,512,fp1);
        while ((tempCharP=strstr(buf,start))==NULL)
        {
            tempCharP4=fgets(buf,512,fp1);
            if (!tempCharP4)
            {
                break;
            }
        }
        if (!tempCharP)
        {
            fclose(fp1);
            return 0;
        }
        tempCharP+=strlen(start);
        tempCharP2=strstr(buf,end);
        for (tempCharP3=tempCharP;tempCharP3<tempCharP2;++tempCharP3)
        {
            buf2[tempCharP3-tempCharP]=*tempCharP3;
        }
        buf2[tempCharP3-tempCharP]='\0';
        strcpy(buf,buf2);
        fclose(fp1);
    }
    if ((file==NULL)&&start&&end)
    {
        tempCharP=strstr(buf,start);
        if (!tempCharP)
        {
            return 0;
        }
        tempCharP+=strlen(start);
        tempCharP2=strstr(buf,end);
        for (tempCharP3=tempCharP;tempCharP3<tempCharP2;++tempCharP3)
        {
            buf2[tempCharP3-tempCharP]=*tempCharP3;
        }
        buf2[tempCharP3-tempCharP]='\0';
        strcpy(buf,buf2);
    }

    return 1;
}

void page1Generate(void)
{
    char buf[512];
    char buf2[512];

    logo = g_object_new(GTK_TYPE_IMAGE, "file", "logo", NULL);
    hostNameLabel = gtk_label_new("Info not ready!");
    gtk_label_set_markup(GTK_LABEL(hostNameLabel),"<span font_desc=\"Times New Bold 20\">Hostname</span>");
    hostNameText = gtk_label_new("Info not ready!");
    getInfo("/proc/sys/kernel/hostname",buf,NULL,NULL);
    gtk_label_set_label(GTK_LABEL(hostNameText),buf);
    linuxSystemLabel = gtk_label_new("Info not ready!");
    gtk_label_set_markup(GTK_LABEL(linuxSystemLabel),"<span font_desc=\"Times New Bold 20\">Linux System</span>");
    kernelVersionText = gtk_label_new("Info not ready!");
    getInfo("/proc/sys/kernel/osrelease",buf,NULL,NULL);
    sprintf(buf2,"Kernel Version: ");
    strcat(buf2,buf);
    gtk_label_set_label(GTK_LABEL(kernelVersionText),buf2);
    hardwareBriefLabel = gtk_label_new("Info not ready!");
    gtk_label_set_markup(GTK_LABEL(hardwareBriefLabel),"<span font_desc=\"Times New Bold 20\">Hardware Brief Information</span>");
    cpuInfoText = gtk_label_new("Info not ready!");
    getInfo("/proc/cpuinfo",buf,"model name	: ","\n");
    sprintf(buf2,"CPU Model Name: ");
    strcat(buf2,buf);
    gtk_label_set_label(GTK_LABEL(cpuInfoText),buf2);
    memInfoText = gtk_label_new("Info not ready!");
    getInfo("/proc/meminfo",buf,"MemTotal:      ","\n");
    sprintf(buf2,"Total Memory: ");
    strcat(buf2,buf);
    gtk_label_set_label(GTK_LABEL(memInfoText),buf2);
    hSep1=g_object_new(GTK_TYPE_HSEPARATOR, NULL);
    authorInfo = gtk_label_new("Info not ready!");
    gtk_label_set_justify(authorInfo,GTK_JUSTIFY_CENTER);
    gtk_label_set_markup(GTK_LABEL(authorInfo),"<span font_desc=\"Purisa Bold 23\" foreground=\"#000066\">Author: Atlantis and YangChengwang</span>\n<span font_desc=\"URW Chancery L 15\" style=\"italic\" foreground=\"#00AAAA\">HIT CS 2012\n Compile succeed under ubuntu 11.04</span>");

    gtk_table_attach(GTK_TABLE(page_table[0]),GTK_WIDGET(logo),
            0,gridQuanH/10,
            0,gridQuanV,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            0,0
            );
    gtk_table_attach(GTK_TABLE(page_table[0]),GTK_WIDGET(hostNameLabel),
            gridQuanH/10,gridQuanH,
            gridQuanV/100*10,gridQuanV/100*13,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            0,0
            );
    gtk_table_attach(GTK_TABLE(page_table[0]),GTK_WIDGET(hostNameText),
            gridQuanH/10,gridQuanH,
            gridQuanV/100*13,gridQuanV/100*14,
            GTK_FILL,GTK_FILL,
            0,0
            );
    gtk_table_attach(GTK_TABLE(page_table[0]),GTK_WIDGET(linuxSystemLabel),
            gridQuanH/10,gridQuanH,
            gridQuanV/100*17,gridQuanV/100*20,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            0,0
            );
    gtk_table_attach(GTK_TABLE(page_table[0]),GTK_WIDGET(kernelVersionText),
            gridQuanH/10,gridQuanH,
            gridQuanV/100*20,gridQuanV/100*21,
            GTK_FILL,GTK_FILL,
            0,0
            );
    gtk_table_attach(GTK_TABLE(page_table[0]),GTK_WIDGET(hardwareBriefLabel),
            gridQuanH/10,gridQuanH,
            gridQuanV/100*24,gridQuanV/100*27,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            0,0
            );
    gtk_table_attach(GTK_TABLE(page_table[0]),GTK_WIDGET(cpuInfoText),
            gridQuanH/10,gridQuanH,
            gridQuanV/100*27,gridQuanV/100*28,
            GTK_FILL,GTK_FILL,
            0,0
            );
    gtk_table_attach(GTK_TABLE(page_table[0]),GTK_WIDGET(memInfoText),
            gridQuanH/10,gridQuanH,
            gridQuanV/100*28,gridQuanV/100*29,
            GTK_FILL,GTK_FILL,
            0,0
            );
    gtk_table_attach(GTK_TABLE(page_table[0]),GTK_WIDGET(hSep1),
            gridQuanH/10,gridQuanH,
            gridQuanV/100*40,gridQuanV/100*41,
            GTK_FILL,GTK_FILL,
            0,0
            );
    gtk_table_attach(GTK_TABLE(page_table[0]),GTK_WIDGET(authorInfo),
            gridQuanH/10,gridQuanH,
            gridQuanV/100*55,gridQuanV/100*65,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            0,0
            );


    page1UpdateInfo();
}


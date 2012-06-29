float getCPUUsage(void)
{
    char buf[512];
    char buf2[512];
    int i,j,len,start;
    long jiff0;
    long jiff1;
    float cpuUsage;

    if (!countFlag)
    {
        getInfo("/proc/stat",buf,"cpu  ","\n");
        countFlag=1;
        for (i=0;i<7;++i)
        {
            j=0;
            while (buf[j]!=' ')
            {
                buf2[j]=buf[j];
                j++;
            }
            buf2[j]='\0';
            cpuStruct2[i]=atol(buf2);
            len=strlen(buf);
            start=j+1;
            for(j=start;j<len;++j)
            {
                buf[j-start]=buf[j];
            }
        }
        return 0;
    }

    getInfo("/proc/stat",buf,"cpu  ","\n");
    for (i=0;i<7;++i)
    {
        j=0;
        while (buf[j]!=' ')
        {
            buf2[j]=buf[j];
            j++;
        }
        buf2[j]='\0';
        cpuStruct1[i]=cpuStruct2[i];
        cpuStruct2[i]=atol(buf2);
        len=strlen(buf);
        start=j+1;
        for(j=start;j<len;++j)
        {
            buf[j-start]=buf[j];
        }
    }
    jiff0=0;
    jiff1=0;
    for (i=0;i<7;++i)
    {
        jiff0+=cpuStruct1[i];
        jiff1+=cpuStruct2[i];
    }
    cpuUsage=100-(cpuStruct2[3]-cpuStruct1[3])*1.0/(jiff1-jiff0)*100;
    return cpuUsage;
}

void getOveralLoad(char* buf)
{
    int i,j,start,len;
    char buf2[512];

    getInfo("/proc/loadavg",buf,NULL,NULL);
    for (i=0;i<3;++i)
    {
        j=0;
        while (buf[j]!=' ')
        {
            buf2[j]=buf[j];
            j++;
        }
        buf2[j]='\0';
        overallLoad[i]=atof(buf2);
        len=strlen(buf);
        start=j+1;
        for(j=start;j<len;++j)
        {
            buf[j-start]=buf[j];
        }
    }
    sprintf(buf,"Last 1 minute: %.2f; Last 5 minute: %.2f; Last 15 minute: %.2f",overallLoad[0],overallLoad[1],overallLoad[2]);
}

void getMemInfo()
{
    char buf[512];

    getInfo("/proc/meminfo",buf,"MemTotal:      "," kB");
    memAll=atoi(buf)/1024;
    getInfo("/proc/meminfo",buf,"MemFree:       "," kB");
    memFree=atoi(buf)/1024;
    memPercent=1-memFree*1.0/memAll;
    getInfo("/proc/meminfo",buf,"SwapTotal:     "," kB");
    swapAll=atoi(buf)/1024;
    getInfo("/proc/meminfo",buf,"SwapFree:      "," kB");
    swapFree=atoi(buf)/1024;
    swapPercent=1-swapFree*1.0/swapAll;
}

float getSpecNumberFromBuf(char* buf,int numNo)
{
    float returnValue=-1;
    int i,j;
    int current=0;
    int start=-1;
    int end=-1;
    char buf2[512];

    for (i=0;(i<numNo)&&(current<strlen(buf));)
    {
        if ((buf[current]>='0')&&(buf[current]<='9')&&(buf[current-1]==':'||buf[current-1]==' '||buf[current-1]=='-'||buf[current-1]=='\t'||current==0)&&((buf[current+1]>='0')&&(buf[current+1]<='9')||buf[current+1]==' '||buf[current+1]=='\t'||buf[current+1]=='\n'))
        {
            start=current;
            if (buf[current-1]=='-')
            {
                start-=1;
            }
            while ((buf[current]!=' ')&&(buf[current]!='\t'))
            {
                current++;
            }
            end=current-1;
            for (j=start;j<=end;++j)
            {
                buf2[j-start]=buf[j];
            }
            buf2[j-start]='\0';
            returnValue=atof(buf2);
            i++;
            continue;
        }
        else
        {
            current++;
            continue;
        }
    }

    return returnValue;
}

float getSpecNumberFromFile(char* filename,int lineNo,int numNo)
{
    FILE *fp1=NULL;
    int i;
    char buf[512];
    float returnValue=0;

    fp1=fopen(filename,"r");
    i=0;
    while ((i++)<lineNo)
    {
        fgets(buf,512,fp1);
    }
    fclose(fp1);

    returnValue=getSpecNumberFromBuf(buf,numNo);
    
    return returnValue;
}

void getNetInfo()
{
    FILE *fp1=NULL;
    static int flag=0;
    char buf[512];
    float rawReceive=0;
    float rawSend=0;
    static int sendLast=0;
    static int sendNow=0;
    static int receiveNow=0;
    static int receiveLast=0;

    fp1=fopen("/proc/net/dev","r");
    while (fgets(buf,512,fp1)!=NULL)
    {
        if (strchr(buf,':'))
        {
            rawReceive=rawReceive+getSpecNumberFromBuf(buf,1);
            rawSend=rawSend+getSpecNumberFromBuf(buf,9);
        }
    }
    fclose(fp1);

    netReceive=(int)(rawReceive/1024);
    netSend=(int)(rawSend/1024);

    receiveNow=netReceive;
    sendNow=netSend;

    if (flag)
    {
        receiveSpeed=(receiveNow-receiveLast)*1.0/RefershCycle;
        sendSpeed=(sendNow-sendLast)*1.0/RefershCycle;
        receiveLast=receiveNow;
        sendLast=sendNow;
    }
    else
    {
        flag=1;
        receiveSpeed=0;
        sendSpeed=0;
    }
}

void updatePage2(void)
{
    getOveralLoad(overallLoadLabelBuffer);
    markup1 = g_markup_printf_escaped ("<span font_desc=\"URW Palladio Bold 15\">Last 1 minute:</span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\"> %.2f</span><span font_desc=\"URW Palladio Bold 15\">; Last 5 minutes:</span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\"> %.2f</span><span font_desc=\"URW Palladio Bold 15\">; Last 15 minutes:</span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\"> %.2f</span>", overallLoad[0],overallLoad[1],overallLoad[2]);
    gtk_label_set_markup (GTK_LABEL (avgLoadLabel), markup1);
    g_free (markup1);

    cpuUsage=getCPUUsage();
    gtk_progress_set_percentage(GTK_PROGRESS(cpuUsageProgressBar),(double)(cpuUsage/100));
    sprintf(cpuUsageText,"CPU usage is %.2f%%",cpuUsage);
    g_object_set(cpuUsageProgressBar,"text", cpuUsageText,NULL);

    getMemInfo();
    gtk_progress_set_percentage(GTK_PROGRESS(memProgressBar),(double)(memPercent));
    sprintf(memText,"Total physical memory is %d MB, %.2f%% are currently used",memAll,memPercent*100);
    g_object_set(memProgressBar,"text", memText,NULL);
    gtk_progress_set_percentage(GTK_PROGRESS(swapProgressBar),(double)(swapPercent));
    sprintf(swapText,"Total swap memory is %d MB, %.2f%% are currently used",swapAll,swapPercent*100);
    g_object_set(swapProgressBar,"text", swapText,NULL);

    getNetInfo();
    markup1 = g_markup_printf_escaped ("<span font_desc=\"URW Palladio Bold 15\">Network totally receive </span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\">%dKB</span><span font_desc=\"URW Palladio Bold 15\"> data till now, totally send </span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\">%dKB</span><span font_desc=\"URW Palladio Bold 15\"> data till now\n\nNetwork receive speed is </span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\">%.2fKB/s</span><span font_desc=\"URW Palladio Bold 15\">, send speed is </span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\">%.2fKB/s</span>",netReceive,netSend,receiveSpeed,sendSpeed);
    gtk_label_set_markup (GTK_LABEL (net1), markup1);
    g_free (markup1);
}

void page2Generate(void)
{
    int i;

    cpuStruct1=malloc(7*sizeof(int));
    cpuStruct2=malloc(7*sizeof(int));

    for (i=0;i<4;++i)
    {
        page2Frame[i]=g_object_new(GTK_TYPE_FRAME, "label", page2FrameName[i], NULL);
        gtk_table_attach(GTK_TABLE(page_table[1]),GTK_WIDGET(page2Frame[i]),
            0,gridQuanH,
            i*(gridQuanV/4),(i+1)*(gridQuanV/4),
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            10,7
            );
    }

    getOveralLoad(overallLoadLabelBuffer);
    avgLoadLabel=gtk_label_new(overallLoadLabelBuffer);
    markup1 = g_markup_printf_escaped ("<span font_desc=\"URW Palladio Bold 15\">Last 1 minute:</span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\"> %.2f</span><span font_desc=\"URW Palladio Bold 15\">; Last 5 minutes:</span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\"> %.2f</span><span font_desc=\"URW Palladio Bold 15\">; Last 15 minutes:</span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\"> %.2f</span>", overallLoad[0],overallLoad[1],overallLoad[2]);
    gtk_label_set_markup (GTK_LABEL (avgLoadLabel), markup1);
    g_free (markup1);
    gtk_table_attach(GTK_TABLE(page_table[1]),GTK_WIDGET(avgLoadLabel),
            0,gridQuanH,
            0,gridQuanV/4,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            10,20
            );

    cpuUsageProgressBar = g_object_new(GTK_TYPE_PROGRESS_BAR, NULL);
    cpuUsage=getCPUUsage();
    gtk_progress_set_percentage(GTK_PROGRESS(cpuUsageProgressBar),(double)(cpuUsage/100));
    sprintf(cpuUsageText,"CPU usage is %.2f%%",cpuUsage);
    g_object_set(cpuUsageProgressBar,"text", cpuUsageText,NULL);
    gtk_table_attach(GTK_TABLE(page_table[1]),GTK_WIDGET(cpuUsageProgressBar),
            0,gridQuanH,
            gridQuanV/4,gridQuanV/2,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            50,50
            );

    getMemInfo();
    memProgressBar = g_object_new(GTK_TYPE_PROGRESS_BAR, NULL);
    gtk_progress_set_percentage(GTK_PROGRESS(memProgressBar),(double)(memPercent));
    sprintf(memText,"Total physical memory is %d MB, %.2f%% are currently used",memAll,memPercent*100);
    g_object_set(memProgressBar,"text", memText,NULL);
    gtk_table_attach(GTK_TABLE(page_table[1]),GTK_WIDGET(memProgressBar),
            0,gridQuanH,
            gridQuanV/2+10,gridQuanV/2+gridQuanV/8,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            50,8
            );
    swapProgressBar = g_object_new(GTK_TYPE_PROGRESS_BAR, NULL);
    gtk_progress_set_percentage(GTK_PROGRESS(swapProgressBar),(double)(swapPercent));
    sprintf(swapText,"Total swap memory is %d MB, %.2f%% are currently used",swapAll,swapPercent*100);
    g_object_set(swapProgressBar,"text", swapText,NULL);
    gtk_table_attach(GTK_TABLE(page_table[1]),GTK_WIDGET(swapProgressBar),
            0,gridQuanH,
            gridQuanV/2+gridQuanV/8,gridQuanV/4*3-10,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            50,8
            );

    getNetInfo();
    sprintf(bufNet,"Network totally receive %dKB data till now, totally send %dKB data till now.\n\nNetwork receive speed is %.2fKB/s, send speed %.2fKB/s.",netReceive,netSend,receiveSpeed,sendSpeed);
    net1 = gtk_label_new(bufNet);
    gtk_label_set_justify(GTK_LABEL(net1),GTK_JUSTIFY_CENTER);
    markup1 = g_markup_printf_escaped ("<span font_desc=\"URW Palladio Bold 15\">Network totally receive </span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\">%dKB</span><span font_desc=\"URW Palladio Bold 15\"> data till now, totally send </span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\">%dKB</span><span font_desc=\"URW Palladio Bold 15\"> data till now\n\nNetwork receive speed is </span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\">%.2fKB/s</span><span font_desc=\"URW Palladio Bold 15\">, send speed is </span><span font_desc=\"URW Palladio Bold Italic 15\" foreground=\"#880000\">%.2fKB/s</span>",netReceive,netSend,receiveSpeed,sendSpeed);
    gtk_label_set_markup (GTK_LABEL (net1), markup1);
    g_free (markup1);
    gtk_table_attach(GTK_TABLE(page_table[1]),GTK_WIDGET(net1),
            0,gridQuanH,
            gridQuanV/4*3,gridQuanV,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            50,20
            );
}


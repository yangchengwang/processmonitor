void getProcessID()
{
    struct dirent **namelist;
    int n=0;

    n = scandir("/proc", &namelist, 0, alphasort);
    if (processID)
    {
        free(processID);
    }
    processID=malloc(n*sizeof(int));
    processNum=0;
    while (n--)
    {
        if (((namelist[n]->d_name)[0]>='0')&&((namelist[n]->d_name)[0]<='9'))
        {
            processID[processNum++]=atoi(namelist[n]->d_name);
        }
        free(namelist[n]);
    }
    free(namelist);
}

void getUID(char* currentFilename,char* buf)
{
    FILE* fp1;
    int i;
    int uid;

    fp1=fopen(currentFilename,"r");
    for (i=0;i<7;++i)
    {
        fgets(buf,512,fp1);
    }
    uid=(int)(getSpecNumberFromBuf(buf,1));
    sprintf(buf,"%d",uid);
    fclose(fp1);
}

void buildAllPCB(void)
{
    static char currentFilename[512];
    int i;
    static char buf[512];

    R_process_num=0;
    S_process_num=0;
    D_process_num=0;
    Z_process_num=0;
    T_process_num=0;

    getProcessID();
    
    if (allProcessPCB)
    {
        free(allProcessPCB);
    }
    allProcessPCB=malloc(processNum*sizeof(struct PCB));

    for (i=0;i<processNum;++i)
    {
        sprintf(currentFilename,"/proc/%d/status",processID[i]);
        
        if (!getInfo(currentFilename,buf,"Pid:	","\n"))
        {
            allProcessPCB[i].PID=-1;
        }
        else
        {
            allProcessPCB[i].PID=atoi(buf);
        }

        if (!getInfo(currentFilename,buf,"Name:	","\n"))
        {
            allProcessPCB[i].name=NULL;
        }
        else
        {
            allProcessPCB[i].name=malloc(512*sizeof(char));
            strcpy(allProcessPCB[i].name,buf);
        }

        if (!getInfo(currentFilename,buf,"State:	","\n"))
        {
            allProcessPCB[i].status=NULL;
        }
        else
        {
            allProcessPCB[i].status=malloc(512*sizeof(char));
            strcpy(allProcessPCB[i].status,buf);
            switch (buf[0])
            {
                case 'R':R_process_num++;break;
                case 'S':S_process_num++;break;
                case 'D':D_process_num++;break;
                case 'Z':Z_process_num++;break;
                case 'T':T_process_num++;break;
            };
        }

        getUID(currentFilename,buf);
        if (!buf)
        {
            allProcessPCB[i].UID=-1;
        }
        else
        {
            allProcessPCB[i].UID=atoi(buf);
        }

        if (!getInfo(currentFilename,buf,"VmRSS:	    "," kB"))
        {
            allProcessPCB[i].memUseVmRSS=-1;
        }
        else
        {
            allProcessPCB[i].memUseVmRSS=atof(buf);
        }

        sprintf(currentFilename,"/proc/%d/cmdline",processID[i]);
        if (!getInfo(currentFilename,buf,NULL,NULL))
        {
            allProcessPCB[i].cmdLine=NULL;
        }
        else
        {
            allProcessPCB[i].cmdLine=malloc(512*sizeof(char));
            strcpy(allProcessPCB[i].cmdLine,buf);
        }

        sprintf(currentFilename,"/proc/%d/stat",processID[i]);
        if (!getInfo(currentFilename,buf,NULL,NULL))
        {
            allProcessPCB[i].pri=-10000;
        }
        else
        {
            allProcessPCB[i].pri=(int)(getSpecNumberFromBuf(buf,16));
        }
    }
}

void updatePage3()
{
    int i;
    static char buf[7][512];
    static int flag=0;
    
    buildAllPCB();

    if (flag)
    {
        markup1 = g_markup_printf_escaped ("<span font_desc=\"URW Gothic L Bold 17\">In total, %d process exists.</span><span font_desc=\"URW Gothic L 12\">\n--------------------------------------------------------------------------------\n%d process in runnable(R) state\n%d process in sleeping(S) state\n%d process in uninterruptible sleep(D) state\n%d process in zombile(Z) state\n%d process in traced or stopped(T) state</span>"
            ,processNum,R_process_num,S_process_num,D_process_num,Z_process_num,T_process_num);
        gtk_label_set_markup (GTK_LABEL (page3Label), markup1);
        g_free (markup1);
    }

    flag=1;
    
    gtk_list_store_clear(GTK_LIST_STORE(list_p3));

    /* put data into the list */
    for (i = 0; i < processNum; i++)
    {
        if (allProcessPCB[i].PID==-1)
        {
            sprintf(buf[0],"N/A   ");
        }
        else
        {
            sprintf(buf[0],"%d   ",allProcessPCB[i].PID);
        }

        if (allProcessPCB[i].name==NULL)
        {
            sprintf(buf[1],"N/A   ");
        }
        else
        {
            sprintf(buf[1],"%s   ",allProcessPCB[i].name);
        }

        if (allProcessPCB[i].status==NULL)
        {
            sprintf(buf[2],"N/A   ");
        }
        else
        {
            sprintf(buf[2],"%s   ",allProcessPCB[i].status);
        }

        if (allProcessPCB[i].UID==-1)
        {
            sprintf(buf[4],"N/A   ");
        }
        else
        {
            sprintf(buf[4],"%d   ",allProcessPCB[i].UID);
        }

        if (allProcessPCB[i].memUseVmRSS==-1)
        {
            sprintf(buf[5],"N/A   ");
        }
        else
        {
            sprintf(buf[5],"%.1f   ",allProcessPCB[i].memUseVmRSS);
        }

        if (allProcessPCB[i].cmdLine==NULL)
        {
            sprintf(buf[6],"N/A   ");
        }
        else
        {
            sprintf(buf[6],"%s   ",allProcessPCB[i].cmdLine);
        }

        if (allProcessPCB[i].pri==-10000)
        {
            sprintf(buf[3],"N/A   ");
        }
        else
        {
            sprintf(buf[3],"%d   ",allProcessPCB[i].pri);
        }

        gtk_list_store_append(list_p3, &iter_p3);
        gtk_list_store_set(list_p3, &iter_p3,
                0,buf[0],
                1,buf[1],
                2,buf[2],
                3,buf[3],
                4,buf[4],
                5,buf[5],
                6,buf[6],
                -1);
    }
}

page3Generate()
{
    /* create a 7−column list */
    list_p3 = gtk_list_store_new(7,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);

    updatePage3();

    /* create tree view for the list */
    view_p3 = g_object_new(GTK_TYPE_TREE_VIEW,
            "model", list_p3,
            "reorderable", TRUE,
            NULL);
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(view_p3),TRUE);

    /* create and initialize text renderer for cells */
    text_renderer_p3_1 = gtk_cell_renderer_text_new();
    g_object_set(text_renderer_p3_1, "font", "URW Gothic L","weight",1500,NULL);
    text_renderer_p3_2 = gtk_cell_renderer_text_new();
    g_object_set(text_renderer_p3_2, "font", "URW Gothic L",NULL);
    text_renderer_p3_3 = gtk_cell_renderer_text_new();
    g_object_set(text_renderer_p3_3, "font", "URW Gothic L",NULL);
    text_renderer_p3_4 = gtk_cell_renderer_text_new();
    g_object_set(text_renderer_p3_4, "font", "URW Gothic L",NULL);
    text_renderer_p3_5 = gtk_cell_renderer_text_new();
    g_object_set(text_renderer_p3_5, "font", "URW Gothic L",NULL);
    text_renderer_p3_6 = gtk_cell_renderer_text_new();
    g_object_set(text_renderer_p3_6, "font", "URW Gothic L",NULL);
    text_renderer_p3_7 = gtk_cell_renderer_text_new();
    g_object_set(text_renderer_p3_7, "font", "Purisa",NULL);

    /* create column views */
    pid_column = gtk_tree_view_column_new_with_attributes("PID",text_renderer_p3_1,"text", 0,NULL);
    g_object_set(pid_column,
            "resizable", TRUE,
            "clickable", FALSE,
            "reorderable", TRUE,
            NULL);
    name_column = gtk_tree_view_column_new_with_attributes("Name",text_renderer_p3_2,"text", 1,NULL);
    g_object_set(name_column,
            "resizable", TRUE,
            "clickable", FALSE,
            "reorderable", TRUE,
            NULL);
    status_column = gtk_tree_view_column_new_with_attributes("Status",text_renderer_p3_3,"text", 2,NULL);
    g_object_set(status_column,
            "resizable", TRUE,
            "clickable", FALSE,
            "reorderable", TRUE,
            NULL);
    pri_column = gtk_tree_view_column_new_with_attributes("Priority",text_renderer_p3_4,"text", 3,NULL);
    g_object_set(pri_column,
            "resizable", TRUE,
            "clickable", FALSE,
            "reorderable", TRUE,
            NULL);
    uid_column = gtk_tree_view_column_new_with_attributes("UID",text_renderer_p3_5,"text", 4,NULL);
    g_object_set(uid_column,
            "resizable", TRUE,
            "clickable", FALSE,
            "reorderable", TRUE,
            NULL);
    mem_column = gtk_tree_view_column_new_with_attributes("Physical Memory Usage\nin KB (VmRSS)",text_renderer_p3_6,"text", 5,NULL);
    g_object_set(mem_column,
            "resizable", TRUE,
            "clickable", FALSE,
            "reorderable", TRUE,
            NULL);
    cmd_column = gtk_tree_view_column_new_with_attributes("Command Line",text_renderer_p3_7,"text", 6,NULL);
    g_object_set(cmd_column,
            "resizable", TRUE,
            "clickable", FALSE,
            "reorderable", TRUE,
            NULL);

    /* insert columns into the view */
    gtk_tree_view_append_column(view_p3,pid_column);
    gtk_tree_view_append_column(view_p3,name_column);
    gtk_tree_view_append_column(view_p3,status_column);
    gtk_tree_view_append_column(view_p3,pri_column);
    gtk_tree_view_append_column(view_p3,uid_column);
    gtk_tree_view_append_column(view_p3,mem_column);
    gtk_tree_view_append_column(view_p3,cmd_column);

    

    /* put everything into a scrolled window */
    scrolled_window_p3 = g_object_new(GTK_TYPE_SCROLLED_WINDOW, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window_p3), GTK_WIDGET(view_p3));
    gtk_table_attach(GTK_TABLE(page_table[2]),GTK_WIDGET(scrolled_window_p3),
            0,gridQuanH,
            0,gridQuanV/4*3,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            10,10
            );

    page3Frame=g_object_new(GTK_TYPE_FRAME, "label", "Process Overall Views", NULL);
    gtk_table_attach(GTK_TABLE(page_table[2]),GTK_WIDGET(page3Frame),
            0,gridQuanH,
            gridQuanV/4.0*3.7,gridQuanV,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            10,5
            );

    sprintf(processOverallDataBuffer,"In total, %d process exists.\n====================\n%d process in runnable(R) state\n%d process in sleeping(S) state\n%d process in uninterruptible sleep(D) state\n%d process in zombile(Z) state\n%d process in traced or stopped(T) state\n===================="
            ,processNum,R_process_num,S_process_num,D_process_num,Z_process_num,T_process_num);
    page3Label=gtk_label_new(processOverallDataBuffer);
    gtk_label_set_justify(GTK_LABEL(page3Label),GTK_JUSTIFY_CENTER);
    markup1 = g_markup_printf_escaped ("<span font_desc=\"URW Palladio Bold 10\">In total, %d process exists.\n====================\n%d process in runnable(R) state\n%d process in sleeping(S) state\n%d process in uninterruptible sleep(D) state\n%d process in zombile(Z) state\n%d process in traced or stopped(T) state\n====================</span>"
            ,processNum,R_process_num,S_process_num,D_process_num,Z_process_num,T_process_num);
    gtk_label_set_markup (GTK_LABEL (page3Label), markup1);
    g_free (markup1);
    gtk_table_attach(GTK_TABLE(page_table[2]),GTK_WIDGET(page3Label),
            0,gridQuanH,
            gridQuanV/4.0*3.7,gridQuanV,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            10,0
            );
}



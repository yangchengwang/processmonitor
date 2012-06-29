void getModuleInfo()
{
    FILE* fp1;
    char buf3[512];
    int i,j;

    moduleNum=0;

    fp1=fopen("/proc/modules","r");
    while(fgets(buf3,512,fp1))
    {
        moduleNum++;
    }
    fclose(fp1);

    if (allModule)
    {
        free(allModule);
    }
    allModule=malloc(moduleNum*sizeof(struct MODULE));

    fp1=fopen("/proc/modules","r");
    for (i=0;i<moduleNum;++i)
    {
        fgets(buf3,512,fp1);

        j=0;
        while(buf3[j]!=' ')
        {
            (allModule[i].name)[j]=buf3[j];
            j++;
        }
        (allModule[i].name)[j]='\0';

        allModule[i].mem=(int)(getSpecNumberFromBuf(buf3,1));

        j=(int)(getSpecNumberFromBuf(buf3,2));
        sprintf(allModule[i].times,"%d",j);
    }
    fclose(fp1);
}

void updatePage4()
{
    int i;
    static char buf4[3][512];
    static int flag2=0;

    getModuleInfo();

    if (flag2)
    {
        markup1 = g_markup_printf_escaped ("<span font_desc=\"URW Gothic L Bold 20\">In total, %d modules installed into the kernel.</span>",moduleNum);
        gtk_label_set_markup (GTK_LABEL (page4Label), markup1);
        g_free (markup1);
    }

    flag2=1;

    gtk_list_store_clear(GTK_LIST_STORE(list_p4));

    /* put data into the list */
    for (i = 0; i < moduleNum; i++)
    {
        if (allModule[i].name==NULL)
        {
            sprintf(buf4[0],"N/A   ");
        }
        else
        {
            sprintf(buf4[0],"%s   ",allModule[i].name);
        }

        if (allModule[i].mem==-1)
        {
            sprintf(buf4[1],"N/A   ");
        }
        else
        {
            sprintf(buf4[1],"%d   ",allModule[i].mem);
        }

        if (allModule[i].times==NULL)
        {
            sprintf(buf4[2],"N/A   ");
        }
        else
        {
            sprintf(buf4[2],"%s   ",allModule[i].times);
        }
        
        gtk_list_store_append(list_p4, &iter_p4);
        gtk_list_store_set(list_p4, &iter_p4,
                0,buf4[0],
                1,buf4[1],
                2,buf4[2],
                -1);
    }
}

page4Generate()
{
    /* create a 3−column list */
    list_p4 = gtk_list_store_new(3,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);

    updatePage4();

    /* create tree view for the list */
    view_p4 = g_object_new(GTK_TYPE_TREE_VIEW,
            "model", list_p4,
            "reorderable", TRUE,
            NULL);
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(view_p4),TRUE);

    /* create and initialize text renderer for cells */
    text_renderer_p4_1 = gtk_cell_renderer_text_new();
    g_object_set(text_renderer_p4_1, "font", "URW Gothic L","weight",1500,NULL);
    text_renderer_p4_2 = gtk_cell_renderer_text_new();
    g_object_set(text_renderer_p4_2, "font", "URW Gothic L",NULL);
    text_renderer_p4_3 = gtk_cell_renderer_text_new();
    g_object_set(text_renderer_p4_3, "font", "URW Gothic L",NULL);

    /* create column views */
    name_column_p4 = gtk_tree_view_column_new_with_attributes("Name",text_renderer_p4_1,"text", 0,NULL);
    g_object_set(name_column_p4,
            "resizable", TRUE,
            "clickable", FALSE,
            "reorderable", TRUE,
            NULL);
    mem_column_p4 = gtk_tree_view_column_new_with_attributes("Size",text_renderer_p4_2,"text", 1,NULL);
    g_object_set(mem_column_p4,
            "resizable", TRUE,
            "clickable", FALSE,
            "reorderable", TRUE,
            NULL);
    times_column_p4 = gtk_tree_view_column_new_with_attributes("Used times by other modules",text_renderer_p4_3,"text", 2,NULL);
    g_object_set(times_column_p4,
            "resizable", TRUE,
            "clickable", FALSE,
            "reorderable", TRUE,
            NULL);

    /* insert columns into the view */
    gtk_tree_view_append_column(view_p4,name_column_p4);
    gtk_tree_view_append_column(view_p4,mem_column_p4);
    gtk_tree_view_append_column(view_p4,times_column_p4);



    /* put everything into a scrolled window */
    scrolled_window_p4 = g_object_new(GTK_TYPE_SCROLLED_WINDOW, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window_p4), GTK_WIDGET(view_p4));
    gtk_table_attach(GTK_TABLE(page_table[3]),GTK_WIDGET(scrolled_window_p4),
            0,gridQuanH,
            0,gridQuanV/10*9,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            10,10
            );

    page4Frame=g_object_new(GTK_TYPE_FRAME, "label", "Modules Overall View", NULL);
    gtk_table_attach(GTK_TABLE(page_table[3]),GTK_WIDGET(page4Frame),
            0,gridQuanH,
            gridQuanV/10*9,gridQuanV,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            10,5
            );

    sprintf(moduleOverallDataBuffer,"In total, %d modules loads into kernel",moduleNum);
    page4Label=gtk_label_new(moduleOverallDataBuffer);
    gtk_label_set_justify(GTK_LABEL(page4Label),GTK_JUSTIFY_CENTER);
    markup1 = g_markup_printf_escaped ("<span font_desc=\"URW Palladio Bold 17\">In total, %d modules loads into kernel</span>",moduleNum);
    gtk_label_set_markup (GTK_LABEL (page4Label), markup1);
    g_free (markup1);
    gtk_table_attach(GTK_TABLE(page_table[3]),GTK_WIDGET(page4Label),
            0,gridQuanH,
            gridQuanV/10*9,gridQuanV,
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            10,0
            );
}
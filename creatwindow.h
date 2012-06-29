void end_program(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

void creatWindow(void)
{
    int i;

    window = g_object_new(GTK_TYPE_WINDOW,"title", "Process Monitor","default_height", windowHeight,"default_width", windowWidth,NULL);
    gtk_container_border_width(GTK_CONTAINER(window),10);
    notebook = g_object_new(GTK_TYPE_NOTEBOOK, NULL);
    for (i=0;i<page_quantity;++i)
    {
        page_title[i]=g_object_new(GTK_TYPE_LABEL, "label", page_title_char[i], NULL);
        page_table[i] = gtk_table_new(gridQuanV,gridQuanH,FALSE);
    }

    g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

    gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(notebook));
    for (i=0;i<page_quantity;++i)
    {
        gtk_notebook_append_page(GTK_NOTEBOOK(notebook), GTK_WIDGET(page_table[i]), GTK_WIDGET(page_title[i]));
    }
}

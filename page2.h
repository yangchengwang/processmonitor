double stod(char * s,int size){
	int i = 0;
	int state = 0;
	double station = 1;
	double result = 0;
	while((s[i] != '\0') && ( i < size)){
		if(s[i] == '-' && i == 0){
			i++;
			continue;
		}
		else if(s[i] == '.'){
			if( state == 0)
			state=1;
			else if( state == 1) {
				state = -1;
				break;
			}
		}
		
		else if(s[i] >= '0' && s[i] <= '9') {
			if(state == 0)
				result = result * 10 + (double)s[i] - (double)'0';
			else if(state == 1) {
				result += ((double)s[i] - (double)'0')/pow(10,station);
				station+=1;
			}
		}
		else{
			state=-1;
			break;
		}
		i++;	
	}
	if(state==-1){
		printf("Not number!\n");
		return 0;
	}
	else {
		if(s[0]=='-')
			result = -result;
		return result;	
	}
}
double  get_cput(){
	char name[10];
	char user[10];
	char nice[10];
	char system[10];
	char idle[10];
	char iowait[10];
	char irq[10];
	char softirq[10];
	char stealstolen[10];
	char guest[10];
	char unknown[10];
	FILE * stat;
	if((stat=fopen("/proc/stat","r"))!=NULL){
		char buffer[255];
		fgets(buffer,255,stat);
		sscanf(buffer,"%s%s%s%s%s%s%s%s%s%s%s",name,user,nice,system,idle,iowait,irq,softirq,stealstolen,guest,unknown);
		if(!strcmp(name,"cpu")){
			cput_new=stod(user,strlen(user))+stod(nice,strlen(nice))+stod(system,strlen(system))+stod(idle,strlen(idle))+stod(iowait,strlen(iowait))+stod(irq,strlen(irq))+stod(softirq,strlen(softirq))+stod(stealstolen,strlen(stealstolen))+stod(guest,strlen(guest))+stod(unknown,strlen(unknown));
			idle_new=stod(idle,strlen(idle));
			idle_change=idle_new-idle_old;
			
			cput=cput_new-cput_old;
			
			cput_old=cput_new;
			idle_old=idle_new;
			//printf("%f/%f\n",idle_change,cput);
		}
		fclose(stat);
	}
	cpu_rate=100-idle_change/cput*100;
	return cpu_rate;
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





void updatePage2(void)
{

    cpuUsage=get_cput();
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

   
}

void page2Generate(void)
{
    int i;

    cpuStruct1=malloc(7*sizeof(int));
    cpuStruct2=malloc(7*sizeof(int));

    for (i=0;i<2;++i)
    {
        page2Frame[i]=g_object_new(GTK_TYPE_FRAME, "label", page2FrameName[i], NULL);
        gtk_table_attach(GTK_TABLE(page_table[1]),GTK_WIDGET(page2Frame[i]),
            0,gridQuanH,
            i*(gridQuanV/2),(i+1)*(gridQuanV/2),
            GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,
            10,7
            );
    }

    
    cpuUsageProgressBar = g_object_new(GTK_TYPE_PROGRESS_BAR, NULL);
    cpuUsage=get_cput();
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

    
}


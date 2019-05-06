    #include <sys/stat.h>
    #include <stdio.h>
     
    int main() {
    	
        struct stat statStruct;
        char * dirname;
        dirname = "/home/mitesh/WorkSpace_Mitesh/NO_Document/Final_SODD";
     
        stat(dirname, &statStruct);
     
        if (!S_ISDIR(statStruct.st_mode)) {
            printf("%s does not exists.\n", dirname);
        } else {
            printf("%s exist.\n", dirname);
        }
        
        return 0;
    }

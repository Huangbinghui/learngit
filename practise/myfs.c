#include "std.h"
#include "myfs.h"

const char USERNAME[] = "hbh";

unsigned char *myvhard;
useropen openfilelist[MAXOPENFILE];
int curdirid;//指向用户当前打开文件在文件表中的id

unsigned char *blockaddr[BLOCKNUM];
block0 initblock;
fat fat1[BLOCKNUM],fat2[BLOCKNUM];



char str[SIZE];

int main(){
    int fd;
    char command[DIRLEN << 1];

    startsys();

    printf("%s %s:",USERNAME,openfilelist[curdirid].dir);
    while (~scanf("%s",command))
    {
        if (!strcmp(command,"exit"))
        {
            break;
        }
        else if (!strcmp(command,"ls"))
        {
            my_ls();
        }
        else if (!strcmp(command,"mkdir"))
        {
            scanf("%s",command);
            my_mkdir(command);
        }
        else if (!strcmp(comand,"close"))
        {
            scnaf("%d",&fd);
            my_close(fd);
        }
        else if (!strcmp(command,"open"))
        {
            scanf("%s",command);
            fd = my_open(command);
            if (0 <= fd && fd < MAXOPENFILE) {}
            {
                if (!openfilelist[fd].open_fcb.attribute)
                {
                    my_close(fd);
                    printf("%s is directory, can not open as a file.\n")
                }else
                {
                    printf("%s is open, it\'s id is %d\n",openfilelist[fd].dir,fd);
                }
            }
        }

        else if (!strcmp(command,"cd"))
        {
            scanf("%s",command);
            my_cd(command);
        }

        else if (!strcmp(command."create"))
        {
            scanf("%s",command);
            fd = my_create(command);
            if (0 < fd && fd < MAXOPENFILE)
            {
                printf("%s is created, it\'s id is %d\n",openfilelist[fd]);
            }
            
        }
        else if (!strcmp(command,"rm"))
        {
            scanf("%s",command);
            my_rm(command);
        }
        else if (!strcmp(command,"rmdir"))
        {
            scanf("%s",command);
            my_rmdir(command);
        }
        else if (!strcmp(command,"read"))
        {
            scanf("%d",&fd);
            my_read(fd);
        }
        else if (!strcmp(command,"write"))
        {
            scanf("%d",&fd);
            my_write(fd);
        }
        else if (!strcmp(command,"sf"))
        {
            for (int i = 0; i < MAXOPENFILE; ++i)
            {
                if (openfilelist[i].t_openfile)
                {
                    printf("%d : %s\n",i,openfilelist[i].dir);
                }
                
            }
            
        }
        else if (!strcmp(command,"format"))
        {
            scanf("%s",command);
            my_format();
        }
        else
        {
            printf("command %s: no such command!",command);
        }

        my_reload(curdirid);

        printf("%s %s:",USERNAME,openfilelist[curdirid].dir); 
    }
    my_exitsys();
    return 0;
}


int strcmp(const char* a,const char *b,int len) {
    for (int i = 0; i < len; ++i)
    {
        if (a[i] != b[i])
        {
            if (a[i] < b[i])
            {
                return -1;
            }else
            {
                return 1;
            }
        }
        
    }
    return 0;
}

void fcb_init(fcb *new_fcb, const char *filename,unsigned short first,unsigned char attribute) {
    strcpy(new_fcb->filename,filename);
    new_fcb->first = first;
    new_fcb->attribute = attribute;
    new_fcb->free = 0;
    if (attribute) new_fcb->length = 0;
    else new_fcb->length = 2 * sizeof fcb;
}

void fatFree(int id) {
    if (id == END) return;
    if (fat1[id] != END) fatFree(fat1[id].id);
    fat1[id].id = FREE;
}

int getFreeFatId() {
    for (int i = 0; i < MAXOPENFILE; ++i) if (fat1[i].id == FREE) return i;
    return END;
}

int getFreeOpenlist() {
    for (int i = 0; i < MAXOPENFILE; ++i) if (!openfilelist[i].t_openfile) return i;
    return -1;
}

int read_ls(int fd,unsigned char *text,int len){
    int tcount = openfilelist[fd].count;
    openfilelist[fd].count = 0;
    int ret = do_read(fd,text,len);
    openfilelist[fd].count = tcount;
    return ret;
}

void startsys() {
    myvhard = (unsigned char*)malloc(SIZE);
    for (int i = 0; i < BLOCKNUM; ++i) blockaddr[i] = i*BLOCKSIZE + myvhard;
    for (int i = 0; i < MAXOPENFILE; ++i) openfilelist[i].t_openfile = 0;

    FILE *fd = fopen("myfsys","rb");
    char need_format = 0;

    if (fd != NULL) 
    {
        unsigned char* buf = (unsigned char*)malloc(SIZE);
        fread(buf, 1, SIZE, fd);
        memcpy(myvhard, buf, SIZE);
        memcpy(&initblock, blockaddr[0], sizeof block0);
        if (strcmp(initblock.info,"10101010") != 0) need_format = 1;
        free(buf);
        fclose(fd);
    }
    else
    {
        need_format = 1;
    }
    
    if (!need_format) {
        memcpy(fat1, blockaddr[1], sizeof fat1);
        memcpy(fat2, blockaddr[3], sizeof fat2);
    }
    else
    {
        printf("myfsys 文件系统不存在，现在开始创建文件系统。\n");
        my_format();
    }
    
    curdirid = 0;
    memcpy(&openfilelist[curdirid].open_fcb,blockaddr[5],sizeof fcb);
#ifdef DEBUG_INFO
    printf("starsys:%s\n",openfilelist[curdirid].open_fcb.filename);
#endif //DEBUG_INFO
    useropen_init(&openfilelist[curdirid],5,0,"~/");
}

int fat_read(unsigned short id,unsigned char)
#define FREE 0
#define DIRLEN 80
#define END 65535
#define SIZE 1024000
#define BLOCKNUM 1000
#define BLOCKSIZE 1024
#define MAXOPENFILE 10
#define ROOTBLOCKNUM 2


#define SAYERROR printf("ERROR:")
#define max(X,Y) (((X) > (Y)) ? (X) : (Y))
#define min(X,Y) (((X) < (Y)) ? (X) : (Y))

#define GRN "DIR:"
#define RESET "\x1B[0m"

typedef struct FCB
{
    /* FCB结构体 */

    char free;//用来标记FCB是否已被删除。
    char exname[3];
    char filename[DIRLEN];
    unsigned short time;
    unsigned short data;
    unsigned short first;//文件起始盘块号
    unsigned long length;//文件实际长度
    unsigned char attribute;//文件属性字段：0表示目录文件，1表示数据文件
} fcb;

typedef struct FAT
{
    unsigned short id;
}fat;

//目录的fcb，length字段等于本结构体的count字段。
//文件的fcb的count会根据打开方式不同、读写方式不同而不同。
typedef struct USEROPEN
{
    fcb open_fcb;//fcb文件的内容
    int count;//读写指针在文件中的位置
    int dirno;//相应打开文件的目录项在父目录中的盘块号
    int diroff;//相应打开文件的目录项在父目录文件的dirno盘块中起始位置
    char fcb_state;//是否修改了文件的FCB内容，修改置1，没有置0
    char t_openfile;//表示用户打开的表项是否为空。0为空，1为已被打开。
    char dir[DIRLEN];//打开文件的绝对路径名。
}useropen;

typedef struct BLOCK0
{
    unsigned short root;
    char info[200];
    unsigned char *start_block;
}block0;

int my_write(int fd);
int my_open(char *filename);
int my_create(char *filename);
int my_read(int fd);
int do_read(int fd,unsigned char *text,int len);

//直接针fat内存块写入，属于do_read子函数
int fat_write(unsigned short id,unsigned char *text, int block_offset,int len);
//对fd指向的打开文件的文件指针处填上text中len长度字节，只会改变打开的fcb信息，不会改变打开文件的文件指针信息。
int do_write(int fd,unsigned char *text,int len);
void my_reload(int fd);

//创建一个文件然后由my_mkdir和my_create继续处理
int my_touch(char *filename,int attribute,int *rpafd);


void my_ls();
void startsys();
void my_format();
void my_exitsys();
void my_save(int fd);
void my_close(int fd);

void my_cd(char *dir_name);
void my_rm(char *filename);
void my_mkdir(char *dir_name);
void my_rmdir(char *dir_name);

//初始化打开文件项，需要先把fcb拷贝进openfile的fcb中，其文件指针默认为其所占磁盘大小
void useropen_init(useropen *openfile,int dirno,int diroff,const char *dir);

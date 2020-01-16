#define MESSAGE_LENGTH 100
#define SERVER_SENDER "server-client"
#define SERVER_READER "client-server"

enum TypeM {
    REGULAR_MESSAGE = 1,
    SESION_OFF = 255
};

typedef struct mymsgbuf
{
    long mtype;
    char mtext[MESSAGE_LENGTH]; 
} Msgbuf;

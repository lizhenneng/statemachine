#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define DEBUG_MODE 1

void debug_output(const char *format, ...) {
  va_list args;

  va_start(args, format);
  if (DEBUG_MODE) {
    vfprintf(stderr, format, args);
  }
}

typedef enum
{
    GET_UP,
    GO_TO_SCHOOL,
    HAVE_LUNCH,
    DO_HOMEWORK,
    SLEEP,
}STATE_TYPE;

typedef enum
{
    EVENT1 = 1,
    EVENT2,
    EVENT3,
}EVENT_TYPE;

typedef struct FsmTable_s
{
    EVENT_TYPE event;   //事件
    STATE_TYPE CurState;  //当前状态
    void (*eventActFun)();  //函数指针
    STATE_TYPE NextState;  //下一个状态
}FsmTable_t;

typedef struct
{
	STATE_TYPE curState;
	FsmTable_t *FsmTable;
}FSM_s;

void GetUp() 	 	{ debug_output("Exec GetUp()\n"); 		}
void Go2School() 	{ debug_output("Exec Go2School()\n"); 	}
void HaveLunch()	{ debug_output("Exec HaveLunch()\n"); 	}
void DoHomework() 	{ debug_output("Exec DoHomework()\n"); 	}
void Go2Bed()		{ debug_output("Exec Go2Bed()\n"); 		}

FsmTable_t XiaoMingTable[] =
{
    //{到来的事件，当前的状态，将要要执行的函数，下一个状态}
    { EVENT2,  SLEEP,           GetUp,        GET_UP },
    { EVENT1,  GET_UP,          Go2School,    GO_TO_SCHOOL },
    { EVENT2,  GO_TO_SCHOOL,    HaveLunch,    HAVE_LUNCH },
    { EVENT3,  HAVE_LUNCH,      DoHomework,   DO_HOMEWORK },
    { EVENT1,  DO_HOMEWORK,     Go2Bed,       SLEEP },

    //add your codes here
};
int g_max_num = sizeof(XiaoMingTable) / sizeof(FsmTable_t);
/*状态机注册*/
void FSM_Regist(FSM_s* pFsm, FsmTable_t* pTable)
{
    pFsm->FsmTable = pTable;
}
/*状态机初始化*/
void InitFsm(FSM_s *pFsm)
{	
	pFsm->curState = SLEEP;
	FSM_Regist(pFsm, XiaoMingTable);
}
/*状态迁移*/
void FSM_StateTransfer(FSM_s* pFsm, STATE_TYPE state)
{
    pFsm->curState = state;
}

/*事件处理*/
void FSM_EventHandle(FSM_s* pFsm, EVENT_TYPE event)
{
    FsmTable_t* pActTable = pFsm->FsmTable;
    void (*eventActFun)() = NULL;  //函数指针初始化为空
    STATE_TYPE NextState;
    STATE_TYPE CurState = pFsm->curState;
    int flag = 0; //标识是否满足条件
    int i;

    /*获取当前动作函数*/
    for (i = 0; i<g_max_num; i++)
    {
        //当且仅当当前状态下来个指定的事件，我才执行它
        if (event == pActTable[i].event && CurState == pActTable[i].CurState)
        {
            flag = 1;
            eventActFun = pActTable[i].eventActFun;
            NextState = pActTable[i].NextState;
            break;
        }
    }


    if (flag) //如果满足条件了
    {
        /*动作执行*/
        if (eventActFun)
        {
            eventActFun();
        }

        //跳转到下一个状态
        FSM_StateTransfer(pFsm, NextState);
    }
    else
    {
        // do nothing
    }
}

void getEvent(EVENT_TYPE* event)
{
	*event = rand()%3+1;
}
int main(int argc, char *argv[])
{
    FSM_s fsm;
    InitFsm(&fsm);
    EVENT_TYPE event = EVENT1; 
    unsigned long count = 1000000000;
    if(argc > 1) count = atoi(argv[1]);
    while (count--)
    {
        debug_output("event %d is coming at current state %d...\n", event, fsm.curState);
        FSM_EventHandle(&fsm, event);
        debug_output("fsm current state %d\n", fsm.curState);
        getEvent(&event);
#if DEBUG_MODE
	sleep(1);
#endif
    }

    return 0;
}



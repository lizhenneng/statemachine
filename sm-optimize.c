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
    MAX_STATES
}STATE_TYPE;

typedef enum
{
    EVENT1 = 1,
    EVENT2,
    EVENT3,
    MAX_EVENTS
}EVENT_TYPE;

typedef struct FsmTable_s
{
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

FsmTable_t XiaoMingTable[MAX_STATES][MAX_EVENTS];

/*状态机注册*/
void FSM_Regist(FSM_s* pFsm, FsmTable_t *pTable)
{
    pFsm->FsmTable = pTable;
}
/*状态机初始化*/
void InitFsm(FSM_s *pFsm)
{	
	int i, j;
	for(i = 0; i < MAX_STATES; i++)
		for(j = 0; j < MAX_EVENTS; j++)
		{
			XiaoMingTable[i][j].eventActFun = NULL;
			XiaoMingTable[i][j].NextState = -1;
		}
	XiaoMingTable[SLEEP][EVENT2].eventActFun 	= GetUp;
	XiaoMingTable[SLEEP][EVENT2].NextState	 	= GET_UP;
	XiaoMingTable[GET_UP][EVENT1].eventActFun 	= Go2School;
	XiaoMingTable[GET_UP][EVENT1].NextState 	= GO_TO_SCHOOL;
	XiaoMingTable[GO_TO_SCHOOL][EVENT2].eventActFun = HaveLunch;
	XiaoMingTable[GO_TO_SCHOOL][EVENT2].NextState	= HAVE_LUNCH;
	XiaoMingTable[HAVE_LUNCH][EVENT3].eventActFun	= DoHomework;
	XiaoMingTable[HAVE_LUNCH][EVENT3].NextState 	= DO_HOMEWORK;
	XiaoMingTable[DO_HOMEWORK][EVENT1].eventActFun	= Go2Bed;
	XiaoMingTable[DO_HOMEWORK][EVENT1].NextState	= SLEEP;

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
	if( NULL != (pFsm->FsmTable+pFsm->curState*MAX_EVENTS+event)->eventActFun && -1 != (pFsm->FsmTable+pFsm->curState*MAX_EVENTS+event)->NextState ) {
		(pFsm->FsmTable+pFsm->curState*MAX_EVENTS+event)->eventActFun();
	        //跳转到下一个状态
        	FSM_StateTransfer(pFsm, (pFsm->FsmTable+pFsm->curState*MAX_EVENTS+event)->NextState);
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



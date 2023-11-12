#include "snake.h"
#include "vga.h"
#include "seg.h"
#include "stdlib.h"
#include "delay.h"
#include "timer.h"

//#define	Snake_FOOD					randseed()//生成随机数函数(暂定，后续修改)
 
_border_limit 	border_limit;		//游戏边界信息
_food_Info 		food_Info[5];	        //生成食物信息
//_obstacle_Info  obstacle_Info,obstacle_Info2,obstacle_Info3,obstacle_Info4;
_snake_Info 	snake_Info;			//蛇的身体信息
_snake_Info 	snake_Info_next;	//蛇下一坐标信息（存放蛇下一步要走的位置）

//extern u16 gImage_cs[60][80];
extern uint32_t VGA_clear_finish;   //vga清屏结束标志
extern u8 keyFlag;					//按键值传递
extern u8 startGame;                //游戏开始标志
u16 speed = 300;						//游戏进行速度
 
struct List* snake_list;		// 创建双向循环链表
 
 
//双向链表结构体，节点
struct Node
{
	u16 x;//蛇身所在x轴坐标
	u16 y;//蛇身所在y轴坐标
	struct Node* left;//前驱指针
	struct Node* right;//后继指针
 
};
  
//创建节点
struct Node* createNode( u16 x, u16 y )
{
	struct Node* newNode = (struct Node*)malloc( sizeof(struct Node) );
	newNode->x = x;
	newNode->y = y;
	newNode->left = newNode->right = NULL;
	
	return newNode;
};
 
//链表
//用指针来存储链表哪一个节点是第一、最后一个节点
struct List
{
	int size;
	
	struct Node* firstNode;		//第一个节点
	struct Node* lastNode;		//最后一个节点
};
 
 
//创造双向链表 
static struct List* createList()
{
	//申请内存
	struct List* list = (struct List*)malloc( sizeof(struct List) );
	//初始化变量
	list->size = 0;
	list->firstNode = list->lastNode = NULL;
	
	return list;
}
 
 
//求大小,判断是否为NULL
static u8 listSize( struct List* list )
{
	return list->size;
}
static u8 emptyList( struct List* list )
{
	if( list->size == 0 )
		return 0;
	else
		return 1;
}

//检查链表中数据是否有与传入的数据相同，0 没有相同的数据，1 有相同的数据
static u8 listCheck(  struct List* list, u16 x, u16 y )
{
	if( list->size == 0 ){}
		//Snake_printf( "无法比较，链表为空\n" );
	else
	{
		struct Node* pMove = list->firstNode;		//创建移动链表指针
		list->lastNode->right = NULL;				//断开双向循环链表,否则一直循环
		while( pMove )
		{
			if( pMove->x == x && pMove->y == y )
			{
				list->lastNode->right = list->firstNode;
				return 1;							//重新建立双向循环链表,并返回1
			}
			else
				pMove = pMove->right;				//向链表尾部遍历
		}
 
		list->lastNode->right = list->firstNode;	//重新建立双向循环链表,并返回0
		return 0;
	}
	return NULL;
}
 
 
//打印链表
static void printfList( struct List* list )
{
	if( list->size == 0 ){}
		//Snake_printf( "无法打印，链表为空\n" );
	else
	{
		struct Node* pMove = list->firstNode;
		list->lastNode->right = NULL;				//断开双向循环链表
		while( pMove )
		{
			//Snake_printf( "%d, %d \n", pMove->x, pMove->y );
			pMove = pMove->right;
		}
		//Snake_printf( "打印完成\n\n" );
		list->lastNode->right = list->firstNode;	//重新建立双向循环链表
	}
}
 
 
//销毁双向链表
static void ListDestory( struct List* list )
{
	if( list->size == 0){}
		//Snake_printf( "无法销毁，链表为空\n" );
	else
	{
		struct Node* pMove = list->firstNode;
		list->lastNode->right = NULL;	//断开双向循环链表
		while( pMove )
		{
			//Snake_printf( "%d, %d  销毁\n", pMove->x, pMove->y );
			free( pMove );
			pMove = pMove->right;
		}
		//Snake_printf( "销毁完成，链表已为空\n\n" );
	}
}
 
 
//表头插入
static void InsertNodeByHead( struct List* list, u16 x, u16 y )
{
	struct Node* newNode = createNode( x, y );
 
	if( list->firstNode == NULL )		//如果链表为空，则头尾节点都是新节点
	{
		list->lastNode = newNode;
	}
	else	//否则则头节点的左边是新节点，新节点右边是头节点
	{
		list->firstNode->left = newNode;
		newNode->right = list->firstNode;
	}
	
	list->firstNode = newNode;					//设新节点为头节点
	list->firstNode->left = list->lastNode;		//建立表头表尾循环
	list->lastNode->right = list->firstNode;
	
	list->size++;
}
 
 
//表尾插入
static void InsertNodeByTail( struct List* list, u16 x, u16 y )
{
	struct Node* newNode = createNode( x, y );
 
	if( list->lastNode == NULL )		//如果链表为空，则头尾节点都是新节点
	{
		list->firstNode = newNode;
	}
	else	//否则则尾节点的右边是新节点，新节点左边是尾节点
	{
		list->lastNode->right = newNode;
		newNode->left = list->lastNode;
	}
	
	list->lastNode = newNode;					//设新节点为尾节点
	list->firstNode->left = list->lastNode;		//建立表头表尾循环
	list->lastNode->right = list->firstNode;
	
	list->size++;
}
 
 
//双向链表移动，原表尾移动为新表头，并重新赋值	
static void ListTailMoveToHead( struct List* list, u16 x, u16 y )
{
	list->firstNode = list->lastNode;			//把表头赋值为现表尾
	list->lastNode = list->lastNode->left;		//把表尾重新赋值为现表尾的左边那个节点
 
	list->firstNode->x = x;		//对新表头重新赋值
	list->firstNode->y = y;
}
 
/*                                                                       */ 
// 本地MS粗略延时函数，减少移植时对外部文件依赖；
static void Delay_ms( u32 ms )
{
    ms=ms*6500;                  
    for(u32 i=0; i<ms; i++);	// 72MHz系统时钟下，多少个空循环约耗时1ms
}

uint16_t randseed()
{
	static uint16_t seed = 1;
	seed +=1;
	if(seed == 1000)
	{
		seed =0;
	}
	return seed;
}
/*                                                                        */

/*****************************************************************
 * 函  数： Snake_Draw_Erase_OneJoint
 * 功  能： 蛇的一个关节/食物绘画，一个关节/食物擦除 用于蛇初始化过程
 * 参  数： u16 sX, sY			起点坐标	
 *          u16 color			绘画，或者擦除成的颜色	
 *
 * 返回值： 无
 *
 * 备  注： 
******************************************************************/
static void Snake_Draw_Erase_OneJoint( u16 sX, u16 sY, u16 color )
{
	u16 sX2, sY2;
 
	sX2 = sX + SNAKE_LENGTH_PIXELS-1;
	sY2 = sY + SNAKE_LENGTH_PIXELS-1;
 
	Snake_LCD_Fill( sX, sY, sX2, sY2, color );
}
 
 
/*****************************************************************
 * 函  数： Snake_Draw_Erase
 * 功  能： 绘画，擦除蛇一个关节的一格像素 用于蛇移动过程中
 * 参  数： u16 sX, sY			起点坐标	
 *          u16 color			绘画，或者擦除成的颜色	
 *
 * 返回值： 无
 *
 * 备  注： 为了蛇移动画面显示流畅，所以一个关节画一个像素头边擦除一个尾边
******************************************************************/
static void Snake_Draw_Erase(  u16 sX, u16 sY, u8 dir, u16 color )
{
	u16 sX2 = sX, sY2 = sY;
 
	if( dir == 3 || dir == 4 )
		sY2 = sY + SNAKE_LENGTH_PIXELS-1;
	else
		sX2 = sX + SNAKE_LENGTH_PIXELS-1;
	
	Snake_LCD_Fill( sX, sY, sX2, sY2, color );
}
 
 
/*****************************************************************
 * 函  数： Snake_Update_HeadXY
 * 功  能： 更新蛇头显示，显示蛇头;在移动的下一格画出蛇头
 * 参  数： u8 i		显示蛇头关节的第几个像素块	
 *
 * 返回值： 无
 *
 * 备  注：
******************************************************************/
static void Snake_Update_HeadXY( u8 i )
{
	if( snake_Info.sHeadDir == 4 )				//蛇头向右
		Snake_Draw_Erase( snake_Info_next.sHeadx + i, snake_Info_next.sHeady, snake_Info.sHeadDir, SNAKE_HEAD_COLOR );
 
	else if( snake_Info.sHeadDir == 3 )			//蛇头向左
		Snake_Draw_Erase( snake_Info_next.sHeadx + SNAKE_MOVE_DISTANCE - i -1, snake_Info_next.sHeady, snake_Info.sHeadDir, SNAKE_HEAD_COLOR );
 
	else if( snake_Info.sHeadDir == 2 )			//蛇头向下
		Snake_Draw_Erase(  snake_Info_next.sHeadx, snake_Info_next.sHeady + i, snake_Info.sHeadDir, SNAKE_HEAD_COLOR );
 
	else if( snake_Info.sHeadDir == 1 )			//蛇头向上
		Snake_Draw_Erase(  snake_Info_next.sHeadx, snake_Info_next.sHeady + SNAKE_MOVE_DISTANCE - i -1, snake_Info.sHeadDir, SNAKE_HEAD_COLOR );
}
 
 
/*****************************************************************
 * 函  数： Snake_Update_EndXY
 * 功  能： 更新蛇尾显示，擦除蛇尾
 * 参  数： u8 i		擦除蛇尾关节的第几个像素块	
 *
 * 返回值： 无
 *
 * 备  注：
******************************************************************/
//static void Snake_Update_EndXY( u8 i )		//更新蛇尾显示，擦除蛇尾
//{	
//	u16 x_offest = border_limit.yLeftLimit + SNAKE_LENGTH_PIXELS*(1+SNAKE_INIT_LENGTH) + SNAKE_LENGTH_PIXELS + 1;
//	u16 y_offest = border_limit.xUpLimit + SNAKE_LENGTH_PIXELS + 1;
//	if( snake_list->lastNode->y == snake_list->lastNode->left->y )			// 蛇尾如果和上一节蛇身在同一y轴，则是往左右走
//	{
//		if( snake_list->lastNode->left->x - snake_list->lastNode->x > 0 )
//			{	// 如果蛇尾的上一节蛇身x坐标减去蛇尾x坐标大于0，则是左走
//				//u8 re_color = VGA_Rd_Color(snake_Info.sEndx + i,snake_Info.sEndy);
//				Snake_Draw_Erase( snake_Info.sEndx + i, snake_Info.sEndy, 4, gImage_cs[snake_Info.sEndy][snake_Info.sEndx + i]);
//			}				//擦除蛇尾
//		else
//		{			// 反之右走
//			//u8 re_color = VGA_Rd_Color(snake_Info.sEndx + SNAKE_MOVE_DISTANCE - i -1,snake_Info.sEndy);
//			Snake_Draw_Erase( snake_Info.sEndx + SNAKE_MOVE_DISTANCE - i -1, snake_Info.sEndy, 3, gImage_cs[snake_Info.sEndy][snake_Info.sEndx + SNAKE_MOVE_DISTANCE - i -1]);	//擦除蛇尾
//		}
//	}
//	else																	// 蛇尾如果和上一节蛇身在同一x轴，则是往左右走
//	{
//		if( snake_list->lastNode->left->y - snake_list->lastNode->y > 0 )	// 如果蛇尾的上一节蛇身y坐标减去蛇尾y坐标大于0，则是左走下走
//		{
//			//u8 re_color = VGA_Rd_Color(snake_Info.sEndx , snake_Info.sEndy + i);
//			Snake_Draw_Erase( snake_Info.sEndx , snake_Info.sEndy + i, 2, gImage_cs[snake_Info.sEndy + i][snake_Info.sEndx]);		//擦除蛇尾
//		}
//		else																// 反之上走
//		{
//			//u8 re_color = VGA_Rd_Color(snake_Info.sEndx, snake_Info.sEndy + SNAKE_MOVE_DISTANCE - i -1);
//			Snake_Draw_Erase( snake_Info.sEndx, snake_Info.sEndy + SNAKE_MOVE_DISTANCE - i -1, 1, gImage_cs[snake_Info.sEndy + SNAKE_MOVE_DISTANCE - i -1][snake_Info.sEndx]);	//擦除蛇尾
//		}
//	}
//}

static void Snake_Update_EndXY( u8 i )		//更新蛇尾显示，擦除蛇尾
{	
	u16 x_offest = border_limit.yLeftLimit + SNAKE_LENGTH_PIXELS*(1+SNAKE_INIT_LENGTH) + SNAKE_LENGTH_PIXELS + 1;
	u16 y_offest = border_limit.xUpLimit + SNAKE_LENGTH_PIXELS + 1;
	if( snake_list->lastNode->y == snake_list->lastNode->left->y )			// 蛇尾如果和上一节蛇身在同一y轴，则是往左右走
	{
		if( snake_list->lastNode->left->x - snake_list->lastNode->x > 0 )
			{	// 如果蛇尾的上一节蛇身x坐标减去蛇尾x坐标大于0，则是左走
				//u8 re_color = VGA_Rd_Color(snake_Info.sEndx + i,snake_Info.sEndy);
				Snake_Draw_Erase( snake_Info.sEndx, snake_Info.sEndy, 4, BLACK);
			}				//擦除蛇尾
		else
		{			// 反之右走
			//u8 re_color = VGA_Rd_Color(snake_Info.sEndx + SNAKE_MOVE_DISTANCE - i -1,snake_Info.sEndy);
			Snake_Draw_Erase( snake_Info.sEndx , snake_Info.sEndy, 3, BLACK);	//擦除蛇尾
		}
	}
	else																	// 蛇尾如果和上一节蛇身在同一x轴，则是往左右走
	{
		if( snake_list->lastNode->left->y - snake_list->lastNode->y > 0 )	// 如果蛇尾的上一节蛇身y坐标减去蛇尾y坐标大于0，则是左走下走
		{
			//u8 re_color = VGA_Rd_Color(snake_Info.sEndx , snake_Info.sEndy + i);
			Snake_Draw_Erase( snake_Info.sEndx , snake_Info.sEndy, 2, BLACK);		//擦除蛇尾
		}
		else																// 反之上走
		{
			//u8 re_color = VGA_Rd_Color(snake_Info.sEndx, snake_Info.sEndy + SNAKE_MOVE_DISTANCE - i -1);
			Snake_Draw_Erase( snake_Info.sEndx, snake_Info.sEndy, 1, BLACK);	//擦除蛇尾
		}
	}
}
 
 
/*****************************************************************
 * 函  数： Snake_Update_BodyXY
 * 功  能： 更新蛇身坐标
 * 参  数：
 *
 * 返回值： 无
 *
 * 备  注：蛇每移动完一个关节，更新一次蛇身坐标
******************************************************************/
static void Snake_Update_BodyXY(void)
{	
	if( snake_list->size < snake_Info.sLength )			//如果吃到食物，则只增加表头
		InsertNodeByHead( snake_list, snake_Info_next.sHeadx, snake_Info_next.sHeady );
	else
		//更新蛇头蛇尾链表坐标，消除原蛇尾坐标，把链表表尾移到表头，并赋值为新蛇头坐标
		ListTailMoveToHead( snake_list, snake_Info_next.sHeadx, snake_Info_next.sHeady );	
	
	snake_Info.sHeadx = snake_Info_next.sHeadx;		//更新现蛇头坐标
	snake_Info.sHeady = snake_Info_next.sHeady;	
	
	snake_Info.sEndx = snake_list->lastNode->x;		//更新现蛇尾坐标
	snake_Info.sEndy = snake_list->lastNode->y;
}
 
 
/*****************************************************************
 * 函  数： Snake_Display_Init
 * 功  能： 蛇的信息及显示初始化
 * 参  数：
 *
 * 返回值： 无
 *
 * 备  注：如果改蛇初始化位置及方向状态则在这改
******************************************************************/
static void Snake_Display_Init(void)
{
	//Snake_printf( "*****! 正在初始化蛇体信息 !*****\n" );
	u8 i = 0;
	snake_Info.sHeadDir = 4;		//初始化蛇头朝向（1上 2下 3左 4右）
	snake_Info.sEndDir = 4;			//初始化蛇尾朝向（1上 2下 3左 4右）
	snake_Info.sLife = 1;			//初始化蛇生命
	snake_Info.sFraction = 0;		//得分初始化
	for(i=0;i<5;i++)
	{food_Info[i].validity = 0;}			//食物初始化
	
	snake_Info.sLength = SNAKE_INIT_LENGTH + 1 ;			//蛇身长度
	
	//蛇头蛇尾坐标初始化(一开始的位置在左上角，往右走)
	snake_Info.sHeadx = border_limit.yLeftLimit + SNAKE_LENGTH_PIXELS*(1+SNAKE_INIT_LENGTH) + SNAKE_LENGTH_PIXELS + 1; 
	snake_Info.sHeady = border_limit.xUpLimit + SNAKE_LENGTH_PIXELS + 1;
	snake_Info.sEndx = snake_Info.sHeadx;
	snake_Info.sEndy = snake_Info.sHeady;
	
	snake_Info_next.sHeadx = snake_Info.sHeadx;						
	snake_Info_next.sHeady = snake_Info.sHeady;	
	
	snake_list = createList();		// 创建双向循环链表
	InsertNodeByHead( snake_list, snake_Info.sHeadx, snake_Info.sHeady );		//表头插入蛇头位置
 
	//增加蛇头（此关节是必需的）
	Snake_Draw_Erase_OneJoint( snake_Info.sHeadx, snake_Info.sHeady, SNAKE_HEAD_COLOR );
	
	//增加 SNAKE_INIT_LENGTH 个关节
	for( int count=1; count <= SNAKE_INIT_LENGTH; count++ )
	{
		snake_Info.sEndx -= SNAKE_LENGTH_PIXELS;
		InsertNodeByTail( snake_list, snake_Info.sEndx, snake_Info.sEndy );		//表尾循环插入蛇身数据
		Snake_Draw_Erase_OneJoint( snake_Info.sEndx, snake_Info.sEndy, SNAKE_COLOR );	//添加蛇身
	}
}
 
 
/*****************************************************************
 * 函  数： Border_Display_Init
 * 功  能： 游戏边界初始化及显示
 * 参  数：
 *
 * 返回值： 无
 *
 * 备  注：
******************************************************************/
static void Border_Display_Init(void)
{
	//Snake_printf("*****! 正在初始化游戏界面 !*****\n");
 
	u8 col1,col2,col3,col4,row1,row2,row3,row4;
	
	border_limit.xUpLimit = BORDER_LIMIT_UP;
	border_limit.xDownLimit = BORDER_LIMIT_DOWN;	
	border_limit.yLeftLimit = BORDER_LIMIT_LEFT;
	border_limit.yRightLimit = BORDER_LIMIT_RIGHT;
	
	//游戏界面检查（界面长宽距离必须要是蛇关节长度的倍数）
	if( (BORDER_LIMIT_DOWN - BORDER_LIMIT_UP  - 1) % SNAKE_LENGTH_PIXELS != 0 ||
	    (BORDER_LIMIT_RIGHT - BORDER_LIMIT_LEFT - 1) % SNAKE_LENGTH_PIXELS != 0 )
	{
		//Snake_printf("xxxxx! 游戏界面初始化失败 !xxxxx\n");
		while(1);
	}
	
	Snake_LCD_DrawRectangle	( border_limit.yLeftLimit,
							  border_limit.xUpLimit,  
							  border_limit.yRightLimit, 
						      border_limit.xDownLimit, BORDER_WIDTH, BORDER_COLOR );
	
	
	col1 = rand()%21+15;
	row1 = rand()%15+5;
	VGA_DRAW_LINE(col1,row1,col1+4,row1,WHITE);
	VGA_DRAW_LINE(col1,row1,col1,row1+4,WHITE);
	col2 = rand()%29+45;
	row2 = rand()%20+6;
	VGA_DRAW_LINE(col2-4,row2,col2,row2,WHITE);
	VGA_DRAW_LINE(col2,row2,col2,row2+4,WHITE);
	col3 = rand()%30+6;
	row3 = rand()%19+35;
	VGA_DRAW_LINE(col3,row3,col3+4,row3,WHITE);
	VGA_DRAW_LINE(col3,row3-4,col3,row3,WHITE);
	col4 = rand()%29+45;
	row4 = rand()%19+35;
	VGA_DRAW_LINE(col4-4,row4,col4,row4,WHITE);
	VGA_DRAW_LINE(col4,row4-4,col4,row4,WHITE);
	
	
	//Snake_printf("*****! 游戏界面初始化完成 !*****\n\n");
	
}
 
 
/*****************************************************************
 * 函  数： Snake_HTW
 * 功  能： 判断蛇下一步是否撞墙
 * 参  数：
 *
 * 返回值： 0死亡	1正常
 *
 * 备  注：
******************************************************************/
static u8 Snake_HTW(void)
{
	u8 row, col;
	
//	if( snake_Info_next.sHeady <= border_limit.xUpLimit   || 
//		snake_Info_next.sHeady >= border_limit.xDownLimit ||
//		snake_Info_next.sHeadx <= border_limit.yLeftLimit ||
//		snake_Info_next.sHeadx >= border_limit.yRightLimit )
//	{
//		//Snake_printf("触碰边界，死亡！\n");
//		
//		//显示撞墙蛇头
//		Snake_Draw_Erase_OneJoint( snake_Info.sHeadx, snake_Info.sHeady, SNAKE_DIE_COLOR );
//		
//		col = ( BORDER_LIMIT_RIGHT - BORDER_LIMIT_LEFT - 1 - 9*12 )/2 + 9;
//		row = ( BORDER_LIMIT_DOWN - BORDER_LIMIT_UP - 1 )/2 + 16;
//		
//		//Snake_LCD_String( col, row,100, 24, 12, "Game Over" );
//        Snake_LCD_String(1,1,60,24,WHITE,BLACK,12,"GAME OVER");
//		//更新蛇生命状态
//		snake_Info.sLife = 0;
//	}
//	
//	return snake_Info.sLife;
	u8 rc_color;
	rc_color = VGA_Rd_Color(snake_Info_next.sHeadx,snake_Info_next.sHeady);
	if(rc_color == WHITE)
	{
		//显示撞墙蛇头
		Snake_Draw_Erase_OneJoint( snake_Info.sHeadx, snake_Info.sHeady, SNAKE_DIE_COLOR );

        Snake_LCD_String(1,1,60,24,WHITE,BLACK,12,"GAME OVER");
		//更新蛇生命状态
		snake_Info.sLife = 0;
	}
	
	return snake_Info.sLife;
}
 
 
/*****************************************************************
 * 函  数： Snake_TouchMyself
 * 功  能： 判断蛇是否吃到自己
 * 参  数：
 *
 * 返回值： 0死亡	1正常
 *
 * 备  注：
******************************************************************/
static u8 Snake_TouchMyself(void)
{
	u8 snakeSta = 1, row, col;
		
	//遍历链表数据，检查蛇头是否碰到蛇身，因为返回值1为有重复值，所以1-运算，赋值为0
	snakeSta = 1 - listCheck( snake_list, snake_Info_next.sHeadx, snake_Info_next.sHeady );
	
	if(snakeSta == 0)
	{	
		//Snake_printf("吃到自己，死亡！\n");
		
		//显示吃到自己的蛇头
		Snake_Draw_Erase_OneJoint( snake_Info.sHeadx, snake_Info.sHeady, SNAKE_DIE_COLOR );
		
		col = ( BORDER_LIMIT_RIGHT - BORDER_LIMIT_LEFT - 1 - 9*12 )/2 + 9;
		row = ( BORDER_LIMIT_DOWN - BORDER_LIMIT_UP - 1 )/2 + 16;
		
		Snake_LCD_String(1,1,60,24,WHITE,BLACK,12,"GAME OVER");
		
		//更新蛇生命状态
		snake_Info.sLife = 0;
	}	
	
	return snake_Info.sLife;
}
 
 
/*****************************************************************
 * 函  数： Snake_Move
 * 功  能： 蛇身移动
 * 参  数：
 *
 * 返回值： 0死亡	1正常
 *
 * 备  注：
******************************************************************/
static u8 Snake_Move(void)
{
	u8 upDateSnakeEnd = 0;	//更新尾巴标志位（0更新，1不更新）
		
	//刷新分数界面
	//Snake_LCD_ShowNum( border_limit.yRightLimit/2, border_limit.xUpLimit-30, snake_Info.sFraction, 3, 12 );
	Sz_Dsiplay(snake_Info.sFraction);
	//判断蛇下一步坐标是否会撞墙
	u8 SnakeSta = Snake_HTW();
	if( SnakeSta == 0 )
		return SnakeSta;	
 
	//判断先导坐标是否会吃到自己
	SnakeSta = Snake_TouchMyself();
	if( SnakeSta == 0 )
		return SnakeSta;
	
	//判断蛇下一步坐标是否与食物一样，代表吃到食物，则此次将不删除尾巴
	u8 rc_color;
	rc_color = VGA_Rd_Color(snake_Info_next.sHeadx,snake_Info_next.sHeady);
	if(rc_color == FOOD_COLOR)
	{upDateSnakeEnd = 1;}
	//判断蛇下一步坐标是否与食物一样，代表吃到食物，则此次将不删除尾巴
//	if( snake_Info_next.sHeadx == food_Info.xFood && snake_Info_next.sHeady == food_Info.yFood )
//		upDateSnakeEnd = 1;	
	
	//更新蛇的显示，为了显示流畅，选择显示一格蛇头消除一格蛇尾，循环一节蛇身次
	for( int i=0; i<SNAKE_MOVE_DISTANCE; i++ )
	{
		//Snake_LCD_DrawPoint( snake_Info.sHeadx, snake_Info.sHeady, SNAKE_COLOR );//补充蛇头移动前位置，以蛇身的颜色填充
		Snake_Draw_Erase_OneJoint(snake_Info.sHeadx, snake_Info.sHeady, SNAKE_COLOR);
		Snake_Update_HeadXY( i );		//更新蛇头	
		
		if( upDateSnakeEnd == 0 )		//如果吃到食物则不更新蛇尾
			Snake_Update_EndXY( i );
		
		Delay_ms( speed );				//延时
	}
	//snake_Info	
	//Snake_LCD_DrawPoint( snake_Info_next.sHeadx, snake_Info_next.sHeady, SNAKE_HEAD_COLOR );	//在蛇头位置画点
	if(upDateSnakeEnd == 1)
	{	
		snake_Info.sLength++;		//更新蛇身长度
		snake_Info.sFraction++;		//加分数
		u8 i = 0;
		for(i=0;i<5;i++)
		{
			if( snake_Info_next.sHeadx == food_Info[i].xFood && snake_Info_next.sHeady == food_Info[i].yFood )
			{
			//更新食物状态
				food_Info[i].validity = 0;	
			}
		}
		
		if( snake_Info.sFraction%2 == 0 && speed>0 )	//分数每多2分速度增加
			speed--;
	}
		
	//更新蛇身坐标
	Snake_Update_BodyXY();
	
	return 1;
}
 
 
/*****************************************************************
 * 函  数： Create_Food
 * 功  能： 生成食物
 * 参  数：
 *
 * 返回值： 无
 *
 * 备  注：
******************************************************************/
static _food_Info Create_Food(_food_Info food_Info_c)
{
	uint16_t rowTotal, colTotal, row, col;
    u32 temp1 = 0;
	u8 rd_color;
	if( food_Info_c.validity == 0 )		//如果食物已经被吃掉
		
	{
		
			LED_SetMode(0x03);
		  delay_timer_nms(500);
		  LED_SetMode(0x01);
		//计算食物的行列范围
		colTotal = ( BORDER_LIMIT_RIGHT - BORDER_LIMIT_LEFT - 1) / FOOD_LENGTH_PIXELS;//70
		rowTotal = ( BORDER_LIMIT_DOWN - BORDER_LIMIT_UP - 1) / FOOD_LENGTH_PIXELS;//50		
		
		do
		{
			temp1 += Snake_FOOD();					//读取定时器的值
	
			//随机生成某行某列
			srand( temp1 );							//随机种子
			
			col = rand() % (colTotal);//0~69
			row = rand() % (rowTotal);//0~49
			
			//根据某行某列计算出食物的实际像素坐标
			//假设FOOD_LENGTH_PIXELS=5,0 x 5 + 4 + 1 = 5,9 x 5 + 4 + 1 = 50,而边界为4,55，有效区域为5~54
			food_Info_c.xFood = (col * FOOD_LENGTH_PIXELS) + BORDER_LIMIT_LEFT + 1;//5~70 
			food_Info_c.yFood = (row * FOOD_LENGTH_PIXELS) + BORDER_LIMIT_UP + 1;//5~50
			
			food_Info_c.validity = 1;			//已生成食物
			
			rd_color = VGA_Rd_Color(food_Info_c.xFood,food_Info_c.yFood);		
			//检查随机数是否会超出边界
			if( food_Info_c.xFood <= BORDER_LIMIT_LEFT 	|| 
			    food_Info_c.xFood >= BORDER_LIMIT_RIGHT	||
			    food_Info_c.yFood <= BORDER_LIMIT_UP	||
			    food_Info_c.yFood >= BORDER_LIMIT_DOWN	)
			{
				food_Info_c.validity = 0;
			}
			else if(rd_color == WHITE || rd_color == FOOD_COLOR)
			{
				food_Info_c.validity = 0;
			}
			else 
			{	
				//遍历链表数据，检查随机数是否与蛇体重合，因为返回值1为有重复值，所以1-运算，赋值为0
				food_Info_c.validity = 1 - listCheck( snake_list, food_Info_c.xFood, food_Info_c.yFood );
			}
			
		}while( food_Info_c.validity == 0 );		
		Snake_Draw_Erase_OneJoint( food_Info_c.xFood, food_Info_c.yFood, FOOD_COLOR );		//生成新的食物		
	}
	return food_Info_c;	
}
 
void Snake_Game(void)
{
	u8 snakeSta = 1;
	//Snake_Show_BackGround(gImage_cs);
	Border_Display_Init();			//初始化游戏边界
	Snake_Display_Init();			//初始化蛇体
	Timer_Config(3,5000);
	//LED_SetMode(0x01);
	
	while(startGame == 0);			//按下key_up键游戏开始
	keyFlag = snake_Info.sHeadDir;  //开始朝蛇头方向前进
	while(1)
	{
		while( snakeSta != 0 )			
		{	
			switch(keyFlag)
			{
				case SNAKE_UP:				//向上走
					if( snake_Info.sHeadDir != 2 )
					{
						snake_Info_next.sHeady -= SNAKE_MOVE_DISTANCE;
						snake_Info.sHeadDir = 1;
					} else {
						keyFlag = 2;
						continue;
					}
					break;
					
				case SNAKE_DOWN:			//向下走
					if( snake_Info.sHeadDir != 1 )
					{
						snake_Info_next.sHeady += SNAKE_MOVE_DISTANCE;
						snake_Info.sHeadDir = 2;
					} else {
						keyFlag = 1;
						continue;
					}
					break;
					
				case SNAKE_LEFT:			//向左走
					if( snake_Info.sHeadDir != 4 )
					{
						snake_Info_next.sHeadx -= SNAKE_MOVE_DISTANCE;
						snake_Info.sHeadDir = 3;
					} else {
						keyFlag = 4;
						continue;
					}
					break;
					
				case SNAKE_RIGHT:			//向右走
					if( snake_Info.sHeadDir != 3 )
					{
						snake_Info_next.sHeadx += SNAKE_MOVE_DISTANCE;
						snake_Info.sHeadDir = 4;
					} else {
						keyFlag = 3;
						continue;
					}
					break;
			}			
			//蛇体移动
			snakeSta = Snake_Move();
			
			//检查是否需要更新食物
			u8 i =0;
			for(i=0;i<5;i++)
			{food_Info[i]=Create_Food(food_Info[i]);}
			
			
			//移动速度
			Delay_ms( 250 );
		}
		
		if( snakeSta == 0 )		//游戏结束
		{
			ListDestory( snake_list );		//摧毁双向链表
			Timer_DISENABLE();
			//LED_SetMode(0X00);
			startGame = 0;
			//break;
		}
		while(startGame != 1);//等待开始键按下
		if(startGame == 1)	  //按下key_up重新初始化
		{
			startGame = 0;
			snakeSta = 1;			
			//清屏
			VGA_CLEAN(BLACK);
			while(!VGA_clear_finish);
			VGA_clear_finish = 0;
			delay_systick_nms(100);
			//Snake_Show_BackGround(gImage_cs);
			Border_Display_Init();
			Snake_Display_Init();
			Timer_Config(3,5000);
					
			//LED_SetMode(0X01);
			while(startGame == 0);		//再次按下key_up游戏重新开始
			keyFlag = snake_Info.sHeadDir;	
		}
	}
}

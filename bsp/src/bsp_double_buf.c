#include "bsp_double_buf.h"
#include <string.h>

//分配buf空间
uint8_t DoubleBuf_buf1[DOUBLE_BUF_MAXDEEP]={0};
uint8_t DoubleBuf_buf2[DOUBLE_BUF_MAXDEEP]={0};

//定义并初始化双buf
DoubleBuf_Typedef DoubuleBuf={
	
	.buf1={
		.deep=SINGLE_BUF_DEEP_DEFAULT,
		.count=SINGLE_BUF_COUNT_DEFAULT,
		.index=SINGLE_BUF_INDEX_DEFAULT,
		.buf=DoubleBuf_buf1,
		.status=SINGLE_BUF_STATUS_DEFAULT
	},
	.buf2={
		.deep=SINGLE_BUF_DEEP_DEFAULT,
		.count=SINGLE_BUF_COUNT_DEFAULT,
		.index=SINGLE_BUF_INDEX_DEFAULT,
		.buf=DoubleBuf_buf2,
		.status=SINGLE_BUF_STATUS_DEFAULT
	},
	.buf_load_index=LOAD_INDEX_BUF1,
	.Get_Double_Deep_Callback=NULL//注册设置双缓冲数据深度的回调函数
};

//复位单缓冲器
static void ResetSingleBuf(SingleBuf_Typedef *buf_obj){
	//memset(&buf_obj->buf[0],0,buf_obj->deep);  //如果读取函数在中断中调用 注释掉本行，因为消耗时间过多
	buf_obj->count=0;
	buf_obj->index=0;
	buf_obj->status=SINGLE_BUF_IS_EMPTY;
}

//复位双缓冲器
static void ResetDoubleBuf(DoubleBuf_Typedef *buf_obj){
	ResetSingleBuf(&buf_obj->buf1);
	ResetSingleBuf(&buf_obj->buf2);
	buf_obj->buf_load_index=LOAD_INDEX_BUF1;
}

//初始化双缓冲器
void DoubleBuf_Init(DoubleBuf_Typedef *buf_obj){
	//设置双缓冲器数据深度
	uint32_t deep=buf_obj->Get_Double_Deep_Callback();
	buf_obj->buf1.deep=deep;
	buf_obj->buf2.deep=deep;
	ResetDoubleBuf(buf_obj);
}

//装载数据函数  定时中断设置采样事件    主函数采样装载
uint8_t DoubleBuf_LoadData(DoubleBuf_Typedef *dbuf,uint8_t data){
	SingleBuf_Typedef *buf_obj;
	
	if(dbuf->buf_load_index==LOAD_INDEX_BUF1){//目前正在装载Buf1
		if(dbuf->buf1.status==SINGLE_BUF_IS_FULL){//如果装满了，切换装载指针到buf2
			dbuf->buf_load_index=LOAD_INDEX_BUF2;//切换装载buf1到buf2
			buf_obj=&dbuf->buf2;
		}
	}else{ 
		if(dbuf->buf2.status==SINGLE_BUF_IS_FULL){//如果装满了，切换装载指针到buf1
			dbuf->buf_load_index=LOAD_INDEX_BUF1;//切换装载buf2到buf1
			buf_obj=&dbuf->buf1;
		}
	}
	if(buf_obj->count==buf_obj->deep){
		return DOUBLEBUF_FALSE;
	}
	//装载数据
	buf_obj->buf[buf_obj->index++]=data;
	buf_obj->count++;
	if(buf_obj->count==buf_obj->deep){buf_obj->status=SINGLE_BUF_IS_FULL;}
	return DOUBLEBUF_SUCESS;
}


//从双缓冲中读取数据
uint8_t DoubleBuf_ReadData(DoubleBuf_Typedef *dbuf,SingleBuf_Typedef *buf_obj){
	//判断非法
	uint8_t res=DOUBLEBUF_FALSE;
	if(dbuf->buf1.status==SINGLE_BUF_IS_FULL){
		memcpy(buf_obj->buf,dbuf->buf1.buf,dbuf->buf1.deep);//读取数据
		ResetSingleBuf(&dbuf->buf1);//恢复状态
		res=DOUBLEBUF_SUCESS;
	}else if(dbuf->buf2.status==SINGLE_BUF_IS_FULL){
		memcpy(buf_obj->buf,dbuf->buf2.buf,dbuf->buf2.deep);//读取数据
		ResetSingleBuf(&dbuf->buf2);	//恢复状态
		res=DOUBLEBUF_SUCESS;
	}else{
		res=DOUBLEBUF_FALSE;
	}
	return res;
}



//如果需要设置缓冲器深度   需要写该函数  并重新初始化双缓冲器
__weak uint32_t Get_Double_Deep_Callback(void){

	return SINGLE_BUF_DEEP_DEFAULT;
}


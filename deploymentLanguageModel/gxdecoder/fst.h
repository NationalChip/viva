#ifndef __FST_H__
#define __FST_H__
#include "gxdecoder_conf.h"
#pragma pack(4)
typedef struct {
    unsigned short ilabel:9;
    unsigned short olabel:10;
    unsigned short next_state:13;
    float weight;
} ARC;

int ArcCompare(ARC *a, ARC *b);

typedef struct {
	unsigned short key;
	/*算法中主要使用hash表存float和地址*/
    float f;
} FST_NODE;
typedef struct {
    int start_;
    unsigned short arc_offset_[MAX_STATES];
    ARC arcs_[MAX_ARCS];
    int arcs_cnt;
    int arc_offset_cnt;
    FST_NODE final_node[FINALS_LENGTH];
    int final_cnt;
} FST; 

#endif


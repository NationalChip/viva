#include <stdio.h>
#include <string.h>
#include "fst.h"
#include <stdlib.h>
#include <unistd.h>

static unsigned int ReadVarint32(FILE *fp)
{
    unsigned int ret = 0;
    int offset = 0, size = 0;
    while (1) {
        unsigned char ch = (unsigned char)fgetc(fp);
        ret |= (unsigned int)(ch & 0x7f) << offset;
        if (ch < 0x80) break;
        offset += 7;
        size += 1;
        if (size > 5)
            return -1;
    }

    ret = (ret >> 1)^ - (ret & 1);
    return ret;
}

static int ReadFst(FST *fst, char *file_name)
{
    printf("read file : %s  \n", file_name);
    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL) {
        printf("=== Warning === open file(%s) error!!! \n", file_name);
        return -1;
    }

    int num_states, num_finals, num_arcs;
    fst->start_ = ReadVarint32(fp);
    num_states = ReadVarint32(fp);
    num_finals = ReadVarint32(fp);
    num_arcs = ReadVarint32(fp);

    // fscanf(fp, "%d, %d, %d, %d", &fst->start_, &num_states, &num_finals, &num_arcs);

    printf("#### start:%d, num_states:%d, num_finals:%d, num_arcs:%d \n", fst->start_, num_states, num_finals, num_arcs);

    fst->arc_offset_cnt = num_states;
    for (int i = 0; i < num_states; i++) {
        fst->arc_offset_[i] = ReadVarint32(fp);
    }

    // umap_init(&(fst->finals_));
    for (int i = 0; i < num_finals; i++) {
        int state;
        float weight;

        state = ReadVarint32(fp);
        fread(&weight, sizeof(float), 1, fp);
        // printf("state: %d , weight : %f \n", state, weight);

        FST_NODE v = {
            .key = state,
            .f = weight
        };
        fst->final_node[fst->final_cnt++] = v;
        // umap_insert(&(fst->finals_), v);
    }

    fst->arcs_cnt = num_arcs;
    for (int i = 0; i < num_arcs; i++) {
        int ilabel = ReadVarint32(fp);
        // float weight = ReadVarint32(fp);
        float weight;
        fread(&weight, sizeof(float), 1, fp);
        int olabel = ReadVarint32(fp);
        int next_state = ReadVarint32(fp);

        ARC arc = {
            .ilabel = ilabel,
            .olabel = olabel,
            .weight = weight,
            .next_state = next_state
        };
        fst->arcs_[i] = arc;
        // printf("%d, %d, %f, %d \n", arc.ilabel, arc.olabel, arc.weight, arc.next_state);
    }

    return 0;
}

int main(int argc, char **argv)
{
    char hclg_file[40] = "";
    int opt;
    if(argc < 2) {
        fprintf(stderr, "使用方法： %s -f [fst_file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // 解析命令行参数
    while ((opt = getopt(argc, argv, "f:d:")) != -1) {
        switch (opt) {
        case 'f':
            printf("选项 -f 的值是：%s\n", optarg);
            strcpy(hclg_file, optarg);
            break;
        default: /* '?' */
            fprintf(stderr, "使用方法： %s -f [fst_file]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    FST fst;
    memset(&fst, 0, sizeof(fst));
    int beam = 4;
    int max_active = 50;
    float am_scale = 1.0f;
    // char hclg_file[] = "tlg_gxdecoder_variant.fst";
    // char word_file[] = "words.txt";

    printf("fst memory:  %ld \n", sizeof(FST));
    printf("ARC memory:  %ld \n", sizeof(ARC));
    // printf("UMAP memory:  %ld \n", sizeof(UMAP));
    FILE *fst_bin = fopen("fst.bin", "wb+");
    FILE *input_bin = fopen("input.bin", "wb+");
    int ret = ReadFst(&fst, hclg_file);
    if (ret != 0) {
        return 31;
    }
    fwrite(&fst, sizeof(FST), 1, fst_bin);
    printf("-------fst.arcs_cnt:%d\n", fst.arcs_cnt);
    printf("-------fst.final_cnt:%d\n", fst.final_cnt);

#if 0
    printf("start_:%d ,arc_offset_[0]:%d %d,arcs[0].ilable:%d %d, arcs_cnt:%d, arc_offset_cnt:%d\n",
    fst.start_, fst.arc_offset_[0], fst.arc_offset_[1],fst.arcs_->ilabel,fst.arcs_->olabel, fst.arcs_cnt, fst.arcs_cnt);
#endif
    // UMAP fst_sfinals;
    // umap_init(&fst_finals);
    // for (int i = 0; i < fst.final_cnt; i++) {
    //     KV_NODE v = {
    //         .key = fst.final_node[i].key,
    //         .value.f = fst.final_node[i].f
    //     };

    //     umap_insert(&fst_finals, v);

    // }
    // printf("line:%d",__LINE__);
    // FastDecoderInit(&fst, &op);
    // InitDecoding(&fst);

    // int result[MAX_RESULTS];
    // float graph_cost = 0.0;
    // float ac_cost = 0.0;

    // int num_frames = 38;
    // int num_classes = 64;

    // float loglike[num_frames][num_classes];
    // FILE *fprob = fopen("phone_prob.txt", "r");
    // if (fprob == NULL) {
    //     printf("no such file!\n");
    // }
    // // char buffer[1024] = {0};
    // for (int i = 0; i < num_frames; i++) {
    //     for (int j = 0; j < num_classes; j++) {
    //         fscanf(fprob, "%f", &loglike[i][j]);
    //     }
    // }
    // // printf("prob done!\n");
    // AdvanceDecoding(&fst, loglike, num_frames);
    // int result_cnt = GetBestPath(&fst, result, MAX_RESULTS, &graph_cost, &ac_cost, &fst_finals);
    // float per_likelihood = (-graph_cost - ac_cost) / num_frames;

    // printf("result: \n");
    // for (int i = 0; i < result_cnt; i++) {
    //     printf("%d ", result[i]);
    // }
    // printf("\n\n");

    // printf("graph_cost = %f \n", graph_cost);
    // printf("ac_costs = %f \n", ac_cost);
    // printf("log-like per frame is : %f over %d frames \n", per_likelihood, num_frames);

    // printf("fst memory:  %ld \n", sizeof(FST));
    // printf("prob memory: %ld \n", sizeof(loglike));
    // get_queue_max_memory();
    // get_umap_max_memory();
    // GetTpoolMaxMemory();
    // get_tmp_array_max_memory();

    return 0;
}